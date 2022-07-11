#include "cpp_quiz.hpp"

void Delay(const float seconds);

struct Score
{
  int total;
  int correct;
  int wrong;

  inline std::string GetPercentage()
  {
    return std::to_string((float)correct / (float)total * 100) + '%';
  }
};

struct Feedback
{
  std::string correct;
  std::string incorrect;
};

struct Answer
{
  std::string A;
  std::string B;
  std::string C;
  std::string D;
  char Ans;
};

struct Question
{
  std::string Prompt;
  Answer AnswerOptions;
  Feedback Feedback;

  bool CheckAnswer(char ans)
  {
    if(ans == AnswerOptions.Ans)
    {
      std::cout << "Correct!" << std::endl;
      if (Feedback.correct != "Null")
      {
        std::cout << "\n" << GetDL() << "\n\n" << Feedback.correct << "\n\n" << GetDL() << std::endl;
        Delay(3.5);
      }
      else Delay(1.1);
      return true;
    }

    std::string answer_phrase;
    if (AnswerOptions.Ans == 'a') answer_phrase = AnswerOptions.A;
    else if (AnswerOptions.Ans == 'b') answer_phrase = AnswerOptions.B;
    else if (AnswerOptions.Ans == 'c') answer_phrase = AnswerOptions.C;
    else answer_phrase = AnswerOptions.D;

    std::cout << "Incorrect! The answer was " << AnswerOptions.Ans << " (" << answer_phrase << ")" << std::endl;
    if (Feedback.incorrect != "Null")
    {
      std::cout << "\n" << GetDL() << "\n\n" << Feedback.incorrect << "\n\n" << GetDL() << std::endl;
      Delay(3.5);
    }
    else Delay(1.75);
    return false;
  }
};

Score DisplayQuestions(std::vector<Question> questions, const int questions_len);
void RemoveConsoleScrollbars();
void ClearConsole();
void SetCursorPosition(const short x, const short y);
void DisplayResults(Score score);
std::vector<Question> ParseQuestions(std::vector<std::string> lines);

int main(const int argc, const char** argv)
{
  std::cout.precision(3);
  //RemoveConsoleScrollbars();
  ClearConsole();

  // Get flashcard file
  std::string filename;
  bool given = argc >= 2;
  if (argc > 2) { std::cout << "Usage: ./Quiz.exe <flashcards.qfc>" << std::endl; return 1; }

  while (1)
  {
    if (!given)
    {
      std::cout << "Enter the name of your flashcard file (*.qfc): ";
      getline(std::cin, filename);
      filename = trim(filename);
    } else filename = argv[1];
    given = false;
    if (filename.length() > 4 && filename.substr(filename.length() - 4, 4).compare(".qfc") == 0 && FileExists(filename)) break;
    else std::cout << "Unable to open file; it either does not exist or is not of the \".qfc\" extension\n" << std::endl;
  }

  std::vector<std::string> encoded_lines = ReadFile(filename);
  std::vector<std::string> lines = DecodeLines(encoded_lines);
  std::vector<Question> questions = ParseQuestions(lines);
  ClearConsole();

  std::cout << DL << "\n\nPlease answer each question with just one letter: A, B, C, or D\nGet ready for the first question ...\n" << std::endl;
  std::cout << DL << '\n' << std::endl;
  Delay(2);

  Score results = DisplayQuestions(questions, size(questions));

  Delay(1.5);
  ClearConsole();
  DisplayResults(results);
  std:: cin >> filename; // wait to clear terminal
}

Score DisplayQuestions(std::vector<Question> questions, const int questions_len)
{
  Score score = { questions_len, 0, 0 };
  std::string response;

  for (int i = 0; i < questions_len; i++)
  {
    ClearConsole();

    size_t sl = 2;

    // prompt and answer options
    std::cout << DL << "\n\n Question " << i + 1 << ": " << questions[i].Prompt << std::endl;
    std::cout << "    - A: " << questions[i].AnswerOptions.A << std::endl;
    std::cout << "    - B: " << questions[i].AnswerOptions.B << std::endl;
    if (questions[i].AnswerOptions.C != "Null")
    { std::cout << "    - C: " << questions[i].AnswerOptions.C << std::endl; sl--; }
    if (questions[i].AnswerOptions.D != "Null")
    { std::cout << "    - D: " << questions[i].AnswerOptions.D << std::endl; sl--; }

    std::cout << "\n Your Answer: \n\n" << DL << std::endl;
    SetCursorPosition(14, 8 - sl);
    getline(std::cin, response);
    response = trim(response);
    if (response == "A") response = "a";
    if (response == "B") response = "b";
    if (response == "C") response = "c";
    if (response == "D") response = "d";
    SetCursorPosition(14 + response.length(), 8 - sl);
    std::cout << " ... ";

    if (response.length() != 1)
    {
      // an invalid answer choice is entered into the CheckAnswer function if a valid answer is not given to avoid boilerplate
      questions[i].CheckAnswer('e');
      score.wrong++;
      continue;
    }

    bool isCorrect = questions[i].CheckAnswer(response[0]);
    if (isCorrect) score.correct++;
    else score.wrong++;
  }

  return score;
}

void RemoveConsoleScrollbars()
{
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

  CONSOLE_SCREEN_BUFFER_INFO scrBufferInfo;
  GetConsoleScreenBufferInfo(hOut, &scrBufferInfo);

  short winWidth = scrBufferInfo.srWindow.Right - scrBufferInfo.srWindow.Left + 1;
  short winHeight = scrBufferInfo.srWindow.Bottom - scrBufferInfo.srWindow.Top + 1;

  short scrBufferWidth = scrBufferInfo.dwSize.X;
  short scrBufferHeight = scrBufferInfo.dwSize.Y;

  COORD newSize;
  newSize.X = scrBufferWidth;
  newSize.Y = winHeight;

  SetConsoleScreenBufferSize(hOut, newSize);
}

void Delay(const float seconds)
{
  const int microToSeconds = 1000000;
  const double time = seconds * microToSeconds;

  usleep(time);
}

void DisplayResults(Score score)
{
  std::string result = std::to_string(score.correct) + '/' + std::to_string(score.total);
  std::string percentage = score.GetPercentage();
  std::string note;

  // Set note
  float rawp = std::stof(percentage.substr(0, percentage.length() - 1));
  if (rawp == 100) note = "You answered every question perfectly! You get a gold star ^.^";
  else if (rawp >= 90) note = "Fantastic! Try this quiz again for a 100% :l";
  else if (rawp >= 80) note = "Great job! You've got just a few more questions to master :O";
  else if (rawp >= 70) note = "Hmm... You can do better T^T";
  else if (rawp >= 60) note = "An okay attempt... +_+";
  else if (rawp >= 50) note = "You failed! Step it up :{";
  else if (rawp >= 40) note = "Wow... that was terrible :p";
  else if (rawp >= 30) note = "Did you guess on every question?? :|";
  else if (rawp >= 20) note = "Might want to take an IQ test... :#";
  else if (rawp >= 10) note = percentage + "... What would your dad say... >:|";
  else if (rawp >= 0) note = "Just kys at this point ;-;";

  std::cout << DL << "\n\nTotal Questions: " << score.total << "\nTotal Correct: " << score.correct << std::endl;
  std::cout << "Total Wrong: " << score.wrong << "\n\nFinal Score: " << result << std::endl;
  std::cout << "Percentage: " << percentage << "\n\nNote: " << note << "\n\n" << DL << std::endl;
}

std::vector<Question> ParseQuestions(std::vector<std::string> lines)
{
  std::vector<Question> questions;
  
  for (auto &line : lines)
  {
    std::string prompt, a, b, c, d, feedback_correct, feedback_incorrect;
    char ans;

    uint64_t start, end;
    prompt = line.substr(0, line.find(" - "));

    start = prompt.length() + 3;
    end = line.find(" | ");
    a = line.substr(start, end - start);

    start += a.length() + 3;
    end = line.find(" | ", start + 1);
    b = line.substr(start, end - start);

    start += b.length() + 3;
    end = line.find(" | ", start + 1);
    c = line.substr(start, end - start);

    start += c.length() + 3;
    end = line.find(" || ");
    d = line.substr(start, end - start);

    start += d.length() + 4;
    end = line.find(" || ", start + 1);
    ans = line.substr(start, 1)[0];

    start += 1 + 4;
    end = line.find(" || ", start + 1);
    feedback_correct = line.substr(start, end - start);
  
    start += feedback_correct.length() + 4;
    feedback_incorrect = line.substr(start, line.length() - start); 
  
    Feedback feedback = { feedback_correct, feedback_incorrect };
    Answer answer_options = { a, b, c, d, ans };
    Question question = { prompt, answer_options, feedback };
    questions.push_back(question);
  }

  return questions;
}
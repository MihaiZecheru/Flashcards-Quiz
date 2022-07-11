#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <cmath>

#include "cpp_quiz.hpp"

std::string GetFilename(void);
std::vector<std::string> CreateQuestions(void);
inline void CreateFile(const std::string fname, const std::vector<std::string> lines);

int main(const int argc, const char** argv)
{
  ClearConsole();
  std::string filename = GetFilename();

  std::vector<std::string> lines = CreateQuestions();
  CreateFile(filename, lines);

  std::cout << "Your finished set includes " << lines.size() << " flashcard" << (lines.size() > 1 ? "s" : "") << ". The flashcard file you've made is called " << "\"" << filename << "\"" << std::endl;
  getline(std::cin, filename); // keep program open until enter is pressed
  return 0;
}

std::string GetFilename()
{
  std::string filename;
  while (1)
  {
    std::cout << "Enter the name of your flashcard file (*.qfc): ";
    getline(std::cin, filename);
    if (filename.length() > 4 && filename.substr(filename.length() - 4, 4).compare(".qfc") == 0 && !FileExists(filename)) break;
    else std::cout << "Unable to name your file \"" << filename << "\"; it either already exists or is not of the \".qfc\" extension\n" << std::endl;
  };

  ClearConsole();
  return filename;
}

inline void WriteTopInformation(int& qnum)
{
  std::string qs = " Question #" + std::to_string(qnum) + ' ';
  std::string HDL = DL.substr(0, std::floor((DL.length() - (qs.length() + 1)) / 2));
  ClearConsole();
  std::cout << DL << "\n\nEnter the proper information for the following flashcard fields. If you make a mistake, don't worry;";
  std::cout << "\nthe final prompt asks you if you want to redo the current flashcard.\n\nFor each flashcard, you must provide one question and at least 2 answer choices." << std::endl;
  std::cout << '\n' << HDL << qs << HDL << '\n' << std::endl;
}

inline bool WriteQuestionsPrompt(std::vector<std::string>& vctr, int& qnum)
{
  // Prepare Screen
  WriteTopInformation(qnum);

  /* Get Fields */

  std::string prompt, a, b, c, d, ans, feedback_correct, feedback_incorrect;

  while (prompt.empty())
  {
    WriteTopInformation(qnum);
    std::cout << "Question: ";
    getline(std::cin, prompt);
    trim(prompt);
  }

  while (a.empty())
  {
    WriteTopInformation(qnum);
    std::cout << "Question: " << prompt << std::endl;

    std::cout << "Answer A: ";
    getline(std::cin, a);
    trim(a);
  }

  while (b.empty())
  {
    WriteTopInformation(qnum);
    std::cout << "Question: " << prompt << std::endl;
    std::cout << "Answer A: " << a << std::endl;

    std::cout << "Answer B: ";
    getline(std::cin, b);
    trim(b);
  }

  std::cout << "Answer C: ";
  getline(std::cin, c);
  trim(c);

  if (c.empty()) c = "Null";
  WriteTopInformation(qnum);
  std::cout << "Question: " << prompt << std::endl;
  std::cout << "Answer A: " << a << std::endl;
  std::cout << "Answer B: " << b << std::endl;
  std::cout << "Answer C: " << c << std::endl;

  std::cout << "Answer D: ";
  getline(std::cin, d);
  trim(d);

  if (d.empty()) d = "Null";
  WriteTopInformation(qnum);
  std::cout << "Question: " << prompt << std::endl;
  std::cout << "Answer A: " << a << std::endl;
  std::cout << "Answer B: " << b << std::endl;
  std::cout << "Answer C: " << c << std::endl;
  std::cout << "Answer D: " << d << std::endl;
  
  while (ans.compare("a") != 0 && ans.compare("b") != 0 && ans.compare("c") != 0 && ans.compare("d") != 0)
  {
    WriteTopInformation(qnum);
    std::cout << "Question: " << prompt << std::endl;
    std::cout << "Answer A: " << a << std::endl;
    std::cout << "Answer B: " << b << std::endl;
    std::cout << "Answer C: " << c << std::endl;
    std::cout << "Answer D: " << d << std::endl;

    std::cout << "\nCorrect Answer Letter (A, B, C, or D): ";
    getline(std::cin, ans);
    for (int i = 0; i < ans.length(); i++) ans[i] = ::tolower(ans[i]);
  }

  std::cout << "Feedback When Correct: ";
  getline(std::cin, feedback_correct);
  trim(feedback_correct);

  if (feedback_correct.empty()) feedback_correct = "Null";
  WriteTopInformation(qnum);
  std::cout << "Question: " << prompt << std::endl;
  std::cout << "Answer A: " << a << std::endl;
  std::cout << "Answer B: " << b << std::endl;
  std::cout << "Answer C: " << c << std::endl;
  std::cout << "Answer D: " << d << std::endl;
  std::cout << "\nCorrect Answer Letter (A, B, C, or D): " << ans << std::endl;
  std::cout << "Feedback When Correct: " << feedback_correct << std::endl;

  std::cout << "Feedback When Incorrect: ";
  getline(std::cin, feedback_incorrect);
  trim(feedback_incorrect);

  if (feedback_incorrect.empty()) feedback_incorrect = "Null";
  WriteTopInformation(qnum);
  std::cout << "Question: " << prompt << std::endl;
  std::cout << "Answer A: " << a << std::endl;
  std::cout << "Answer B: " << b << std::endl;
  std::cout << "Answer C: " << c << std::endl;
  std::cout << "Answer D: " << d << std::endl;
  std::cout << "\nCorrect Answer Letter (A, B, C, or D): " << ans << std::endl;
  std::cout << "Feedback When Correct: " << feedback_correct << std::endl;
  std::cout << "Feedback When Incorrect: " << feedback_incorrect << std::endl;  

  std::string response;
  std::cout << '\n' << DL << "\n\nThis flashcard is done. Type any key other than \"q\" or \"r\" to make another.\n\nTo redo the current card, type \"r\".";
  std::cout << "\nIf you've made enough flashcards, type \"q\" to build your .qfc file: \n\n" << DL << std::endl;
  SetCursorPosition(68, 24);
  getline(std::cin, response);
  trim(response);

  bool quit, redo, cont;

  quit = response.compare("q") == 0;
  redo = response.compare("r") == 0;
  cont = !quit && !redo;

  if (quit || cont)
  {
    std::string line = prompt + " - " + a + " | " + b + " | " + c + " | " + d + " || " + ans[0] + " || " + feedback_correct + " || " + feedback_incorrect;
    vctr.push_back(line);
  }
  else if (redo)
    return WriteQuestionsPrompt(vctr, qnum);

  qnum++;
  return quit;
}

std::vector<std::string> CreateQuestions(void)
{
  std::vector<std::string> lines;
  int qnum = 1;

  // Form initial structure
  while (1) if(WriteQuestionsPrompt(lines, qnum)) break;
  ClearConsole();

  // Encode all lines
  size_t KEY = 7;
  for (int i = 0; i < lines.size(); i++)
  {
    for (int j = 0; j < lines[i].length(); j++)
    {
      int char_ = lines[i][j] == 'C' ? '%' : lines[i][j] == '!' ? '*' : lines[i][j] == ' ' ? '<' : lines[i][j] - KEY != '<' ? lines[i][j] - KEY : '}';

      lines[i][j] = (char)char_;
    }
  }

  return lines;
}

inline void CreateFile(const std::string fname, const std::vector<std::string> lines)
{
  std::ofstream f(fname);

  for (auto &l : lines)
    f << l << std::endl;

  f.close();
}

#include "cpp_quiz.hpp"

int main(const int argc, const char** argv)
{
  if (argc != 2)
  {
    std::cout << "Usage: ./_decode_qfc.exe <qfc_filename>" << std::endl;
    return 1;
  }

  const std::string fname = argv[1];

  if (!FileExists(fname))
  {
    std::cout << "Given file does not exist." << std::endl;
    return 1;
  }

  const std::vector<std::string> lines = DecodeLines(ReadFile(fname));

  for (int i = 0; i < lines.size(); i++)
  {
    std::cout << lines[i] << std::endl;
  }

  return 0;
}
// HEADER => General Purpose Functions

#ifndef CPP_QUIZ_H
#define CPP_QUIZ_H

#include <iostream>
#include <vector>
#include <fstream>
#include <windows.h>
#include <string>
#include <unistd.h>
#include <cstring>

int GetConsoleWidth()
{
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  int width;

  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  return width;
}

inline std::string GetDL() { std::string dl = ""; for (int i = 0; i < GetConsoleWidth(); i++) dl += '-'; return dl; }
const std::string DL = GetDL();

const char* ws = " \t\n";

inline std::string& rtrim(std::string& s, const char* t = ws)
{
  s.erase(s.find_last_not_of(t) + 1);
  return s;
}

inline std::string& ltrim(std::string& s, const char* t = ws)
{
  s.erase(0, s.find_first_not_of(t));
  return s;
}

inline std::string& trim(std::string& s, const char* t = ws)
{
  return ltrim(rtrim(s, t), t);
}

void SetCursorPosition(const short x, const short y)
{
  COORD pos = { x, y };
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleCursorPosition(hConsole, pos);
}


void ClearConsole(void)
{
  system("cls");
}

std::vector<std::string> ReadFile(std::string fname)
{
  std::vector<std::string> lines;
  std::string line;
  std::ifstream file (fname);

  if (file.is_open())
  {
    while (getline(file, line)) lines.push_back(line);
    file.close();
  }

  else std::cout << "Unable to open file" << std::endl;
  return lines;
}

std::vector<std::string> DecodeLines(std::vector<std::string> lines)
{
  size_t KEY = 7;

  for (int i = 0; i < lines.size(); i++)
  {
    for (int j = 0; j < lines[i].length(); j++)
    {
      int char_ = lines[i][j] == '%' ? 'C' : lines[i][j] == '*' ? '!' : lines[i][j] == '<' ? ' ' : lines[i][j] + KEY != '}' ? lines[i][j] + KEY : '<';
      lines[i][j] = (char)char_;
    }
  }

  return lines;
}

inline bool FileExists(const std::string &name) {
  std::ifstream f(name.c_str());
  return f.good();
}

#endif
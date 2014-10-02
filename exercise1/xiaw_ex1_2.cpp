/*Copyright 2014 xiaw */
#include <iostream>
#include <string>
int main() {
  std::string input;
  std::cout << "Enter a line:";
  std::getline(std::cin, input);
  std::cout << "Length of line:" << input.length() << std::endl;
  std::cout << "First character: '" << input[0] << "'"<< std::endl;
  std::cout << "Last character: '" << input[input.length() -1]
            << "'" << std::endl;
  std::size_t e = input.find_first_of(' ');
  std::cout << "First word:  " << input.substr(0, e) << std::endl;
  std::size_t b = input.find_last_of(' ');
  if (b == std::string::npos) b = 0;
  std::cout << "Last word:  " <<input.substr(b) << std::endl;
}

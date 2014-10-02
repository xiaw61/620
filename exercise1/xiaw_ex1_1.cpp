/* Copyright 2014 xiaw */
#include <iostream>
#include <cmath>
int main() {
  std::cout << "Enter a number:";
  double input;
  std::cin >> input;
  std::cout << "The number you entered is  :"<< input << std::endl;
  std::cout << "Is the number positive? ";
  if (input >=0) std::cout << "Yes" << std::endl;
  else
    std::cout << "No " << std::endl;
  std::cout << "The sine of the number is :" << std::sin(input) << std::endl;
  std::cout << "The cosine of the number is :" << std::cos(input) << std::endl;
  std::cout << "The tangent of the number is :" << std::tan(input) << std::endl;
}

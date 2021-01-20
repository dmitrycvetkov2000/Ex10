// Copyright 2020 A.SHT
#include <iostream>
#include <string>

#include "postfix.h"

int main() {
    std::string inf("(2 + 60) * 3 / (4.23 - 2 )");
    std::cout << "Infix: " << inf << std::endl;
    std::string postf = infix2postfix(inf);
    std::cout << "Postfix:  " << postf << std::endl;
    std::cout << "Expected: 2 60 + 3 * 4.23 2 - /" << std::endl;
  return 0;
}

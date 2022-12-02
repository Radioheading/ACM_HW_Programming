/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"
#include <unordered_map>
extern std::set<std::string> reserved;
extern bool end;
extern int running_line;
/* Implementation of the Statement class */

Statement::Statement() = default;

Statement::~Statement() = default;

//todo
let::let(std::string info) {
  info = info;
}
void let::execute(EvalState &state, Program &program, TokenScanner &scanner) {
  // copy version of basic.cpp
  info = scanner.nextToken();
  std::string temp_operator;
  auto iter = reserved.find(info);
  if (iter != reserved.end()) {
    throw ErrorException("SYNTAX ERROR");
  } else {
    temp_operator = scanner.nextToken();
    if (temp_operator != "=") throw ErrorException("SYNTAX ERROR");
    Expression *deleter;
    try {
      deleter = parseExp(scanner);
      int ans = deleter->eval(state);
      delete deleter;
      state.setValue(info, ans);
    }
    catch (ErrorException &ex) {
      std::cout << ex.getMessage() << std::endl;
      delete deleter;
    }
  }
  if (program.getNextLineNumber(running_line) != -1)
    running_line = program.getNextLineNumber(running_line);
  else end = true;
}
print::print(std::string info) {
  info = info;
}
void print::execute(EvalState &state, Program &program, TokenScanner &scanner) {
  // copy version of basic.cpp
  Expression *deleter;
  try {
    deleter = parseExp(scanner);
    int ans = deleter->eval(state);
    delete deleter;
    std::cout << ans << '\n';
  }
  catch (ErrorException &ex) {
    std::cout << ex.getMessage() << std::endl;
    delete deleter;
  }
  if (program.getNextLineNumber(running_line) != -1)
    running_line = program.getNextLineNumber(running_line);
  else end = true;
}
rem::rem(std::string info) {
  info = info;
}
void rem::execute(EvalState &state, Program &program, TokenScanner &scanner) {
  // do nothing and turn to the next line
  if (program.getNextLineNumber(running_line) != -1)
    running_line = program.getNextLineNumber(running_line);
  else end = true;
}
End::End(std::string info) {
  info = info;
}
void End::execute(EvalState &state, Program &program, TokenScanner &scanner) {
  end = true;
}

Goto::Goto(std::string info) {
  info = info;
}
void Goto::execute(EvalState &state, Program &program, TokenScanner &scanner) {
  try {
    int next = stringToInteger(scanner.nextToken());
    if (program.LineNumberCheck(next)) running_line = next;
    else {
      if (program.getNextLineNumber(running_line) != -1)
        running_line = program.getNextLineNumber(running_line);
      else end = true;
      throw ErrorException("LINE NUMBER ERROR");
    }
  }
  catch (ErrorException &ex) {
    std::cout << ex.getMessage() << std::endl;
  }
}
input::input(std::string info) {
  info = info;
}
void input::execute(EvalState &state, Program &program, TokenScanner &scanner) {
  // copy version of basic.cpp
  std::string assign_variable = scanner.nextToken();
  std::cout << " ? ";
  std::string assign_value;
  getline(std::cin, assign_value);
  TokenScanner temp;
  temp.setInput(assign_value);
  bool negative_check = true;
  std::string token;
  Expression *deleter;
  int a;
  a = temp.getChar();
  if (a != 45) {
    temp.ungetChar(a);
    negative_check = false;
  }
  try {
    token = temp.nextToken();
    if (temp.hasMoreTokens()) {
      throw ErrorException("INVALID NUMBER");
    }
    if (temp.getTokenType(token) != NUMBER) {
      throw ErrorException("INVALID NUMBER");
    }
    for (int i = 0; i < token.size(); ++i) {
      if (token[i] == '.') {
        throw ErrorException("INVALID NUMBER");
      }
    }
    int ans;
    ans = stringToInteger(token);
    if (negative_check) ans = -ans;
    state.setValue(assign_variable, ans);
    if (program.getNextLineNumber(running_line) != -1)
      running_line = program.getNextLineNumber(running_line);
    else end = true;
  }
  catch (ErrorException &ex) {
    std::cout << ex.getMessage() << std::endl;
  }
}
If::If(std::string info) {
  info = info;
}
void If::execute(EvalState &state, Program &program, TokenScanner &scanner) {
  bool turner = false;
  TokenScanner temp_1;
  TokenScanner temp_2;
  int value_1; // left
  int value_2; // right
  std::string expression_1 = "";
  std::string expression_2 = "";
  std::string temp_operator; // operator
  std::string token;
  while (true) { // return expression_1 as the longest expression
    token = scanner.nextToken();
    if (token != "=" && token != "<" && token != ">") {
      expression_1 += token;
    } else {
      temp_operator = token;
      break;
    }
  }
  Expression *deleter;
  temp_1.setInput(expression_1);
  try {
    deleter = parseExp(temp_1);
    value_1 = deleter->eval(state);
    delete deleter;
  }
  catch (ErrorException &ex) {
    std::cout << ex.getMessage() << std::endl;
    delete deleter;
  }
  while (true) { // return expression_2 as the longest expression after operator
    token = scanner.nextToken();
    if (token != "THEN") {
      expression_2 += token;
    } else {
      break;
    }
  }
  temp_2.setInput(expression_2);
  try {
    deleter = parseExp(temp_2);
    value_2 = deleter->eval(state);
    delete deleter;
  }
  catch (ErrorException &ex) {
    std::cout << ex.getMessage() << std::endl;
    delete deleter;
  }
  if (temp_operator == "=" && value_1 == value_2) {
    turner = true;
  }
  if (temp_operator == "<" && value_1 < value_2) {
    turner = true;
  }
  if (temp_operator == ">" && value_1 > value_2) {
    turner = true;
  }
  if (turner) {
    try {
      int next = stringToInteger(scanner.nextToken());
      if (program.LineNumberCheck(next)) running_line = next;
      else {
        if (program.getNextLineNumber(running_line) != -1)
          running_line = program.getNextLineNumber(running_line);
        else end = true;
        throw ErrorException("LINE NUMBER ERROR");
      }
    }
    catch (ErrorException &ex) {
      std::cout << ex.getMessage() << std::endl;
    }
  } else {
    if (program.getNextLineNumber(running_line) != -1)
      running_line = program.getNextLineNumber(running_line);
    else end = true;
  }
}

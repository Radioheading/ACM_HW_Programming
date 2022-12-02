/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"
std::set<std::string> reserved = {"IF", "LET", "RUN", "GOTO", "LIST", "QUIT", "END", "PRINT", "REM"};
// check whether the variable is valid
bool end = false; // to set the end of one running process
bool finish = false; // to set the end of the program
bool input_check = false; // whether we're to input numbers in this line
int running_line; // what line we're processing now
/* Function prototypes */
std::string var; // what variable we're setting now
void processLine(std::string line, Program &program, EvalState &state);
/* Main program */

int main() {
  EvalState state;
  Program program;
  //cout << "Stub implementation of BASIC" << endl;
  while (true) {
    if (finish) return 0;
    try {
      std::string input;
      getline(std::cin, input);
      if (input.empty())
        return 0;
      processLine(input, program, state);
    } catch (ErrorException &ex) {
      std::cout << ex.getMessage() << std::endl;
    }
  }
  return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 *
 */
void run_line(int line_number, Program &program, EvalState &state) {
  // processing every line when the command is "RUN"
  bool valid = true;
  TokenScanner scanner;
  scanner.ignoreWhitespace();
  scanner.scanNumbers(); // default settings
  std::string all_line = program.line_information[line_number].all_information;
  scanner.setInput(all_line);
  scanner.nextToken(); // get rid of line_number
  std::string type = scanner.nextToken(); // get operation_type
  Statement *execution;
  if (type == "LET") execution = new let(all_line);
  else if (type == "PRINT") execution = new print(all_line);
  else if (type == "REM") execution = new rem(all_line);
  else if (type == "GOTO") execution = new Goto(all_line);
  else if (type == "END") execution = new End(all_line);
  else if (type == "INPUT") execution = new input(all_line);
  else if (type == "IF") execution = new If(all_line);
  else {
    valid = false;
    throw ErrorException("SYNTAX ERROR");
  }
  if (valid) execution->execute(state, program, scanner);
  delete execution;
}
void processLine(std::string line, Program &program, EvalState &state) {
  TokenScanner scanner;
  scanner.ignoreWhitespace();
  scanner.scanNumbers();
  scanner.setInput(line);
  bool first = false;
  int temp_number;
  std::string token;
  if (input_check) { // we are to input numbers now
    bool negative_check = true; // deal with negative numbers
    int a;
    a = scanner.getChar();
    if (a != 45) { // if the number is negative
      scanner.ungetChar(a);
      negative_check = false;
    }
    try {
      token = scanner.nextToken();
      if (scanner.hasMoreTokens()) {
        throw ErrorException("INVALID NUMBER");
      }
      if (scanner.getTokenType(token) != NUMBER) {
        throw ErrorException("INVALID NUMBER");
      }
      for (int i = 0; i < token.size(); ++i) {
        if (token[i] == '.') { // decimal isn't allowed
          throw ErrorException("INVALID NUMBER");
        }
      }
      int ans;
      ans = stringToInteger(token);
      if (negative_check) ans = -ans;
      state.setValue(var, ans);
      input_check = false;
    }
    catch (ErrorException &ex) {
      std::cout << ex.getMessage() << std::endl;
      std::cout << " ? ";
    }
  } else {
    while (scanner.hasMoreTokens()) {
      token = scanner.nextToken();
      if (!first) {
        if (scanner.getTokenType(token) == NUMBER) { // store the line
          temp_number = stringToInteger(token);
          if (token != line) program.line_information[temp_number].all_information = line;
          else { // an empty line
            program.removeSourceLine(temp_number);
          }
        } else if (scanner.getTokenType(token) == WORD) {
          if (token == "LET") {
            var = scanner.nextToken();
            std::string temp_operator;
            auto iter = reserved.find(var);
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
                state.setValue(var, ans); // input done
              }
              catch (ErrorException &ex) {
                std::cout << ex.getMessage() << std::endl;
                delete deleter;
              }
            }
          }
          if (token == "PRINT") {
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
          }
          if (token == "INPUT") {
            var = scanner.nextToken();
            auto iter = reserved.find(var);
            if (iter != reserved.end()) {
              throw ErrorException("SYNTAX ERROR");
            }
            input_check = true;
            std::cout << " ? ";
          }
        } else {
          throw ErrorException("SYNTAX ERROR");
        }
        first = true;
      } else {

      }
    }
    if (token == "LIST") {
      // traverse the set
      for (auto iter = program.line_information.begin(); iter != program.line_information.end(); ++iter) {
        std::cout << iter->second.all_information << '\n';
      }
    }
    if (token == "QUIT") {
      program.clear();
      state.Clear();
      finish = true;
    }
    if (token == "CLEAR") {
      program.clear();
      state.Clear();
    }
    if (token == "HELP") {
      std::cout
          << "BASIC interpreter is an invention in the microcomputer era, which can execute a program written in BASIC"
          << '\n';
      std::cout << "Begin typing your code here :-)" << '\n';
    }
    if (token == "RUN") {
      end = false;
      running_line = program.getFirstLineNumber();
      while (!end) {
        //std::cout<<running_line<<'\n';
        run_line(running_line, program, state);
      }
    }
  }
}

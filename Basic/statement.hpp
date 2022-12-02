/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include <string>
#include <sstream>
#include "evalstate.hpp"
#include "exp.hpp"
#include "Utils/tokenScanner.hpp"
#include "program.hpp"
#include"parser.hpp"
#include "Utils/error.hpp"
#include "Utils/strlib.hpp"
class Program;

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

 public:
  std::string info;
/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

  Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

  virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

  virtual void execute(EvalState &state, Program &program, TokenScanner &scanner) = 0;

};

/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */
class let : public Statement {
 public:
  let(std::string info);
  virtual void execute(EvalState &state, Program &program, TokenScanner &scanner);
};
class print : public Statement {
 public:
  print(std::string info);
  virtual void execute(EvalState &state, Program &program, TokenScanner &scanner);
};
class rem : public Statement {
 public:
  rem(std::string info);
  virtual void execute(EvalState &state, Program &program, TokenScanner &scanner);
};
class End : public Statement {
 public:
  End(std::string info);
  virtual void execute(EvalState &state, Program &program, TokenScanner &scanner);
};
class Goto : public Statement {
 public:
  Goto(std::string info);
  virtual void execute(EvalState &state, Program &program, TokenScanner &scanner);
};
class input : public Statement {
 public:
  input(std::string info);
  virtual void execute(EvalState &state, Program &program, TokenScanner &scanner);
};
class If : public Statement {
 public:
  If(std::string info);
  virtual void execute(EvalState &state, Program &program, TokenScanner &scanner);
};
#endif

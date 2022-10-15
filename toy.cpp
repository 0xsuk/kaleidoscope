#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <string>

enum Token {
  tok_eof = -1,

  // commands
  tok_def = -2,
  tok_extern = -3,

  // primary
  tok_identifier = -4,
  tok_number = -5
};

static std::string IdentifierStr;
static double NumVal;

// lexer
static int gettok() {
  static int LastChar{' '};
  // skip any whitespace
  while (isspace(LastChar)) LastChar = getchar();

  if (isalpha(LastChar)) {  // match a-zA-Z
    IdentifierStr = LastChar;
    while (isalnum((LastChar = getchar())))
      IdentifierStr += LastChar;  // match a-zA-Z0-9
    if (IdentifierStr == "def") return tok_def;
    if (IdentifierStr == "extern") return tok_extern;
    return tok_identifier;
  }

  if (isdigit(LastChar) || LastChar == '.') {  // match 0-9.
    std::string NumStr;
    do {
      NumStr += LastChar;
      LastChar = getchar();
    } while (isdigit(LastChar) || LastChar == '.');

    NumVal = strtod(
        NumStr.c_str(),
        nullptr);  // convert str to double //TODO: error prone, could parse
    return tok_number;
  }

  if (LastChar == '#') {
    // Comment until end of line.
    do LastChar = getchar();
    while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

    if (LastChar != EOF)
      return gettok();  //\n or \r will be process as ascii value
  }

  if (LastChar == EOF) {
    return tok_eof;
  }

  int ThisChar = LastChar;
  LastChar = getchar();  // before return update static variable; otherwise next
                         // function call would reutrn the same
  return ThisChar;       // ascii 0-255
}

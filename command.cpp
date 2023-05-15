#include "command.hpp"
#include <string>
Command stringToCommand(std::string str) {
  Command cmd = Command::invalid_command;
  if (str == "world") {
    cmd = Command::world;
  } else if (str == "import") {
    cmd = Command::import;
  } else if (str == "import") {
    cmd = Command::import;
  } else if (str == "debug") {
    cmd = Command::debug;
  } else if (str == "quit") {
    cmd = Command::quit;
  } else if (str == "what_is_at") {
    cmd = Command::what_is_at;
  } else if (str == "what_is") {
    cmd = Command::what_is;
  } else if (str == "what_is_in") {
    cmd = Command::what_is_in;
  } else if (str[0] == ';') {
    cmd = Command::comment;
  }
  return cmd;
}

std::string commandToString(Command cmd) {
  std::string str= "invalid_command";
  if (cmd == Command::world) {
    str = "world";
  } else if (cmd == Command::import) {
    str = "import";
  } else if (cmd == Command::import) {
    str = "import";
  } else if (cmd == Command::debug) {
    str = "debug";
  } else if (cmd == Command::quit) {
    str = "quit";
  } else if (cmd == Command::what_is_at) {
    str = "what_is_at";
  } else if (cmd == Command::what_is) {
    str = "what_is";
  } else if (cmd == Command::what_is_in) {
    str = "what_is_in";
  } else if (cmd == Command::comment) {
    str = "comment";
  }
  return str;
}

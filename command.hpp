#include <string>
enum class Command {
  world,
  import,
  debug,
  quit,
  what_is_at,
  what_is,
  what_is_in,
  comment,
  invalid_command,
};

Command stringToCommand(std::string str);
std::string commandToString(Command cmd);

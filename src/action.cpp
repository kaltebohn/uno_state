#include "action.hpp"

std::string action2String(const Action action) {
  if (std::holds_alternative<std::monostate>(action)) {
    return "Empty";
  } else if(std::holds_alternative<Card>(action)) {
    return std::get<Card>(action).toString();
  } else if (std::holds_alternative<Color>(action)) {
    return color2String(std::get<Color>(action));
  } else if (std::holds_alternative<ChallengeFlag>(action)) {
    return std::to_string(std::get<ChallengeFlag>(action));
  } else {
    assert(false);
  }
} 

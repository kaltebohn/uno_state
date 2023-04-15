#include "move.hpp"

std::string move2String(const Move move) {
  if (std::holds_alternative<std::monostate>(move)) {
    return "Empty";
  } else if(std::holds_alternative<Card>(move)) {
    return std::get<Card>(move).toString();
  } else if (std::holds_alternative<Color>(move)) {
    return color2String(std::get<Color>(move));
  } else if (std::holds_alternative<ChallengeFlag>(move)) {
    return std::to_string(std::get<ChallengeFlag>(move));
  } else {
    assert(false);
  }
} 

#include "move.hpp"

std::string move2String(const Move move) {
  if (std::holds_alternative<std::monostate>(move)) {
    return "空の行動";
  } else if(std::holds_alternative<Submission>(move)) {
    return std::get<Submission>(move).toString();
  } else if (std::holds_alternative<Color>(move)) {
    return color2String(std::get<Color>(move));
  } else if (std::holds_alternative<ChallengeFlag>(move)) {
    return std::to_string(std::get<ChallengeFlag>(move));
  } else {
    assert(false);
  }
} 

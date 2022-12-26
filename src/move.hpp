#ifndef MOVE_HPP_
#define MOVE_HPP_

#include <variant>

#include "submission.hpp"

using ChallengeFlag = bool;
using Move = std::variant<std::monostate, Submission, Color, ChallengeFlag>;

std::string move2String(const Move move);

#endif // MOVE_HPP_

#ifndef MOVE_HPP_
#define MOVE_HPP_

#include <variant>

#include "submission.hpp"

using ChallengeFlag = bool;
using Move = std::variant<std::monostate, Submission, Color, ChallengeFlag>;

#endif // MOVE_HPP_

#ifndef MOVE_HPP_
#define MOVE_HPP_

#include <variant>

#include "card.hpp"

using ChallengeFlag = bool;
using Move = std::variant<std::monostate, Card, Color, ChallengeFlag>;

std::string move2String(const Move move);

#endif // MOVE_HPP_

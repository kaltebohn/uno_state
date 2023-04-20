#ifndef ACTION_HPP_
#define ACTION_HPP_

#include <variant>

#include "card.hpp"

using ChallengeFlag = bool;
using Action = std::variant<std::monostate, Card, Color, ChallengeFlag>;

std::string action2String(const Action action);

#endif // ACTION_HPP_

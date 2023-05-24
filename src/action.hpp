#ifndef ACTION_HPP_
#define ACTION_HPP_

#include <variant>

#include "card.hpp"

using ChallengeFlag = bool;

/* 着手を共用体で一般化した型。
   - Card: 提出手を表現。
   - Color: 場の色の指定を表現。
   - ChallengeFlag: チャレンジするかしないかを表現。
*/
using Action = std::variant<std::monostate, Card, Color, ChallengeFlag>;

std::string action2String(const Action action);

#endif // ACTION_HPP_

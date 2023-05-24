#ifndef SCHLANGE_HPP_
#define SCHLANGE_HPP_

#include <algorithm>
#include <unordered_map>

#include "action.hpp"
#include "action_type.hpp"
#include "observation.hpp"

/* ルールベースの対戦プレイヤ。 */
class Schlange {
 public:
  /* 与えられた観測における次の手を決める。 */
  static Action nextAction(const Observation& observation);

  static Color ModeOfColorsInHand(const Cards& hand);

 private:
  static Card selectSubmission(const Cards& hand, const Color& table_color, const CardPattern& table_pattern);

  static int evaluateSubmission(const Card& card);
};

#endif // SCHLANGE_HPP_

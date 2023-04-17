#ifndef SCHLANGE_HPP_
#define SCHLANGE_HPP_

#include <algorithm>
#include <unordered_map>

#include "move.hpp"
#include "move_type.hpp"
#include "observation.hpp"

class Schlange {
 public:
  static Move nextMove(const Observation& observation);

  static Color ModeOfColorsInHand(const Cards& hand);

  static Card selectSubmission(const Cards& hand, const Color& table_color, const CardPattern& table_pattern);

  static int evaluateSubmission(const Card& card);
};

#endif // SCHLANGE_HPP_

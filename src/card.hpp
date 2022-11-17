#ifndef CARD_HPP_
#define CARD_HPP_

#include <variant>
#include <vector>

#include "uno_consts.hpp"

enum class Color {
  kBlue,
  kGreen,
  kRed,
  kYellow,
  kWild,
  kNull
};

enum class CardNumber {
  kZero,
  kOne,
  kTwo,
  kThree,
  kFour,
  kFive,
  kSix,
  kSeven,
  kEight,
  kNine
};

enum class CardAction {
  kDrawTwo,
  kReverse,
  kSkip,
  kWild,
  kWildDraw4,
  kWildShuffleHands,
  kWildCustomizable
};

using CardPattern = std::variant<std::monostate, CardNumber, CardAction>;

using Cards = std::vector<Card>;

/* インスタンス1つで1種類のカードを表す。デフォルトで空のカードを表現する(出せるカードがない場合に合法手として使う)。 */
class Card {
 public:
  constexpr Card() = default;

  constexpr Card(const Color c, const CardPattern p) : color_(c), pattern_(p) {}

  constexpr Color getColor() const { return color_; }
  constexpr CardPattern getPattern() const { return pattern_; }

  constexpr bool isEmpty() const { return pattern_.valueless_by_exception(); }

 private:
  const Color color_{Color::kNull};
  const CardPattern pattern_{};
};

static constexpr int kNumOfCardTypes = 56;
static constexpr int kNumOfCards = 112;

#endif // CARD_HPP_

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

/* インスタンス1つで1種類のカードを表す。デフォルトで空のカードを表現する(出せるカードがない場合に合法手として使う)。 */
class Card {
 public:
  constexpr Card() = default;

  constexpr Card(const Color c, const CardPattern p) : color_(c), pattern_(p) {}

  constexpr Color getColor() const { return color_; }
  constexpr CardPattern getPattern() const { return pattern_; }

  constexpr bool isEmpty() const { return std::holds_alternative<std::monostate>(pattern_); }

  constexpr bool operator ==(const Card& c) const { return color_ == c.color_ && pattern_ == c.pattern_; }
  constexpr bool operator !=(const Card& c) const { return !(*this == c); }

  static const Card  kBlueZero;
  static const Card  kBlueOne;
  static const Card  kBlueTwo;
  static const Card  kBlueThree;
  static const Card  kBlueFour;
  static const Card  kBlueFive;
  static const Card  kBlueSix;
  static const Card  kBlueSeven;
  static const Card  kBlueEight;
  static const Card  kBlueNine;
  static const Card  kBlueDrawTwo;
  static const Card  kBlueReverse;
  static const Card  kBlueSkip;
  static const Card  kGreenZero;
  static const Card  kGreenOne;
  static const Card  kGreenTwo;
  static const Card  kGreenThree;
  static const Card  kGreenFour;
  static const Card  kGreenFive;
  static const Card  kGreenSix;
  static const Card  kGreenSeven;
  static const Card  kGreenEight;
  static const Card  kGreenNine;
  static const Card  kGreenDrawTwo;
  static const Card  kGreenReverse;
  static const Card  kGreenSkip;
  static const Card  kRedZero;
  static const Card  kRedOne;
  static const Card  kRedTwo;
  static const Card  kRedThree;
  static const Card  kRedFour;
  static const Card  kRedFive;
  static const Card  kRedSix;
  static const Card  kRedSeven;
  static const Card  kRedEight;
  static const Card  kRedNine;
  static const Card  kRedDrawTwo;
  static const Card  kRedReverse;
  static const Card  kRedSkip;
  static const Card  kYellowZero;
  static const Card  kYellowOne;
  static const Card  kYellowTwo;
  static const Card  kYellowThree;
  static const Card  kYellowFour;
  static const Card  kYellowFive;
  static const Card  kYellowSix;
  static const Card  kYellowSeven;
  static const Card  kYellowEight;
  static const Card  kYellowNine;
  static const Card  kYellowDrawTwo;
  static const Card  kYellowReverse;
  static const Card  kYellowSkip;
  static const Card  kWild;
  static const Card  kWildDraw4;
  static const Card  kWildShuffleHands;
  static const Card  kWildCustomizable;

 private:
  Color color_{Color::kNull};
  CardPattern pattern_{};
};

using Cards = std::vector<Card>;

static constexpr int kNumOfCardTypes = 56;
static constexpr int kNumOfCards = 112;

Cards allCards();

#endif // CARD_HPP_

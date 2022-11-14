#ifndef CARDS_HPP_
#define CARDS_HPP_

#include <array>
#include <variant>

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

  constexpr Card(const Color c, const CardPattern p, const int i, const int q = 0) : color_(c), pattern_(p), quantity_(q), index_(i) {}

  Card(const Color c, const CardPattern p, const int q = 0) : color_(c), pattern_(p), quantity_(q) {
    index_ = Card::card2Key(*this);
  }

  constexpr Color getColor() const { return color_; }
  constexpr CardPattern getPattern() const { return pattern_; }
  constexpr int getQuantity() const { return quantity_; }
  constexpr int getIndex() const { return index_; }

  // TODO: バリデーションすべき。そしたらconstexprはあきらめよう。枚数制限用に、各カードの枚数上限を持つ配列を定義する。
  constexpr Card operator ++() { quantity_++; return *this; }
  constexpr Card operator --() { quantity_--; return *this; }
  constexpr Card operator +(int i) { return Card(color_, pattern_, index_, quantity_ + i); }
  constexpr Card operator -(int i) { return Card(color_, pattern_, index_, quantity_ - i); }
  constexpr Card operator +=(int i) { quantity_ += i; return *this; }
  constexpr Card operator -=(int i) { quantity_ -= i; return *this; }

  constexpr bool isEmpty() const { return pattern_.valueless_by_exception(); }

  static int Card::card2Key(Card c);

 private:
  const Color color_{Color::kNull};
  const CardPattern pattern_{};
  int quantity_{};
  int index_{CardSet::kNullKey};
};

/* カードの集合。カードの配列を持ち、配列の添え字とカードの種類は一対一対応している。*/
class CardSet {
 public:
  CardSet(const std::array<Card, UnoConsts::kNumOfCardTypes> c, const int q)
      : cards_(c), quantity_(q) {}
  
  CardSet(const bool is_full = false) {
    if (is_full) {
      std::copy(fullCards.begin(), fullCards.end(), cards_.begin());
      quantity_ = UnoConsts::kNumOfCards;
    } else {
      std::copy(emptyCards.begin(), emptyCards.end(), cards_.begin());
      quantity_ = 0;
    }
  }

  int getQuantity() const { return quantity_; }

  std::array<Card, UnoConsts::kNumOfCardTypes> getCards() const { return cards_; }

  CardSet operator +=(Card card) { cards_.at(card.getIndex()) += card.getQuantity(); quantity_ += card.getQuantity(); return *this; }
  CardSet operator -=(Card card) { cards_.at(card.getIndex()) -= card.getQuantity(); quantity_ -= card.getQuantity(); return *this; }

  static constexpr int kNullKey = -1;
  static constexpr int kBlueZeroKey = 0;
  static constexpr int kBlueOneKey = 1;
  static constexpr int kBlueTwoKey = 2;
  static constexpr int kBlueThreeKey = 3;
  static constexpr int kBlueFourKey = 4;
  static constexpr int kBlueFiveKey = 5;
  static constexpr int kBlueSixKey = 6;
  static constexpr int kBlueSevenKey = 7;
  static constexpr int kBlueEightKey = 8;
  static constexpr int kBlueNineKey = 9;
  static constexpr int kBlueDrawTwoKey = 10;
  static constexpr int kBlueReverseKey = 11;
  static constexpr int kBlueSkipKey = 12;
  static constexpr int kGreenZeroKey = 13;
  static constexpr int kGreenOneKey = 14;
  static constexpr int kGreenTwoKey = 15;
  static constexpr int kGreenThreeKey = 16;
  static constexpr int kGreenFourKey = 17;
  static constexpr int kGreenFiveKey = 18;
  static constexpr int kGreenSixKey = 19;
  static constexpr int kGreenSevenKey = 20;
  static constexpr int kGreenEightKey = 21;
  static constexpr int kGreenNineKey = 22;
  static constexpr int kGreenDrawTwoKey = 23;
  static constexpr int kGreenReverseKey = 24;
  static constexpr int kGreenSkipKey = 25;
  static constexpr int kRedZeroKey = 26;
  static constexpr int kRedOneKey = 27;
  static constexpr int kRedTwoKey = 28;
  static constexpr int kRedThreeKey = 29;
  static constexpr int kRedFourKey = 30;
  static constexpr int kRedFiveKey = 31;
  static constexpr int kRedSixKey = 32;
  static constexpr int kRedSevenKey = 33;
  static constexpr int kRedEightKey = 34;
  static constexpr int kRedNineKey = 35;
  static constexpr int kRedDrawTwoKey = 36;
  static constexpr int kRedReverseKey = 37;
  static constexpr int kRedSkipKey = 38;
  static constexpr int kYellowZeroKey = 39;
  static constexpr int kYellowOneKey = 40;
  static constexpr int kYellowTwoKey = 41;
  static constexpr int kYellowThreeKey = 42;
  static constexpr int kYellowFourKey = 43;
  static constexpr int kYellowFiveKey = 44;
  static constexpr int kYellowSixKey = 45;
  static constexpr int kYellowSevenKey = 46;
  static constexpr int kYellowEightKey = 47;
  static constexpr int kYellowNineKey = 48;
  static constexpr int kYellowDrawTwoKey = 49;
  static constexpr int kYellowReverseKey = 50;
  static constexpr int kYellowSkipKey = 51;
  static constexpr int kWildKey = 52;
  static constexpr int kWildDraw4Key = 53;
  static constexpr int kWildShuffleHandsKey = 54;
  static constexpr int kWildCustomizableKey = 55;

 private:
  std::array<Card, UnoConsts::kNumOfCardTypes> cards_;
  int quantity_;

  static constexpr std::array<Card, UnoConsts::kNumOfCardTypes> emptyCards{{
      Card{Color::kBlue,   (CardPattern)(CardNumber::kZero),           kBlueZeroKey,         0},
      Card{Color::kBlue,   (CardPattern)(CardNumber::kOne),            kBlueOneKey,          0},
      Card{Color::kBlue,   (CardPattern)(CardNumber::kTwo),            kBlueTwoKey,          0},
      Card{Color::kBlue,   (CardPattern)(CardNumber::kThree),          kBlueThreeKey,        0},
      Card{Color::kBlue,   (CardPattern)(CardNumber::kFour),           kBlueFourKey,         0},
      Card{Color::kBlue,   (CardPattern)(CardNumber::kFive),           kBlueFiveKey,         0},
      Card{Color::kBlue,   (CardPattern)(CardNumber::kSix),            kBlueSixKey,          0},
      Card{Color::kBlue,   (CardPattern)(CardNumber::kSeven),          kBlueSevenKey,        0},
      Card{Color::kBlue,   (CardPattern)(CardNumber::kEight),          kBlueEightKey,        0},
      Card{Color::kBlue,   (CardPattern)(CardNumber::kNine),           kBlueNineKey,         0},
      Card{Color::kBlue,   (CardPattern)(CardAction::kDrawTwo),        kBlueDrawTwoKey,      0},
      Card{Color::kBlue,   (CardPattern)(CardAction::kReverse),        kBlueReverseKey,      0},
      Card{Color::kBlue,   (CardPattern)(CardAction::kSkip),           kBlueSkipKey,         0},
      Card{Color::kGreen,  (CardPattern)(CardNumber::kZero),           kGreenZeroKey,        0},
      Card{Color::kGreen,  (CardPattern)(CardNumber::kOne),            kGreenOneKey,         0},
      Card{Color::kGreen,  (CardPattern)(CardNumber::kTwo),            kGreenTwoKey,         0},
      Card{Color::kGreen,  (CardPattern)(CardNumber::kThree),          kGreenThreeKey,       0},
      Card{Color::kGreen,  (CardPattern)(CardNumber::kFour),           kGreenFourKey,        0},
      Card{Color::kGreen,  (CardPattern)(CardNumber::kFive),           kGreenFiveKey,        0},
      Card{Color::kGreen,  (CardPattern)(CardNumber::kSix),            kGreenSixKey,         0},
      Card{Color::kGreen,  (CardPattern)(CardNumber::kSeven),          kGreenSevenKey,       0},
      Card{Color::kGreen,  (CardPattern)(CardNumber::kEight),          kGreenEightKey,       0},
      Card{Color::kGreen,  (CardPattern)(CardNumber::kNine),           kGreenNineKey,        0},
      Card{Color::kGreen,  (CardPattern)(CardAction::kDrawTwo),        kGreenDrawTwoKey,     0},
      Card{Color::kGreen,  (CardPattern)(CardAction::kReverse),        kGreenReverseKey,     0},
      Card{Color::kGreen,  (CardPattern)(CardAction::kSkip),           kGreenSkipKey,        0},
      Card{Color::kRed,    (CardPattern)(CardNumber::kZero),           kRedZeroKey,          0},
      Card{Color::kRed,    (CardPattern)(CardNumber::kOne),            kRedOneKey,           0},
      Card{Color::kRed,    (CardPattern)(CardNumber::kTwo),            kRedTwoKey,           0},
      Card{Color::kRed,    (CardPattern)(CardNumber::kThree),          kRedThreeKey,         0},
      Card{Color::kRed,    (CardPattern)(CardNumber::kFour),           kRedFourKey,          0},
      Card{Color::kRed,    (CardPattern)(CardNumber::kFive),           kRedFiveKey,          0},
      Card{Color::kRed,    (CardPattern)(CardNumber::kSix),            kRedSixKey,           0},
      Card{Color::kRed,    (CardPattern)(CardNumber::kSeven),          kRedSevenKey,         0},
      Card{Color::kRed,    (CardPattern)(CardNumber::kEight),          kRedEightKey,         0},
      Card{Color::kRed,    (CardPattern)(CardNumber::kNine),           kRedNineKey,          0},
      Card{Color::kRed,    (CardPattern)(CardAction::kDrawTwo),        kRedDrawTwoKey,       0},
      Card{Color::kRed,    (CardPattern)(CardAction::kReverse),        kRedReverseKey,       0},
      Card{Color::kRed,    (CardPattern)(CardAction::kSkip),           kRedSkipKey,          0},
      Card{Color::kYellow, (CardPattern)(CardNumber::kZero),           kYellowZeroKey,       0},
      Card{Color::kYellow, (CardPattern)(CardNumber::kOne),            kYellowOneKey,        0},
      Card{Color::kYellow, (CardPattern)(CardNumber::kTwo),            kYellowTwoKey,        0},
      Card{Color::kYellow, (CardPattern)(CardNumber::kThree),          kYellowThreeKey,      0},
      Card{Color::kYellow, (CardPattern)(CardNumber::kFour),           kYellowFourKey,       0},
      Card{Color::kYellow, (CardPattern)(CardNumber::kFive),           kYellowFiveKey,       0},
      Card{Color::kYellow, (CardPattern)(CardNumber::kSix),            kYellowSixKey,        0},
      Card{Color::kYellow, (CardPattern)(CardNumber::kSeven),          kYellowSevenKey,      0},
      Card{Color::kYellow, (CardPattern)(CardNumber::kEight),          kYellowEightKey,      0},
      Card{Color::kYellow, (CardPattern)(CardNumber::kNine),           kYellowNineKey,       0},
      Card{Color::kYellow, (CardPattern)(CardAction::kDrawTwo),        kYellowDrawTwoKey,    0},
      Card{Color::kYellow, (CardPattern)(CardAction::kReverse),        kYellowReverseKey,    0},
      Card{Color::kYellow, (CardPattern)(CardAction::kSkip),           kYellowSkipKey,       0},
      Card{Color::kWild, (CardPattern)(CardAction::kWild),             kWildKey,             0},
      Card{Color::kWild, (CardPattern)(CardAction::kWildDraw4),        kWildDraw4Key,        0},
      Card{Color::kWild, (CardPattern)(CardAction::kWildShuffleHands), kWildShuffleHandsKey, 0},
      Card{Color::kWild, (CardPattern)(CardAction::kWildCustomizable), kWildCustomizableKey, 0}
  }};

  static constexpr std::array<Card, UnoConsts::kNumOfCardTypes> fullCards{{
      Card{Color::kBlue,   (CardPattern)(CardNumber::kZero),           kBlueZeroKey,         1},
      Card{Color::kBlue,   (CardPattern)(CardNumber::kOne),            kBlueOneKey,          2},
      Card{Color::kBlue,   (CardPattern)(CardNumber::kTwo),            kBlueTwoKey,          2},
      Card{Color::kBlue,   (CardPattern)(CardNumber::kThree),          kBlueThreeKey,        2},
      Card{Color::kBlue,   (CardPattern)(CardNumber::kFour),           kBlueFourKey,         2},
      Card{Color::kBlue,   (CardPattern)(CardNumber::kFive),           kBlueFiveKey,         2},
      Card{Color::kBlue,   (CardPattern)(CardNumber::kSix),            kBlueSixKey,          2},
      Card{Color::kBlue,   (CardPattern)(CardNumber::kSeven),          kBlueSevenKey,        2},
      Card{Color::kBlue,   (CardPattern)(CardNumber::kEight),          kBlueEightKey,        2},
      Card{Color::kBlue,   (CardPattern)(CardNumber::kNine),           kBlueNineKey,         2},
      Card{Color::kBlue,   (CardPattern)(CardAction::kDrawTwo),        kBlueDrawTwoKey,      2},
      Card{Color::kBlue,   (CardPattern)(CardAction::kReverse),        kBlueReverseKey,      1},
      Card{Color::kBlue,   (CardPattern)(CardAction::kSkip),           kBlueSkipKey,         2},
      Card{Color::kGreen,  (CardPattern)(CardNumber::kZero),           kGreenZeroKey,        2},
      Card{Color::kGreen,  (CardPattern)(CardNumber::kOne),            kGreenOneKey,         2},
      Card{Color::kGreen,  (CardPattern)(CardNumber::kTwo),            kGreenTwoKey,         2},
      Card{Color::kGreen,  (CardPattern)(CardNumber::kThree),          kGreenThreeKey,       2},
      Card{Color::kGreen,  (CardPattern)(CardNumber::kFour),           kGreenFourKey,        2},
      Card{Color::kGreen,  (CardPattern)(CardNumber::kFive),           kGreenFiveKey,        2},
      Card{Color::kGreen,  (CardPattern)(CardNumber::kSix),            kGreenSixKey,         2},
      Card{Color::kGreen,  (CardPattern)(CardNumber::kSeven),          kGreenSevenKey,       2},
      Card{Color::kGreen,  (CardPattern)(CardNumber::kEight),          kGreenEightKey,       2},
      Card{Color::kGreen,  (CardPattern)(CardNumber::kNine),           kGreenNineKey,        2},
      Card{Color::kGreen,  (CardPattern)(CardAction::kDrawTwo),        kGreenDrawTwoKey,     2},
      Card{Color::kGreen,  (CardPattern)(CardAction::kReverse),        kGreenReverseKey,     2},
      Card{Color::kGreen,  (CardPattern)(CardAction::kSkip),           kGreenSkipKey,        2},
      Card{Color::kRed,    (CardPattern)(CardNumber::kZero),           kRedZeroKey,          1},
      Card{Color::kRed,    (CardPattern)(CardNumber::kOne),            kRedOneKey,           2},
      Card{Color::kRed,    (CardPattern)(CardNumber::kTwo),            kRedTwoKey,           2},
      Card{Color::kRed,    (CardPattern)(CardNumber::kThree),          kRedThreeKey,         2},
      Card{Color::kRed,    (CardPattern)(CardNumber::kFour),           kRedFourKey,          2},
      Card{Color::kRed,    (CardPattern)(CardNumber::kFive),           kRedFiveKey,          2},
      Card{Color::kRed,    (CardPattern)(CardNumber::kSix),            kRedSixKey,           2},
      Card{Color::kRed,    (CardPattern)(CardNumber::kSeven),          kRedSevenKey,         2},
      Card{Color::kRed,    (CardPattern)(CardNumber::kEight),          kRedEightKey,         2},
      Card{Color::kRed,    (CardPattern)(CardNumber::kNine),           kRedNineKey,          2},
      Card{Color::kRed,    (CardPattern)(CardAction::kDrawTwo),        kRedDrawTwoKey,       2},
      Card{Color::kRed,    (CardPattern)(CardAction::kReverse),        kRedReverseKey,       2},
      Card{Color::kRed,    (CardPattern)(CardAction::kSkip),           kRedSkipKey,          2},
      Card{Color::kYellow, (CardPattern)(CardNumber::kZero),           kYellowZeroKey,       1},
      Card{Color::kYellow, (CardPattern)(CardNumber::kOne),            kYellowOneKey,        2},
      Card{Color::kYellow, (CardPattern)(CardNumber::kTwo),            kYellowTwoKey,        2},
      Card{Color::kYellow, (CardPattern)(CardNumber::kThree),          kYellowThreeKey,      2},
      Card{Color::kYellow, (CardPattern)(CardNumber::kFour),           kYellowFourKey,       2},
      Card{Color::kYellow, (CardPattern)(CardNumber::kFive),           kYellowFiveKey,       2},
      Card{Color::kYellow, (CardPattern)(CardNumber::kSix),            kYellowSixKey,        2},
      Card{Color::kYellow, (CardPattern)(CardNumber::kSeven),          kYellowSevenKey,      2},
      Card{Color::kYellow, (CardPattern)(CardNumber::kEight),          kYellowEightKey,      2},
      Card{Color::kYellow, (CardPattern)(CardNumber::kNine),           kYellowNineKey,       2},
      Card{Color::kYellow, (CardPattern)(CardAction::kDrawTwo),        kYellowDrawTwoKey,    2},
      Card{Color::kYellow, (CardPattern)(CardAction::kReverse),        kYellowReverseKey,    2},
      Card{Color::kYellow, (CardPattern)(CardAction::kSkip),           kYellowSkipKey,       2},
      Card{Color::kWild, (CardPattern)(CardAction::kWild),             kWildKey,             4},
      Card{Color::kWild, (CardPattern)(CardAction::kWildDraw4),        kWildDraw4Key,        4},
      Card{Color::kWild, (CardPattern)(CardAction::kWildShuffleHands), kWildShuffleHandsKey, 1},
      Card{Color::kWild, (CardPattern)(CardAction::kWildCustomizable), kWildCustomizableKey, 3}
  }};
};


#endif // CARDS_HPP_

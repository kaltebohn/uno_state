#ifndef CARD_HPP_
#define CARD_HPP_

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
  static constexpr int kNumOfCardTypes = 56;
  static constexpr int kNumOfCards = 112;

  CardSet(const std::array<Card, CardSet::kNumOfCardTypes> c, const int q)
      : cards_(c), quantity_(q) {}
  
  CardSet(const bool is_full = false) {
    if (is_full) {
      std::copy(fullCards.begin(), fullCards.end(), cards_.begin());
      quantity_ = kNumOfCards;
    } else {
      std::copy(emptyCards.begin(), emptyCards.end(), cards_.begin());
      quantity_ = 0;
    }
  }

  int getQuantity() const { return quantity_; }

  std::array<Card, CardSet::kNumOfCardTypes> getCards() const { return cards_; }

  CardSet operator +=(Card card) { cards_.at(card.getIndex()) += card.getQuantity(); quantity_ += card.getQuantity(); return *this; }
  CardSet operator -=(Card card) { cards_.at(card.getIndex()) -= card.getQuantity(); quantity_ -= card.getQuantity(); return *this; }

  static constexpr int kNullKey            {-1};
  static constexpr int kBlueZeroKey        {0};
  static constexpr int kBlueOneKey         {1};
  static constexpr int kBlueTwoKey         {2};
  static constexpr int kBlueThreeKey       {3};
  static constexpr int kBlueFourKey        {4};
  static constexpr int kBlueFiveKey        {5};
  static constexpr int kBlueSixKey         {6};
  static constexpr int kBlueSevenKey       {7};
  static constexpr int kBlueEightKey       {8};
  static constexpr int kBlueNineKey        {9};
  static constexpr int kBlueDrawTwoKey     {10};
  static constexpr int kBlueReverseKey     {11};
  static constexpr int kBlueSkipKey        {12};
  static constexpr int kGreenZeroKey       {13};
  static constexpr int kGreenOneKey        {14};
  static constexpr int kGreenTwoKey        {15};
  static constexpr int kGreenThreeKey      {16};
  static constexpr int kGreenFourKey       {17};
  static constexpr int kGreenFiveKey       {18};
  static constexpr int kGreenSixKey        {19};
  static constexpr int kGreenSevenKey      {20};
  static constexpr int kGreenEightKey      {21};
  static constexpr int kGreenNineKey       {22};
  static constexpr int kGreenDrawTwoKey    {23};
  static constexpr int kGreenReverseKey    {24};
  static constexpr int kGreenSkipKey       {25};
  static constexpr int kRedZeroKey         {26};
  static constexpr int kRedOneKey          {27};
  static constexpr int kRedTwoKey          {28};
  static constexpr int kRedThreeKey        {29};
  static constexpr int kRedFourKey         {30};
  static constexpr int kRedFiveKey         {31};
  static constexpr int kRedSixKey          {32};
  static constexpr int kRedSevenKey        {33};
  static constexpr int kRedEightKey        {34};
  static constexpr int kRedNineKey         {35};
  static constexpr int kRedDrawTwoKey      {36};
  static constexpr int kRedReverseKey      {37};
  static constexpr int kRedSkipKey         {38};
  static constexpr int kYellowZeroKey      {39};
  static constexpr int kYellowOneKey       {40};
  static constexpr int kYellowTwoKey       {41};
  static constexpr int kYellowThreeKey     {42};
  static constexpr int kYellowFourKey      {43};
  static constexpr int kYellowFiveKey      {44};
  static constexpr int kYellowSixKey       {45};
  static constexpr int kYellowSevenKey     {46};
  static constexpr int kYellowEightKey     {47};
  static constexpr int kYellowNineKey      {48};
  static constexpr int kYellowDrawTwoKey   {49};
  static constexpr int kYellowReverseKey   {50};
  static constexpr int kYellowSkipKey      {51};
  static constexpr int kWildKey            {52};
  static constexpr int kWildDraw4Key       {53};
  static constexpr int kWildShuffleHandsKey{54};
  static constexpr int kWildCustomizableKey{55};

  static constexpr int kBlueZeroMaxQuantity        {1};
  static constexpr int kBlueOneMaxQuantity         {2};
  static constexpr int kBlueTwoMaxQuantity         {2};
  static constexpr int kBlueThreeMaxQuantity       {2};
  static constexpr int kBlueFourMaxQuantity        {2};
  static constexpr int kBlueFiveMaxQuantity        {2};
  static constexpr int kBlueSixMaxQuantity         {2};
  static constexpr int kBlueSevenMaxQuantity       {2};
  static constexpr int kBlueEightMaxQuantity       {2};
  static constexpr int kBlueNineMaxQuantity        {2};
  static constexpr int kBlueDrawTwoMaxQuantity     {2};
  static constexpr int kBlueReverseMaxQuantity     {1};
  static constexpr int kBlueSkipMaxQuantity        {2};
  static constexpr int kGreenZeroMaxQuantity       {2};
  static constexpr int kGreenOneMaxQuantity        {2};
  static constexpr int kGreenTwoMaxQuantity        {2};
  static constexpr int kGreenThreeMaxQuantity      {2};
  static constexpr int kGreenFourMaxQuantity       {2};
  static constexpr int kGreenFiveMaxQuantity       {2};
  static constexpr int kGreenSixMaxQuantity        {2};
  static constexpr int kGreenSevenMaxQuantity      {2};
  static constexpr int kGreenEightMaxQuantity      {2};
  static constexpr int kGreenNineMaxQuantity       {2};
  static constexpr int kGreenDrawTwoMaxQuantity    {2};
  static constexpr int kGreenReverseMaxQuantity    {2};
  static constexpr int kGreenSkipMaxQuantity       {2};
  static constexpr int kRedZeroMaxQuantity         {1};
  static constexpr int kRedOneMaxQuantity          {2};
  static constexpr int kRedTwoMaxQuantity          {2};
  static constexpr int kRedThreeMaxQuantity        {2};
  static constexpr int kRedFourMaxQuantity         {2};
  static constexpr int kRedFiveMaxQuantity         {2};
  static constexpr int kRedSixMaxQuantity          {2};
  static constexpr int kRedSevenMaxQuantity        {2};
  static constexpr int kRedEightMaxQuantity        {2};
  static constexpr int kRedNineMaxQuantity         {2};
  static constexpr int kRedDrawTwoMaxQuantity      {2};
  static constexpr int kRedReverseMaxQuantity      {2};
  static constexpr int kRedSkipMaxQuantity         {2};
  static constexpr int kYellowZeroMaxQuantity      {1};
  static constexpr int kYellowOneMaxQuantity       {2};
  static constexpr int kYellowTwoMaxQuantity       {2};
  static constexpr int kYellowThreeMaxQuantity     {2};
  static constexpr int kYellowFourMaxQuantity      {2};
  static constexpr int kYellowFiveMaxQuantity      {2};
  static constexpr int kYellowSixMaxQuantity       {2};
  static constexpr int kYellowSevenMaxQuantity     {2};
  static constexpr int kYellowEightMaxQuantity     {2};
  static constexpr int kYellowNineMaxQuantity      {2};
  static constexpr int kYellowDrawTwoMaxQuantity   {2};
  static constexpr int kYellowReverseMaxQuantity   {2};
  static constexpr int kYellowSkipMaxQuantity      {2};
  static constexpr int kWildMaxQuantity            {4};
  static constexpr int kWildDraw4MaxQuantity       {4};
  static constexpr int kWildShuffleHandsMaxQuantity{1};
  static constexpr int kWildCustomizableMaxQuantity{3};

 private:
  std::array<Card, CardSet::kNumOfCardTypes> cards_;
  int quantity_;

  static constexpr std::array<Card, CardSet::kNumOfCardTypes> emptyCards{{
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

  static constexpr std::array<Card, CardSet::kNumOfCardTypes> fullCards{{
      Card{Color::kBlue,   (CardPattern)(CardNumber::kZero),             kBlueZeroKey,         kBlueZeroMaxQuantity        },
      Card{Color::kBlue,   (CardPattern)(CardNumber::kOne),              kBlueOneKey,          kBlueOneMaxQuantity         },
      Card{Color::kBlue,   (CardPattern)(CardNumber::kTwo),              kBlueTwoKey,          kBlueTwoMaxQuantity         },
      Card{Color::kBlue,   (CardPattern)(CardNumber::kThree),            kBlueThreeKey,        kBlueThreeMaxQuantity       },
      Card{Color::kBlue,   (CardPattern)(CardNumber::kFour),             kBlueFourKey,         kBlueFourMaxQuantity        },
      Card{Color::kBlue,   (CardPattern)(CardNumber::kFive),             kBlueFiveKey,         kBlueFiveMaxQuantity        },
      Card{Color::kBlue,   (CardPattern)(CardNumber::kSix),              kBlueSixKey,          kBlueSixMaxQuantity         },
      Card{Color::kBlue,   (CardPattern)(CardNumber::kSeven),            kBlueSevenKey,        kBlueSevenMaxQuantity       },
      Card{Color::kBlue,   (CardPattern)(CardNumber::kEight),            kBlueEightKey,        kBlueEightMaxQuantity       },
      Card{Color::kBlue,   (CardPattern)(CardNumber::kNine),             kBlueNineKey,         kBlueNineMaxQuantity        },
      Card{Color::kBlue,   (CardPattern)(CardAction::kDrawTwo),          kBlueDrawTwoKey,      kBlueDrawTwoMaxQuantity     },
      Card{Color::kBlue,   (CardPattern)(CardAction::kReverse),          kBlueReverseKey,      kBlueReverseMaxQuantity     },
      Card{Color::kBlue,   (CardPattern)(CardAction::kSkip),             kBlueSkipKey,         kBlueSkipMaxQuantity        },
      Card{Color::kGreen,  (CardPattern)(CardNumber::kZero),             kGreenZeroKey,        kGreenZeroMaxQuantity       },
      Card{Color::kGreen,  (CardPattern)(CardNumber::kOne),              kGreenOneKey,         kGreenOneMaxQuantity        },
      Card{Color::kGreen,  (CardPattern)(CardNumber::kTwo),              kGreenTwoKey,         kGreenTwoMaxQuantity        },
      Card{Color::kGreen,  (CardPattern)(CardNumber::kThree),            kGreenThreeKey,       kGreenThreeMaxQuantity      },
      Card{Color::kGreen,  (CardPattern)(CardNumber::kFour),             kGreenFourKey,        kGreenFourMaxQuantity       },
      Card{Color::kGreen,  (CardPattern)(CardNumber::kFive),             kGreenFiveKey,        kGreenFiveMaxQuantity       },
      Card{Color::kGreen,  (CardPattern)(CardNumber::kSix),              kGreenSixKey,         kGreenSixMaxQuantity        },
      Card{Color::kGreen,  (CardPattern)(CardNumber::kSeven),            kGreenSevenKey,       kGreenSevenMaxQuantity      },
      Card{Color::kGreen,  (CardPattern)(CardNumber::kEight),            kGreenEightKey,       kGreenEightMaxQuantity      },
      Card{Color::kGreen,  (CardPattern)(CardNumber::kNine),             kGreenNineKey,        kGreenNineMaxQuantity       },
      Card{Color::kGreen,  (CardPattern)(CardAction::kDrawTwo),          kGreenDrawTwoKey,     kGreenDrawTwoMaxQuantity    },
      Card{Color::kGreen,  (CardPattern)(CardAction::kReverse),          kGreenReverseKey,     kGreenReverseMaxQuantity    },
      Card{Color::kGreen,  (CardPattern)(CardAction::kSkip),             kGreenSkipKey,        kGreenSkipMaxQuantity       },
      Card{Color::kRed,    (CardPattern)(CardNumber::kZero),             kRedZeroKey,          kRedZeroMaxQuantity         },
      Card{Color::kRed,    (CardPattern)(CardNumber::kOne),              kRedOneKey,           kRedOneMaxQuantity          },
      Card{Color::kRed,    (CardPattern)(CardNumber::kTwo),              kRedTwoKey,           kRedTwoMaxQuantity          },
      Card{Color::kRed,    (CardPattern)(CardNumber::kThree),            kRedThreeKey,         kRedThreeMaxQuantity        },
      Card{Color::kRed,    (CardPattern)(CardNumber::kFour),             kRedFourKey,          kRedFourMaxQuantity         },
      Card{Color::kRed,    (CardPattern)(CardNumber::kFive),             kRedFiveKey,          kRedFiveMaxQuantity         },
      Card{Color::kRed,    (CardPattern)(CardNumber::kSix),              kRedSixKey,           kRedSixMaxQuantity          },
      Card{Color::kRed,    (CardPattern)(CardNumber::kSeven),            kRedSevenKey,         kRedSevenMaxQuantity        },
      Card{Color::kRed,    (CardPattern)(CardNumber::kEight),            kRedEightKey,         kRedEightMaxQuantity        },
      Card{Color::kRed,    (CardPattern)(CardNumber::kNine),             kRedNineKey,          kRedNineMaxQuantity         },
      Card{Color::kRed,    (CardPattern)(CardAction::kDrawTwo),          kRedDrawTwoKey,       kRedDrawTwoMaxQuantity      },
      Card{Color::kRed,    (CardPattern)(CardAction::kReverse),          kRedReverseKey,       kRedReverseMaxQuantity      },
      Card{Color::kRed,    (CardPattern)(CardAction::kSkip),             kRedSkipKey,          kRedSkipMaxQuantity         },
      Card{Color::kYellow, (CardPattern)(CardNumber::kZero),             kYellowZeroKey,       kYellowZeroMaxQuantity      },
      Card{Color::kYellow, (CardPattern)(CardNumber::kOne),              kYellowOneKey,        kYellowOneMaxQuantity       },
      Card{Color::kYellow, (CardPattern)(CardNumber::kTwo),              kYellowTwoKey,        kYellowTwoMaxQuantity       },
      Card{Color::kYellow, (CardPattern)(CardNumber::kThree),            kYellowThreeKey,      kYellowThreeMaxQuantity     },
      Card{Color::kYellow, (CardPattern)(CardNumber::kFour),             kYellowFourKey,       kYellowFourMaxQuantity      },
      Card{Color::kYellow, (CardPattern)(CardNumber::kFive),             kYellowFiveKey,       kYellowFiveMaxQuantity      },
      Card{Color::kYellow, (CardPattern)(CardNumber::kSix),              kYellowSixKey,        kYellowSixMaxQuantity       },
      Card{Color::kYellow, (CardPattern)(CardNumber::kSeven),            kYellowSevenKey,      kYellowSevenMaxQuantity     },
      Card{Color::kYellow, (CardPattern)(CardNumber::kEight),            kYellowEightKey,      kYellowEightMaxQuantity     },
      Card{Color::kYellow, (CardPattern)(CardNumber::kNine),             kYellowNineKey,       kYellowNineMaxQuantity      },
      Card{Color::kYellow, (CardPattern)(CardAction::kDrawTwo),          kYellowDrawTwoKey,    kYellowDrawTwoMaxQuantity   },
      Card{Color::kYellow, (CardPattern)(CardAction::kReverse),          kYellowReverseKey,    kYellowReverseMaxQuantity   },
      Card{Color::kYellow, (CardPattern)(CardAction::kSkip),             kYellowSkipKey,       kYellowSkipMaxQuantity      },
      Card{Color::kWild,   (CardPattern)(CardAction::kWild),             kWildKey,             kWildMaxQuantity            },
      Card{Color::kWild,   (CardPattern)(CardAction::kWildDraw4),        kWildDraw4Key,        kWildDraw4MaxQuantity       },
      Card{Color::kWild,   (CardPattern)(CardAction::kWildShuffleHands), kWildShuffleHandsKey, kWildShuffleHandsMaxQuantity},
      Card{Color::kWild,   (CardPattern)(CardAction::kWildCustomizable), kWildCustomizableKey, kWildCustomizableMaxQuantity}
  }};
};


#endif // CARD_HPP_

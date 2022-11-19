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

  static Cards allCards() {
    return {{
        Card{Color::kBlue,   (CardPattern)(CardNumber::kZero),            },
        Card{Color::kBlue,   (CardPattern)(CardNumber::kOne),             },
        Card{Color::kBlue,   (CardPattern)(CardNumber::kTwo),             },
        Card{Color::kBlue,   (CardPattern)(CardNumber::kThree),           },
        Card{Color::kBlue,   (CardPattern)(CardNumber::kFour),            },
        Card{Color::kBlue,   (CardPattern)(CardNumber::kFive),            },
        Card{Color::kBlue,   (CardPattern)(CardNumber::kSix),             },
        Card{Color::kBlue,   (CardPattern)(CardNumber::kSeven),           },
        Card{Color::kBlue,   (CardPattern)(CardNumber::kEight),           },
        Card{Color::kBlue,   (CardPattern)(CardNumber::kNine),            },
        Card{Color::kBlue,   (CardPattern)(CardAction::kDrawTwo),         },
        Card{Color::kBlue,   (CardPattern)(CardAction::kReverse),         },
        Card{Color::kBlue,   (CardPattern)(CardAction::kSkip),            },
        Card{Color::kGreen,  (CardPattern)(CardNumber::kZero),            },
        Card{Color::kGreen,  (CardPattern)(CardNumber::kOne),             },
        Card{Color::kGreen,  (CardPattern)(CardNumber::kTwo),             },
        Card{Color::kGreen,  (CardPattern)(CardNumber::kThree),           },
        Card{Color::kGreen,  (CardPattern)(CardNumber::kFour),            },
        Card{Color::kGreen,  (CardPattern)(CardNumber::kFive),            },
        Card{Color::kGreen,  (CardPattern)(CardNumber::kSix),             },
        Card{Color::kGreen,  (CardPattern)(CardNumber::kSeven),           },
        Card{Color::kGreen,  (CardPattern)(CardNumber::kEight),           },
        Card{Color::kGreen,  (CardPattern)(CardNumber::kNine),            },
        Card{Color::kGreen,  (CardPattern)(CardAction::kDrawTwo),         },
        Card{Color::kGreen,  (CardPattern)(CardAction::kReverse),         },
        Card{Color::kGreen,  (CardPattern)(CardAction::kSkip),            },
        Card{Color::kRed,    (CardPattern)(CardNumber::kZero),            },
        Card{Color::kRed,    (CardPattern)(CardNumber::kOne),             },
        Card{Color::kRed,    (CardPattern)(CardNumber::kTwo),             },
        Card{Color::kRed,    (CardPattern)(CardNumber::kThree),           },
        Card{Color::kRed,    (CardPattern)(CardNumber::kFour),            },
        Card{Color::kRed,    (CardPattern)(CardNumber::kFive),            },
        Card{Color::kRed,    (CardPattern)(CardNumber::kSix),             },
        Card{Color::kRed,    (CardPattern)(CardNumber::kSeven),           },
        Card{Color::kRed,    (CardPattern)(CardNumber::kEight),           },
        Card{Color::kRed,    (CardPattern)(CardNumber::kNine),            },
        Card{Color::kRed,    (CardPattern)(CardAction::kDrawTwo),         },
        Card{Color::kRed,    (CardPattern)(CardAction::kReverse),         },
        Card{Color::kRed,    (CardPattern)(CardAction::kSkip),            },
        Card{Color::kYellow, (CardPattern)(CardNumber::kZero),            },
        Card{Color::kYellow, (CardPattern)(CardNumber::kOne),             },
        Card{Color::kYellow, (CardPattern)(CardNumber::kTwo),             },
        Card{Color::kYellow, (CardPattern)(CardNumber::kThree),           },
        Card{Color::kYellow, (CardPattern)(CardNumber::kFour),            },
        Card{Color::kYellow, (CardPattern)(CardNumber::kFive),            },
        Card{Color::kYellow, (CardPattern)(CardNumber::kSix),             },
        Card{Color::kYellow, (CardPattern)(CardNumber::kSeven),           },
        Card{Color::kYellow, (CardPattern)(CardNumber::kEight),           },
        Card{Color::kYellow, (CardPattern)(CardNumber::kNine),            },
        Card{Color::kYellow, (CardPattern)(CardAction::kDrawTwo),         },
        Card{Color::kYellow, (CardPattern)(CardAction::kReverse),         },
        Card{Color::kYellow, (CardPattern)(CardAction::kSkip),            },
        Card{Color::kWild,   (CardPattern)(CardAction::kWild),            },
        Card{Color::kWild,   (CardPattern)(CardAction::kWildDraw4),       },
        Card{Color::kWild,   (CardPattern)(CardAction::kWildShuffleHands) },
        Card{Color::kWild,   (CardPattern)(CardAction::kWildCustomizable) }
    }};
  }

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

const Card Card::kBlueZero        {Color::kBlue,   (CardPattern)(CardNumber::kZero)             };
const Card Card::kBlueOne         {Color::kBlue,   (CardPattern)(CardNumber::kOne)              };
const Card Card::kBlueTwo         {Color::kBlue,   (CardPattern)(CardNumber::kTwo)              };
const Card Card::kBlueThree       {Color::kBlue,   (CardPattern)(CardNumber::kThree)            };
const Card Card::kBlueFour        {Color::kBlue,   (CardPattern)(CardNumber::kFour)             };
const Card Card::kBlueFive        {Color::kBlue,   (CardPattern)(CardNumber::kFive)             };
const Card Card::kBlueSix         {Color::kBlue,   (CardPattern)(CardNumber::kSix)              };
const Card Card::kBlueSeven       {Color::kBlue,   (CardPattern)(CardNumber::kSeven)            };
const Card Card::kBlueEight       {Color::kBlue,   (CardPattern)(CardNumber::kEight)            };
const Card Card::kBlueNine        {Color::kBlue,   (CardPattern)(CardNumber::kNine)             };
const Card Card::kBlueDrawTwo     {Color::kBlue,   (CardPattern)(CardAction::kDrawTwo)          };
const Card Card::kBlueReverse     {Color::kBlue,   (CardPattern)(CardAction::kReverse)          };
const Card Card::kBlueSkip        {Color::kBlue,   (CardPattern)(CardAction::kSkip)             };
const Card Card::kGreenZero       {Color::kGreen,  (CardPattern)(CardNumber::kZero)             };
const Card Card::kGreenOne        {Color::kGreen,  (CardPattern)(CardNumber::kOne)              };
const Card Card::kGreenTwo        {Color::kGreen,  (CardPattern)(CardNumber::kTwo)              };
const Card Card::kGreenThree      {Color::kGreen,  (CardPattern)(CardNumber::kThree)            };
const Card Card::kGreenFour       {Color::kGreen,  (CardPattern)(CardNumber::kFour)             };
const Card Card::kGreenFive       {Color::kGreen,  (CardPattern)(CardNumber::kFive)             };
const Card Card::kGreenSix        {Color::kGreen,  (CardPattern)(CardNumber::kSix)              };
const Card Card::kGreenSeven      {Color::kGreen,  (CardPattern)(CardNumber::kSeven)            };
const Card Card::kGreenEight      {Color::kGreen,  (CardPattern)(CardNumber::kEight)            };
const Card Card::kGreenNine       {Color::kGreen,  (CardPattern)(CardNumber::kNine)             };
const Card Card::kGreenDrawTwo    {Color::kGreen,  (CardPattern)(CardAction::kDrawTwo)          };
const Card Card::kGreenReverse    {Color::kGreen,  (CardPattern)(CardAction::kReverse)          };
const Card Card::kGreenSkip       {Color::kGreen,  (CardPattern)(CardAction::kSkip)             };
const Card Card::kRedZero         {Color::kRed,    (CardPattern)(CardNumber::kZero)             };
const Card Card::kRedOne          {Color::kRed,    (CardPattern)(CardNumber::kOne)              };
const Card Card::kRedTwo          {Color::kRed,    (CardPattern)(CardNumber::kTwo)              };
const Card Card::kRedThree        {Color::kRed,    (CardPattern)(CardNumber::kThree)            };
const Card Card::kRedFour         {Color::kRed,    (CardPattern)(CardNumber::kFour)             };
const Card Card::kRedFive         {Color::kRed,    (CardPattern)(CardNumber::kFive)             };
const Card Card::kRedSix          {Color::kRed,    (CardPattern)(CardNumber::kSix)              };
const Card Card::kRedSeven        {Color::kRed,    (CardPattern)(CardNumber::kSeven)            };
const Card Card::kRedEight        {Color::kRed,    (CardPattern)(CardNumber::kEight)            };
const Card Card::kRedNine         {Color::kRed,    (CardPattern)(CardNumber::kNine)             };
const Card Card::kRedDrawTwo      {Color::kRed,    (CardPattern)(CardAction::kDrawTwo)          };
const Card Card::kRedReverse      {Color::kRed,    (CardPattern)(CardAction::kReverse)          };
const Card Card::kRedSkip         {Color::kRed,    (CardPattern)(CardAction::kSkip)             };
const Card Card::kYellowZero      {Color::kYellow, (CardPattern)(CardNumber::kZero)             };
const Card Card::kYellowOne       {Color::kYellow, (CardPattern)(CardNumber::kOne)              };
const Card Card::kYellowTwo       {Color::kYellow, (CardPattern)(CardNumber::kTwo)              };
const Card Card::kYellowThree     {Color::kYellow, (CardPattern)(CardNumber::kThree)            };
const Card Card::kYellowFour      {Color::kYellow, (CardPattern)(CardNumber::kFour)             };
const Card Card::kYellowFive      {Color::kYellow, (CardPattern)(CardNumber::kFive)             };
const Card Card::kYellowSix       {Color::kYellow, (CardPattern)(CardNumber::kSix)              };
const Card Card::kYellowSeven     {Color::kYellow, (CardPattern)(CardNumber::kSeven)            };
const Card Card::kYellowEight     {Color::kYellow, (CardPattern)(CardNumber::kEight)            };
const Card Card::kYellowNine      {Color::kYellow, (CardPattern)(CardNumber::kNine)             };
const Card Card::kYellowDrawTwo   {Color::kYellow, (CardPattern)(CardAction::kDrawTwo)          };
const Card Card::kYellowReverse   {Color::kYellow, (CardPattern)(CardAction::kReverse)          };
const Card Card::kYellowSkip      {Color::kYellow, (CardPattern)(CardAction::kSkip)             };
const Card Card::kWild            {Color::kWild,   (CardPattern)(CardAction::kWild)             };
const Card Card::kWildDraw4       {Color::kWild,   (CardPattern)(CardAction::kWildDraw4)        };
const Card Card::kWildShuffleHands{Color::kWild,   (CardPattern)(CardAction::kWildShuffleHands) };
const Card Card::kWildCustomizable{Color::kWild,   (CardPattern)(CardAction::kWildCustomizable) };

#endif // CARD_HPP_

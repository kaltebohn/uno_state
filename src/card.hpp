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
    }};
  }

 private:
  Color color_{Color::kNull};
  CardPattern pattern_{};
};

using Cards = std::vector<Card>;

static constexpr int kNumOfCardTypes = 56;
static constexpr int kNumOfCards = 112;

#endif // CARD_HPP_

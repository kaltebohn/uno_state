#include "card.hpp"

std::string color2String(const Color& color) {
  switch (color) {
    case Color::kBlue:
      return "青";
    case Color::kGreen:
      return "緑";
    case Color::kRed:
      return "赤";
    case Color::kYellow:
      return "黄";
    case Color::kWild:
      return "ワイルド";
    case Color::kNull:
      return "空の色";
    default:
      return "不適切な色";
  }
}

std::string cardNumber2String(const CardNumber card_number) {
  switch (card_number) {
    case CardNumber::kZero:
      return "0";
    case CardNumber::kOne:
      return "1";
    case CardNumber::kTwo:
      return "2";
    case CardNumber::kThree:
      return "3";
    case CardNumber::kFour:
      return "4";
    case CardNumber::kFive:
      return "5";
    case CardNumber::kSix:
      return "6";
    case CardNumber::kSeven:
      return "7";
    case CardNumber::kEight:
      return "8";
    case CardNumber::kNine:
      return "9";
    default:
      return "不適切な数字";
  }
}

std::string cardAction2String(const CardAction card_action) {
  switch (card_action) {
    case CardAction::kDrawTwo:
      return "ドロー2";
    case CardAction::kReverse:
      return "リバース";
    case CardAction::kSkip:
      return "スキップ";
    case CardAction::kWild:
      return "ワイルド";
    case CardAction::kWildDraw4:
      return "ワイルドドロー4";
    case CardAction::kWildShuffleHands:
      return "シャッフルワイルド";
    case CardAction::kWildCustomizable:
      return "白いワイルド";
    default:
      return "不適切な記号";
  }
}

std::string cardPattern2String(const CardPattern card_pattern) {
  if (card_pattern.valueless_by_exception()) {
    return "空の模様";
  } else if (std::holds_alternative<CardNumber>(card_pattern)) {
    return cardNumber2String(std::get<CardNumber>(card_pattern));
  } else if (std::holds_alternative<CardAction>(card_pattern)) {
    return cardAction2String(std::get<CardAction>(card_pattern));
  } else {
    return "不適切な模様";
  }
}

const Card Card::kBlueZero         = Card(Color::kBlue,   (CardPattern)(CardNumber::kZero));
const Card Card::kBlueOne          = Card(Color::kBlue,   (CardPattern)(CardNumber::kOne));
const Card Card::kBlueTwo          = Card(Color::kBlue,   (CardPattern)(CardNumber::kTwo));
const Card Card::kBlueThree        = Card(Color::kBlue,   (CardPattern)(CardNumber::kThree));
const Card Card::kBlueFour         = Card(Color::kBlue,   (CardPattern)(CardNumber::kFour));
const Card Card::kBlueFive         = Card(Color::kBlue,   (CardPattern)(CardNumber::kFive));
const Card Card::kBlueSix          = Card(Color::kBlue,   (CardPattern)(CardNumber::kSix));
const Card Card::kBlueSeven        = Card(Color::kBlue,   (CardPattern)(CardNumber::kSeven));
const Card Card::kBlueEight        = Card(Color::kBlue,   (CardPattern)(CardNumber::kEight));
const Card Card::kBlueNine         = Card(Color::kBlue,   (CardPattern)(CardNumber::kNine));
const Card Card::kBlueDrawTwo      = Card(Color::kBlue,   (CardPattern)(CardAction::kDrawTwo));
const Card Card::kBlueReverse      = Card(Color::kBlue,   (CardPattern)(CardAction::kReverse));
const Card Card::kBlueSkip         = Card(Color::kBlue,   (CardPattern)(CardAction::kSkip));
const Card Card::kGreenZero        = Card(Color::kGreen,  (CardPattern)(CardNumber::kZero));
const Card Card::kGreenOne         = Card(Color::kGreen,  (CardPattern)(CardNumber::kOne));
const Card Card::kGreenTwo         = Card(Color::kGreen,  (CardPattern)(CardNumber::kTwo));
const Card Card::kGreenThree       = Card(Color::kGreen,  (CardPattern)(CardNumber::kThree));
const Card Card::kGreenFour        = Card(Color::kGreen,  (CardPattern)(CardNumber::kFour));
const Card Card::kGreenFive        = Card(Color::kGreen,  (CardPattern)(CardNumber::kFive));
const Card Card::kGreenSix         = Card(Color::kGreen,  (CardPattern)(CardNumber::kSix));
const Card Card::kGreenSeven       = Card(Color::kGreen,  (CardPattern)(CardNumber::kSeven));
const Card Card::kGreenEight       = Card(Color::kGreen,  (CardPattern)(CardNumber::kEight));
const Card Card::kGreenNine        = Card(Color::kGreen,  (CardPattern)(CardNumber::kNine));
const Card Card::kGreenDrawTwo     = Card(Color::kGreen,  (CardPattern)(CardAction::kDrawTwo));
const Card Card::kGreenReverse     = Card(Color::kGreen,  (CardPattern)(CardAction::kReverse));
const Card Card::kGreenSkip        = Card(Color::kGreen,  (CardPattern)(CardAction::kSkip));
const Card Card::kRedZero          = Card(Color::kRed,    (CardPattern)(CardNumber::kZero));
const Card Card::kRedOne           = Card(Color::kRed,    (CardPattern)(CardNumber::kOne));
const Card Card::kRedTwo           = Card(Color::kRed,    (CardPattern)(CardNumber::kTwo));
const Card Card::kRedThree         = Card(Color::kRed,    (CardPattern)(CardNumber::kThree));
const Card Card::kRedFour          = Card(Color::kRed,    (CardPattern)(CardNumber::kFour));
const Card Card::kRedFive          = Card(Color::kRed,    (CardPattern)(CardNumber::kFive));
const Card Card::kRedSix           = Card(Color::kRed,    (CardPattern)(CardNumber::kSix));
const Card Card::kRedSeven         = Card(Color::kRed,    (CardPattern)(CardNumber::kSeven));
const Card Card::kRedEight         = Card(Color::kRed,    (CardPattern)(CardNumber::kEight));
const Card Card::kRedNine          = Card(Color::kRed,    (CardPattern)(CardNumber::kNine));
const Card Card::kRedDrawTwo       = Card(Color::kRed,    (CardPattern)(CardAction::kDrawTwo));
const Card Card::kRedReverse       = Card(Color::kRed,    (CardPattern)(CardAction::kReverse));
const Card Card::kRedSkip          = Card(Color::kRed,    (CardPattern)(CardAction::kSkip));
const Card Card::kYellowZero       = Card(Color::kYellow, (CardPattern)(CardNumber::kZero));
const Card Card::kYellowOne        = Card(Color::kYellow, (CardPattern)(CardNumber::kOne));
const Card Card::kYellowTwo        = Card(Color::kYellow, (CardPattern)(CardNumber::kTwo));
const Card Card::kYellowThree      = Card(Color::kYellow, (CardPattern)(CardNumber::kThree));
const Card Card::kYellowFour       = Card(Color::kYellow, (CardPattern)(CardNumber::kFour));
const Card Card::kYellowFive       = Card(Color::kYellow, (CardPattern)(CardNumber::kFive));
const Card Card::kYellowSix        = Card(Color::kYellow, (CardPattern)(CardNumber::kSix));
const Card Card::kYellowSeven      = Card(Color::kYellow, (CardPattern)(CardNumber::kSeven));
const Card Card::kYellowEight      = Card(Color::kYellow, (CardPattern)(CardNumber::kEight));
const Card Card::kYellowNine       = Card(Color::kYellow, (CardPattern)(CardNumber::kNine));
const Card Card::kYellowDrawTwo    = Card(Color::kYellow, (CardPattern)(CardAction::kDrawTwo));
const Card Card::kYellowReverse    = Card(Color::kYellow, (CardPattern)(CardAction::kReverse));
const Card Card::kYellowSkip       = Card(Color::kYellow, (CardPattern)(CardAction::kSkip));
const Card Card::kWild             = Card(Color::kWild,   (CardPattern)(CardAction::kWild));
const Card Card::kWildDraw4        = Card(Color::kWild,   (CardPattern)(CardAction::kWildDraw4));
const Card Card::kWildShuffleHands = Card(Color::kWild,   (CardPattern)(CardAction::kWildShuffleHands));
const Card Card::kWildCustomizable = Card(Color::kWild,   (CardPattern)(CardAction::kWildCustomizable));

Cards allCards() {
  return {{
      Card::kBlueZero,
      Card::kBlueOne,
      Card::kBlueTwo,
      Card::kBlueThree,
      Card::kBlueFour,
      Card::kBlueFive,
      Card::kBlueSix,
      Card::kBlueSeven,
      Card::kBlueEight,
      Card::kBlueNine,
      Card::kBlueDrawTwo,
      Card::kBlueReverse,
      Card::kBlueSkip,
      Card::kGreenZero,
      Card::kGreenOne,
      Card::kGreenTwo,
      Card::kGreenThree,
      Card::kGreenFour,
      Card::kGreenFive,
      Card::kGreenSix,
      Card::kGreenSeven,
      Card::kGreenEight,
      Card::kGreenNine,
      Card::kGreenDrawTwo,
      Card::kGreenReverse,
      Card::kGreenSkip,
      Card::kRedZero,
      Card::kRedOne,
      Card::kRedTwo,
      Card::kRedThree,
      Card::kRedFour,
      Card::kRedFive,
      Card::kRedSix,
      Card::kRedSeven,
      Card::kRedEight,
      Card::kRedNine,
      Card::kRedDrawTwo,
      Card::kRedReverse,
      Card::kRedSkip,
      Card::kYellowZero,
      Card::kYellowOne,
      Card::kYellowTwo,
      Card::kYellowThree,
      Card::kYellowFour,
      Card::kYellowFive,
      Card::kYellowSix,
      Card::kYellowSeven,
      Card::kYellowEight,
      Card::kYellowNine,
      Card::kYellowDrawTwo,
      Card::kYellowReverse,
      Card::kYellowSkip,
      Card::kWild,
      Card::kWildDraw4,
      Card::kWildShuffleHands,
      Card::kWildCustomizable
  }};
}

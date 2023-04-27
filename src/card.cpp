#include "card.hpp"

std::string color2String(const Color& color) {
  switch (color) {
    case Color::kBlue:
      return "B";
    case Color::kGreen:
      return "G";
    case Color::kRed:
      return "R";
    case Color::kYellow:
      return "Y";
    case Color::kWild:
      return "W";
    case Color::kNull:
      return "Empty";
    default:
      assert(false);
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
      assert(false);
  }
}

int cardNumber2Int(const CardNumber card_number) {
  switch (card_number) {
    case CardNumber::kZero:
      return 0;
    case CardNumber::kOne:
      return 1;
    case CardNumber::kTwo:
      return 2;
    case CardNumber::kThree:
      return 3;
    case CardNumber::kFour:
      return 4;
    case CardNumber::kFive:
      return 5;
    case CardNumber::kSix:
      return 6;
    case CardNumber::kSeven:
      return 7;
    case CardNumber::kEight:
      return 8;
    case CardNumber::kNine:
      return 9;
    default:
      assert(false);
  }
}

std::string cardAction2String(const CardAction card_action) {
  switch (card_action) {
    case CardAction::kDrawTwo:
      return "DrawTwo";
    case CardAction::kReverse:
      return "Reverse";
    case CardAction::kSkip:
      return "Skip";
    case CardAction::kWild:
      return "Wild";
    case CardAction::kWildDraw4:
      return "WildDraw4";
    case CardAction::kWildShuffleHands:
      return "WildShuffleHands";
    case CardAction::kWildCustomizable:
      return "WildCustomizable";
    default:
      assert(false);
  }
}

std::string cardPattern2String(const CardPattern card_pattern) {
  if (std::holds_alternative<std::monostate>(card_pattern)) {
    return "Empty";
  } else if (std::holds_alternative<CardNumber>(card_pattern)) {
    return cardNumber2String(std::get<CardNumber>(card_pattern));
  } else if (std::holds_alternative<CardAction>(card_pattern)) {
    return cardAction2String(std::get<CardAction>(card_pattern));
  }
  assert(false);
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

std::string Card::toString() const {
  if (isEmpty()) { return "空"; }

  if (*this == kBlueZero) {
    return "B-0";
  } else if (*this == kBlueOne) {
    return "B-1";
  } else if (*this == kBlueTwo) {
    return "B-2";
  } else if (*this == kBlueThree) {
    return "B-3";
  } else if (*this == kBlueFour) {
    return "B-4";
  } else if (*this == kBlueFive) {
    return "B-5";
  } else if (*this == kBlueSix) {
    return "B-6";
  } else if (*this == kBlueSeven) {
    return "B-7";
  } else if (*this == kBlueEight) {
    return "B-8";
  } else if (*this == kBlueNine) {
    return "B-9";
  } else if (*this == kBlueDrawTwo) {
    return "B-DrawTwo";
  } else if (*this == kBlueReverse) {
    return "B-Reverse";
  } else if (*this == kBlueSkip) {
    return "B-Skip";
  } else if (*this == kGreenZero) {
    return "G-0";
  } else if (*this == kGreenOne) {
    return "G-1";
  } else if (*this == kGreenTwo) {
    return "G-2";
  } else if (*this == kGreenThree) {
    return "G-3";
  } else if (*this == kGreenFour) {
    return "G-4";
  } else if (*this == kGreenFive) {
    return "G-5";
  } else if (*this == kGreenSix) {
    return "G-6";
  } else if (*this == kGreenSeven) {
    return "G-7";
  } else if (*this == kGreenEight) {
    return "G-8";
  } else if (*this == kGreenNine) {
    return "G-9";
  } else if (*this == kGreenDrawTwo) {
    return "G-DrawTwo";
  } else if (*this == kGreenReverse) {
    return "G-Reverse";
  } else if (*this == kGreenSkip) {
    return "G-Skip";
  } else if (*this == kRedZero) {
    return "R-0";
  } else if (*this == kRedOne) {
    return "R-1";
  } else if (*this == kRedTwo) {
    return "R-2";
  } else if (*this == kRedThree) {
    return "R-3";
  } else if (*this == kRedFour) {
    return "R-4";
  } else if (*this == kRedFive) {
    return "R-5";
  } else if (*this == kRedSix) {
    return "R-6";
  } else if (*this == kRedSeven) {
    return "R-7";
  } else if (*this == kRedEight) {
    return "R-8";
  } else if (*this == kRedNine) {
    return "R-9";
  } else if (*this == kRedDrawTwo) {
    return "R-DrawTwo";
  } else if (*this == kRedReverse) {
    return "R-Reverse";
  } else if (*this == kRedSkip) {
    return "R-Skip";
  } else if (*this == kYellowZero) {
    return "Y-0";
  } else if (*this == kYellowOne) {
    return "Y-1";
  } else if (*this == kYellowTwo) {
    return "Y-2";
  } else if (*this == kYellowThree) {
    return "Y-3";
  } else if (*this == kYellowFour) {
    return "Y-4";
  } else if (*this == kYellowFive) {
    return "Y-5";
  } else if (*this == kYellowSix) {
    return "Y-6";
  } else if (*this == kYellowSeven) {
    return "Y-7";
  } else if (*this == kYellowEight) {
    return "Y-8";
  } else if (*this == kYellowNine) {
    return "Y-9";
  } else if (*this == kYellowDrawTwo) {
    return "Y-DrawTwo";
  } else if (*this == kYellowReverse) {
    return "Y-Reverse";
  } else if (*this == kYellowSkip) {
    return "Y-Skip";
  } else if (*this == kWild) {
    return "W-Wild";
  } else if (*this == kWildDraw4) {
    return "W-WildDraw4";
  } else if (*this == kWildShuffleHands) {
    return "W-WildShuffleHands";
  } else if (*this == kWildCustomizable) {
    return "W-WildCustomizable";
  }
  assert(false);
}

int Card::toScore() const {
  if (isEmpty()) {
    return 0;
  } else if (std::holds_alternative<CardNumber>(getPattern())) {
    return cardNumber2Int(std::get<CardNumber>(getPattern()));
  } else if (getColor() != Color::kWild) {
    return UnoConsts::kScoreOfAction;
  } else if (*this == Card::kWild) {
    return UnoConsts::kScoreOfWild;
  } else if (*this == Card::kWildDraw4) {
    return UnoConsts::kScoreOfWildDraw4;
  } else if (*this == Card::kWildShuffleHands) {
    return UnoConsts::kScoreOfWildShuffleHands;
  } else if (*this == Card::kWildCustomizable) {
    return UnoConsts::kScoreOfWildCustomizable;
  }
  assert(false);
}

Cards allCards() {
  return {{
      Card::kBlueZero,
      Card::kBlueOne,
      Card::kBlueOne,
      Card::kBlueTwo,
      Card::kBlueTwo,
      Card::kBlueThree,
      Card::kBlueThree,
      Card::kBlueFour,
      Card::kBlueFour,
      Card::kBlueFive,
      Card::kBlueFive,
      Card::kBlueSix,
      Card::kBlueSix,
      Card::kBlueSeven,
      Card::kBlueSeven,
      Card::kBlueEight,
      Card::kBlueEight,
      Card::kBlueNine,
      Card::kBlueNine,
      Card::kBlueDrawTwo,
      Card::kBlueDrawTwo,
      Card::kBlueReverse,
      Card::kBlueReverse,
      Card::kBlueSkip,
      Card::kBlueSkip,
      Card::kGreenZero,
      Card::kGreenOne,
      Card::kGreenOne,
      Card::kGreenTwo,
      Card::kGreenTwo,
      Card::kGreenThree,
      Card::kGreenThree,
      Card::kGreenFour,
      Card::kGreenFour,
      Card::kGreenFive,
      Card::kGreenFive,
      Card::kGreenSix,
      Card::kGreenSix,
      Card::kGreenSeven,
      Card::kGreenSeven,
      Card::kGreenEight,
      Card::kGreenEight,
      Card::kGreenNine,
      Card::kGreenNine,
      Card::kGreenDrawTwo,
      Card::kGreenDrawTwo,
      Card::kGreenReverse,
      Card::kGreenReverse,
      Card::kGreenSkip,
      Card::kGreenSkip,
      Card::kRedZero,
      Card::kRedOne,
      Card::kRedOne,
      Card::kRedTwo,
      Card::kRedTwo,
      Card::kRedThree,
      Card::kRedThree,
      Card::kRedFour,
      Card::kRedFour,
      Card::kRedFive,
      Card::kRedFive,
      Card::kRedSix,
      Card::kRedSix,
      Card::kRedSeven,
      Card::kRedSeven,
      Card::kRedEight,
      Card::kRedEight,
      Card::kRedNine,
      Card::kRedNine,
      Card::kRedDrawTwo,
      Card::kRedDrawTwo,
      Card::kRedReverse,
      Card::kRedReverse,
      Card::kRedSkip,
      Card::kRedSkip,
      Card::kYellowZero,
      Card::kYellowOne,
      Card::kYellowOne,
      Card::kYellowTwo,
      Card::kYellowTwo,
      Card::kYellowThree,
      Card::kYellowThree,
      Card::kYellowFour,
      Card::kYellowFour,
      Card::kYellowFive,
      Card::kYellowFive,
      Card::kYellowSix,
      Card::kYellowSix,
      Card::kYellowSeven,
      Card::kYellowSeven,
      Card::kYellowEight,
      Card::kYellowEight,
      Card::kYellowNine,
      Card::kYellowNine,
      Card::kYellowDrawTwo,
      Card::kYellowDrawTwo,
      Card::kYellowReverse,
      Card::kYellowReverse,
      Card::kYellowSkip,
      Card::kYellowSkip,
      Card::kWild,
      Card::kWild,
      Card::kWild,
      Card::kWild,
      Card::kWildDraw4,
      Card::kWildDraw4,
      Card::kWildDraw4,
      Card::kWildDraw4,
      Card::kWildShuffleHands,
      // Card::kWildCustomizable,
      // Card::kWildCustomizable,
      // Card::kWildCustomizable
  }};
}

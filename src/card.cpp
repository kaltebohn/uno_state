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
      assert(false);
  }
}

std::string cardPattern2String(const CardPattern card_pattern) {
  if (std::holds_alternative<std::monostate>(card_pattern)) {
    return "空の模様";
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
    return "青0";
  } else if (*this == kBlueOne) {
    return "青1";
  } else if (*this == kBlueTwo) {
    return "青2";
  } else if (*this == kBlueThree) {
    return "青3";
  } else if (*this == kBlueFour) {
    return "青4";
  } else if (*this == kBlueFive) {
    return "青5";
  } else if (*this == kBlueSix) {
    return "青6";
  } else if (*this == kBlueSeven) {
    return "青7";
  } else if (*this == kBlueEight) {
    return "青8";
  } else if (*this == kBlueNine) {
    return "青9";
  } else if (*this == kBlueDrawTwo) {
    return "青ドロー2";
  } else if (*this == kBlueReverse) {
    return "青リバース";
  } else if (*this == kBlueSkip) {
    return "青スキップ";
  } else if (*this == kGreenZero) {
    return "緑0";
  } else if (*this == kGreenOne) {
    return "緑1";
  } else if (*this == kGreenTwo) {
    return "緑2";
  } else if (*this == kGreenThree) {
    return "緑3";
  } else if (*this == kGreenFour) {
    return "緑4";
  } else if (*this == kGreenFive) {
    return "緑5";
  } else if (*this == kGreenSix) {
    return "緑6";
  } else if (*this == kGreenSeven) {
    return "緑7";
  } else if (*this == kGreenEight) {
    return "緑8";
  } else if (*this == kGreenNine) {
    return "緑9";
  } else if (*this == kGreenDrawTwo) {
    return "緑ドロー2";
  } else if (*this == kGreenReverse) {
    return "緑リバース";
  } else if (*this == kGreenSkip) {
    return "緑スキップ";
  } else if (*this == kRedZero) {
    return "赤0";
  } else if (*this == kRedOne) {
    return "赤1";
  } else if (*this == kRedTwo) {
    return "赤2";
  } else if (*this == kRedThree) {
    return "赤3";
  } else if (*this == kRedFour) {
    return "赤4";
  } else if (*this == kRedFive) {
    return "赤5";
  } else if (*this == kRedSix) {
    return "赤6";
  } else if (*this == kRedSeven) {
    return "赤7";
  } else if (*this == kRedEight) {
    return "赤8";
  } else if (*this == kRedNine) {
    return "赤9";
  } else if (*this == kRedDrawTwo) {
    return "赤ドロー2";
  } else if (*this == kRedReverse) {
    return "赤リバース";
  } else if (*this == kRedSkip) {
    return "赤スキップ";
  } else if (*this == kYellowZero) {
    return "黄0";
  } else if (*this == kYellowOne) {
    return "黄1";
  } else if (*this == kYellowTwo) {
    return "黄2";
  } else if (*this == kYellowThree) {
    return "黄3";
  } else if (*this == kYellowFour) {
    return "黄4";
  } else if (*this == kYellowFive) {
    return "黄5";
  } else if (*this == kYellowSix) {
    return "黄6";
  } else if (*this == kYellowSeven) {
    return "黄7";
  } else if (*this == kYellowEight) {
    return "黄8";
  } else if (*this == kYellowNine) {
    return "黄9";
  } else if (*this == kYellowDrawTwo) {
    return "黄ドロー2";
  } else if (*this == kYellowReverse) {
    return "黄リバース";
  } else if (*this == kYellowSkip) {
    return "黄スキップ";
  } else if (*this == kWild) {
    return "ワイルド";
  } else if (*this == kWildDraw4) {
    return "ワイルドドロー4";
  } else if (*this == kWildShuffleHands) {
    return "シャッフルワイルド";
  } else if (*this == kWildCustomizable) {
    return "白いワイルド";
  }
  assert(false);
}

int Card::toScore() const {
  if (std::holds_alternative<CardNumber>(getPattern())) {
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
      Card::kWildCustomizable,
      Card::kWildCustomizable,
      Card::kWildCustomizable
  }};
}

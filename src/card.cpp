#include "card.hpp"

int Card::card2Key(Card c) {
  if (c.isEmpty()) { return CardSet::kNullKey; }

  int index = 0;
  switch (c.color_) {
    case Color::kBlue:
      index += CardSet::kBlueZeroKey;
      break;
    case Color::kGreen:
      index += CardSet::kGreenZeroKey;
      break;
    case Color::kRed:
      index += CardSet::kRedZeroKey;
      break;
    case Color::kYellow:
      index += CardSet::kYellowZeroKey;
      break;
    case Color::kWild:
      if (c.pattern_ == (CardPattern)(CardAction::kWild)) {
        return CardSet::kWildKey;
      } else if (c.pattern_ == (CardPattern)(CardAction::kWildDraw4)) {
        return CardSet::kWildDraw4Key;
      } else if (c.pattern_ == (CardPattern)(CardAction::kWildShuffleHands)) {
        return CardSet::kWildShuffleHandsKey;
      } else if (c.pattern_ == (CardPattern)(CardAction::kWildCustomizable)) {
        return CardSet::kWildCustomizableKey;
      }
      break;
    default:
      break;
  }

  if (std::holds_alternative<CardNumber>(c.pattern_)) {
    switch (std::get<CardNumber>(c.pattern_)) {
      case CardNumber::kZero:
        return index + 0;
      case CardNumber::kOne:
        return index + 1;
      case CardNumber::kTwo:
        return index + 2;
      case CardNumber::kThree:
        return index + 3;
      case CardNumber::kFour:
        return index + 4;
      case CardNumber::kFive:
        return index + 5;
      case CardNumber::kSix:
        return index + 6;
      case CardNumber::kSeven:
        return index + 7;
      case CardNumber::kEight:
        return index + 8;
      case CardNumber::kNine:
        return index + 9;
        break;
    }
  } else if (std::holds_alternative<CardAction>(c.pattern_)) {
    switch (std::get<CardAction>(c.pattern_)) {
      case CardAction::kDrawTwo:
        return index + 10;
      case CardAction::kReverse:
        return index + 11;
      case CardAction::kSkip:
        return index + 12;
      default:
        break;
    }
  }

  return CardSet::kNullKey;
}

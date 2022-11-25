#ifndef CARD_HPP_
#define CARD_HPP_

#include <iostream>
#include <string>
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

std::string color2String(const Color& color);

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

std::string cardNumber2String(const CardNumber card_number);

enum class CardAction {
  kDrawTwo,
  kReverse,
  kSkip,
  kWild,
  kWildDraw4,
  kWildShuffleHands,
  kWildCustomizable
};

std::string cardAction2String(const CardAction card_action);

using CardPattern = std::variant<std::monostate, CardNumber, CardAction>;

std::string cardPattern2String(const CardPattern card_pattern);

/* インスタンス1つで1種類のカードを表す。デフォルトで空のカードを表現する(出せるカードがない場合に合法手として使う)。 */
class Card {
 public:
  friend std::ostream& operator<<(std::ostream& os, const Card& src) {
    os << src.toString();
    return os;
  }

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

  std::string toString() const {
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
    } else {
      // TODO: 色と模様を出力できるようにする。
      return "予期されていないカード";
    }
  }

 private:
  Color color_{Color::kNull};
  CardPattern pattern_{};
};

using Cards = std::vector<Card>;

static constexpr int kNumOfCardTypes = 56;
static constexpr int kNumOfCards = 112;

Cards allCards();

#endif // CARD_HPP_

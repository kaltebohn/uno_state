#include <gtest/gtest.h>

#include "../../src/card.hpp"

TEST(AllCardsTest, NumOfAllCards) {
  // EXPECT_EQ(allCards().size(), 112);
  EXPECT_EQ(allCards().size(), 109);
}

TEST(GetColorTest, NormalColor) {
  Card card1{Color::kBlue, CardNumber::kZero};
  EXPECT_EQ(card1.getColor(), Color::kBlue);

  Card card2{Color::kGreen, CardNumber::kZero};
  EXPECT_EQ(card2.getColor(), Color::kGreen);

  Card card3{Color::kRed, CardNumber::kZero};
  EXPECT_EQ(card3.getColor(), Color::kRed);

  Card card4{Color::kYellow, CardNumber::kZero};
  EXPECT_EQ(card4.getColor(), Color::kYellow);
}

TEST(GetColorTest, Null) {
  Card card{};
  EXPECT_EQ(card.getColor(), Color::kNull);
}

TEST(GetColorTest, Wild) {
  Card card{Color::kWild, CardAction::kWild};
  EXPECT_EQ(card.getColor(), Color::kWild);
}

TEST(GetPatternTest, Number) {
  Card card1{Color::kBlue, CardNumber::kZero};
  EXPECT_EQ(std::get<CardNumber>(card1.getPattern()), CardNumber::kZero);

  Card card2{Color::kBlue, CardNumber::kOne};
  EXPECT_EQ(std::get<CardNumber>(card2.getPattern()), CardNumber::kOne);

  Card card3{Color::kBlue, CardNumber::kTwo};
  EXPECT_EQ(std::get<CardNumber>(card3.getPattern()), CardNumber::kTwo);

  Card card4{Color::kBlue, CardNumber::kThree};
  EXPECT_EQ(std::get<CardNumber>(card4.getPattern()), CardNumber::kThree);

  Card card5{Color::kBlue, CardNumber::kFour};
  EXPECT_EQ(std::get<CardNumber>(card5.getPattern()), CardNumber::kFour);

  Card card6{Color::kBlue, CardNumber::kFive};
  EXPECT_EQ(std::get<CardNumber>(card6.getPattern()), CardNumber::kFive);

  Card card7{Color::kBlue, CardNumber::kSix};
  EXPECT_EQ(std::get<CardNumber>(card7.getPattern()), CardNumber::kSix);

  Card card8{Color::kBlue, CardNumber::kSeven};
  EXPECT_EQ(std::get<CardNumber>(card8.getPattern()), CardNumber::kSeven);

  Card card9{Color::kBlue, CardNumber::kEight};
  EXPECT_EQ(std::get<CardNumber>(card9.getPattern()), CardNumber::kEight);

  Card card10{Color::kBlue, CardNumber::kNine};
  EXPECT_EQ(std::get<CardNumber>(card10.getPattern()), CardNumber::kNine);
}

TEST(GetPatternTest, Action) {
  Card card1{Color::kBlue, CardAction::kDrawTwo};
  EXPECT_EQ(std::get<CardAction>(card1.getPattern()), CardAction::kDrawTwo);

  Card card2{Color::kBlue, CardAction::kReverse};
  EXPECT_EQ(std::get<CardAction>(card2.getPattern()), CardAction::kReverse);

  Card card3{Color::kBlue, CardAction::kSkip};
  EXPECT_EQ(std::get<CardAction>(card3.getPattern()), CardAction::kSkip);

  Card card4{Color::kWild, CardAction::kWild};
  EXPECT_EQ(std::get<CardAction>(card4.getPattern()), CardAction::kWild);

  Card card5{Color::kWild, CardAction::kWildDraw4};
  EXPECT_EQ(std::get<CardAction>(card5.getPattern()), CardAction::kWildDraw4);

  Card card6{Color::kWild, CardAction::kWildShuffleHands};
  EXPECT_EQ(std::get<CardAction>(card6.getPattern()), CardAction::kWildShuffleHands);

  Card card7{Color::kWild, CardAction::kWildCustomizable};
  EXPECT_EQ(std::get<CardAction>(card7.getPattern()), CardAction::kWildCustomizable);
}

TEST(GetPatternTest, Null) {
  Card card{};
  EXPECT_TRUE(std::holds_alternative<std::monostate>(card.getPattern()));
}

TEST(IsEmptyTest, True) {
  Card card{};
  EXPECT_EQ(card.isEmpty(), true);
}

TEST(IsEmptyTest, False) {
  Card card{Color::kBlue, CardNumber::kZero};
  EXPECT_EQ(card.isEmpty(), false);
}

TEST(EqualTest, True) {
  Card card1{Color::kBlue, CardNumber::kZero};
  Card card2{Color::kBlue, CardNumber::kZero};
  EXPECT_TRUE(card1 == card2);

  Card card3{Color::kBlue, CardAction::kDrawTwo};
  Card card4{Color::kBlue, CardAction::kDrawTwo};
  EXPECT_TRUE(card3 == card4);
}

TEST(EqualTest, False) {
  Card card1{Color::kBlue, CardNumber::kZero};
  Card card2{Color::kGreen, CardNumber::kZero};
  EXPECT_FALSE(card1 == card2);

  Card card3{Color::kBlue, CardAction::kDrawTwo};
  Card card4{Color::kBlue, CardAction::kReverse};
  EXPECT_FALSE(card3 == card4);

  Card card5{Color::kBlue, CardNumber::kZero};
  Card card6{Color::kBlue, CardAction::kReverse};
  EXPECT_FALSE(card5 == card6);
}

TEST(NotEqualTest, True) {
  Card card1{Color::kBlue, CardNumber::kZero};
  Card card2{Color::kGreen, CardNumber::kZero};
  EXPECT_TRUE(card1 != card2);

  Card card3{Color::kBlue, CardAction::kDrawTwo};
  Card card4{Color::kBlue, CardAction::kReverse};
  EXPECT_TRUE(card3 != card4);

  Card card5{Color::kBlue, CardNumber::kZero};
  Card card6{Color::kBlue, CardAction::kReverse};
  EXPECT_TRUE(card5 != card6);
}

TEST(NotEqualTest, False) {
  Card card1{Color::kBlue, CardNumber::kZero};
  Card card2{Color::kBlue, CardNumber::kZero};
  EXPECT_FALSE(card1 != card2);

  Card card3{Color::kBlue, CardAction::kDrawTwo};
  Card card4{Color::kBlue, CardAction::kDrawTwo};
  EXPECT_FALSE(card3 != card4);
}

TEST(IsLegalTest, True) {
  Card submission1{};
  EXPECT_TRUE(submission1.isLegal(Color::kBlue, CardNumber::kZero));

  Card submission2{Color::kWild, CardAction::kWild};
  EXPECT_TRUE(submission2.isLegal(Color::kBlue, CardNumber::kZero));

  Card submission3{Color::kBlue, CardNumber::kZero};
  EXPECT_TRUE(submission3.isLegal(Color::kBlue, CardNumber::kZero));

  Card submission4{Color::kGreen, CardNumber::kZero};
  EXPECT_TRUE(submission4.isLegal(Color::kBlue, CardNumber::kZero));

  Card submission5{Color::kBlue, CardNumber::kZero};
  EXPECT_TRUE(submission5.isLegal(Color::kBlue, CardNumber::kOne));

  Card submission6{Color::kBlue, CardNumber::kZero};
  EXPECT_TRUE(submission6.isLegal(Color::kBlue, CardAction::kDrawTwo));
}

TEST(IsLegalTest, False) {
  Card submission1{Color::kBlue, CardNumber::kZero};
  EXPECT_FALSE(submission1.isLegal(Color::kGreen, CardNumber::kOne));

  Card submission2{Color::kBlue, CardAction::kDrawTwo};
  EXPECT_FALSE(submission2.isLegal(Color::kGreen, CardAction::kReverse));

  Card submission3{Color::kBlue, CardNumber::kZero};
  EXPECT_FALSE(submission3.isLegal(Color::kGreen, CardAction::kReverse));
}

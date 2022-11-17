#include <gtest/gtest.h>

#include "../../src/submission.hpp"

TEST(GetCardTest, Normal) {
  Card card{Color::kBlue, CardNumber::kZero};
  Submission submission{card, false};
  EXPECT_EQ(submission.getCard(), card);
}

TEST(GetShouldYellUNOTest, True) {
  Submission submission{{Color::kBlue, CardNumber::kZero}, true};
  EXPECT_TRUE(submission.getShouldYellUNO());
}

TEST(GetShouldYellUNOTest, False) {
  Submission submission{{Color::kBlue, CardNumber::kZero}, false};
  EXPECT_FALSE(submission.getShouldYellUNO());
}

TEST(IsLegalTest, True) {
  Submission submission1{{}, false};
  EXPECT_TRUE(submission1.isLegal(Color::kBlue, CardNumber::kZero));

  Submission submission2{{Color::kWild, CardAction::kWild}, false};
  EXPECT_TRUE(submission2.isLegal(Color::kBlue, CardNumber::kZero));

  Submission submission3{{Color::kBlue, CardNumber::kZero}, false};
  EXPECT_TRUE(submission3.isLegal(Color::kBlue, CardNumber::kZero));

  Submission submission4{{Color::kGreen, CardNumber::kZero}, false};
  EXPECT_TRUE(submission4.isLegal(Color::kBlue, CardNumber::kZero));

  Submission submission5{{Color::kBlue, CardNumber::kZero}, false};
  EXPECT_TRUE(submission5.isLegal(Color::kBlue, CardNumber::kOne));

  Submission submission6{{Color::kBlue, CardNumber::kZero}, false};
  EXPECT_TRUE(submission6.isLegal(Color::kBlue, CardAction::kDrawTwo));
}

TEST(IsLegalTest, False) {
  Submission submission1{{Color::kBlue, CardNumber::kZero}, false};
  EXPECT_FALSE(submission1.isLegal(Color::kGreen, CardNumber::kOne));

  Submission submission2{{Color::kBlue, CardAction::kDrawTwo}, false};
  EXPECT_FALSE(submission2.isLegal(Color::kGreen, CardAction::kReverse));

  Submission submission3{{Color::kBlue, CardNumber::kZero}, false};
  EXPECT_FALSE(submission3.isLegal(Color::kGreen, CardAction::kReverse));
}

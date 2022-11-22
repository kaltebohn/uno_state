#include <gtest/gtest.h>

#include "../../src/uno_state.hpp"

TEST(StateTransitionTest, NotChallenge) {
  Card table_card{Card::kWildDraw4};
  Cards src_deck{allCards()};
  src_deck.erase(std::find(src_deck.begin(), src_deck.end(), table_card));

  std::array<Cards, UnoConsts::kNumOfPlayers> src_player_cards{{
      {{Card::kBlueOne}},
      {{Card::kGreenOne}},
      {{Card::kRedOne}},
      {{Card::kYellowOne}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : src_player_cards.at(i)) {
      src_deck.erase(std::find(src_deck.begin(), src_deck.end(), card));
    }
  }

  UnoState src_state{
      src_deck,
      Cards{Card::kYellowZero, table_card},
      src_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kChallenge,
      3,
      0,
      true,
      Color::kBlue,
      table_card.getPattern(),
      true,
      {}
  };
  UnoState dst_state{src_state.next(false)};

  Cards target_deck{src_deck};
  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  target_player_cards.at(0).push_back(target_deck.back()); target_deck.pop_back();
  target_player_cards.at(0).push_back(target_deck.back()); target_deck.pop_back();
  target_player_cards.at(0).push_back(target_deck.back()); target_deck.pop_back();
  target_player_cards.at(0).push_back(target_deck.back()); target_deck.pop_back();
  UnoState target_state{
      target_deck,
      Cards{Card::kYellowZero, table_card},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      0,
      1,
      true,
      Color::kBlue,
      table_card.getPattern(),
      false,
      Card{}
  };

  EXPECT_EQ(dst_state, target_state);
}

TEST(StateTransitionTest, ChallengeFailed) {
  Card table_card{Card::kWildDraw4};
  Cards src_deck{allCards()};
  src_deck.erase(std::find(src_deck.begin(), src_deck.end(), table_card));

  std::array<Cards, UnoConsts::kNumOfPlayers> src_player_cards{{
      {{Card::kBlueOne}},
      {{Card::kGreenOne}},
      {{Card::kRedOne}},
      {{Card::kYellowOne}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : src_player_cards.at(i)) {
      src_deck.erase(std::find(src_deck.begin(), src_deck.end(), card));
    }
  }

  UnoState src_state{
      src_deck,
      Cards{Card::kBlueZero, table_card},
      src_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kChallenge,
      3,
      0,
      true,
      Color::kBlue,
      table_card.getPattern(),
      false,
      {}
  };
  UnoState dst_state{src_state.next(true)};

  Cards target_deck{src_deck};
  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  target_player_cards.at(0).push_back(target_deck.back()); target_deck.pop_back();
  target_player_cards.at(0).push_back(target_deck.back()); target_deck.pop_back();
  target_player_cards.at(0).push_back(target_deck.back()); target_deck.pop_back();
  target_player_cards.at(0).push_back(target_deck.back()); target_deck.pop_back();
  target_player_cards.at(0).push_back(target_deck.back()); target_deck.pop_back();
  target_player_cards.at(0).push_back(target_deck.back()); target_deck.pop_back();
  UnoState target_state{
      target_deck,
      Cards{Card::kYellowZero, table_card},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      0,
      1,
      true,
      Color::kBlue,
      table_card.getPattern(),
      false,
      Card{}
  };

  EXPECT_EQ(dst_state, target_state);
}

TEST(StateTransitionTest, ChallengeSucceeded) {
  Card table_card{Card::kWildDraw4};
  Cards src_deck{allCards()};
  src_deck.erase(std::find(src_deck.begin(), src_deck.end(), table_card));

  std::array<Cards, UnoConsts::kNumOfPlayers> src_player_cards{{
      {{Card::kBlueOne}},
      {{Card::kGreenOne}},
      {{Card::kRedOne}},
      {{Card::kYellowOne}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : src_player_cards.at(i)) {
      src_deck.erase(std::find(src_deck.begin(), src_deck.end(), card));
    }
  }

  UnoState src_state{
      src_deck,
      Cards{Card::kYellowZero, table_card},
      src_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kChallenge,
      3,
      0,
      true,
      Color::kBlue,
      table_card.getPattern(),
      true,
      {}
  };
  UnoState dst_state{src_state.next(true)};

  Cards target_deck{src_deck};
  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  target_player_cards.at(3).push_back(Card::kWildDraw4);
  target_player_cards.at(3).push_back(target_deck.back()); target_deck.pop_back();
  target_player_cards.at(3).push_back(target_deck.back()); target_deck.pop_back();
  target_player_cards.at(3).push_back(target_deck.back()); target_deck.pop_back();
  target_player_cards.at(3).push_back(target_deck.back()); target_deck.pop_back();
  UnoState target_state{
      target_deck,
      Cards{table_card},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      0,
      3,
      true,
      Color::kBlue,
      table_card.getPattern(),
      false,
      Card{}
  };

  EXPECT_EQ(dst_state, target_state);
}

TEST(StateTransitionTest, ColorChoice) {
  Card table_card{Card::kWild};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));

  std::array<Cards, UnoConsts::kNumOfPlayers> player_cards{{
      {{Card::kBlueOne}},
      {{Card::kGreenOne}},
      {{Card::kRedOne}},
      {{Card::kYellowOne}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }

  UnoState src_state{
      deck,
      Cards{table_card},
      player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kColorChoice,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      {}
  };
  UnoState dst_state{src_state.next(Color::kBlue)};

  UnoState target_state{
      deck,
      Cards{table_card},
      player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      0,
      1,
      true,
      Color::kBlue,
      table_card.getPattern(),
      false,
      Card{}
  };

  EXPECT_EQ(dst_state, target_state);
}

TEST(StateTransitionTest, IlligalColorChoice) {
  Card table_card{Card::kWild};
  Cards src_deck{allCards()};
  src_deck.erase(std::find(src_deck.begin(), src_deck.end(), table_card));

  std::array<Cards, UnoConsts::kNumOfPlayers> src_player_cards{{
      {{Card::kBlueOne}},
      {{Card::kGreenOne}},
      {{Card::kRedOne}},
      {{Card::kYellowOne}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : src_player_cards.at(i)) {
      src_deck.erase(std::find(src_deck.begin(), src_deck.end(), card));
    }
  }

  UnoState src_state{
      src_deck,
      Cards{table_card},
      src_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kColorChoice,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      {}
  };
  UnoState dst_state{src_state.next(Color::kWild)};

  Cards target_deck{src_deck};
  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  target_player_cards.at(0).push_back(target_deck.back()); target_deck.pop_back();
  target_player_cards.at(0).push_back(target_deck.back()); target_deck.pop_back();
  UnoState target_state{
      target_deck,
      Cards{table_card},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      0,
      1,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{}
  };

  EXPECT_EQ(dst_state, target_state);

}

TEST(StateTransitionTest, SubmissionOfDrawnCard) {
  Card table_card{Card::kBlueZero};
  Cards src_deck{allCards()};
  src_deck.erase(std::find(src_deck.begin(), src_deck.end(), table_card));

  Card drawn_card{Card::kBlueTwo};
  std::array<Cards, UnoConsts::kNumOfPlayers> src_player_cards{{
      {{Card::kBlueOne, drawn_card}},
      {{Card::kGreenOne}},
      {{Card::kRedOne}},
      {{Card::kYellowOne}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : src_player_cards.at(i)) {
      src_deck.erase(std::find(src_deck.begin(), src_deck.end(), card));
    }
  }

  UnoState src_state{
      src_deck,
      Cards{table_card},
      src_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmissionOfDrawnCard,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      drawn_card
  };
  UnoState dst_state{src_state.next(Submission(drawn_card, false))};

  Cards target_deck{src_deck};
  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  target_player_cards.at(0).erase(std::find(
      target_player_cards.at(0).begin(),
      target_player_cards.at(0).end(),
      drawn_card
  ));
  UnoState target_state{
      target_deck,
      Cards{table_card, drawn_card},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      0,
      1,
      true,
      drawn_card.getColor(),
      drawn_card.getPattern(),
      false,
      Card{}
  };

  EXPECT_EQ(dst_state, target_state);
}

TEST(StateTransitionTest, IllegalSubmissionOfDrawnCard) {
  Card table_card{Card::kGreenZero};
  Cards src_deck{allCards()};
  src_deck.erase(std::find(src_deck.begin(), src_deck.end(), table_card));

  Card drawn_card{Card::kBlueTwo};
  std::array<Cards, UnoConsts::kNumOfPlayers> src_player_cards{{
      {{Card::kBlueOne, drawn_card}},
      {{Card::kGreenOne}},
      {{Card::kRedOne}},
      {{Card::kYellowOne}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : src_player_cards.at(i)) {
      src_deck.erase(std::find(src_deck.begin(), src_deck.end(), card));
    }
  }

  UnoState src_state{
      src_deck,
      Cards{table_card},
      src_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmissionOfDrawnCard,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      drawn_card
  };
  UnoState dst_state{src_state.next(Submission(drawn_card, false))};

  Cards target_deck{src_deck};
  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  target_player_cards.at(0).push_back(target_deck.back()); target_deck.pop_back();
  target_player_cards.at(0).push_back(target_deck.back()); target_deck.pop_back();
  UnoState target_state{
      target_deck,
      Cards{table_card},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      0,
      1,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{}
  };

  EXPECT_EQ(dst_state, target_state);
}

TEST(StateTransitionTest, IlligalSubmission) {
  const Card submitted_card{Card::kBlueThree};
  const Card table_card{Card::kGreenZero};
  Cards src_deck{allCards()};
  src_deck.erase(std::find(src_deck.begin(), src_deck.end(), table_card));
  const std::array<Cards, UnoConsts::kNumOfPlayers> src_player_cards{{
      {{Card::kBlueOne, Card::kBlueTwo, submitted_card}},
      {{Card::kGreenOne}},
      {{Card::kRedOne}},
      {{Card::kYellowOne}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : src_player_cards.at(i)) {
      src_deck.erase(std::find(src_deck.begin(), src_deck.end(), card));
    }
  }
  UnoState src_state{
      src_deck,
      Cards{table_card},
      src_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{}
  };
  UnoState dst_state{src_state.next(Submission(submitted_card, false))};

  Cards target_deck{src_deck};
  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  target_player_cards.at(0).push_back(target_deck.back()); target_deck.pop_back();
  target_player_cards.at(0).push_back(target_deck.back()); target_deck.pop_back();
  UnoState target_state{
      target_deck,
      Cards{table_card},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      0,
      1,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{}
  };

  EXPECT_EQ(dst_state, target_state);
}

TEST(StateTransitionTest, EmptyCardSubmission) {
  Card table_card{Card::kBlueZero};
  Cards src_deck{allCards()};
  src_deck.erase(std::find(src_deck.begin(), src_deck.end(), table_card));
  std::array<Cards, UnoConsts::kNumOfPlayers> src_player_cards{{
      {{Card::kBlueOne}},
      {{Card::kGreenOne}},
      {{Card::kRedOne}},
      {{Card::kYellowOne}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : src_player_cards.at(i)) {
      src_deck.erase(std::find(src_deck.begin(), src_deck.end(), card));
    }
  }
  UnoState src_state{
      src_deck,
      Cards{table_card},
      src_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{}
  };
  UnoState dst_state{src_state.next(Submission({}, false))};

  Card drawn_card{Card::kBlueTwo};
  Cards target_deck{src_deck};
  target_deck.erase(std::find(target_deck.begin(), target_deck.end(), drawn_card));
  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  target_player_cards.at(0).push_back(drawn_card);
  UnoState target_state{
      target_deck,
      Cards{table_card},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmissionOfDrawnCard,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      drawn_card
  };

  EXPECT_EQ(dst_state, target_state);
}

TEST(StateTransitionTest, NumberSubmission) {
  const Card submitted_card{Card::kBlueThree};
  const Card table_card{Card::kBlueZero};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));
  const std::array<Cards, UnoConsts::kNumOfPlayers> src_player_cards{{
      {{Card::kBlueOne, Card::kBlueTwo, submitted_card}},
      {{Card::kGreenOne}},
      {{Card::kRedOne}},
      {{Card::kYellowOne}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : src_player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }
  UnoState src_state{
      deck,
      Cards{table_card},
      src_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{}
  };
  UnoState dst_state{src_state.next(Submission(submitted_card, false))};

  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  target_player_cards.at(0).erase(std::find(
    target_player_cards.at(0).begin(),
    target_player_cards.at(0).end(),
    submitted_card
  ));
  UnoState target_state{
      deck,
      Cards{table_card},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      0,
      1,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{submitted_card}
  };

  EXPECT_EQ(dst_state, target_state);
}

TEST(StateTransitionTest, DrawTwoSubmission) {
  const Card submitted_card{Card::kBlueDrawTwo};
  const Card table_card{Card::kBlueZero};
  Cards src_deck{allCards()};
  src_deck.erase(std::find(src_deck.begin(), src_deck.end(), table_card));
  const std::array<Cards, UnoConsts::kNumOfPlayers> src_player_cards{{
      {{Card::kBlueOne, Card::kBlueTwo, submitted_card}},
      {{Card::kGreenOne}},
      {{Card::kRedOne}},
      {{Card::kYellowOne}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : src_player_cards.at(i)) {
      src_deck.erase(std::find(src_deck.begin(), src_deck.end(), card));
    }
  }
  UnoState src_state{
      src_deck,
      Cards{table_card},
      src_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{}
  };
  UnoState dst_state{src_state.next(Submission(submitted_card, false))};

  Cards target_deck{src_deck};
  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  target_player_cards.at(0).erase(std::find(
    target_player_cards.at(0).begin(),
    target_player_cards.at(0).end(),
    submitted_card
  ));
  target_player_cards.at(1).push_back(target_deck.back()); target_deck.pop_back();
  target_player_cards.at(1).push_back(target_deck.back()); target_deck.pop_back();
  UnoState target_state{
      target_deck,
      Cards{table_card},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      0,
      2,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{submitted_card}
  };

  EXPECT_EQ(dst_state, target_state);
}

TEST(StateTransitionTest, ReverseSubmission) {
  const Card submitted_card{Card::kBlueReverse};
  const Card table_card{Card::kBlueZero};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));
  const std::array<Cards, UnoConsts::kNumOfPlayers> src_player_cards{{
      {{Card::kBlueOne, Card::kBlueTwo, submitted_card}},
      {{Card::kGreenOne}},
      {{Card::kRedOne}},
      {{Card::kYellowOne}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : src_player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }
  UnoState src_state{
      deck,
      Cards{table_card},
      src_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{}
  };
  UnoState dst_state{src_state.next(Submission(submitted_card, false))};

  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  target_player_cards.at(0).erase(std::find(
    target_player_cards.at(0).begin(),
    target_player_cards.at(0).end(),
    submitted_card
  ));
  UnoState target_state{
      deck,
      Cards{table_card},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      0,
      3,
      false,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{submitted_card}
  };

  EXPECT_EQ(dst_state, target_state);
}

TEST(StateTransitionTest, SkipSubmission) {
  const Card submitted_card{Card::kBlueSkip};
  const Card table_card{Card::kBlueZero};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));
  const std::array<Cards, UnoConsts::kNumOfPlayers> src_player_cards{{
      {{Card::kBlueOne, Card::kBlueTwo, submitted_card}},
      {{Card::kGreenOne}},
      {{Card::kRedOne}},
      {{Card::kYellowOne}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : src_player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }
  UnoState src_state{
      deck,
      Cards{table_card},
      src_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{}
  };
  UnoState dst_state{src_state.next(Submission(submitted_card, false))};

  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  target_player_cards.at(0).erase(std::find(
    target_player_cards.at(0).begin(),
    target_player_cards.at(0).end(),
    submitted_card
  ));
  UnoState target_state{
      deck,
      Cards{table_card},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      0,
      2,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{submitted_card}
  };

  EXPECT_EQ(dst_state, target_state);
}

TEST(StateTransitionTest, WildSubmission) {
  const Card submitted_card{Card::kWild};
  const Card table_card{Card::kBlueZero};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));
  const std::array<Cards, UnoConsts::kNumOfPlayers> src_player_cards{{
      {{Card::kBlueOne, Card::kBlueTwo, submitted_card}},
      {{Card::kGreenOne}},
      {{Card::kRedOne}},
      {{Card::kYellowOne}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : src_player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }
  UnoState src_state{
      deck,
      Cards{table_card},
      src_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{}
  };
  UnoState dst_state{src_state.next(Submission(submitted_card, false))};

  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  target_player_cards.at(0).erase(std::find(
    target_player_cards.at(0).begin(),
    target_player_cards.at(0).end(),
    submitted_card
  ));
  UnoState target_state{
      deck,
      Cards{table_card},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kColorChoice,
      0,
      1,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{submitted_card}
  };

  EXPECT_EQ(dst_state, target_state);
}

TEST(StateTransitionTest, WildCustomizableSubmission) {
  // TODO
}

TEST(StateTransitionTest, WildDraw4Submission) {
  const Card submitted_card{Card::kWildDraw4};
  const Card table_card{Card::kBlueZero};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));
  const std::array<Cards, UnoConsts::kNumOfPlayers> src_player_cards{{
      {{Card::kBlueOne, Card::kBlueTwo, submitted_card}},
      {{Card::kGreenOne}},
      {{Card::kRedOne}},
      {{Card::kYellowOne}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : src_player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }
  UnoState src_state{
      deck,
      Cards{table_card},
      src_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{}
  };
  UnoState dst_state{src_state.next(Submission(submitted_card, false))};

  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  target_player_cards.at(0).erase(std::find(
    target_player_cards.at(0).begin(),
    target_player_cards.at(0).end(),
    submitted_card
  ));
  UnoState target_state{
      deck,
      Cards{table_card},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kColorChoice,
      0,
      1,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{submitted_card}
  };

  EXPECT_EQ(dst_state, target_state);
}

TEST(StateTransitionTest, WildShuffleHandsSubmission) {
  const Card submitted_card{Card::kWildShuffleHands};
  const Card table_card{Card::kBlueZero};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));
  const std::array<Cards, UnoConsts::kNumOfPlayers> src_player_cards{{
      {{Card::kBlueOne, Card::kBlueTwo, submitted_card}},
      {{Card::kGreenOne}},
      {{Card::kRedOne}},
      {{Card::kYellowOne}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : src_player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }
  UnoState src_state{
      deck,
      Cards{table_card},
      src_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{}
  };
  UnoState dst_state{src_state.next(Submission(submitted_card, false))};

  /* シャッフルワイルドを出した後の各プレイヤの手札はランダムに定まるので、着手の型と手札枚数の確認だけする。 */
  /* 他の効果は、他のカードに対するテストで確かめられていると思うことにする。 */
  EXPECT_EQ(dst_state.getCurrentMoveType(), MoveType::kColorChoice);
  for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
    if (i == 1) {
      EXPECT_EQ(dst_state.getPlayerCards(i).size(), 2);
    } else {
      EXPECT_EQ(dst_state.getPlayerCards(i).size(), 1);
    }
  }
}

TEST(LegalMovesTest, ColorChoice) {
  Card table_card{Card::kWild};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));

  std::array<Cards, UnoConsts::kNumOfPlayers> player_cards{{
      {{Card::kBlueZero}},
      {{Card::kBlueOne}},
      {{Card::kBlueTwo}},
      {{Card::kBlueThree}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }

  UnoState state{
      deck,
      Cards{table_card},
      player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kColorChoice,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{}
  };

  auto legal_moves = state.legalMoves();

  EXPECT_EQ(legal_moves.size(), 4);
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Color::kBlue), legal_moves.end());
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Color::kGreen), legal_moves.end());
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Color::kRed), legal_moves.end());
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Color::kYellow), legal_moves.end());
}

TEST(LegalMovesTest, Challenge) {
  Card table_card{Card::kWildDraw4};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));

  std::array<Cards, UnoConsts::kNumOfPlayers> player_cards{{
      {{Card::kBlueZero}},
      {{Card::kBlueOne}},
      {{Card::kBlueTwo}},
      {{Card::kBlueThree}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }

  UnoState state{
      deck,
      Cards{table_card},
      player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kChallenge,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{}
  };

  auto legal_moves = state.legalMoves();

  EXPECT_EQ(legal_moves.size(), 2);
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)true), legal_moves.end());
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)false), legal_moves.end());
}

TEST(LegalMovesTest, SubmissionOfDrawnCard) {
  Card table_card{Card::kBlueZero};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));

  Card drawn_card{Card::kBlueThree};
  std::array<Cards, UnoConsts::kNumOfPlayers> player_cards{{
      {{Card::kBlueOne, Card::kBlueTwo, drawn_card}},
      {{Card::kGreenOne}},
      {{Card::kRedOne}},
      {{Card::kYellowOne}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }

  UnoState state{
      deck,
      Cards{table_card},
      player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmissionOfDrawnCard,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      drawn_card
  };

  auto legal_moves = state.legalMoves();

  EXPECT_EQ(legal_moves.size(), 1);
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Submission{drawn_card, false}), legal_moves.end());
}

TEST(LegalMovesTest, SubmissionOfDrawnCardNothing) {
  Card table_card{Card::kGreenZero};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));

  Card drawn_card{Card::kBlueThree};
  std::array<Cards, UnoConsts::kNumOfPlayers> player_cards{{
      {{Card::kBlueOne, Card::kBlueTwo, drawn_card}},
      {{Card::kGreenOne}},
      {{Card::kRedOne}},
      {{Card::kYellowOne}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }

  UnoState state{
      deck,
      Cards{table_card},
      player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmissionOfDrawnCard,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      drawn_card
  };

  auto legal_moves = state.legalMoves();

  EXPECT_EQ(legal_moves.size(), 1);
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Submission{{}, false}), legal_moves.end());
}

TEST(LegalMovesTest, SubmissionOfDrawnCardOnUNO) {
  Card table_card{Card::kBlueZero};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));

  Card drawn_card{Card::kBlueTwo};
  std::array<Cards, UnoConsts::kNumOfPlayers> player_cards{{
      {{Card::kBlueOne, drawn_card}},
      {{Card::kGreenOne}},
      {{Card::kRedOne}},
      {{Card::kYellowOne}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }

  UnoState state{
      deck,
      Cards{table_card},
      player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmissionOfDrawnCard,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      drawn_card
  };

  auto legal_moves = state.legalMoves();

  EXPECT_EQ(legal_moves.size(), 1);
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Submission{drawn_card, true}), legal_moves.end());
}

TEST(LegalMovesTest, SubmissionMultiple) {
  Card table_card{Card::kBlueZero};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));

  std::array<Cards, UnoConsts::kNumOfPlayers> player_cards{{
      {{Card::kBlueOne, Card::kBlueDrawTwo, Card::kGreenZero, Card::kGreenOne, Card::kGreenDrawTwo}},
      {{Card::kRedZero}},
      {{Card::kRedOne}},
      {{Card::kRedTwo}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }

  UnoState state{
      deck,
      Cards{table_card},
      player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmissionOfDrawnCard,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      {}
  };

  auto legal_moves = state.legalMoves();

  EXPECT_EQ(legal_moves.size(), 3);
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Submission{Card::kBlueOne, false}), legal_moves.end());
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Submission{Card::kBlueDrawTwo, false}), legal_moves.end());
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Submission{Card::kGreenZero, false}), legal_moves.end());
}

TEST(LegalMovesTest, SubmissionMultipleOnUno) {
  Card table_card{Card::kBlueZero};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));

  std::array<Cards, UnoConsts::kNumOfPlayers> player_cards{{
      {{Card::kBlueOne, Card::kBlueDrawTwo}},
      {{Card::kRedZero}},
      {{Card::kRedOne}},
      {{Card::kRedTwo}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }

  UnoState state{
      deck,
      Cards{table_card},
      player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmissionOfDrawnCard,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      {}
  };

  auto legal_moves = state.legalMoves();

  EXPECT_EQ(legal_moves.size(), 3);
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Submission{Card::kBlueOne, true}), legal_moves.end());
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Submission{Card::kBlueDrawTwo, true}), legal_moves.end());
}

TEST(LegalMovesTest, SubmissionNothing) {
  Card table_card{Card::kBlueZero};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));

  std::array<Cards, UnoConsts::kNumOfPlayers> player_cards{{
      {{Card::kRedOne}},
      {{Card::kRedTwo}},
      {{Card::kRedThree}},
      {{Card::kRedFour}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }

  UnoState state{
      deck,
      Cards{table_card},
      player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmissionOfDrawnCard,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      {}
  };

  auto legal_moves = state.legalMoves();

  EXPECT_EQ(legal_moves.size(), 1);
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Submission{{}, false}), legal_moves.end());
}

TEST(IsFinishedTest, True) {
  Card table_card{Card::kWild};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));

  std::array<Cards, UnoConsts::kNumOfPlayers> player_cards{{
      {{Card::kBlueZero}},
      {{Card::kBlueOne}},
      {{Card::kBlueTwo}},
      {}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }

  UnoState state{
      deck,
      Cards{table_card},
      player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kColorChoice,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{}
  };

  EXPECT_TRUE(state.isFinished());
}

TEST(IsFinishedTest, False) {
  Card table_card{Card::kWild};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));

  std::array<Cards, UnoConsts::kNumOfPlayers> player_cards{{
      {{Card::kBlueZero}},
      {{Card::kBlueOne}},
      {{Card::kBlueTwo}},
      {{Card::kBlueThree}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }

  UnoState state{
      deck,
      Cards{table_card},
      player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kColorChoice,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{}
  };

  EXPECT_FALSE(state.isFinished());
}

TEST(GetScoreTest, NotFinished) {
  Card table_card{Card::kWild};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));

  std::array<Cards, UnoConsts::kNumOfPlayers> player_cards{{
      {{Card::kBlueZero}},
      {{Card::kBlueOne}},
      {{Card::kBlueTwo}},
      {{Card::kBlueThree}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }

  UnoState state{
      deck,
      Cards{table_card},
      player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kColorChoice,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{}
  };

  for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
    EXPECT_EQ(state.getScore(0), 0);
  }
}

TEST(GetScoreTest, Number) {
  Card table_card{Card::kWild};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));

  std::array<Cards, UnoConsts::kNumOfPlayers> player_cards{{
      {{Card::kBlueZero}},
      {{Card::kGreenOne}},
      {{Card::kYellowZero, Card::kYellowOne, Card::kYellowTwo, Card::kYellowThree, Card::kYellowFour, Card::kYellowFive, Card::kYellowSix, Card::kYellowSeven, Card::kYellowEight, Card::kYellowNine}},
      {}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }

  UnoState state{
      deck,
      Cards{table_card},
      player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kColorChoice,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{}
  };

  EXPECT_EQ(state.getScore(0), 0);
  EXPECT_EQ(state.getScore(1), -1);
  EXPECT_EQ(state.getScore(2), -45);
  EXPECT_EQ(state.getScore(3), 46);
}

TEST(GetScoreTest, Action) {
  Card table_card{Card::kWild};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));

  std::array<Cards, UnoConsts::kNumOfPlayers> player_cards{{
      {{Card::kBlueDrawTwo}},
      {{Card::kWild}},
      {{Card::kYellowDrawTwo,
        Card::kYellowReverse,
        Card::kYellowSkip,
        Card::kWild,
        Card::kWildDraw4,
        Card::kWildShuffleHands,
        Card::kWildCustomizable}},
      {}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }

  UnoState state{
      deck,
      Cards{table_card},
      player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kColorChoice,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{}
  };

  EXPECT_EQ(state.getScore(0), -20);
  EXPECT_EQ(state.getScore(1), -50);
  EXPECT_EQ(state.getScore(2), -240);
  EXPECT_EQ(state.getScore(3), 310);
}

TEST(GetCurrentPlayerNum, Normal) {
  Card table_card{Card::kWild};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));

  std::array<Cards, UnoConsts::kNumOfPlayers> player_cards{{
      {{Card::kBlueZero}},
      {{Card::kBlueOne}},
      {{Card::kBlueTwo}},
      {{Card::kBlueThree}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }

  UnoState state{
      deck,
      Cards{table_card},
      player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kColorChoice,
      0,
      1,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{}
  };

  auto legal_moves = state.legalMoves();

  EXPECT_EQ(state.getCurrentPlayerNum(), 1);
}

TEST(GetPlayerCards, Normal) {
  Card table_card{Card::kWild};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));

  std::array<Cards, UnoConsts::kNumOfPlayers> player_cards{{
      {{Card::kBlueZero, Card::kBlueOne, Card::kBlueTwo}},
      {{Card::kRedZero, Card::kRedOne, Card::kRedTwo}},
      {{Card::kGreenDrawTwo}},
      {{Card::kWild}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : player_cards.at(i)) {
      deck.erase(std::find(deck.begin(), deck.end(), card));
    }
  }

  UnoState state{
      deck,
      Cards{table_card},
      player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kColorChoice,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{}
  };

  auto legal_moves = state.legalMoves();

  for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
    const auto cards = state.getPlayerCards(i);
    EXPECT_TRUE(std::equal(cards.begin(), cards.end(), player_cards.at(i).begin()));
  }
}

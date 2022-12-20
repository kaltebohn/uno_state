#include <gtest/gtest.h>

#include "../../src/uno_state.hpp"

/*
  状態遷移テスト。チャレンジができる状態で、チャレンジをしない場合。
  前状態: 前プレイヤがワイルドドロー4を出した。
  着手: チャレンジしない。
  次状態: 現プレイヤが山札から4枚引いて、次プレイヤに手番が移る。 
*/
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
      {},
      XorShift64(),
      {},
      {Cards(), Cards(), Cards(), Cards{Card::kWildDraw4}}
  };
  UnoState dst_state{src_state.next(false)};

  Cards target_deck{src_deck};
  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  Cards target_add_cards{
      Card::kWildCustomizable,
      Card::kWildCustomizable,
      Card::kWildCustomizable,
      Card::kWildShuffleHands
  };
  std::copy(target_add_cards.begin(), target_add_cards.end(), std::back_inserter(target_player_cards.at(0)));
  target_deck.erase(target_deck.end() - 4, target_deck.end());
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
      Card{},
      XorShift64(),
      {target_add_cards},
      {}
  };

  ASSERT_EQ(dst_state, target_state);
}

/*
  状態遷移テスト。チャレンジができる状態で、チャレンジをして失敗した場合。
  前状態: 前プレイヤがワイルドドロー4を出した。
  着手: チャレンジする。
  次状態: チャレンジフラグを戻し、現プレイヤが山札から6枚引いて、次プレイヤに手番が移る。 
*/
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
      {},
      XorShift64(),
      {},
      {Cards(), Cards(), Cards(), Cards{Card::kWildDraw4}}
  };
  UnoState dst_state{src_state.next(true)};

  Cards target_deck{src_deck};
  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  Cards target_add_cards{
      Card::kWildCustomizable,
      Card::kWildCustomizable,
      Card::kWildCustomizable,
      Card::kWildShuffleHands,
      Card::kWildDraw4,
      Card::kWildDraw4
  };
  std::copy(target_add_cards.begin(), target_add_cards.end(), std::back_inserter(target_player_cards.at(0)));
  target_deck.erase(target_deck.end() - 6, target_deck.end());
  UnoState target_state{
      target_deck,
      Cards{Card::kBlueZero, table_card},
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
      Card{},
      XorShift64(),
      {target_add_cards},
      {}
  };

  EXPECT_EQ(dst_state, target_state);
}

/*
  状態遷移テスト。チャレンジができる状態で、チャレンジをして成功した場合。
  前状態: 前プレイヤがワイルドドロー4を出した。
  着手: チャレンジする。
  次状態: チャレンジフラグを戻し、前プレイヤが山札から4枚引いて、ワイルドドロー4を前プレイヤの手札に戻し、前プレイヤに手番が移る。 
*/
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
      {},
      XorShift64(),
      {},
      {Cards(), Cards(), Cards(), Cards{Card::kWildDraw4}}
  };
  UnoState dst_state{src_state.next(true)};

  Cards target_deck{src_deck};
  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  Cards target_add_cards{
      Card::kWildDraw4,
      Card::kWildCustomizable,
      Card::kWildCustomizable,
      Card::kWildCustomizable,
      Card::kWildShuffleHands
  };
  std::copy(target_add_cards.begin(), target_add_cards.end(), std::back_inserter(target_player_cards.at(3)));
  target_deck.erase(target_deck.end() - 4, target_deck.end());
  UnoState target_state{
      target_deck,
      Cards{Card::kYellowZero},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      0,
      3,
      true,
      Color::kYellow,
      CardNumber::kZero,
      false,
      Card{},
      XorShift64(),
      {Cards(), Cards(), Cards(), target_add_cards},
      {}
  };

  EXPECT_EQ(dst_state, target_state);
}

/*
  状態遷移テスト。ワイルド系提出後、色を選択する場合。
  前状態: 現プレイヤがワイルドを出した。
  着手: 青色を選択する。
  次状態: チャレンジフラグを戻し、場の色が青色になって、現プレイヤの提出手番になる。
*/
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
      {},
      XorShift64(),
      {},
      {Cards{Card::kWildDraw4}}
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
      Card{},
      XorShift64(),
      {},
      {}
  };

  EXPECT_EQ(dst_state, target_state);
}

/*
  状態遷移テスト。ワイルド系提出後、選べない色を選択する場合。
  前状態: 前プレイヤが青0を出した後、現プレイヤがワイルドを出した。
  着手: ワイルド色(ソース上で型として指定されているが、UNOの場の色ではありえない)を選択する。
  次状態: ワイルドを現プレイヤに返して場のカードを青0に戻し、現プレイヤが山札から2枚引いて、次プレイヤに手番が移る。
*/
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
      Cards{Card::kBlueZero, table_card},
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
      {},
      XorShift64(),
      {},
      {Cards{Card::kWild}}
  };
  UnoState dst_state{src_state.next(Color::kWild)};

  Cards target_deck{src_deck};
  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  Cards target_add_cards{
      table_card,
      Card::kWildCustomizable,
      Card::kWildCustomizable,
  };
  std::copy(target_add_cards.begin(), target_add_cards.end(), std::back_inserter(target_player_cards.at(0)));
  target_deck.erase(target_deck.end() - 2, target_deck.end());
  UnoState target_state{
      target_deck,
      Cards{Card::kBlueZero},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      0,
      1,
      true,
      Color::kBlue,
      CardPattern(CardNumber::kZero),
      false,
      Card{},
      XorShift64(),
      {target_add_cards},
      {}
  };

  EXPECT_EQ(dst_state, target_state);

}

/*
  状態遷移テスト。カードを引いた後、引いたカードを提出する場合。
  前状態: 前プレイヤが青0を出した後、現プレイヤが空の手を出した。カードとして、青3を受け取る。
  着手: 引いたカード(青3)を提出する。
  次状態: 提出が受理され、次プレイヤに手番が移る。
*/
TEST(StateTransitionTest, SubmissionOfDrawnCard) {
  Card table_card{Card::kBlueZero};
  Cards src_deck{allCards()};
  src_deck.erase(std::find(src_deck.begin(), src_deck.end(), table_card));

  Card drawn_card{Card::kBlueThree};
  std::array<Cards, UnoConsts::kNumOfPlayers> src_player_cards{{
      {{Card::kBlueOne, Card::kBlueTwo, drawn_card}},
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
      drawn_card,
      XorShift64(),
      {Cards{drawn_card}},
      {}
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
      Card{},
      XorShift64(),
      {},
      {Cards{drawn_card}}
  };

  EXPECT_EQ(dst_state, target_state);
}

/*
  状態遷移テスト。カードを引いた後、引いたカードを違法だが提出する場合。
  前状態: 前プレイヤが緑0を出した後、現プレイヤが空の手を出した。カードとして、青2を受け取る。
  着手: 引いたカード(青2)を提出する。
  次状態: 提出が拒否され、現プレイヤが山札から2枚引いて、次プレイヤの提出手番になる。
*/
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
      drawn_card,
      XorShift64(),
      {Cards{drawn_card}},
      {}
  };
  UnoState dst_state{src_state.next(Submission(drawn_card, false))};

  Cards target_deck{src_deck};
  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  Cards target_add_cards{
      Card::kWildCustomizable,
      Card::kWildCustomizable
  };
  std::copy(target_add_cards.begin(), target_add_cards.end(), std::back_inserter(target_player_cards.at(0)));
  target_deck.erase(target_deck.end() - 2, target_deck.end());
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
      Card{},
      XorShift64(),
      {target_add_cards},
      {}
  };

  EXPECT_EQ(dst_state, target_state);
}

/*
  状態遷移テスト。カードを引いた後、空のカードを提出する場合。
  前状態: 前プレイヤが青0を出した後、現プレイヤが空の手を出した。カードとして、青2を受け取る。
  着手: 空のカードを提出する。
  次状態: 次プレイヤに手番が移る。
*/
TEST(StateTransitionTest, EmptySubmissionOfDrawnCard) {
  Card table_card{Card::kBlueZero};
  Cards src_deck{allCards()};
  src_deck.erase(std::find(src_deck.begin(), src_deck.end(), table_card));

  Card drawn_card{Card::kBlueTwo};
  std::array<Cards, UnoConsts::kNumOfPlayers> player_cards{{
      {{Card::kBlueOne, drawn_card}},
      {{Card::kGreenOne}},
      {{Card::kRedOne}},
      {{Card::kYellowOne}}
  }};
  for (int i = 0; i < 4; i++) {
    for (const Card& card : player_cards.at(i)) {
      src_deck.erase(std::find(src_deck.begin(), src_deck.end(), card));
    }
  }

  UnoState src_state{
      src_deck,
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
      drawn_card,
      XorShift64(),
      {},
      {}
  };
  UnoState dst_state{src_state.next(Submission({}, false))};

  Cards target_deck{src_deck};
  UnoState target_state{
      target_deck,
      Cards{table_card},
      player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      0,
      1,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{},
      XorShift64(),
      {},
      {}
  };

  EXPECT_EQ(dst_state, target_state);
}

/*
  状態遷移テスト。違法な提出の場合。
  前状態: 前プレイヤが緑0を出した。
  着手: 青3を提出する。
  次状態: 提出が拒否され、現プレイヤが山札から2枚引いて、次プレイヤに手番が移る。
*/
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
      Card{},
      XorShift64(),
      {},
      {Cards{table_card}}
  };
  UnoState dst_state{src_state.next(Submission(submitted_card, false))};

  Cards target_deck{src_deck};
  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  Cards target_add_cards{
      Card::kWildCustomizable,
      Card::kWildCustomizable
  };
  std::copy(target_add_cards.begin(), target_add_cards.end(), std::back_inserter(target_player_cards.at(0)));
  target_deck.erase(target_deck.end() - 2, target_deck.end());
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
      Card{},
      XorShift64(),
      {target_add_cards},
      {}
  };

  EXPECT_EQ(dst_state, target_state);
}

/*
  状態遷移テスト。空の提出の場合。
  前状態: 前プレイヤが青0を出した。
  着手: 空のカードを提出する。
  次状態: 現プレイヤが山札から1枚引いて、現プレイヤの引いたカードの提出手番になる。
*/
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
      Card{},
      XorShift64(),
      {},
      {Cards{table_card}}
  };
  UnoState dst_state{src_state.next(Submission({}, false))};

  Card drawn_card{Card::kWildCustomizable};
  Cards target_deck{src_deck};
  target_deck.erase(std::find(target_deck.begin(), target_deck.end(), drawn_card));
  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  Cards target_add_cards{
    Card::kWildCustomizable
  };
  std::copy(target_add_cards.begin(), target_add_cards.end(), std::back_inserter(target_player_cards.at(0)));
  target_deck.erase(target_deck.end() - 1, target_deck.end());
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
      drawn_card,
      XorShift64(),
      {Cards{target_add_cards}},
      {}
  };

  EXPECT_EQ(dst_state, target_state);
}

/*
  状態遷移テスト。数字カードの提出の場合。
  前状態: 前プレイヤが青0を出した。
  着手: 青3を提出する。
  次状態: 提出が受理され、次プレイヤに手番が移る。
*/
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
      Card{},
      XorShift64(),
      {},
      {Cards(), Cards(), Cards(), Cards{table_card}}
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
      Cards{table_card, submitted_card},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      0,
      1,
      true,
      submitted_card.getColor(),
      submitted_card.getPattern(),
      false,
      Card{},
      XorShift64(),
      {},
      {Cards{submitted_card}}
  };

  EXPECT_EQ(dst_state, target_state);
}

/*
  状態遷移テスト。ドロー2の提出の場合。
  前状態: 前プレイヤが青0を出した。
  着手: 青ドロー2を提出する。
  次状態: 提出が受理され、次プレイヤが山札から2枚引いて、その次のプレイヤに手番が移る。
*/
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
      Card{},
      XorShift64(),
      {},
      {Cards{table_card}}
  };
  UnoState dst_state{src_state.next(Submission(submitted_card, false))};

  Cards target_deck{src_deck};
  std::array<Cards, UnoConsts::kNumOfPlayers> target_player_cards{src_player_cards};
  target_player_cards.at(0).erase(std::find(
    target_player_cards.at(0).begin(),
    target_player_cards.at(0).end(),
    submitted_card
  ));
  Cards target_add_cards{
      Card::kWildCustomizable,
      Card::kWildCustomizable
  };
  std::copy(target_add_cards.begin(), target_add_cards.end(), std::back_inserter(target_player_cards.at(1)));
  target_deck.erase(target_deck.end() - 2, target_deck.end());
  UnoState target_state{
      target_deck,
      Cards{table_card, submitted_card},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      0,
      2,
      true,
      submitted_card.getColor(),
      submitted_card.getPattern(),
      false,
      Card{},
      XorShift64(),
      {Cards(), target_add_cards},
      {Cards{submitted_card}}
  };

  EXPECT_EQ(dst_state, target_state);
}

/*
  状態遷移テスト。リバースの提出の場合。
  前状態: 前プレイヤが青0を出した。
  着手: 青リバースを提出する。
  次状態: 提出が受理され、周り順が逆転し、その順番における次プレイヤに手番が移る。
*/
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
      Card{},
      XorShift64(),
      {},
      {Cards{table_card}}
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
      Cards{table_card, submitted_card},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      0,
      3,
      false,
      submitted_card.getColor(),
      submitted_card.getPattern(),
      false,
      Card{},
      XorShift64(),
      {},
      {Cards{submitted_card}}
  };

  EXPECT_EQ(dst_state, target_state);
}

/*
  状態遷移テスト。スキップの提出の場合。
  前状態: 前プレイヤが青0を出した。
  着手: 青スキップを提出する。
  次状態: 提出が受理され、次の次のプレイヤに手番が移る。
*/
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
      Card{},
      XorShift64(),
      {},
      {Cards(), Cards(), Cards(), Cards{table_card}}
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
      Cards{table_card, submitted_card},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kSubmission,
      0,
      2,
      true,
      submitted_card.getColor(),
      submitted_card.getPattern(),
      false,
      Card{},
      XorShift64(),
      {},
      {Cards{submitted_card}}
  };

  EXPECT_EQ(dst_state, target_state);
}

/*
  状態遷移テスト。ワイルドの提出の場合。
  前状態: 前プレイヤが青0を出した。
  着手: ワイルドを提出する。
  次状態: 提出が受理され、現プレイヤの色選択手番になる。
*/
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
      Card{},
      XorShift64(),
      {},
      {Cards(), Cards(), Cards(), Cards{table_card}}
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
      Cards{table_card, submitted_card},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kColorChoice,
      0,
      1,
      true,
      submitted_card.getColor(),
      submitted_card.getPattern(),
      false,
      Card{},
      XorShift64(),
      {},
      {Cards{submitted_card}}
  };

  EXPECT_EQ(dst_state, target_state);
}

/*
  状態遷移テスト。白いワイルドの提出の場合。
  前状態: 前プレイヤが青0を出した。
  着手: 白いワイルドを提出する。
  次状態: 提出が受理され、白いワイルドの効果が発揮される。
*/
TEST(StateTransitionTest, WildCustomizableSubmission) {
  // TODO
}

/*
  状態遷移テスト。ワイルドドロー4の提出の場合。
  前状態: 前プレイヤが青0を出した。
  着手: ワイルドドロー4を提出する。
  次状態: 提出が受理され、チャレンジ有効フラグを更新した後、現プレイヤの色選択手番になる。
*/
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
      Card{},
      XorShift64(),
      {},
      {Cards(), Cards(), Cards(), Cards{table_card}}
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
      Cards{table_card, submitted_card},
      target_player_cards,
      std::array<int, UnoConsts::kNumOfPlayers>{0, 1, 2, 3},
      std::array<int, UnoConsts::kNumOfPlayers>{},
      MoveType::kColorChoice,
      0,
      1,
      true,
      submitted_card.getColor(),
      submitted_card.getPattern(),
      false,
      Card{},
      XorShift64(),
      {},
      {Cards{submitted_card}}
  };

  EXPECT_EQ(dst_state, target_state);
}

/*
  状態遷移テスト。シャッフルワイルドの提出の場合。
  前状態: 前プレイヤが青0を出した。
  着手: シャッフルワイルドを提出する。
  次状態: 提出が受理され、チャレンジ有効フラグを更新した後、現プレイヤの色選択手番になる。
*/
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
      submitted_card.getColor(),
      submitted_card.getPattern(),
      false,
      Card{},
      XorShift64(),
      {},
      {Cards(), Cards(), Cards(), Cards{table_card}}
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

/*
  状態遷移テスト。カードを提出したプレイヤが上がる場合。
  前状態: プレイヤが青0を出した。
  着手: 青1を提出する。
  次状態: 提出が受理され、現在プレイヤが上がり、得点が計算される。
*/
TEST(StateTransitionTest, FinishRound) {
  // TODO
}

/*
  状態遷移テスト。ゲームが既に終了している場合。
  前状態: 前プレイヤが青0を出して、ゲームが終了した。
  着手: 青1を提出する。
  次状態: 状態は変わらない。
*/
TEST(StateTransitionTest, AlreadyFinished) {
  const Card submitted_card{Card::kBlueOne};
  const Card table_card{Card::kBlueZero};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));
  const std::array<Cards, UnoConsts::kNumOfPlayers> src_player_cards{{
      {{Card::kBlueOne}},
      {{Card::kGreenOne}},
      {{Card::kRedOne}},
      {}
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
      std::array<int, UnoConsts::kNumOfPlayers>{-1, -1, -1, 3},
      MoveType::kSubmission,
      3,
      0,
      true,
      submitted_card.getColor(),
      submitted_card.getPattern(),
      false,
      Card{},
      XorShift64(),
      {},
      {Cards(), Cards(), Cards(), Cards{table_card}}
  };
  UnoState dst_state{src_state.next(Submission(submitted_card, false))};

  EXPECT_EQ(dst_state, src_state);
}

/*
  合法手列挙テスト。色選択の場合。
  期待する出力: {青, 緑, 赤, 黄色}。
*/
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
      Card{},
      XorShift64(),
      {},
      {Cards{table_card}}
  };

  auto legal_moves = state.legalMoves();

  EXPECT_EQ(legal_moves.size(), 4);
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Color::kBlue), legal_moves.end());
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Color::kGreen), legal_moves.end());
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Color::kRed), legal_moves.end());
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Color::kYellow), legal_moves.end());
}

/*
  合法手列挙テスト。チャレンジの場合。
  期待する出力: {True, False}。
*/
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
      Card{},
      XorShift64(),
      {},
      {Cards{table_card}}
  };

  auto legal_moves = state.legalMoves();

  EXPECT_EQ(legal_moves.size(), 2);
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)true), legal_moves.end());
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)false), legal_moves.end());
}

/*
  合法手列挙テスト。引いたカードの提出の場合。
  期待する出力: {空のカード, 引いたカード}。
*/
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
      drawn_card,
      XorShift64(),
      {Cards{drawn_card}},
      {}
  };

  auto legal_moves = state.legalMoves();

  EXPECT_EQ(legal_moves.size(), 2);
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Submission{Card{}, false}), legal_moves.end());
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Submission{drawn_card, false}), legal_moves.end());
}

/*
  合法手列挙テスト。引いたカードの提出で、出せるカードがない場合。
  期待する出力: {空のカード}。
*/
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
      drawn_card,
      XorShift64(),
      {Cards{drawn_card}},
      {}
  };

  auto legal_moves = state.legalMoves();

  EXPECT_EQ(legal_moves.size(), 1);
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Submission{{}, false}), legal_moves.end());
}

/*
  合法手列挙テスト。引いたカードの提出で、現プレイヤの手札が2枚の場合。
  期待する出力: {空のカード, 引いたカード}。引いたカードのUNO宣言フラグが立っている。
*/
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
      drawn_card,
      XorShift64(),
      {Cards{drawn_card}},
      {}
  };

  auto legal_moves = state.legalMoves();

  EXPECT_EQ(legal_moves.size(), 2);
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Submission{drawn_card, true}), legal_moves.end());
}

/*
  合法手列挙テスト。複数枚のカードの提出の場合。
  期待する出力: {空のカード, 青1, 青ドロー2, 緑0}。
*/
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
      MoveType::kSubmission,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      {},
      XorShift64(),
      {},
      {Cards(), Cards(), Cards(), Cards{table_card}}
  };

  auto legal_moves = state.legalMoves();

  EXPECT_EQ(legal_moves.size(), 4);
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Submission{Card::kBlueOne, false}), legal_moves.end());
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Submission{Card::kBlueDrawTwo, false}), legal_moves.end());
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Submission{Card::kGreenZero, false}), legal_moves.end());
}

/*
  合法手列挙テスト。複数枚のカードの提出の場合。
  期待する出力: {空のカード, 青1, 青ドロー2, 緑0}。空のカード以外のUNO宣言フラグが立っている。
*/
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
      MoveType::kSubmission,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      {},
      XorShift64(),
      {},
      {Cards(), Cards(), Cards(), Cards{table_card}}
  };

  auto legal_moves = state.legalMoves();

  EXPECT_EQ(legal_moves.size(), 3);
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Submission{Card::kBlueOne, true}), legal_moves.end());
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Submission{Card::kBlueDrawTwo, true}), legal_moves.end());
}

/*
  合法手列挙テスト。提出で、出せるカードがない場合。
  期待する出力: {空のカード}。
*/
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
      MoveType::kSubmission,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      {},
      XorShift64(),
      {},
      {Cards(), Cards(), Cards(), Cards{table_card}}
  };

  auto legal_moves = state.legalMoves();

  EXPECT_EQ(legal_moves.size(), 1);
  EXPECT_NE(std::find(legal_moves.begin(), legal_moves.end(), (Move)Submission{{}, false}), legal_moves.end());
}

/*
  合法手列挙テスト。既にラウンドが終了している状態。
  期待する出力: {}。
*/
TEST(LegalMovesTest, AlreadyFinished) {
  // TODO
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
      Card{},
      XorShift64(),
      {},
      {Cards(), Cards(), Cards(), Cards{table_card}}
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
      Card{},
      XorShift64(),
      {},
      {Cards(), Cards(), Cards(), Cards{table_card}}
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
      Card{},
      XorShift64(),
      {},
      {Cards(), Cards(), Cards(), Cards{table_card}}
  };

  for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
    EXPECT_EQ(state.getScore(0), 0);
  }
}

TEST(GetScoreTest, Number) {
  Card table_card{Card::kBlueZero};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));

  const Card submitted_card{Card::kBlueOne};
  std::array<Cards, UnoConsts::kNumOfPlayers> player_cards{{
      {{submitted_card}},
      {{Card::kGreenOne}},
      {{Card::kRedZero, Card::kRedOne, Card::kRedTwo, Card::kRedThree, Card::kRedFour, Card::kRedFive, Card::kRedSix, Card::kRedSeven, Card::kRedEight, Card::kRedNine}},
      {{Card::kYellowZero}}
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
      MoveType::kSubmission,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{},
      XorShift64(),
      {},
      {Cards(), Cards(), Cards(), Cards{table_card}}
  };
  state = state.next(Submission(submitted_card, false));

  EXPECT_EQ(state.getScore(0), 46);
  EXPECT_EQ(state.getScore(1), -1);
  EXPECT_EQ(state.getScore(2), -45);
  EXPECT_EQ(state.getScore(3), 0);
}

TEST(GetScoreTest, Action) {
  Card table_card{Card::kBlueZero};
  Cards deck{allCards()};
  deck.erase(std::find(deck.begin(), deck.end(), table_card));
  const Card submitted_card{Card::kBlueOne};
  std::array<Cards, UnoConsts::kNumOfPlayers> player_cards{{
      {submitted_card},
      {{Card::kWild}},
      {{Card::kRedDrawTwo,
        Card::kRedReverse,
        Card::kRedSkip,
        Card::kWild,
        Card::kWildDraw4,
        Card::kWildShuffleHands,
        Card::kWildCustomizable}},
      {{Card::kYellowDrawTwo}}
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
      MoveType::kSubmission,
      3,
      0,
      true,
      table_card.getColor(),
      table_card.getPattern(),
      false,
      Card{},
      XorShift64(),
      {},
      {Cards(), Cards(), Cards(), Cards{table_card}}
  };
  state = state.next(Submission(submitted_card, false));

  EXPECT_EQ(state.getScore(0), 310);
  EXPECT_EQ(state.getScore(1), -50);
  EXPECT_EQ(state.getScore(2), -240);
  EXPECT_EQ(state.getScore(3), -20);
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
      Card{},
      XorShift64(),
      {},
      {Cards(), Cards(), Cards(), Cards{table_card}}
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
      Card{},
      XorShift64(),
      {},
      {Cards(), Cards(), Cards(), Cards{table_card}}
  };

  auto legal_moves = state.legalMoves();

  for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
    const auto cards = state.getPlayerCards(i);
    EXPECT_TRUE(std::equal(cards.begin(), cards.end(), player_cards.at(i).begin()));
  }
}

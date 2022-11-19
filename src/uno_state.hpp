#ifndef UNO_STATE_HPP_
#define UNO_STATE_HPP_

#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

#include "card.hpp"
#include "move.hpp"
#include "uno_consts.hpp"

enum class MoveType {
  kSubmission,
  kSubmissionOfDrawnCard,
  kColorChoice,
  kChallenge
};

class UnoState {
 public:
  /* ゲーム開始用。 */
  UnoState(const Cards& initial_deck, Card initial_table_card = {})
      : deck_(initial_deck),
        discards_(),
        player_cards_(),
        player_seats_({0, 1, 2, 3}),
        prev_player_(-1),
        current_player_(-1),
        is_normal_order_(true),
        table_color_(),
        table_pattern_(),
        has_prev_player_not_yelled_uno_(),
        is_challenge_valid_(),
        drawn_card_() {
    /* 山札をシャッフル。 */
    refreshDeck();
    
    /* プレイヤにカードを7枚ずつ分配。 */
    for (int player_num = 0; player_num < UnoConsts::kNumOfPlayers; player_num++) {
      std::copy(deck_.begin(), deck_.begin() + 7, player_cards_.at(player_num).begin());
      deck_.erase(deck_.begin(), deck_.begin() + 7);
    }

    /* 席順をシャッフル。 */
    // TODO: xorshiftに置き換える。
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::shuffle(player_seats_.begin(), player_seats_.end(), engine);

    /* 最初の1枚を出して、効果を処理する。 */
    /* ワイルドドロー4、シャッフルワイルド、白いワイルドの場合は仕切り直し。 */
    // TODO: ロジック正しいか確認。
    // TODO: 効果を処理する。
    while (initial_table_card.isEmpty()) {
      const Card tmp_card{deck_.back()};
      if (!(tmp_card == Card(Color::kWild, CardAction::kWildDraw4) ||
            tmp_card == Card(Color::kWild, CardAction::kWildShuffleHands) ||
            tmp_card == Card(Color::kWild, CardAction::kWildCustomizable))) {
        deck_.pop_back();
        initial_table_card = tmp_card;
      } else {
        refreshDeck();
      }
    }
    discards_.push_back(initial_table_card);
    table_color_ = initial_table_card.getColor();
    table_pattern_ =initial_table_card.getPattern();
  }

  UnoState(std::vector<Card> deck, std::vector<Card> discards, std::array<Cards, UnoConsts::kNumOfPlayers> player_cards, std::array<int, UnoConsts::kNumOfPlayers> player_seats, std::array<int, UnoConsts::kNumOfPlayers> player_scores, int prev_player, int current_player, bool is_normal_order, Color table_color, CardPattern table_pattern, bool has_prev_player_not_yelled_uno, bool is_challenge_valid, Card drawn_card)
      : deck_(deck),
        discards_(discards),
        player_cards_(player_cards),
        player_seats_(player_seats),
        player_scores_(player_scores),
        prev_player_(prev_player),
        current_player_(current_player),
        is_normal_order_(is_normal_order),
        table_color_(table_color),
        table_pattern_(table_pattern),
        has_prev_player_not_yelled_uno_(has_prev_player_not_yelled_uno),
        is_challenge_valid_(is_challenge_valid),
        drawn_card_(drawn_card)
      {}

  /* 受け取った手を適用して得られる状態を返す。 */
  UnoState next(Move move) const;

  /* 合法着手の全体を返す。 */
  std::vector<Move> legalMoves() const;

  /* ゲームが終了しているか？ */
  bool isFinished() const {
    return std::any_of(player_cards_.cbegin(), player_cards_.cend(),
        [](Cards cards){ cards.size() == 0; });
  }

  /* 指定されたプレイヤ番号の現時点での得点を返す。ゲームが終わっていなければ0。 */
  double getScore(const int player_num) const { return player_scores_.at(player_num); }

  /* 現時点が誰の手番か？ */
  int getCurrentPlayerNum() const { return current_player_; }

  /* player_num番目のプレイヤの手札を返す。 */
  Cards getPlayerCards(int player_num) const { return player_cards_.at(player_num); }

 private:
  std::vector<Card> deck_;
  std::vector<Card> discards_;
  std::array<Cards, UnoConsts::kNumOfPlayers> player_cards_; // プレイヤ番号で各プレイヤの手札にアクセス。
  std::array<int, UnoConsts::kNumOfPlayers> player_seats_; // プレイヤ番号で各プレイヤの席にアクセス。
  std::array<int, UnoConsts::kNumOfPlayers> player_scores_; // プレイヤ番号で各プレイヤの得点にアクセス(ゲーム終了時以外は0)。
  MoveType current_event_;
  int prev_player_;
  int current_player_;
  bool is_normal_order_;
  Color table_color_;
  CardPattern table_pattern_;
  bool has_prev_player_not_yelled_uno_;
  bool is_challenge_valid_;
  Card drawn_card_; // 直前にプレイヤが引いたカード。

  /* 可能な提出カードの全体を返す。 */
  std::vector<Submission> legalSubmissions() const;

  /* 現在のプレイヤがUNOを宣言すべきか(プレイヤの手札枚数が2枚か)？ */
  bool currentPlayerShouldYellUNO() const { return player_cards_.at(current_player_).size() == 2; }

  UnoState nextWhenColorChoice(const Color color) const;

  UnoState nextWhenChallenge(const ChallengeFlag will_challenge) const;

  UnoState nextWhenSubmission(const Submission& submission) const;

  int nextPlayerOf(const int player_num) const {
    const auto iter = is_normal_order_ ?
        std::find(player_seats_.begin(), player_seats_.end(), player_seats_.at(player_num) + 1) :
        std::find(player_seats_.begin(), player_seats_.end(), player_seats_.at(player_num) - 1);
    return *iter;
  }

  int nextPlayer() const { return nextPlayerOf(current_player_); }

  void giveCards(const int player_number, const int num) {
    for (int i = 0; i < num; i++) {
      if (deck_.size() == 0) { refreshDeck(); }
      const Card card = deck_.back(); deck_.pop_back();
      player_cards_.at(player_number).push_back(card);
    }
  }

  void shuffleCards(std::vector<Card> cards) {
    // TODO: xorshiftに置き換える。
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::shuffle(cards.begin(), cards.end(), engine);
  }

  void refreshDeck() {
    std::copy(discards_.begin(), discards_.end(), std::back_inserter(deck_));
    discards_.clear();
    shuffleCards(deck_);
  }

  Card lastSubmittedCard() { return discards_.back(); }
};

#endif // UNO_STATE_HPP_

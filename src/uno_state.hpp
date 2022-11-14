#ifndef UNO_STATE_HPP_
#define UNO_STATE_HPP_

#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

#include "cards.hpp"
#include "move.hpp"
#include "uno_consts.hpp"

enum class MoveType {
  kSubmission,
  kColorChoice,
  kChallenge
};

class UnoState {
 public:
  UnoState(CardSet deck, CardSet discards, std::array<CardSet, UnoConsts::kNumOfPlayers> player_cards, std::array<int, UnoConsts::kNumOfPlayers> player_seats, std::array<int, UnoConsts::kNumOfPlayers> player_scores, int current_player, bool is_normal_order, Color table_color, CardPattern table_pattern, bool has_prev_player_not_yelled_uno, bool can_challenge, bool is_challenge_valid)
      : deck_(deck),
        discards_(discards),
        player_cards_(player_cards),
        player_seats_(player_seats),
        player_scores_(player_scores),
        current_player_(current_player),
        is_normal_order_(is_normal_order),
        table_color_(table_color),
        table_pattern_(table_pattern),
        has_prev_player_not_yelled_uno_(has_prev_player_not_yelled_uno),
        can_challenge_(can_challenge),
        is_challenge_valid_(is_challenge_valid)
      {}

  /* 受け取った手を適用して得られる状態を返す。 */
  UnoState next(Move) const;

  /* 合法着手の全体を返す。 */
  std::vector<Move> legalMoves() const;

  /* 可能な提出カードの全体を返す。 */
  std::vector<Submission> legalSubmissions() const;

  /* ゲームが終了しているか？ */
  bool isFinished() const {
    return std::any_of(player_cards_.cbegin(), player_cards_.cend(),
        [](CardSet card){
          return card.getQuantity() == 0;
        });
  };

  /* 現在のプレイヤがUNOを宣言すべきか(プレイヤの手札枚数が2枚か)？ */
  bool currentPlayerShouldYellUNO() const { return player_cards_.at(current_player_).getQuantity() == 2; }

  /* 指定されたプレイヤ番号の現時点での得点を返す。ゲームが終わっていなければ0。 */
  double getScore(const int player_num) const { return player_scores_.at(player_num); };

  /* 現時点が誰の手番か？ */
  int getCurrentPlayerNum() const { return current_player_; };

  /* player_num番目のプレイヤの手札を返す。 */
  CardSet getPlayerCards(int player_num) const { return player_cards_.at(player_num); }

 private:
  CardSet deck_;
  CardSet discards_;
  std::array<CardSet, UnoConsts::kNumOfPlayers> player_cards_;
  std::array<int, UnoConsts::kNumOfPlayers> player_seats_;
  std::array<int, UnoConsts::kNumOfPlayers> player_scores_;
  MoveType current_event_;
  int current_player_;
  bool is_normal_order_;
  Color table_color_;
  CardPattern table_pattern_;
  bool has_prev_player_not_yelled_uno_;
  bool can_challenge_;
  bool is_challenge_valid_;
};

#endif // UNO_STATE_HPP_

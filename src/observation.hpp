#ifndef OBSERVATION_HPP
#define OBSERVATION_HPP

#include <array>

#include "card.hpp"
#include "action.hpp"
#include "action_type.hpp"

/* ゲームの観測。ゲーム中にプレイヤが知りうる情報。 */
struct Observation {
  Cards discards_;
  Cards player_card_;
  std::vector<Action> legal_actions_;
  std::array<int, UnoConsts::kNumOfPlayers> player_card_qtys_; // プレイヤ番号で各プレイヤの手札枚数にアクセス。
  std::array<int, UnoConsts::kNumOfPlayers> player_seats_; // プレイヤ番号で各プレイヤの席にアクセス。
  ActionType current_action_type_;
  int prev_player_;
  int current_player_;
  bool is_normal_order_;
  Color table_color_;
  CardPattern table_pattern_;
};

#endif // OBSERVATION_HPP

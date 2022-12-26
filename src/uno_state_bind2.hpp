#ifndef UNO_STATE_BIND2_HPP_
#define UNO_STATE_BIND2_HPP_

#include "uno_state.hpp"

/* 白いワイルドの効果が「バインド2」の場合のクラス。 */
class UnoStateBind2 final : public UnoState {
 public:
  using UnoState::UnoState;

  friend std::ostream& operator<<(std::ostream& os, const UnoStateBind2& src) {
    os << src.toString();
    return os;
  }

  /* ベクタに格納するのに必要。何か意味のある状態ではない。 */
  UnoStateBind2()
      : UnoState::UnoState(),
        bound_player_(),
        bound_turn_() {}

  /* ゲーム開始用。 */
  UnoStateBind2(const Cards& first_deck, XorShift64::result_type random_seed = 0, Card first_table_card = {})
      : UnoState::UnoState(first_deck, random_seed, first_table_card) {}

  UnoStateBind2(const std::vector<Card> deck,
                const std::vector<Card> discards,
                const std::array<Cards, UnoConsts::kNumOfPlayers> player_cards,
                const std::array<int, UnoConsts::kNumOfPlayers> player_seats,
                const std::array<int, UnoConsts::kNumOfPlayers> player_scores,
                const MoveType current_event,
                const int prev_player,
                const int current_player,
                const bool is_normal_order,
                const Color table_color,
                const CardPattern table_pattern,
                const bool is_challenge_valid,
                const Card drawn_card,
                const Move last_move,
                const XorShift64 random_engine,
                const std::array<Cards, 4>& add_cards,
                const std::array<Cards, 4>& sub_cards,
                const int bound_player,
                const int bound_turn)
      : UnoState::UnoState(deck,
                           discards,
                           player_cards,
                           player_seats,
                           player_scores,
                           current_event,
                           prev_player,
                           current_player,
                           is_normal_order,
                           table_color,
                           table_pattern,
                           is_challenge_valid,
                           drawn_card,
                           last_move,
                           random_engine,
                           add_cards,
                           sub_cards),
        bound_player_(bound_player),
        bound_turn_(bound_turn)
      {}

  UnoStateBind2(const UnoState& state, const int bound_player, const int bound_turn)
      : UnoState::UnoState(state),
        bound_player_(bound_player),
        bound_turn_(bound_turn)
      {}

  UnoStateBind2 next(const Move& move) const;

  int getBoundPlayer() const { return bound_player_; }
  int getBoundTurn() const { return bound_turn_; }

  virtual std::string toString() const;

  void print() const override { std::cout << *this; }

 private:
  int bound_player_{-1};
  int bound_turn_{};

  /* 共通処理を実施した状態に、白いワイルドの効果を反映させる。 */
  UnoStateBind2 nextWhenWildCustomizableSubmission(UnoStateBind2& state) const {
    const Color table_color{table_color_}; // バインド2の場合、色はそれを出す前のものにする。
    const int current_player{current_player_};
    const int next_player{nextPlayer()};
    const int next_of_next_player{nextPlayerOf(next_player)};

    state.current_move_type_ = MoveType::kSubmission;
    state.table_color_ = table_color;
    state.prev_player_ = current_player;
    state.current_player_ = next_of_next_player;
    state.bound_player_ = next_player;
    state.bound_turn_ = 1; // 次飛ばされ、その次に1回飛ばされ、計2回行動不能になる。

    return state;
  }
};

#endif // UNO_STATE_BIND2_HPP_

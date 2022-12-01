#ifndef UNO_STATE_BIND2_HPP_
#define UNO_STATE_BIND2_HPP_

#include "uno_state.hpp"

/* 白いワイルドの効果が「バインド2」の場合のクラス。 */
class UnoStateBind2 final : public UnoState {
 public:
  using UnoState::UnoState;

  /* ゲーム開始用。 */
  UnoStateBind2(const Cards& first_deck, Card first_table_card = {})
      : UnoState::UnoState(first_deck, first_table_card) {}

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
                const int bound_player,
                const int bound_turn)
      : UnoState::UnoState(deck, discards, player_cards, player_seats, player_scores, current_event, prev_player, current_player, is_normal_order, table_color, table_pattern, is_challenge_valid, drawn_card),
        bound_player_(bound_player),
        bound_turn_(bound_turn)
      {}

  UnoStateBind2(const UnoState& state, const int bound_player, const int bound_turn)
      : UnoState::UnoState(state),
        bound_player_(bound_player),
        bound_turn_(bound_turn)
      {}

  UnoStateBind2 next(const Move& move) const {
    /* 既に上がっていたら状態遷移しない。 */
    if (isFinished()) { return *this; }

    if (std::holds_alternative<Submission>(move) &&
        (std::get<Submission>(move).getCard() == Card::kWildCustomizable)) {
      const Submission submission{std::get<Submission>(move)};
      UnoStateBind2 state{*this};
      state.acceptSubmission(submission);
      return nextWhenWildCustomizableSubmission(state);
    }

    UnoStateBind2 state{UnoState::next(move), bound_player_, bound_turn_};

    /* バインド2の効果を処理する。 */
    if (state.bound_turn_ > 0 &&
        (state.current_player_ == state.bound_player_) &&
        state.isFinished()) {
      assert(state.bound_player_ != -1);

      const int current_player{state.current_player_};
      const int next_player{state.nextPlayer()};
      
      state.bound_turn_--;
      state.giveCards(state.current_player_, 1);
      state.prev_player_ = current_player;
      state.current_player_ = next_player;

      if (state.bound_turn_ <= 0) {
        state.bound_player_ = -1;
      }
    }

    return state;
  }

 private:
  int bound_player_{-1};
  int bound_turn_{};

  /* 共通処理を実施した状態に、白いワイルドの効果を反映させる。 */
  UnoStateBind2 nextWhenWildCustomizableSubmission(UnoStateBind2& state) const {
    const int current_player{current_player_};
    const int next_player{nextPlayer()};

    state.current_move_type_ = MoveType::kSubmission;
    state.prev_player_ = current_player;
    state.current_player_ = next_player;
    state.bound_player_ = next_player;
    state.bound_turn_ = 2;

    return state;
  }
};

#endif // UNO_STATE_BIND2_HPP_

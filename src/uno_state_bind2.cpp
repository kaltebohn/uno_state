#include "uno_state_bind2.hpp"

UnoStateBind2 UnoStateBind2::next(const Move& move) const {
  /* 既に上がっていたら状態遷移しない。 */
  if (isFinished()) { return *this; }

  /* 白いワイルドが出ている場合、その効果処理だけして返す。 */
  if (std::holds_alternative<Submission>(move) &&
      (std::get<Submission>(move).getCard() == Card::kWildCustomizable)) {
    const Submission submission{std::get<Submission>(move)};
    UnoStateBind2 state{*this};

    state.last_move_ = move;
    state.acceptSubmission(submission);
    return nextWhenWildCustomizableSubmission(state);
  }

  /* バインド2以外の効果処理を進める。ここではバインド2を受けたプレイヤの手番になっていないはず。 */
  assert(current_player_ != bound_player_);
  UnoStateBind2 state{UnoState::next(move), bound_player_, bound_turn_};

  /* 次のプレイヤがバインド2を受けていたら、バインド2の効果を処理する。 */
  if (state.bound_turn_ > 0 &&
      (state.current_player_ == state.bound_player_) &&
      !state.isFinished()) {
    assert(state.bound_player_ != -1);

    const int current_player{state.current_player_};
    const int next_player{state.nextPlayer()};
 
    /* バインド2を受けているプレイヤにあり得る着手型は、提出かチャレンジのどちらか。 */
    /* 提出なら、強制的にカードを引かせ、チャレンジなら、チャレンジできない。 */
    if (state.current_move_type_ == MoveType::kChallenge) {
      state.giveCards(state.current_player_, 4);
      state.current_move_type_ = MoveType::kSubmission;
      state.prev_player_ = current_player;
      state.current_player_ = next_player;
    } else if (state.current_move_type_ == MoveType::kSubmission) {
      state.giveCards(state.current_player_, 1);
      state.prev_player_ = current_player;
      state.current_player_ = next_player;
    } else {
      assert(false);
    }

    /* バインドのカウントを減らす。 */
    state.bound_turn_--;
    if (state.bound_turn_ <= 0) {
      state.bound_player_ = -1;
    }
  }

  return state;
}

std::string UnoStateBind2::toString() const {
  std::string result{((UnoState)*this).toString()};

  result += "バインドされたプレイヤ\n";
  result += "  ";
  result += std::to_string(bound_player_);
  result += "\n";

  result += "バインドされる残りターン数\n";
  result += "  ";
  result += std::to_string(bound_turn_);
  result += "\n";

  return result;
}

std::string UnoStateBind2::toJSON() const {
  std::string result{};

  result += "{\"deck\":";
  if (deck_.size() == 0) {
    result += "\"Empty\"";
  } else {
    result += "[";
    for (const Card card : deck_) {
      result += '"' + card.toString() + '"' + ",";
    }
    result.pop_back();
    result += "]";
  }
  result += ",";

  result += "\"discards\":";
  if (discards_.size() == 0) {
    result += "\"Empty\"";
  } else {
    result += "[";
    for (const Card card : discards_) {
      result += '"' + card.toString() + '"' + ",";
    }
    result.pop_back();
    result += "]";
  }
  result += ",";

  result += "\"playerCards\":";
  result += "[";
  for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
    if (player_cards_.at(i).size() == 0) {
      result += "\"Empty\"";
    } else {
      result += "[";
      for (const Card card : player_cards_.at(i)) {
        result += '"' + card.toString() + '"' + ",";
      }
      result.pop_back();
      result += "]";
    }
    result += ",";
  }
  result.pop_back();
  result += "]";
  result += ",";

  result += "\"playerSeats\":";
  result += "[";
  for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
    result += std::to_string(player_seats_.at(i)) + ",";
  }
  result.pop_back();
  result += "]";
  result += ",";

  result += "\"playerScores\":";
  result += "[";
  for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
    result += std::to_string(player_scores_.at(i)) + ",";
  }
  result.pop_back();
  result += "]";
  result += ",";

  result += "\"currentMoveType\":";
  result += '"' + moveType2String(current_move_type_) + '"';
  result += ",";

  result += "\"prevPlayer\":";
  result += std::to_string(prev_player_);
  result += ",";

  result += "\"currentPlayer\":";
  result += std::to_string(current_player_);
  result += ",";

  result += "\"isNormalOrder?\":";
  result += std::to_string(is_normal_order_);
  result += ",";

  result += "\"tableColor\":";
  result += '"' + color2String(table_color_) + '"';
  result += ",";

  result += "\"tablePattern\":";
  result += '"' + cardPattern2String(table_pattern_) + '"';
  result += ",";

  result += "\"isChallengeValid?\":";
  result += std::to_string(is_challenge_valid_);
  result += ",";

  result += "\"drawnCard\":";
  result += '"' + drawn_card_.toString() + '"';
  result += ",";

  result += "\"lastMove\":";
  result += '"' + move2String(last_move_) + '"';
  result += ",";

  result += "\"addCards\":";
  result += "[";
  for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
    if (add_cards_.at(i).size() == 0) {
      result += "\"Empty\"";
    } else {
      result += "[";
      for (const Card card : add_cards_.at(i)) {
        result += '"' + card.toString() + '"' + ",";
      }
      result.pop_back();
      result += "]";
    }
    result += ",";
  }
  result.pop_back();
  result += "]";
  result += ",";

  result += "\"subCards\":";
  result += "[";
  for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
    if (sub_cards_.at(i).size() == 0) {
      result += "\"Empty\"";
    } else {
      result += "[";
      for (const Card card : sub_cards_.at(i)) {
        result += '"' + card.toString() + '"' + ",";
      }
      result.pop_back();
      result += "]";
    }
    result += ",";
  }
  result.pop_back();
  result += "]";
  result += ",";
  
  result += "\"boundPlayer\":";
  result += std::to_string(bound_player_);
  result += ",";

  result += "\"boundTurn\":";
  result += std::to_string(bound_turn_);

  result += "}";

  return result;
}

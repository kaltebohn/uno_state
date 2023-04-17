#include "uno_state.hpp"

std::string moveType2String(const MoveType move_type) {
  switch (move_type) {
    case MoveType::kSubmission:
      return "Submission";
    case MoveType::kSubmissionOfDrawnCard:
      return "SubmissionOfDrawCard";
    case MoveType::kColorChoice:
      return "ColorChoice";
    case MoveType::kChallenge:
      return "Challenge";
    default:
      assert(false);
  }
}

UnoState UnoState::next(Move move) const {
  /* 既に上がっていたら状態遷移しない。 */
  if (isFinished()) { return *this; }

  /* 自分のコピー。こいつの状態をどんどん書き換えて次状態にし、返す。 */
  UnoState state{*this};

  /* カードの効果に関する処理はすべてkSubmissionのときに行う。 */
  /* 他のイベントでは、着手への対応と現在プレイヤの変更だけする。 */
  if (current_move_type_ == MoveType::kColorChoice) {
    return nextWhenColorChoice(state, std::get<Color>(move));
  }

  if (current_move_type_ == MoveType::kChallenge) {
    return nextWhenChallenge(state, std::get<ChallengeFlag>(move));
  }

  if (current_move_type_ == MoveType::kSubmissionOfDrawnCard) {
    Card submission = std::get<Card>(move);

    if (submission.isEmpty()) {
      state.current_move_type_ = MoveType::kSubmission;
      state.prev_player_ = current_player_;
      state.current_player_ = nextPlayer();
      return state;
    } else {
      /* 着手が引いたカード(手札の最後)かつ合法でなければ、ペナルティ。 */
      if (submission != player_cards_.at(current_player_).back() ||
          !submission.isLegal(table_color_, table_pattern_)) {
        /* 着手が違法なら、ペナルティとして2枚カードを引く。 */
        state.giveCards(current_player_, 2);
        state.current_move_type_ = MoveType::kSubmission;
        state.prev_player_ = current_player_;
        state.current_player_ = nextPlayer();
        return state;
      } else {
        return nextWhenSubmission(state, std::get<Card>(move));
      }
    }
  }

  /* 以下カード提出の場合。 */
  assert(std::holds_alternative<Card>(move));
  return nextWhenSubmission(state, std::get<Card>(move));
}

UnoState UnoState::nextWhenColorChoice(UnoState& state, const Color color) const {
  /* 青緑赤黄以外は選べない。 */
  if (!(color == Color::kBlue || color == Color::kGreen || color == Color::kRed || color == Color::kYellow)) {
    return nextWhenIlligalColorChoice(state);
  }

  state.table_color_ = color;
  state.prev_player_ = current_player_;
  state.current_player_ = nextPlayer();

  /* ワイルドドロー4の場合、色選択後にチャレンジが発生する。 */
  if (std::holds_alternative<CardAction>(table_pattern_) && std::get<CardAction>(table_pattern_) == CardAction::kWildDraw4) {
    state.current_move_type_ = MoveType::kChallenge;
  } else {
    state.current_move_type_ = MoveType::kSubmission;
  }
  return state;
}

UnoState UnoState::nextWhenIlligalColorChoice(UnoState& state) const {
  const int current_player{current_player_};

  /* 前のプレイヤが最後に出したカード(ワイルドカード)を手札に戻す。 */
  state.returnCards(current_player, 1);

  /* 場をカードが出される前に戻す。 */
  state.table_color_ = state.discards_.back().getColor();
  state.table_pattern_ = state.discards_.back().getPattern();

  /* ペナルティを課す。 */
  state.giveCards(current_player_, 2);

  /* 手番を飛ばす。 */
  state.current_move_type_ = MoveType::kSubmission;
  state.current_player_ = nextPlayer();
  state.prev_player_ = current_player;

  return state;
}

UnoState UnoState::nextWhenChallenge(UnoState& state, const ChallengeFlag will_challenge) const {
  const int challenging_player = current_player_;
  const int challenged_player = prev_player_;

  /* チャレンジ後は必ずカード提出。 */
  state.current_move_type_ = MoveType::kSubmission;

  /* チャレンジしなかった場合、今のプレイヤに4枚引かせる。 */
  if (!will_challenge) {
    /* 次のプレイヤに手番を移す。 */
    state.prev_player_ = challenging_player;
    state.current_player_ = nextPlayerOf(challenging_player);

    state.giveCards(challenging_player, 4);

    return state;
  }

  /* チャレンジ成功判定。 */
  assert(discards_.size() >= 2); // チャレンジ時には、ワイルドドロー4の他にもう一枚捨て札があるはず。
  const Card& prev_table_card{discards_.at(discards_.size() - 2)};
  const bool is_challenge_valid = std::any_of(player_cards_.at(prev_player_).begin(), player_cards_.at(prev_player_).end(),
      [&prev_table_card](Card card) {
        return (!card.isEmpty() &&
                card.isLegal(prev_table_card.getColor(), prev_table_card.getPattern()) &&
                    (std::holds_alternative<CardNumber>(card.getPattern()) ||
                    std::get<CardAction>(card.getPattern()) != CardAction::kWildDraw4));
      });

  /* チャレンジが成功した場合、前のプレイヤが出したカードを戻し、4枚引かせ、手番を戻す。 */
  if (is_challenge_valid) {
    /* 前のプレイヤのターンに戻す。 */
    state.current_player_ = challenged_player;
    state.prev_player_ = challenging_player;

    /* 前のプレイヤが最後に出したカード(ワイルドカード)を手札に戻す。 */
    state.returnCards(challenged_player, 1);

    /* 場をカードが出される前に戻す。 */
    state.table_color_ = state.discards_.back().getColor();
    state.table_pattern_ = state.discards_.back().getPattern();

    /* 前のプレイヤに手札を4枚引かせる。 */
    state.giveCards(challenged_player, 4);
    // TODO: ALGORIに合わせるなら、must_play_card_validみたいなフラグがいる。
    return state;
  }

  /* チャレンジが失敗した場合、今のプレイヤに6枚引かせる。 */
  state.giveCards(challenging_player, 6);

  /* 次のプレイヤに手番を移す。 */
  state.prev_player_ = challenging_player;
  state.current_player_ = nextPlayerOf(challenging_player);

  return state;
}

UnoState UnoState::nextWhenSubmission(UnoState&state, const Card& submission) const {
  if (!submission.isLegal(table_color_, table_pattern_)) {
    return nextWhenIlligalSubmission(state);
  }

  if (submission.isEmpty()) {
    return nextWhenEmptyCardSubmission(state);
  }

  /* カードを場に出す。 */
  state.acceptSubmission(submission);

  /* 記号カードでなければここで終わり。 */
  if (!std::holds_alternative<CardAction>(submission.getPattern())) {
    state.current_move_type_ = MoveType::kSubmission;
    state.current_player_ = nextPlayer();
    state.prev_player_ = current_player_;
    return state;
  }

  const CardAction submission_action = std::get<CardAction>(submission.getPattern());
  if (submission_action == CardAction::kDrawTwo) {
    return nextWhenDrawTwoSubmission(state);
  } else if (submission_action == CardAction::kReverse) {
    return nextWhenReverseSubmission(state);
  } else if (submission_action == CardAction::kSkip) {
    return nextWhenSkipSubmission(state);
  } else if (submission_action == CardAction::kWild) {
    return nextWhenWildSubmission(state);
  } else if (submission_action == CardAction::kWildCustomizable) {
    return nextWhenWildCustomizableSubmission(state);
  } else if (submission_action == CardAction::kWildDraw4) {
    return nextWhenWildDraw4Submission(state);
  } else if (submission_action == CardAction::kWildShuffleHands) {
    return nextWhenWildShuffleHandsSubmission(state);
  }

  assert(false);
}

UnoState UnoState::nextWhenIlligalSubmission(UnoState& state) const {
  const int current_player{current_player_};
  state.current_move_type_ = MoveType::kSubmission;
  state.giveCards(current_player_, 2);
  state.current_player_ = nextPlayer();
  state.prev_player_ = current_player;
  return state;
}

UnoState UnoState::nextWhenEmptyCardSubmission(UnoState& state) const {
  const int current_player{current_player_};
  state.current_move_type_ = MoveType::kSubmissionOfDrawnCard;
  state.giveCards(current_player, 1);
  return state;
}

/* 合法手submissionを現在のプレイヤが出した場合の、カードの効果以外の処理を行う。 */
void UnoState::acceptSubmission(const Card& submission) {
  /* カードを場に出し、プレイヤの手札から除く。 */
  discardCard(current_player_, submission);

  /* 上がったプレイヤがいたら点数を付ける。 */
  if (isFinished()) {
    scoreToPlayers();
  }
}

std::vector<Move> UnoState::legalActions() const {
  if (isFinished()) {
    return {};
  }

  if (current_move_type_ == MoveType::kSubmission) {
    std::vector<Card> submissions{legalCards()};
    std::vector<Move> result(submissions.size());
    std::transform(submissions.begin(), submissions.end(), result.begin(),
        [](Card submission) {
          return (Move)submission;
        });
    return result;
  } else if (current_move_type_ == MoveType::kSubmissionOfDrawnCard) {
    const Card submission_of_drawn_card{player_cards_.at(current_player_).back()};
    std::vector<Card> submissions{Card{}};
    if (submission_of_drawn_card.isLegal(table_color_, table_pattern_)) {
      submissions.push_back(submission_of_drawn_card);
    }

    std::vector<Move> result(submissions.size());
    std::transform(submissions.begin(), submissions.end(), result.begin(),
        [](Card submission) {
          return (Move)submission;
        });

    return result;
  } else if (current_move_type_ == MoveType::kColorChoice) {
    return {Color::kBlue, Color::kGreen, Color::kRed, Color::kYellow};
  } else if (current_move_type_ == MoveType::kChallenge) {
    return {true, false};
  }

  assert(false);
}

std::vector<Card> UnoState::legalCards() const {
  std::vector<Card> result{Card{}}; // 空のカードは必ず選択肢に含める。
  for (const Card& card : player_cards_.at(current_player_)) {
    if (card.isLegal(table_color_, table_pattern_)) {
      result.push_back(card);
    }
  }
  return result;
};

std::string UnoState::toString() const {
  std::string result{};

  result += "Deck\n";
  result += "  ";
  if (deck_.size() == 0) {
    result += "Empty";
  } else {
    for (const Card card : deck_) {
      result += card.toString() + " ";
    }
  }
  result += "\n";

  result += "Discards\n";
  result += "  ";
  if (discards_.size() == 0) {
    result += "Empty";
  } else {
    for (const Card card : discards_) {
      result += card.toString() + " ";
    }
  }
  result += "\n";

  result += "PlayerCards\n";
  for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
    result += "  ";
    if (player_cards_.at(i).size() == 0) {
      result += "Empty";
    } else {
      for (const Card card : player_cards_.at(i)) {
        result += card.toString() + " ";
      }
    }
    result += "\n";
  }

  result += "PlayerSeats\n";
  result += "  ";
  for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
    result += std::to_string(player_seats_.at(i)) + " ";
  }
  result += "\n";

  result += "PlayerScores\n";
  result += "  ";
  for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
    result += std::to_string(player_scores_.at(i)) + " ";
  }
  result += "\n";

  result += "CurrentMoveType\n";
  result += "  ";
  result += moveType2String(current_move_type_);
  result += "\n";

  result += "PrevPlayer\n";
  result += "  ";
  result += std::to_string(prev_player_);
  result += "\n";

  result += "CurrentPlayer\n";
  result += "  ";
  result += std::to_string(current_player_);
  result += "\n";

  result += "IsNormalOrder?\n";
  result += "  ";
  result += std::to_string(is_normal_order_);
  result += "\n";

  result += "TableColor\n";
  result += "  ";
  result += color2String(table_color_);
  result += "\n";

  result += "TablePattern\n";
  result += "  ";
  result += cardPattern2String(table_pattern_);
  result += "\n";

  return result;
}

std::string UnoState::toJSON() const {
  std::string result{};

  result += "{\"deck\":";
  if (deck_.size() == 0) {
    result += "\"Empty\"";
  } else {
    result += "[";
    for (const Card card : deck_) {
      result += '"' + card.toString() + '"' + ",";
    }
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
      result += "]";
    }
    result += ",";
  }
  result += "]";
  result += ",";

  result += "\"playerSeats\":";
  result += "[";
  for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
    result += std::to_string(player_seats_.at(i)) + ",";
  }
  result += "]";
  result += ",";

  result += "\"playerScores\":";
  result += "[";
  for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
    result += std::to_string(player_scores_.at(i)) + ",";
  }
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

  result += "}";

  return result;
}

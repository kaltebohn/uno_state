#include "uno_state.hpp"

std::string moveType2String(const MoveType move_type) {
  switch (move_type) {
    case MoveType::kSubmission:
      return "提出";
    case MoveType::kSubmissionOfDrawnCard:
      return "引いたカードの提出";
    case MoveType::kColorChoice:
      return "色の選択";
    case MoveType::kChallenge:
      return "チャレンジ";
    default:
      return "不適切な着手型";
  }
}

UnoState UnoState::next(Move move) const {
  /* 既に上がっていたら状態遷移しない。 */
  if (isFinished()) { return *this; }

  /* 自分のコピー。こいつの状態をどんどん書き換えて次状態にし、返す。 */
  UnoState state{*this};

  state.last_move_ = move;

  /* 現状態に遷移する際発生した各プレイヤの手札の差分をリセット。 */
  state.add_cards_.fill({});
  state.sub_cards_.fill({});

  /* カードの効果に関する処理はすべてkSubmissionのときに行う。 */
  /* 他のイベントでは、着手への対応と現在プレイヤの変更だけする。 */
  if (current_move_type_ == MoveType::kColorChoice) {
    return nextWhenColorChoice(state, std::get<Color>(move));
  }

  if (current_move_type_ == MoveType::kChallenge) {
    return nextWhenChallenge(state, std::get<ChallengeFlag>(move));
  }

  if (current_move_type_ == MoveType::kSubmissionOfDrawnCard) {
    state.drawn_card_ = Card{}; // drawn_card_をチャラにする。
    Submission submission = std::get<Submission>(move);
    Card card = submission.getCard();

    if (card.isEmpty()) {
      state.current_move_type_ = MoveType::kSubmission;
      state.prev_player_ = current_player_;
      state.current_player_ = nextPlayer();
      return state;
    } else {
      if (card != drawn_card_ || !submission.isLegal(table_color_, table_pattern_)) {
        /* 着手が違法なら、ペナルティとして2枚カードを引く。 */
        state.giveCards(current_player_, 2);
        state.current_move_type_ = MoveType::kSubmission;
        state.prev_player_ = current_player_;
        state.current_player_ = nextPlayer();
        return state;
      } else {
        return nextWhenSubmission(state, std::get<Submission>(move));
      }
    }
  }

  /* 以下カード提出の場合。 */
  assert(std::holds_alternative<Submission>(move));
  return nextWhenSubmission(state, std::get<Submission>(move));
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
    state.is_challenge_valid_ = false; // フラグを戻す。

    /* 次のプレイヤに手番を移す。 */
    state.prev_player_ = challenging_player;
    state.current_player_ = nextPlayerOf(challenging_player);

    state.giveCards(challenging_player, 4);

    return state;
  }

  /* チャレンジが成功した場合、前のプレイヤが出したカードを戻し、4枚引かせ、手番を戻す。 */
  if (is_challenge_valid_) {
    state.is_challenge_valid_ = false; // フラグを戻す。

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

UnoState UnoState::nextWhenSubmission(UnoState&state, const Submission& submission) const {
  const Card card = submission.getCard();

  if (!submission.isLegal(table_color_, table_pattern_)) {
    return nextWhenIlligalSubmission(state);
  }

  if (submission.getCard().isEmpty()) {
    return nextWhenEmptyCardSubmission(state);
  }

  /* カードを場に出す。 */
  state.acceptSubmission(submission);

  /* 記号カードでなければここで終わり。 */
  if (!std::holds_alternative<CardAction>(card.getPattern())) {
    state.current_move_type_ = MoveType::kSubmission;
    state.current_player_ = nextPlayer();
    state.prev_player_ = current_player_;
    return state;
  }

  const CardAction submission_action = std::get<CardAction>(card.getPattern());
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
  state.drawn_card_ = state.player_cards_.at(current_player).back();
  return state;
}

/* 合法手submissionを現在のプレイヤが出した場合の、カードの効果以外の処理を行う。 */
void UnoState::acceptSubmission(const Submission& submission) {
  /* カードを場に出し、プレイヤの手札から除く。 */
  const Card card = submission.getCard();
  discardCard(current_player_, card);

  /* 合理的なプレイヤはUNO宣言忘れを必ず指摘するので、このクラスでは自動でペナルティを課す。 */
  if (player_cards_.at(current_player_).size() == 1 && !submission.getShouldYellUNO()) {
    giveCards(current_player_, 2);
  }

  /* 上がったプレイヤがいたら点数を付ける。 */
  if (isFinished()) {
    scoreToPlayers();
  }
}

std::vector<Move> UnoState::legalMoves() const {
  if (isFinished()) {
    return {};
  }

  if (current_move_type_ == MoveType::kSubmission) {
    std::vector<Submission> submissions{legalSubmissions()};
    std::vector<Move> result(submissions.size());
    std::transform(submissions.begin(), submissions.end(), result.begin(),
        [](Submission submission) {
          return (Move)submission;
        });
    return result;
  } else if (current_move_type_ == MoveType::kSubmissionOfDrawnCard) {
    const bool should_yell_UNO{currentPlayerShouldYellUNO()};

    assert(!drawn_card_.isEmpty()); // ここはカードを引いた際に移る処理だから、drawn_card_は空になり得ない。
    const Submission submission_of_drawn_card{drawn_card_, should_yell_UNO};
    std::vector<Submission> submissions{Submission{Card{}, should_yell_UNO}};
    if (submission_of_drawn_card.isLegal(table_color_, table_pattern_)) {
      submissions.push_back(submission_of_drawn_card);
    }

    std::vector<Move> result(submissions.size());
    std::transform(submissions.begin(), submissions.end(), result.begin(),
        [](Submission submission) {
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

std::vector<Submission> UnoState::legalSubmissions() const {
  const bool should_yell_UNO{currentPlayerShouldYellUNO()};
  std::vector<Submission> result{{Card{}, false}}; // 空のカードは必ず選択肢に含める。
  for (const Card& card : player_cards_.at(current_player_)) {
    Submission submission{card, should_yell_UNO};
    if (submission.isLegal(table_color_, table_pattern_)) {
      result.push_back(submission);
    }
  }
  return result;
};

std::string UnoState::toString() const {
  std::string result{};

  result += "山札\n";
  result += "  ";
  for (const Card card : deck_) {
    result += card.toString() + " ";
  }
  result += "\n";
  result += "\n";

  result += "捨札\n";
  result += "  ";
  for (const Card card : discards_) {
    result += card.toString() + " ";
  }
  result += "\n";
  result += "\n";

  result += "プレイヤの手札\n";
  for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
    result += "  ";
    for (const Card card : player_cards_.at(i)) {
      result += card.toString() + " ";
    }
    result += "\n";
  }
  result += "\n";

  result += "プレイヤの席\n";
  result += "  ";
  for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
    result += std::to_string(player_seats_.at(i)) + " ";
  }
  result += "\n";
  result += "\n";

  result += "プレイヤの得点\n";
  result += "  ";
  for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
    result += std::to_string(player_scores_.at(i)) + " ";
  }
  result += "\n";
  result += "\n";

  result += "着手の型\n";
  result += "  ";
  result += moveType2String(current_move_type_);
  result += "\n";
  result += "\n";

  result += "前のプレイヤ\n";
  result += "  ";
  result += std::to_string(prev_player_);
  result += "\n";
  result += "\n";

  result += "現在のプレイヤ\n";
  result += "  ";
  result += std::to_string(current_player_);
  result += "\n";
  result += "\n";

  result += "正順？\n";
  result += "  ";
  result += std::to_string(is_normal_order_);
  result += "\n";
  result += "\n";

  result += "場の色\n";
  result += "  ";
  result += color2String(table_color_);
  result += "\n";
  result += "\n";

  result += "場の模様\n";
  result += "  ";
  result += cardPattern2String(table_pattern_);
  result += "\n";
  result += "\n";

  result += "チャレンジが成功する？\n";
  result += "  ";
  result += std::to_string(is_challenge_valid_);
  result += "\n";
  result += "\n";

  result += "直前にプレイヤが引いたカード\n";
  result += "  ";
  result += drawn_card_.toString();
  result += "\n";
  result += "\n";

  result += "直前の行動\n";
  result += "  ";
  result += move2String(last_move_);;
  result += "\n";
  result += "\n";

  result += "遷移時にプレイヤに追加されたカード\n";
  for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
    result += "  ";
    for (const Card card : add_cards_.at(i)) {
      result += card.toString() + " ";
    }
    result += "\n";
  }
  result += "\n";

  result += "遷移時にプレイヤから削除されたカード\n";
  for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
    result += "  ";
    for (const Card card : sub_cards_.at(i)) {
      result += card.toString() + " ";
    }
    result += "\n";
  }
  result += "\n";

  return result;
}

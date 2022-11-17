#include "uno_state.hpp"

UnoState UnoState::next(Move move) const {
  /* カードの効果に関する処理はすべてkSubmissionのときに行う。 */
  /* 他のイベントでは、着手への対応と現在プレイヤの変更だけする。 */
  if (current_event_ == MoveType::kColorChoice) {
    return nextWhenColorChoice(std::get<Color>(move));
  }

  if (current_event_ == MoveType::kChallenge) {
    return nextWhenChallenge(std::get<ChallengeFlag>(move));
  }

  if (current_event_ == MoveType::kSubmissionOfDrawnCard) {
    UnoState next_state{*this};
    Submission submission = std::get<Submission>(move);
    Card card = submission.getCard();

    if (submission.getCard().isEmpty()) {
      next_state.current_player_ = nextPlayer();
    } else {
      if (submission.getCard() != drawn_card_ || !submission.isLegal(table_color_, table_pattern_)) {
        next_state.giveCards(current_player_, 2);
      } else {
        return nextWhenSubmission(std::get<Submission>(move));
      }
    }
  }

  /* 以下カード提出の場合。 */
  return nextWhenSubmission(std::get<Submission>(move));
}

UnoState UnoState::nextWhenColorChoice(const Color color) const {
  UnoState next_state{*this};
  next_state.table_color_ = color;
  next_state.prev_player_ = current_player_;
  next_state.current_player_ = nextPlayer();

  /* ワイルドドロー4の場合、色選択後にチャレンジが発生する。 */
  if (std::get<CardAction>(table_pattern_) == CardAction::kWildDraw4) {
    next_state.current_event_ = MoveType::kChallenge;
  } else {
    next_state.current_event_ = MoveType::kSubmission;
  }
  return next_state;
}

UnoState UnoState::nextWhenChallenge(const ChallengeFlag will_challenge) const {
  UnoState next_state{*this};

  const int challenging_player = current_player_;
  const int challenged_player = prev_player_;

  /* チャレンジ後は必ずカード提出。 */
  next_state.current_event_ = MoveType::kSubmission;

  /* チャレンジしなかった場合、今のプレイヤに4枚引かせる。 */
  if (!will_challenge) {
    next_state.giveCards(challenging_player, 4);
    return next_state;
  }

  /* チャレンジが成功した場合、前のプレイヤが出したカードを戻し、4枚引かせ、手番を戻す。 */
  if (is_challenge_valid_) {
    next_state.is_challenge_valid_ = false; // フラグを戻す。

    /* 前のプレイヤのターンに戻す。 */
    next_state.current_player_ = challenged_player;

    /* 前のプレイヤが最後に出したカード(ワイルドカード)を手札に戻す。 */
    next_state.player_cards_.at(challenged_player).push_back(next_state.discards_.back()); next_state.discards_.pop_back();

    /* 場をカードが出される前に戻す。 */
    next_state.table_color_ = next_state.discards_.front().getColor();
    next_state.table_pattern_ = next_state.discards_.front().getPattern();

    /* 前のプレイヤに手札を4枚引かせる。 */
    next_state.giveCards(challenged_player, 4);
    // TODO: ALGORIに合わせるなら、must_play_card_validみたいなフラグがいる。
    return next_state;
  }

  /* チャレンジが失敗した場合、今のプレイヤに6枚引かせる。 */
  next_state.giveCards(challenging_player, 6);
  return next_state;
}

UnoState UnoState::nextWhenSubmission(const Submission& submission) const {
  UnoState next_state{*this};
  const Card card = submission.getCard();
  const int current_player = current_player_;

  /* カードが空なら、ドロー */
  if (submission.getCard().isEmpty()) {
    next_state.current_event_ = MoveType::kSubmissionOfDrawnCard;
    next_state.giveCards(current_player, 1);
    next_state.drawn_card_ = next_state.player_cards_.at(current_player).back();
    return next_state;
  }

  /* 合法手でなければ2枚引かせて手番を飛ばす。 */
  if (!submission.isLegal(table_color_, table_pattern_)) {
    next_state.current_event_ = MoveType::kSubmission;
    next_state.giveCards(current_player_, 2);
    next_state.current_player_ = nextPlayer();
    next_state.prev_player_ = current_player;
    return next_state;
  }

  /* カードを場に出す。 */
  next_state.discards_.push_back(card);
  next_state.player_cards_.at(current_player).push_back(card);
  next_state.table_color_ = card.getColor();
  next_state.table_pattern_ = card.getPattern();

  /* 記号カードでなければここで終わり。 */
  if (!std::holds_alternative<CardAction>(card.getPattern())) {
    next_state.current_event_ = MoveType::kSubmission;
    next_state.current_player_ = nextPlayer();
    return next_state;
  }

  const CardAction submission_action = std::get<CardAction>(card.getPattern());
  if (submission_action == CardAction::kDrawTwo) {
    next_state.current_event_ = MoveType::kSubmission;
    next_state.giveCards(nextPlayer(), 2);
    next_state.current_player_ = nextPlayerOf(nextPlayerOf(current_player));
  } else if (submission_action == CardAction::kReverse) {
    next_state.current_event_ = MoveType::kSubmission;
    next_state.is_normal_order_ != is_normal_order_;
    next_state.current_player_ = next_state.nextPlayer();
  } else if (submission_action == CardAction::kSkip) {
    next_state.current_event_ = MoveType::kSubmission;
    next_state.current_player_ = nextPlayerOf(nextPlayerOf(current_player));
  } else if (submission_action == CardAction::kWild) {
    next_state.current_event_ = MoveType::kColorChoice;
    next_state.current_player_ = nextPlayer();
  } else if (submission_action == CardAction::kWildCustomizable) {
    // TODO
  } else if (submission_action == CardAction::kWildDraw4) {
    next_state.current_event_ = MoveType::kColorChoice;
    next_state.current_player_ = nextPlayer();
    const auto legal_moves = legalMoves();
    next_state.is_challenge_valid_ = (std::any_of(legal_moves.begin(), legal_moves.end(),
        [](Move move) {
          const CardPattern pattern = std::get<Submission>(move).getCard().getPattern();
          return std::get<CardAction>(pattern) != CardAction::kWildDraw4;
        }));
  } else if (submission_action == CardAction::kWildShuffleHands) {
    const int next_player = nextPlayer();
    next_state.current_event_ = MoveType::kColorChoice;
    next_state.current_player_ = next_player;

    std::vector<Card> collected_cards;
    for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
      std::copy(next_state.player_cards_.at(i).begin(), next_state.player_cards_.at(i).end(), collected_cards.begin());
      next_state.player_cards_.at(i).clear();
    }

    int player_to_give = next_player;
    for (const Card& card : collected_cards) {
      next_state.player_cards_.at(player_to_give).push_back(card);
      player_to_give = nextPlayerOf(player_to_give);
    }
  }

  return next_state;
}

std::vector<Move> UnoState::legalMoves() const {
  if (current_event_ == MoveType::kSubmission) {
    std::vector<Submission> submissions{legalSubmissions()};
    std::vector<Move> result;
    std::transform(submissions.begin(), submissions.end(), result.begin(),
        [](Submission submission) {
          return (Move)(submission);
        });
    return result;
  } else if (current_event_ == MoveType::kColorChoice) {
    return {Color::kBlue, Color::kGreen, Color::kRed, Color::kYellow};
  } else if (current_event_ == MoveType::kChallenge) {
    return {true, false};
  } else {
    std::cerr << "到達し得ない行に処理が移りました: UnoState::legalMoves。" << std::endl;
    std::exit(1);
  }
}

std::vector<Submission> UnoState::legalSubmissions() const {
  std::vector<Submission> result;
  for (const Card& card : player_cards_.at(current_player_)) {
    Submission submission{card, currentPlayerShouldYellUNO()};
    if (submission.isLegal(table_color_, table_pattern_)) {
      result.push_back(submission);
    }
  }
  return result;
};

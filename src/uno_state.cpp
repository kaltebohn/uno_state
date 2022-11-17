#include "uno_state.hpp"

UnoState UnoState::next(Move move) const {
  /* カードの効果に関する処理はすべてkSubmissionのときに行う。 */
  /* 他のイベントでは、着手への対応と現在プレイヤの変更だけする。 */
  if (current_event_ == MoveType::kColorChoice) {
    UnoState next_state{*this};
    next_state.table_color_ = std::get<Color>(move);
    next_state.current_player_ = nextPlayer();
    /* ワイルドドロー4の場合、色選択後にチャレンジが発生する。 */
    if (std::get<CardAction>(table_pattern_) == CardAction::kWildDraw4) {
      next_state.current_event_ = MoveType::kChallenge;
    } else {
      next_state.current_event_ = MoveType::kSubmission;
    }
    return next_state;
  }

  if (current_event_ == MoveType::kChallenge) {
    /* チャレンジしなかった場合。 */
    if (!std::get<ChallengeFlag>(move)) {
      UnoState next_state{*this};
      next_state.giveCards(next_state.current_player_, 4);
      return next_state;
    }

    /* チャレンジが成功した場合。 */
    if (is_challenge_valid_) {
      /* 前のプレイヤのターンに戻す。 */
      UnoState next_state{*this};
      next_state.current_player_ = prev_player_;

      /* 最後に出したカード(ワイルドカード)を手札に戻す。 */
      next_state.player_cards_.at(prev_player_).push_back(next_state.discards_.back()); next_state.discards_.pop_back();

      /* 場をカードが出される前に戻す。 */
      next_state.table_color_ = next_state.discards_.front().getColor();
      next_state.table_pattern_ = next_state.discards_.front().getPattern();

      /* 前のプレイヤに手札を4枚引かせる。 */
      next_state.giveCards(next_state.current_player_, 4);
      // TODO: ALGORIに合わせるなら、must_play_card_validみたいなフラグがいる。
      return next_state;
    }

    /* チャレンジが失敗した場合。 */
    UnoState next_state{*this};
    next_state.giveCards(next_state.current_player_, 6);
    return next_state;
  }

  /* 以下カード提出の場合。 */
  // TODO: カードを引く場合。

  UnoState next_state{*this};
  Submission submission = std::get<Submission>(move);
  Card card = submission.getCard();

  /* 合法手でなければ2枚引かせて手番を飛ばす。 */
  if (!submission.isLegal(table_color_, table_pattern_)) {
    next_state.giveCards(current_player_, 2);
    next_state.current_player_ = nextPlayer();
    return next_state;
  }

  /* カードを場に出す。 */
  next_state.discards_.push_back(card);
  next_state.player_cards_.at(current_player_).push_back(card);
  next_state.table_color_ = card.getColor();
  next_state.table_pattern_ = card.getPattern();

  /* 記号カードでなければここで終わり。 */
  if (!std::holds_alternative<CardAction>(card.getPattern())) {
    next_state.current_player_ = nextPlayer();
    return next_state;
  }

  const CardAction submission_action = std::get<CardAction>(card.getPattern());
  if (submission_action == CardAction::kDrawTwo) {
    next_state.giveCards(nextPlayer(), 2);
    next_state.current_player_ = next_state.nextPlayer();
    next_state.current_player_ = next_state.nextPlayer();
  } else if (submission_action == CardAction::kReverse) {
    next_state.is_normal_order_ != is_normal_order_;
    next_state.current_player_ = next_state.nextPlayer();
  } else if (submission_action == CardAction::kSkip) {
    next_state.current_player_ = next_state.nextPlayer();
    next_state.current_player_ = next_state.nextPlayer();
  } else if (submission_action == CardAction::kWild) {
    next_state.current_event_ = MoveType::kColorChoice;
    next_state.current_player_ = next_state.nextPlayer();
  } else if (submission_action == CardAction::kWildCustomizable) {
    // TODO
  } else if (submission_action == CardAction::kWildDraw4) {
    next_state.current_event_ = MoveType::kColorChoice;
    next_state.current_player_ = next_state.nextPlayer();
    auto legal_moves = legalMoves();
    if (std::any_of(legal_moves.begin(), legal_moves.end(),
        [](Move move) {
          const CardPattern pattern = std::get<Submission>(move).getCard().getPattern();
          return std::get<CardAction>(pattern) != CardAction::kWildDraw4;
        })) {
      next_state.is_challenge_valid_ = true;
    }
  } else if (submission_action == CardAction::kWildShuffleHands) {
    next_state.current_event_ = MoveType::kColorChoice;
    next_state.current_player_ = next_state.nextPlayer();

    std::vector<Card> collected_cards;
    for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
      std::copy(next_state.player_cards_.at(i).begin(), next_state.player_cards_.at(i).end(), collected_cards.begin());
      next_state.player_cards_.at(i).clear();
    }

    int player_to_give = next_state.current_player_;
    for (const Card& card : collected_cards) {
      next_state.player_cards_.at(player_to_give).push_back(card);
      player_to_give = nextOf(player_to_give);
    }
  }
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

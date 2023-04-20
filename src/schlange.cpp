#include "schlange.hpp"

Action Schlange::nextAction(const Observation& observation) {
  if (observation.current_action_type_ == ActionType::kChallenge) {
    /* チャレンジは常にしない。 */
    return false;
  } else if (observation.current_action_type_ == ActionType::kColorChoice) {
    /* 手札中の一番多い色を選ぶ。 */
    return ModeOfColorsInHand(observation.player_card_);
  } else if (observation.current_action_type_ == ActionType::kSubmission) {
    /* 評価関数が最大値を与えるものを選ぶ。 */
    Cards legal_actions{};
    std::transform(observation.legal_actions_.begin(), observation.legal_actions_.end(),
        std::back_inserter(legal_actions),
        [](const Action& action) { return std::get<Card>(action); });
    return selectSubmission(legal_actions, observation.table_color_, observation.table_pattern_);
  } else if (observation.current_action_type_ == ActionType::kSubmissionOfDrawnCard) {
    /* 引いたカードが出せるなら(合法手中にパス以外のカードがあるなら)必ず出す。 */
    for (const auto& submission : observation.legal_actions_) {
      if (!std::get<Card>(submission).isEmpty()) { return submission; }
    }
    return observation.legal_actions_.at(0);
  } else {
    /* ActionTypeの定義上ここに到達することはないはず。 */
    assert(false);
  }
}

Color Schlange::ModeOfColorsInHand(const Cards& hand) {
  std::unordered_map<Color, int> color_freq{};
  for (const auto& card : hand) {
    const Color color{card.getColor()};
    /* 無色(空のカード)、ワイルドは色として選ばない。 */
    if (color == Color::kNull || color == Color::kWild) { continue; }

    color_freq[card.getColor()]++;
  }
  const auto max_iter = std::max_element(color_freq.begin(), color_freq.end(),
      [](const auto& a, const auto& b) { return a.second < b.second; });

  /* 手札に空のカードとワイルド系しかない場合は、とりあえず赤を返す。 */
  return max_iter == color_freq.end() ? Color::kRed : max_iter->first;
}

int Schlange::evaluateSubmission(const Card& card) {
  if (card.isEmpty()) {
    /* 空のカードは基本的に出さない。 */
    return -10000;
  } else if (std::holds_alternative<CardNumber>(card.getPattern())) {
    /* 数字カードなら、(書かれている数字)を評価値とする。 */
    return cardNumber2Int(std::get<CardNumber>(card.getPattern()));
  } else if (card.getColor() != Color::kWild) {
    /* ワイルド以外の記号カードなら、20点を評価値とする。 */
    return 20;
  } else {
    /* ワイルド系なら、ワイルドは-40、ワイルドドロー4は-100、他は-50とする。 */
    assert(std::holds_alternative<CardAction>(card.getPattern()));
    const CardAction action{std::get<CardAction>(card.getPattern())};
    switch (action) {
      case CardAction::kWild:
        return -40;
      case CardAction::kWildDraw4:
        return -100;
      default:
        return -50;
    }
  }
}

Card Schlange::selectSubmission(const Cards& submissions, const Color& table_color, const CardPattern& table_pattern) {
  assert(submissions.size() > 0); // 合法手には必ずパスが入っているので、長さ1以上のはず。
  if (submissions.size() == 1) { return submissions.at(0); }

  /* 手持ちのカードを評価。 */
  std::vector<int> scores{};
  std::transform(submissions.begin(), submissions.end(), std::back_inserter(scores),
      [](const auto& card) { return evaluateSubmission(card); });
  const auto max_iter = std::max_element(scores.begin(), scores.end());
  return submissions.at(std::distance(scores.begin(), max_iter));
}

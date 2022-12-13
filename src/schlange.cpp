#include "schlange.hpp"

Color Schlange::changeColor() const {
  /* 手札中の一番多い色を選ぶ。 */
  int num_of_blue{}, num_of_green{}, num_of_red{}, num_of_yellow{};

  for (const Submission& submission : legal_submissions_) {
    const Color color{submission.getCard().getColor()};
    switch (color) {
      case Color::kBlue:
        num_of_blue++;
        break;
      case Color::kGreen:
        num_of_green++;
        break;
      case Color::kRed:
        num_of_red++;
        break;
      case Color::kYellow:
        num_of_yellow++;
        break;
      case Color::kWild:
        break;
      default:
        assert(false);
    }
  }

  if (num_of_blue >= num_of_green && num_of_blue >= num_of_red && num_of_blue >= num_of_yellow) {
    return Color::kBlue;
  } else if (num_of_green >= num_of_red && num_of_green >= num_of_yellow) {
    return Color::kGreen;
  } else if (num_of_red >= num_of_yellow) {
    return Color::kRed;
  } else {
    return Color::kYellow;
  }
}

Submission Schlange::submitCard() const {
  if (legal_submissions_.size() == 1) { return legal_submissions_.at(0); }

  const std::vector<Submission>& legal_submissions{legal_submissions_};

  /* 評価値で重みづけする。 */
  std::vector<int> scores(legal_submissions.size());
  int best_idx{0};
  int best_score{-1000000};
  for (unsigned i = 0; i < legal_submissions.size(); i++) {
    const Card card{legal_submissions.at(i).getCard()};
    if (card.isEmpty()) {
      /* 空のカードは基本的に出さない。 */
      scores.at(i) = -10000;
    } else if (std::holds_alternative<CardNumber>(card.getPattern())) {
      /* 数字カードなら、(書かれている数字)を評価値とする。 */
      scores.at(i) = cardNumber2Int(std::get<CardNumber>(card.getPattern()));
    } else if (card.getColor() != Color::kWild) {
      /* ワイルド以外の記号カードなら、20点を評価値とする。 */
      scores.at(i) = 20;
    } else {
      /* ワイルド系なら、ワイルドは-40、ワイルドドロー4は-100、他は-50とする。 */
      assert(std::holds_alternative<CardAction>(card.getPattern()));
      const CardAction action{std::get<CardAction>(card.getPattern())};
      switch (action) {
        case CardAction::kWild:
          scores.at(i) = -40;
          break;
        case CardAction::kWildDraw4:
          scores.at(i) = -100;
          break;
        default:
          scores.at(i) = -50;
          break;
      }
    }

    if (scores.at(i) > best_score) {
      best_idx = i;
      best_score = scores.at(i);
    }
  }

  return legal_submissions.at(best_idx);
}

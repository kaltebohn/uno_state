#include "schlange.hpp"

Move Schlange::move(const std::vector<Move>& legal_moves) const {
  if (legal_moves.size() == 1) { return legal_moves.at(0); }

  if (std::holds_alternative<Submission>(legal_moves.at(0))) {
    /* 評価値で重みづけする。 */
    std::vector<int> scores(legal_moves.size());
    int best_idx{0};
    int best_score{-1000000};
    for (unsigned i = 0; i < legal_moves.size(); i++) {
      assert(std::holds_alternative<Submission>(legal_moves.at(i)));
      const Card card{std::get<Submission>(legal_moves.at(i)).getCard()};
      if (card.isEmpty()) {
        /* 空のカードは基本的に出さない。 */
        scores.at(i) = -10000;
      } else if (std::holds_alternative<CardNumber>(card.getPattern())) {
        /* 数字カードなら、(書かれている数字)を評価値とする。 */
        assert(std::holds_alternative<CardNumber>(card.getPattern()));
        scores.at(i) = -cardNumber2Int(std::get<CardNumber>(card.getPattern()));
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

    return legal_moves.at(best_idx);
  } else if (std::holds_alternative<Color>(legal_moves.at(0))) {
    /* 手札中の一番多い色を選ぶ。 */
    int num_of_blue{}, num_of_green{}, num_of_red{}, num_of_yellow{};

    for (const Move move : legal_moves) {
      assert(std::holds_alternative<Color>(move));
      const Color color{std::get<Color>(move)};
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
        default:
          assert(false);
      }
    }

    if (num_of_blue >= num_of_green && num_of_blue >= num_of_red && num_of_blue >= num_of_yellow) {
      return (Move)(Color::kBlue);
    } else if (num_of_green >= num_of_red && num_of_green >= num_of_yellow) {
      return (Move)(Color::kGreen);
    } else if (num_of_red >= num_of_yellow) {
      return (Move)(Color::kRed);
    } else {
      return (Move)(Color::kYellow);
    }
  } else if (std::holds_alternative<ChallengeFlag>(legal_moves.at(0))) {
    /* チャレンジはしない。 */
    return (Move)false;
  }

  assert(false);
}

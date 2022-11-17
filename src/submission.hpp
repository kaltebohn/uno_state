#ifndef SUBMISSION_HPP_
#define SUBMISSION_HPP_

#include "card.hpp"

/* 提出されるカードをラップしたクラス。 */
class Submission {
 public:
  Submission(Card c, bool s) : card_(c), should_yell_UNO_(s) {}

  constexpr Card getCard() { return card_; }
  constexpr bool getShouldYellUNO() { return should_yell_UNO_; }

  /* この着手が合法か？ワイルドドロー4を反則で出す場合も合法として扱う。 */
  bool isLegal(Color table_color, CardPattern table_pattern) {
    if (card_.getColor() == Color::kNull || card_.getColor() == Color::kWild) { return true; }

    return card_.getColor() == table_color && card_.getPattern() == table_pattern;
  }

 private:
  Card card_{};
  bool should_yell_UNO_{};
};

#endif // SUBMISSION_HPP_

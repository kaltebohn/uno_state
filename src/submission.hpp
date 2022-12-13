#ifndef SUBMISSION_HPP_
#define SUBMISSION_HPP_

#include "card.hpp"

/* 提出されるカードをラップしたクラス。 */
class Submission {
 public:
  friend std::ostream& operator<<(std::ostream& os, const Submission& src) {
    os << src.toString();
    return os;
  }

  Submission() : card_(), should_yell_UNO_(false) {}

  Submission(Card c, bool s) : card_(c), should_yell_UNO_(s) {}

  constexpr Card getCard() const { return card_; }
  constexpr bool getShouldYellUNO() const { return should_yell_UNO_; }

  /* この着手が合法か？ワイルドドロー4を反則で出す場合も合法として扱う。 */
  bool isLegal(Color table_color, CardPattern table_pattern) const {
    if (card_.getColor() == Color::kNull || card_.getColor() == Color::kWild) { return true; }

    return card_.getColor() == table_color || card_.getPattern() == table_pattern;
  }

  constexpr bool operator ==(const Submission& submission) const {
    return card_ == submission.card_ && should_yell_UNO_ == submission.should_yell_UNO_;
  }

  constexpr bool operator !=(const Submission& submission) const { return *this == submission; }

  std::string toString() const {
    return card_.toString() + ", UNOコール" + (should_yell_UNO_ ? "有" : "無");
  }

 private:
  Card card_{};
  bool should_yell_UNO_{};
};

#endif // SUBMISSION_HPP_

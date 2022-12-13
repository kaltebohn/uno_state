#ifndef SCHLANGE_HPP_
#define SCHLANGE_HPP_

#include <algorithm>

#include "move.hpp"

class Schlange {
 public:
  void setMyCards(const Cards& cards) {
    my_cards_.clear();
    std::copy(cards.begin(), cards.end(), std::back_inserter(my_cards_));
    should_yell_UNO_ = my_cards_.size() == 2;
  }

  void setTableColor(const Color color) { table_color_ = color; }

  void setTablePattern(const CardPattern pattern) { table_pattern_ = pattern; }

  /* 合法手を列挙して保存。 */
  /* Schlangeでは出せるカードがあるときにパスをしないので、空のカードは含めない。 */
  void setLegalSubmissions() {
    const bool should_yell_UNO{should_yell_UNO_};

    legal_submissions_.clear();
    for (const Card& card : my_cards_) {
      Submission submission(card, should_yell_UNO);
      if (submission.isLegal(table_color_, table_pattern_)) {
        legal_submissions_.push_back(submission);
      }
    }
  }

  Color changeColor() const;

  bool willPlayDrawnCard(const Card& drawn_card) const {
    return Submission(drawn_card, should_yell_UNO_).isLegal(table_color_, table_pattern_);
  }

  bool willChallenge() const { return false; }

  bool willDraw() const { return legal_submissions_.size() == 0; }

  bool shouldYellUNO() const { return should_yell_UNO_; }

  Submission submitCard() const;

 private:
  Cards my_cards_{};
  Color table_color_{};
  CardPattern table_pattern_{};
  std::vector<Submission> legal_submissions_{};
  bool should_yell_UNO_{};
};

#endif // SCHLANGE_HPP_

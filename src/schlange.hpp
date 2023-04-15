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

    legal_cards_.clear();
    for (const Card& card : my_cards_) {
      Card submission{card};
      if (submission.isLegal(table_color_, table_pattern_)) {
        legal_cards_.push_back(submission);
      }
    }
  }

  Color changeColor() const;

  bool willPlayDrawnCard(const Card& drawn_card) const {
    return Card(drawn_card).isLegal(table_color_, table_pattern_);
  }

  bool willChallenge() const { return false; }

  bool willDraw() const { return legal_cards_.size() == 0; }

  bool shouldYellUNO() const { return should_yell_UNO_; }

  Card submitCard() const;

 private:
  Cards my_cards_{};
  Color table_color_{};
  CardPattern table_pattern_{};
  std::vector<Card> legal_cards_{};
  bool should_yell_UNO_{};
};

#endif // SCHLANGE_HPP_

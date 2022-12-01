#include "uno_state_bind2.hpp"

class Schlange {
 public:
  void setMyCards(const Cards& cards) {
    my_cards_.clear();
    std::copy(cards.begin(), cards.end(), std::back_inserter(my_cards_));
  }

  Move move(const std::vector<Move>& legal_moves) const;

 private:
  Cards my_cards_{};
};

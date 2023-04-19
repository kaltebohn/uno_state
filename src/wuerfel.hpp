#ifndef WUERFEL_HPP_
#define WUERFEL_HPP_

#include "uno_state.hpp"

class Wuerfel {
 public:
  Wuerfel(XorShift64 random_engine) : random_engine_(random_engine) {}

  Move nextMove(const Observation& observation) {
    return randomAction(observation, random_engine_);
  }

  /* ランダムに着手を選択。モンテカルロ法などからも使うので、static関数として公開してやる。 */
  static Move randomAction(const Observation& observation, XorShift64& random_engine) {
    std::vector<Move> moves{observation.legal_actions_};
    assert(moves.size() > 0);
    if (moves.size() == 1) { return moves.at(0); } // 一手しかないなら、それを出す。

    /* UNOでカードを出す場合、本当にランダムな手を出させるとパスしまくって山札がなくなり得るので、空の手は除いておく。 */
    if (observation.current_move_type_ == MoveType::kSubmission) {
      moves.erase(std::find(moves.begin(), moves.end(), (Move)(Card{})));
    }

    std::uniform_int_distribution<int> dist(0, moves.size() - 1);
    return moves.at(dist(random_engine));
  }
 
 private:
  XorShift64 random_engine_;
};

#endif // WUERFEL_HPP_

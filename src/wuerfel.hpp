#ifndef WUERFEL_HPP_
#define WUERFEL_HPP_

#include "uno_state.hpp"

/* ランダムに行動する対戦プレイヤ。 */
class Wuerfel {
 public:
  Wuerfel(XorShift64 random_engine) : random_engine_(random_engine) {}

  /* 与えられた観測における次の手を決める。 */
  Action nextAction(const Observation& observation) {
    return randomAction(observation, random_engine_);
  }

  /* ランダムに着手を選択。モンテカルロ法などからも使うので、static関数として公開してやる。 */
  static Action randomAction(const Observation& observation, XorShift64& random_engine) {
    std::vector<Action> actions{observation.legal_actions_};
    assert(actions.size() > 0);
    if (actions.size() == 1) { return actions.at(0); } // 一手しかないなら、それを出す。

    /* UNOでカードを出す場合、本当にランダムな手を出させるとパスしまくって山札がなくなり得るので、空の手は除いておく。 */
    if (observation.current_action_type_ == ActionType::kSubmission) {
      actions.erase(std::find(actions.begin(), actions.end(), (Action)(Card{})));
    }

    std::uniform_int_distribution<int> dist(0, actions.size() - 1);
    return actions.at(dist(random_engine));
  }

 private:
  XorShift64 random_engine_;
};

#endif // WUERFEL_HPP_

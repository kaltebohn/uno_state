#include "schlange.hpp"
#include "uno_state_bind2.hpp"
#include "wuerfel.hpp"

int main(void) {
  std::random_device seed_gen;
  Schlange player{};
  std::vector<Wuerfel> opponents{};
  for (int i = 0; i < 3; i++) {
    opponents.push_back(Wuerfel(seed_gen()));
  }

  std::array<int, 4> sum_of_score{};
  for (int i = 0; i < 1000; i++) {
    UnoStateBind2 state{allCards(), seed_gen()};
    while (!state.isFinished()) {
      const int current_player{state.getCurrentPlayerNum()};
      if (current_player == 0) {
        state = state.next(player.nextAction(state.getObservation(current_player)));
      } else {
        Wuerfel& opponent{opponents.at(current_player - 1)};
        state = state.next(opponent.nextAction(state.getObservation(current_player)));
      }
    }
    sum_of_score.at(0) += state.getScore(0);
    sum_of_score.at(1) += state.getScore(1);
    sum_of_score.at(2) += state.getScore(2);
    sum_of_score.at(3) += state.getScore(3);
    std::cout << state.getScore(0) << "\t" << state.getScore(1) << "\t" << state.getScore(2) << "\t" << state.getScore(3) << std::endl;
  }
  std::cout << sum_of_score.at(0) << "\t" << sum_of_score.at(1) << "\t" << sum_of_score.at(2) << "\t" << sum_of_score.at(3) << std::endl;

  return 0;
}

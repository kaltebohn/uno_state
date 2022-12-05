#include "schlange.hpp"
#include "uno_state_bind2.hpp"

int main(void) {
  std::array<Schlange, 4> players{};
  std::array<int, 4> sum_of_score{};
  for (int i = 0; i < 1000; i++) {
    UnoStateBind2 state{allCards()};
    while (!state.isFinished()) {
      const int current_player{state.getCurrentPlayerNum()};
      Schlange& player{players.at(current_player)};
      player.setMyCards(state.getPlayerCards(current_player));
      state = state.next(player.move(state.legalMoves()));
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

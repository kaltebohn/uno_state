#include "schlange.hpp"
#include "uno_state_bind2.hpp"

int main(void) {
  std::array<Schlange, 4> players{};

  for (int i = 0; i < 1000; i++) {
    UnoStateBind2 state{allCards()};
    while (!state.isFinished()) {
      const int current_player{state.getCurrentPlayerNum()};
      Schlange& player{players.at(current_player)};
      player.setMyCards(state.getPlayerCards(current_player));
      state = state.next(player.move(state.legalMoves()));
    }
    std::cout << state.getScore(0) << "\t" << state.getScore(1) << "\t" << state.getScore(2) << "\t" << state.getScore(3) << std::endl;
  }

  return 0;
}

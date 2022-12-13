#include "schlange.hpp"
#include "uno_state_bind2.hpp"

Move nextMoveOfPlayer(Schlange& player, const int player_num, const UnoStateBind2& state) {
  switch (state.getCurrentMoveType()) {
    case MoveType::kChallenge:
      return (Move)(player.willChallenge());
    case MoveType::kColorChoice:
      return (Move)(player.changeColor());
    case MoveType::kSubmission:
      player.setMyCards(state.getPlayerCards(player_num));
      player.setTableColor(state.getTableColor());
      player.setTablePattern(state.getTablePattern());
      player.setLegalSubmissions();
      return (player.willDraw()) ? (Move)(Submission(Card(), false)) : (Move)(player.submitCard());
    case MoveType::kSubmissionOfDrawnCard:
      return (player.willPlayDrawnCard(state.getDrawnCard())) ?
          (Move)(Submission(state.getDrawnCard(), player.shouldYellUNO())) : (Move)(Submission());
    default:
      assert(false);
  }
}

int main(void) {
  std::array<Schlange, 4> players{};
  std::array<int, 4> sum_of_score{};
  for (int i = 0; i < 1000; i++) {
    UnoStateBind2 state{allCards()};
    while (!state.isFinished()) {
      const int current_player{state.getCurrentPlayerNum()};
      Schlange& player{players.at(current_player)};
      state = state.next(nextMoveOfPlayer(player, current_player, state));
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

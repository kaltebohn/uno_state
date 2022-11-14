#include "uno_state.hpp"

std::vector<Move> UnoState::legalMoves() const {
  if (current_event_ == MoveType::kSubmission) {
    std::vector<Submission> submissions{legalSubmissions()};
    std::vector<Move> result;
    std::transform(submissions.begin(), submissions.end(), result.begin(),
        [](Submission submission) {
          return (Move)(submission);
        });
    return result;
  } else if (current_event_ == MoveType::kColorChoice) {
    return {Color::kBlue, Color::kGreen, Color::kRed, Color::kYellow};
  } else if (current_event_ == MoveType::kChallenge) {
    return {true, false};
  } else {
    std::cerr << "到達し得ない行に処理が移りました: UnoState::legalMoves。" << std::endl;
    std::exit(1);
  }
}

std::vector<Submission> UnoState::legalSubmissions() const {
  std::vector<Submission> result;
  for (const Card& card : player_cards_.at(current_player_).getCards()) {
    Submission submission{card, currentPlayerShouldYellUNO()};
    if (submission.isLegal(table_color_, table_pattern_)) {
      result.push_back(submission);
    }
  }
  return result;
};

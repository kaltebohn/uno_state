#include "action_type.hpp"

std::string actionType2String(const ActionType action_type) {
  switch (action_type) {
    case ActionType::kSubmission:
      return "Submission";
    case ActionType::kSubmissionOfDrawnCard:
      return "SubmissionOfDrawCard";
    case ActionType::kColorChoice:
      return "ColorChoice";
    case ActionType::kChallenge:
      return "Challenge";
    default:
      assert(false);
  }
}

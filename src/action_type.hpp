#ifndef ACTION_TYPE_HPP
#define ACTION_TYPE_HPP

#include <cassert>
#include <string>

/* UNOにおける行動の種類。
   - kChallenge: チャレンジを行う？
   - kColorChoice: 場の色の指定
   - kSubmission: 提出手
   - kSubmission: ドロー後の提出手
*/
enum class ActionType {
  kChallenge,
  kColorChoice,
  kSubmission,
  kSubmissionOfDrawnCard
};

std::string actionType2String(const ActionType action_type);

#endif // ACTION_TYPE_HPP

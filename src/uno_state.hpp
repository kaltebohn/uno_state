#ifndef UNO_STATE_HPP_
#define UNO_STATE_HPP_

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <iterator>
#include <random>
#include <string>
#include <variant>
#include <vector>

#include "card.hpp"
#include "submission.hpp"
#include "move.hpp"
#include "uno_consts.hpp"

enum class MoveType {
  kChallenge,
  kColorChoice,
  kSubmission,
  kSubmissionOfDrawnCard
};

std::string moveType2String(const MoveType move_type);

class UnoState {
 public:
  friend std::ostream& operator<<(std::ostream& os, const UnoState& src) {
    os << src.toString();
    return os;
  }

  /* ゲーム開始用。 */
  UnoState(const Cards& first_deck, Card first_table_card = {})
      : deck_(first_deck),
        discards_(),
        player_cards_(),
        player_seats_({0, 1, 2, 3}),
        player_scores_(),
        current_move_type_(MoveType::kSubmission),
        prev_player_(-1),
        current_player_(-1),
        is_normal_order_(true),
        table_color_(),
        table_pattern_(),
        is_challenge_valid_(),
        drawn_card_() {
    /* 山札をシャッフル。 */
    refreshDeck();
    
    /* プレイヤにカードを7枚ずつ分配。 */
    for (int player_num = 0; player_num < UnoConsts::kNumOfPlayers; player_num++) {
      std::copy(deck_.begin(), deck_.begin() + 7, std::back_inserter(player_cards_.at(player_num)));
      deck_.erase(deck_.begin(), deck_.begin() + 7);
    }

    /* 席順をシャッフル。 */
    // TODO: xorshiftに置き換える。
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::shuffle(player_seats_.begin(), player_seats_.end(), engine);

    /* 最初の1枚を出す。 */
    /* ワイルドドロー4、シャッフルワイルド、白いワイルドの場合は仕切り直し。 */
    while (first_table_card.isEmpty()) {
      const Card tmp_card{deck_.back()};
      if (!(tmp_card == Card::kWildDraw4 ||
            tmp_card == Card::kWildShuffleHands ||
            tmp_card == Card::kWildCustomizable)) {
        deck_.pop_back();
        first_table_card = tmp_card;
      } else {
        refreshDeck();
      }
    }
    discards_.push_back(first_table_card);
    table_color_ = first_table_card.getColor();
    table_pattern_ =first_table_card.getPattern();

    /* 始点のプレイヤを決定。 */
    int const first_player = *std::find(player_seats_.begin(), player_seats_.end(), 0);

    /* 最初の1枚の効果を反映させる。 */
    if (std::holds_alternative<CardNumber>(first_table_card.getPattern())) {
      /* 数字カードなら場に置くだけ。 */
      current_move_type_ = MoveType::kSubmission;
      current_player_ = first_player;
    } else {
      /* 効果ありカード毎の処理。 */
      const CardAction submission_action = std::get<CardAction>(first_table_card.getPattern());
      if (submission_action == CardAction::kDrawTwo) {
        /* ドロー2: 最初のプレイヤがカードを2枚引き、次のプレイヤに手番が移る。 */
        current_move_type_ = MoveType::kSubmission;
        giveCards(first_player, 2);
        current_player_ = nextPlayerOf(first_player);
      } else if (submission_action == CardAction::kReverse) {
        /* リバース: 手番が逆順になり、本来最後の手番だったプレイヤが最初にカードを出す。 */
        current_move_type_ = MoveType::kSubmission;
        is_normal_order_ = !is_normal_order_;
        current_player_ = nextPlayerOf(first_player);
      } else if (submission_action == CardAction::kSkip) {
        /* スキップ: 最初のプレイヤーは手番を飛ばされ、次のプレイヤに手番が移る。*/
        current_move_type_ = MoveType::kSubmission;
        current_player_ = nextPlayerOf(first_player);
      } else if (submission_action == CardAction::kWild) {
        /* ワイルド: 最初のプレイヤが好きな色を宣言し、次のプレイヤからスタートする。*/
        current_move_type_ = MoveType::kColorChoice;
        current_player_ = first_player;
      } else {
        assert(false);
      }
    }
  }

  UnoState(std::vector<Card> deck, std::vector<Card> discards, std::array<Cards, UnoConsts::kNumOfPlayers> player_cards, std::array<int, UnoConsts::kNumOfPlayers> player_seats, std::array<int, UnoConsts::kNumOfPlayers> player_scores, MoveType current_event, int prev_player, int current_player, bool is_normal_order, Color table_color, CardPattern table_pattern, bool is_challenge_valid, Card drawn_card)
      : deck_(deck),
        discards_(discards),
        player_cards_(player_cards),
        player_seats_(player_seats),
        player_scores_(player_scores),
        current_move_type_(current_event),
        prev_player_(prev_player),
        current_player_(current_player),
        is_normal_order_(is_normal_order),
        table_color_(table_color),
        table_pattern_(table_pattern),
        is_challenge_valid_(is_challenge_valid),
        drawn_card_(drawn_card)
      {}

  /* 受け取った手を適用して得られる状態を返す。 */
  UnoState next(Move move) const;

  /* 合法着手の全体を返す。 */
  virtual std::vector<Move> legalMoves() const;

  /* ゲームが終了しているか？ */
  virtual bool isFinished() const {
    return std::any_of(player_cards_.cbegin(), player_cards_.cend(),
        [](Cards cards){ return cards.size() == 0; });
  }

  /* 指定されたプレイヤ番号の現時点での得点を返す。ゲームが終わっていなければ0。 */
  double getScore(const int player_num) const { return player_scores_.at(player_num); }

  /* 現時点が誰の手番か？ */
  int getCurrentPlayerNum() const { return current_player_; }

  /* player_num番目のプレイヤの手札を返す。 */
  Cards getPlayerCards(int player_num) const { return player_cards_.at(player_num); }

  /* 現時点で期待している着手の型を返す。 */
  MoveType getCurrentMoveType() const { return current_move_type_; }

  Color getTableColor() const { return table_color_; }

  CardPattern getTablePattern() const { return table_pattern_; }

  Card getDrawnCard() const { return drawn_card_; }

  /* テスト用。 */
  virtual bool operator ==(const UnoState& state) const {
    if (!std::equal(state.deck_.cbegin(), state.deck_.cend(), deck_.begin())) { return false; }
    if (!std::equal(state.discards_.cbegin(), state.discards_.cend(), discards_.begin())) { return false; }         
    for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
      if (!std::equal(state.player_cards_.at(i).cbegin(), state.player_cards_.at(i).cend(), player_cards_.at(i).begin())) { return false; }         
    }
    if (!std::equal(state.player_seats_.cbegin(), state.player_seats_.cend(), player_seats_.begin())) { return false; }         
    if (!std::equal(state.player_scores_.cbegin(), state.player_scores_.cend(), player_scores_.begin())) { return false; }         
    if (state.current_move_type_ != current_move_type_) { return false; }
    if (state.prev_player_ != prev_player_) { return false; }
    if (state.current_player_ != current_player_) { return false; }
    if (state.is_normal_order_ != is_normal_order_) { return false; }
    if (state.table_color_ != table_color_) { return false; }
    if (state.table_pattern_ != table_pattern_) { return false; }
    if (state.is_challenge_valid_ != is_challenge_valid_) { return false; }
    if (state.drawn_card_ != drawn_card_) { return false; }

    return true;
  }

  virtual std::string toString() const;

 protected:
  Cards deck_;
  Cards discards_;
  std::array<Cards, UnoConsts::kNumOfPlayers> player_cards_; // プレイヤ番号で各プレイヤの手札にアクセス。
  std::array<int, UnoConsts::kNumOfPlayers> player_seats_; // プレイヤ番号で各プレイヤの席にアクセス。
  std::array<int, UnoConsts::kNumOfPlayers> player_scores_; // プレイヤ番号で各プレイヤの得点にアクセス(ゲーム終了時以外は0)。
  MoveType current_move_type_;
  int prev_player_;
  int current_player_;
  bool is_normal_order_;
  Color table_color_;
  CardPattern table_pattern_;
  bool is_challenge_valid_;
  Card drawn_card_; // 直前にプレイヤが引いたカード。

  /* 可能な提出カードの全体を返す。 */
  virtual std::vector<Submission> legalSubmissions() const;

  /* 現在のプレイヤがUNOを宣言すべきか(プレイヤの手札枚数が2枚か)？ */
  virtual bool currentPlayerShouldYellUNO() const { return player_cards_.at(current_player_).size() == 2; }

  UnoState nextWhenColorChoice(UnoState& state, const Color color) const;

  UnoState nextWhenIlligalColorChoice(UnoState& state) const;

  UnoState nextWhenChallenge(UnoState& state, const ChallengeFlag will_challenge) const;

  UnoState nextWhenSubmission(UnoState& state, const Submission& submission) const;

  /* 合法手でなければ2枚引かせて手番を飛ばす。 */
  UnoState nextWhenIlligalSubmission(UnoState& state) const;

  /* カードが空なら、カードを引こうとしているとみなす。 */
  UnoState nextWhenEmptyCardSubmission(UnoState& state) const;

  /* 共通処理を実施した状態に、ドロー2の効果を反映させる。 */
  UnoState nextWhenDrawTwoSubmission(UnoState& state) const {
    const int next_player{nextPlayer()};
    state.current_move_type_ = MoveType::kSubmission;
    state.giveCards(next_player, 2);
    state.current_player_ = nextPlayerOf(next_player); // 次の人を飛ばす。
    state.prev_player_ = current_player_;
    return state;
  }

  /* 共通処理を実施した状態に、リバースの効果を反映させる。 */
  UnoState nextWhenReverseSubmission(UnoState& state) const {
    state.current_move_type_ = MoveType::kSubmission;
    state.is_normal_order_ = !is_normal_order_;
    state.current_player_ = state.nextPlayer();
    state.prev_player_ = current_player_;
    return state;
  }

  /* 共通処理を実施した状態に、スキップの効果を反映させる。 */
  UnoState nextWhenSkipSubmission(UnoState& state) const {
    const int current_player{current_player_};
    state.current_move_type_ = MoveType::kSubmission;
    state.current_player_ = nextPlayerOf(nextPlayerOf(current_player));
    state.prev_player_ = current_player_;
    return state;
  }

  /* 共通処理を実施した状態に、ワイルドの効果を反映させる。 */
  UnoState nextWhenWildSubmission(UnoState& state) const {
    state.current_move_type_ = MoveType::kColorChoice;
    state.current_player_ = nextPlayer();
    state.prev_player_ = current_player_;
    return state;
  }

  /* 共通処理を実施した状態に、白いワイルドの効果を反映させる。 */
  /* 効果が特定のモノに定まっていないので、効果ごとにクラスを作り、このクラスをラップさせる。 */
  UnoState nextWhenWildCustomizableSubmission(UnoState& state) const {
    assert(false);
  }

  /* 共通処理を実施した状態に、ワイルドドロー4の効果を反映させる。 */
  UnoState nextWhenWildDraw4Submission(UnoState& state) const {
    state.current_move_type_ = MoveType::kColorChoice;
    state.current_player_ = nextPlayer();
    state.prev_player_ = current_player_;
    const auto legal_moves = legalMoves();
    state.is_challenge_valid_ = (std::any_of(legal_moves.begin(), legal_moves.end(),
        [](Move move) {
          const Card card = std::get<Submission>(move).getCard();
          const CardPattern pattern = card.getPattern();
          return (!card.isEmpty() &&
                  !std::holds_alternative<CardNumber>(pattern)) &&
                  (std::get<CardAction>(pattern) != CardAction::kWildDraw4);
        }));
    return state;
  }

  /* 共通処理を実施した状態に、シャッフルワイルドの効果を反映させる。 */
  UnoState nextWhenWildShuffleHandsSubmission(UnoState& state) const {
    const int next_player = nextPlayer();
    state.current_move_type_ = MoveType::kColorChoice;
    state.current_player_ = next_player;
    state.prev_player_ = current_player_;

    std::vector<Card> collected_cards{};
    for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
      std::copy(state.player_cards_.at(i).begin(), state.player_cards_.at(i).end(), std::back_inserter(collected_cards));
      state.player_cards_.at(i).clear();
    }
    shuffleCards(collected_cards);

    int player_to_give = next_player;
    for (const Card& card : collected_cards) {
      state.player_cards_.at(player_to_give).push_back(card);
      player_to_give = nextPlayerOf(player_to_give);
    }
    return state;
  }

  virtual void acceptSubmission(const Submission& submission);

  // TODO: たまに正しく得点を付けられていない気がする。
  virtual void scoreToPlayers() {
    if (!isFinished()) { return; }
    /* カードが残っているプレイヤから減点。 */
    int finished_player{-1};
    int sum_of_scores{};
    for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
      /* 上がったプレイヤには減点しない。次の処理のために番号を控えておく。 */
      if (player_cards_.at(i).size() == 0) {
        finished_player = i;
        continue;
      }

      int const score{std::accumulate(player_cards_.at(i).begin(), player_cards_.at(i).end(), 0,
          [](const int acc, const Card& card) {
            return acc + card.toScore();
          })};
      player_scores_.at(i) = -score;
      sum_of_scores += score;
    }

    /* 各プレイヤの減点分を上がったプレイヤに加算。 */
    assert(finished_player != -1);
    player_scores_.at(finished_player) = sum_of_scores;
  }

  virtual int nextPlayerOf(const int player_num) const {
    const int next_seat = is_normal_order_ ?
        (player_seats_.at(player_num) + 1) % UnoConsts::kNumOfPlayers :
        (player_seats_.at(player_num) - 1 + UnoConsts::kNumOfPlayers) % UnoConsts::kNumOfPlayers;
    const auto iter = std::find(player_seats_.begin(), player_seats_.end(), next_seat);
    return std::distance(player_seats_.begin(), iter);
  }

  int nextPlayer() const { return nextPlayerOf(current_player_); }

  virtual void giveCards(const int player_number, const int num) {
    for (int i = 0; i < num; i++) {
      if (deck_.size() == 0) { refreshDeck(); }

      /* 捨て札も山札も0枚の場合、手札を捨てないプレイヤが存在する。 */
      /* このクラスはそうしたプレイヤを許容する必要がないので、強制終了させる。 */
      assert(deck_.size() > 0);

      const Card card = deck_.back(); deck_.pop_back();
      player_cards_.at(player_number).push_back(card);
    }
  }

  void shuffleCards(std::vector<Card>& cards) const {
    // TODO: xorshiftに置き換える。
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::shuffle(cards.begin(), cards.end(), engine);
  }

  void refreshDeck() {
    std::copy(discards_.begin(), discards_.end(), std::back_inserter(deck_));
    discards_.clear();
    shuffleCards(deck_);
  }

  Card lastSubmittedCard() { return discards_.back(); }
};

#endif // UNO_STATE_HPP_

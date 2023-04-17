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

#include "external/xorshift64.hpp"
#include "card.hpp"
#include "move.hpp"
#include "move_type.hpp"
#include "observation.hpp"
#include "uno_consts.hpp"

std::string moveType2String(const MoveType move_type);

class UnoState {
 public:
  friend std::ostream& operator<<(std::ostream& os, const UnoState& src) {
    os << src.toString();
    return os;
  }

  /* ベクタに格納するのに必要。何か意味のある状態ではない。 */
  UnoState()
      : deck_(),
        discards_(),
        player_cards_(),
        player_seats_(),
        player_scores_(),
        current_move_type_(),
        prev_player_(),
        current_player_(),
        is_normal_order_(),
        table_color_(),
        table_pattern_(),
        is_challenge_valid_(),
        drawn_card_() {}

  /* ゲーム開始用。 */
  UnoState(const Cards& first_deck, XorShift64::result_type random_seed = 0, Card first_table_card = {})
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
    /* 乱数エンジンをセット。 */
    if (random_seed == 0) {
      std::random_device seed_gen;
      random_engine_ = seed_gen();
    } else {
      random_engine_ = random_seed;
    }

    /* 山札をシャッフル。 */
    refreshDeck();

    /* プレイヤにカードを7枚ずつ分配。 */
    for (int player_num = 0; player_num < UnoConsts::kNumOfPlayers; player_num++) {
      std::copy(deck_.begin(), deck_.begin() + 7, std::back_inserter(player_cards_.at(player_num)));
      deck_.erase(deck_.begin(), deck_.begin() + 7);
    }

    /* 席順をシャッフル。 */
    std::shuffle(player_seats_.begin(), player_seats_.end(), random_engine_);

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
    assert(std::find(player_seats_.begin(), player_seats_.end(), 0) != player_seats_.end());
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

  UnoState(std::vector<Card> deck,
           std::vector<Card> discards,
           std::array<Cards,
           UnoConsts::kNumOfPlayers> player_cards,
           std::array<int, UnoConsts::kNumOfPlayers> player_seats,
           std::array<int, UnoConsts::kNumOfPlayers> player_scores,
           MoveType current_event,
           int prev_player,
           int current_player,
           bool is_normal_order,
           Color table_color,
           CardPattern table_pattern,
           bool is_challenge_valid,
           Card drawn_card,
           Move last_move,
           XorShift64 random_engine,
           std::array<Cards, 4> add_cards,
           std::array<Cards, 4> sub_cards
           )
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
        drawn_card_(drawn_card),
        last_move_(last_move),
        random_engine_(random_engine),
        add_cards_(add_cards),
        sub_cards_(sub_cards)
  {}

  /* 受け取った手を適用して得られる状態を返す。 */
  UnoState next(Move move) const;

  /* 合法着手の全体を返す。 */
  virtual std::vector<Move> legalActions() const;

  /* ゲームが終了しているか？ */
  virtual bool isFinished() const {
    return std::any_of(player_cards_.cbegin(), player_cards_.cend(),
        [](Cards cards){ return cards.size() == 0; });
  }

  Observation getObservation(const int player) const {
    std::array<int, UnoConsts::kNumOfPlayers> player_card_qtys{};
    std::transform(player_cards_.begin(), player_cards_.end(), player_card_qtys.begin(),
        [](const Cards& cards) {
          return cards.size();
        });
    return Observation{
      discards_,
      player_cards_.at(player),
      legalActions(),
      player_card_qtys,
      player_seats_,
      current_move_type_,
      prev_player_,
      current_player_,
      is_normal_order_,
      table_color_,
      table_pattern_
    };
  }

  Cards getDiscards() const { return discards_; }
  Cards getPlayerCards(int player_num) const { return player_cards_.at(player_num); }
  int getPlayerSeats(const int player_num) const { return player_seats_.at(player_num); }
  double getScore(const int player_num) const { return player_scores_.at(player_num); }
  MoveType getCurrentMoveType() const { return current_move_type_; }
  int getPrevPlayerNum() const { return prev_player_; }
  std::array<int, UnoConsts::kNumOfPlayers> getAllPlayerSeats() const { return player_seats_; }
  int getCurrentPlayerNum() const { return current_player_; }
  bool getIsNormalOrder() const { return is_normal_order_; }
  Color getTableColor() const { return table_color_; }
  CardPattern getTablePattern() const { return table_pattern_; }
  Card getDrawnCard() const { return drawn_card_; }
  Move getLastAction() const { return last_move_; }
  Cards getAddCards(const int player_num) const { return add_cards_.at(player_num); }
  Cards getSubCards(const int player_num) const { return sub_cards_.at(player_num); }

  std::array<int, UnoConsts::kNumOfPlayers> getQuantityOfPlayerCards() const {
    std::array<int, UnoConsts::kNumOfPlayers> result{};
    std::transform(player_cards_.begin(), player_cards_.end(), result.begin(),
        [](const Cards& cards) {
          return cards.size();
        });
    return result;
  }

  /* テスト用。 */
  virtual bool operator ==(const UnoState& state) const {
    if (!std::equal(state.deck_.cbegin(), state.deck_.cend(), deck_.begin())) { return false; }
    if (!std::equal(state.discards_.cbegin(), state.discards_.cend(), discards_.begin())) { return false; }
    for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
      if (!std::equal(state.player_cards_.at(i).cbegin(), state.player_cards_.at(i).cend(), player_cards_.at(i).begin())) { return false; }
      if (!std::equal(state.add_cards_.at(i).cbegin(), state.add_cards_.at(i).cend(), add_cards_.at(i).begin())) { return false; }
      if (!std::equal(state.sub_cards_.at(i).cbegin(), state.sub_cards_.at(i).cend(), sub_cards_.at(i).begin())) { return false; }
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

  virtual std::string toJSON() const;

  virtual void print() const { std::cout << *this; }

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
  Move last_move_{};
  XorShift64 random_engine_;
  std::array<Cards, 4> add_cards_{}; // 状態遷移時に各プレイヤに追加されたカード。最初とシャッフルワイルド時は考えない。
  std::array<Cards, 4> sub_cards_{}; // 状態遷移時に各プレイヤから削除されたカード。最初とシャッフルワイルド時は考えない。

  /* 可能な提出カードの全体を返す。 */
  virtual std::vector<Card> legalCards() const;

  UnoState nextWhenColorChoice(UnoState& state, const Color color) const;

  UnoState nextWhenIlligalColorChoice(UnoState& state) const;

  UnoState nextWhenChallenge(UnoState& state, const ChallengeFlag will_challenge) const;

  UnoState nextWhenSubmission(UnoState& state, const Card& submission) const;

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
    const auto legal_moves = legalActions();
    state.is_challenge_valid_ = (std::any_of(legal_moves.begin(), legal_moves.end(),
        [](Move move) {
          const Card card = std::get<Card>(move);
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

    std::vector<Card> collected_cards{};
    for (int i = 0; i < UnoConsts::kNumOfPlayers; i++) {
      std::copy(state.player_cards_.at(i).begin(), state.player_cards_.at(i).end(), std::back_inserter(collected_cards));
      state.player_cards_.at(i).clear();
    }
    state.shuffleCards(collected_cards);

    int player_to_give = next_player;
    for (const Card& card : collected_cards) {
      state.player_cards_.at(player_to_give).push_back(card);
      player_to_give = nextPlayerOf(player_to_give);
    }
    return state;
  }

  virtual void acceptSubmission(const Card& submission);

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

  /* プレイヤに山札からnum枚のカードを渡す。 */
  virtual void giveCards(const int player_number, const int num) {
    for (int i = 0; i < num; i++) {
      if (deck_.size() == 0) { refreshDeck(); }

      /* 捨て札も山札も0枚の場合、バグか何かで手札をまったく捨てていないプレイヤが存在する。 */
      /* このクラスはそうしたプレイヤを許容する必要がないので、強制終了させる。 */
      assert(deck_.size() > 0);

      const Card card = deck_.back(); deck_.pop_back();
      player_cards_.at(player_number).push_back(card);
      add_cards_.at(player_number).push_back(card);
    }
  }

  /* プレイヤの手札に捨て札からnum枚のカードを返す。 */
  virtual void returnCards(const int player_number, const int num) {
    for (int i = 0; i < num; i++) {
      assert(discards_.size() > 0); // ルール上、捨て札にはプレイヤが出した分(>= 返す分)以上あるはず。
      player_cards_.at(player_number).push_back(discards_.back());
      add_cards_.at(player_number).push_back(discards_.back());
      discards_.pop_back();
    }
  }

  /* プレイヤの手札を場に捨てる。 */
  virtual void discardCard(const int player_number, const Card& card) {
    assert(std::find(player_cards_.at(player_number).begin(),
                     player_cards_.at(player_number).end(),
                     card) !=
           player_cards_.at(player_number).end());

    discards_.push_back(card);
    player_cards_.at(player_number).erase(
        std::find(player_cards_.at(player_number).begin(),
                  player_cards_.at(player_number).end(),
                  card));
    sub_cards_.at(player_number).push_back(card);

    table_color_ = card.getColor();
    table_pattern_ = card.getPattern();
  }

  void shuffleCards(std::vector<Card>& cards) {
    std::shuffle(cards.begin(), cards.end(), random_engine_);
  }

  void refreshDeck() {
    std::copy(discards_.begin(), discards_.end(), std::back_inserter(deck_));
    discards_.clear();
    shuffleCards(deck_);
  }

  Card lastSubmittedCard() { return discards_.back(); }
};

#endif // UNO_STATE_HPP_

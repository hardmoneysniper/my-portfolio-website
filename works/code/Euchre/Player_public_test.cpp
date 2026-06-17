#include "Player.h"
#include "Card.h"
#include "unit_test_framework.h"
#include <sstream>
using namespace std;

// THESE TEST CASES WILL ONLY TELL YOU IF YOUR CODE COMPILES.
// YOU NEED TO WRITE YOUR OWN COMPREHENSIVE TEST CASES IN Player_tests.cpp

TEST(test_player_insertion) {
  Player * human = Player_factory("NotRobot", "Human");
  ostringstream oss;
  oss << * human;
  ASSERT_EQUAL(oss.str(), "NotRobot");
  oss.str("");
  Player * alice = Player_factory("Alice", "Simple");
  oss << *alice;
  ASSERT_EQUAL(oss.str(), "Alice");
  delete alice;
  delete human;
}

TEST(test_player_get_name) {
  Player * alice = Player_factory("Alice", "Simple");
  ASSERT_EQUAL(alice->get_name(), "Alice");
  delete alice;
}

static void add_cards(Player *player) {
  Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
  for (int i = 0; i < 5; ++i) {
    player->add_card(two_spades);
  }
}

TEST(test_simple_player_make_trump) {
  Player * bob = Player_factory("Bob", "Simple");
  add_cards(bob);

  Card nine_spades(Card::RANK_NINE, Card::SUIT_SPADES);
  string trump = Card::SUIT_SPADES;
  bool orderup = bob->make_trump(
      nine_spades,    //upcard
      true,           //Bob is also the dealer
      1,              //first round
      trump           //suit ordered up (if any)
  );
  ASSERT_FALSE(orderup);
  ASSERT_EQUAL(trump, Card::SUIT_SPADES);

  delete bob;
}

TEST(test_simple_player_lead_card) {
  Player * bob = Player_factory("Bob", "Simple");
  add_cards(bob);
  Card nine_spades(Card::RANK_NINE, Card::SUIT_SPADES);
  bob->add_and_discard(nine_spades);
  Card card_led = bob->lead_card(Card::SUIT_SPADES);
  ASSERT_EQUAL(card_led, nine_spades); //check led card
  delete bob;
}

TEST(test_simple_player_play_card) {
  Player * bob = Player_factory("Bob", "Simple");
  add_cards(bob);
  Card nine_diamonds(Card::RANK_NINE, Card::SUIT_DIAMONDS);
  string trump = Card::SUIT_HEARTS;
  Card card_played = bob->play_card(nine_diamonds, trump); //Diamonds is led
  Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
  ASSERT_EQUAL(card_played, two_spades);
  delete bob;
}
static void add_cards1(Player *player) {
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card jack_heart = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);
	Card nine_club = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	player->add_card(king_spades);
	player->add_card(nine_club);
	player->add_card(jack_diamonds);
	player->add_card(jack_heart);
	player->add_card(nine_spades);


}
TEST(test_simple_player_play_card1) {
	Player * bob = Player_factory("Bob", "Simple");
	add_cards1(bob);
	Card jack_spades(Card::RANK_JACK, Card::SUIT_SPADES);
	Card led_card = jack_spades;
	string trump = Card::SUIT_HEARTS;
	Card card_played = bob->play_card(jack_spades, trump); //Diamonds is led
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);
	ASSERT_EQUAL(card_played, king_spades);
	delete bob;
}

TEST_MAIN()

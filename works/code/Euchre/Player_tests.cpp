#include <iostream>
#include <string>
#include "Player.h"
#include "Card.h"
#include <cassert>
#include "unit_test_framework.h"

using namespace std;

TEST(test_player_insertion) {
	Player * human = Player_factory("Me", "Human");
	ostringstream oss;
	oss << *human;
	ASSERT_EQUAL(oss.str(), "Me");
	oss.str("");
	Player * alex = Player_factory("Alex", "Simple");
	oss << *alex;
	ASSERT_EQUAL(oss.str(), "Alex");
	delete alex;
	delete human;
}

TEST(test_player_get_name_simple) {
	Player * alex = Player_factory("Alex", "Simple");
	ASSERT_EQUAL("Alex", alex->get_name());

	delete alex;
}

TEST(test_player_get_name_human) {
	Player * bob = Player_factory("Bob", "Human");
	ASSERT_EQUAL("Bob", bob->get_name());

	delete bob;
}

static void add_cards(Player *player) {
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);

	player->add_card(ten_spades);
	player->add_card(jack_diamonds);
	player->add_card(king_hearts);
	player->add_card(nine_spades);
	player->add_card(ace_diamonds);
}


TEST(test_simple_player_make_trump1) {
	Player * bert = Player_factory("Bert", "Simple");
	add_cards(bert);

	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	string trump = Card::SUIT_DIAMONDS;
	bool orderup = bert->make_trump(
		queen_diamonds,    //upcard
		false,           //Bert is also the dealer
		1,              //first round
		trump           //suit ordered up (if any)
	);
	ASSERT_TRUE(orderup);
	ASSERT_EQUAL(trump, Card::SUIT_DIAMONDS);

	delete bert;
}

TEST(test_simple_player_make_trump2) {
	Player * jon = Player_factory("Jon", "Simple");
	add_cards(jon);

	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	string trump; //= Card::SUIT_HEARTS;
	bool orderup = jon->make_trump(
		queen_clubs,    //upcard
		false,//true,           //Bert is also the dealer
		2,              //first round
		trump           //suit ordered up (if any)
	);

	ASSERT_FALSE(orderup);
	//JON WILL NOT ORDER UP!
	//ASSERT_TRUE(orderup);
	//ASSERT_EQUAL(trump, Card::SUIT_SPADES);

	delete jon;
}

TEST(test_simple_player_lead_card1) {
	Player * bob = Player_factory("Bob", "Simple");
	add_cards(bob);
	Card ace_diamonds(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card ace_clubs(Card::RANK_ACE, Card::SUIT_CLUBS);
	bob->add_and_discard(ace_clubs);
	Card card_led = bob->lead_card(Card::SUIT_CLUBS);
	ASSERT_EQUAL(card_led, ace_diamonds); //check led card
	delete bob;
}



TEST(test_simple_player_play_card2) {
	Player * bob = Player_factory("Bob", "Simple");
	add_cards(bob);
	Card nine_clubs(Card::RANK_NINE, Card::SUIT_CLUBS);
	string trump = Card::SUIT_HEARTS;
	Card card_played = bob->play_card(nine_clubs, trump); //Diamonds is led
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	ASSERT_EQUAL(card_played, nine_spades);
	delete bob;
}

TEST(whole1) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);

	Player * one = Player_factory("one", "Simple");
	one->add_card(nine_clubs);
	one->add_card(ace_clubs);
	one->add_card(jack_clubs);
	one->add_card(king_clubs);
	one->add_card(ten_clubs);

	string trump = Card::SUIT_DIAMONDS;
	bool orderup = one->make_trump(
		nine_diamonds,   //upcard
		true,           //is also the dealer
		1,              //first round
		trump           //suit ordered up (if any)
	);
	ASSERT_FALSE(orderup);
	ASSERT_EQUAL(trump, Card::SUIT_DIAMONDS);

	delete one;
}

TEST(whole2) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);

	Player *two = Player_factory("two", "Simple");
	Player *joel = Player_factory("joel", "Simple");
	two->add_card(queen_clubs);
	two->add_card(ace_clubs);
	two->add_card(queen_hearts);
	two->add_card(ace_diamonds);
	two->add_card(king_diamonds);
	joel->add_card(ten_spades);
	joel->add_card(jack_clubs);
	joel->add_card(queen_clubs);
	joel->add_card(ace_spades);
	joel->add_card(king_spades);
	string trump;
	bool orderup = two->make_trump(
		nine_clubs,    //upcard
		true,// is the dealer
		1,//first round
		trump);//suit ordered up (if any)
	ASSERT_TRUE(orderup);
	ASSERT_EQUAL(trump, Card::SUIT_CLUBS);

	two->add_and_discard(nine_clubs);

	Card card_led = two->lead_card(Card::SUIT_CLUBS);
	ASSERT_EQUAL(card_led, ace_diamonds);

	Card card_play = joel->play_card(ace_diamonds, Card::SUIT_CLUBS);
	ASSERT_EQUAL(card_play, ten_spades);
	delete two;
	delete joel;
}
TEST(whole3) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player *nine = Player_factory("nine", "Simple");
	nine->add_card(queen_spades);
	nine->add_card(jack_spades);
	nine->add_card(queen_clubs);
	nine->add_card(ace_clubs);
	nine->add_card(king_clubs);
	Player *eight = Player_factory("eight", "Simple");
	eight->add_card(nine_spades);
	eight->add_card(ten_spades);
	eight->add_card(jack_spades);
	eight->add_card(queen_spades);
	eight->add_card(king_spades);
	string trump;
	bool orderup = nine->make_trump(
		ace_spades,    //upcard
		false,// is not the dealer
		1,//first round
		trump);//suit ordered up (if any)
	ASSERT_TRUE(orderup);//ASSERT_FALSE(orderup);
	ASSERT_EQUAL(trump, Card::SUIT_SPADES);

	nine->add_and_discard(ace_spades);

	Card card_led = nine->lead_card(Card::SUIT_SPADES);
	ASSERT_EQUAL(card_led, ace_clubs);

	Card card_play = eight->play_card(ace_clubs, Card::SUIT_SPADES);
	ASSERT_EQUAL(card_play, nine_spades);
	delete nine;
	delete eight;
}

TEST(whole4) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * three = Player_factory("three", "Simple");
	three->add_card(king_clubs);
	three->add_card(ace_diamonds);
	three->add_card(jack_hearts);
	three->add_card(king_spades);
	three->add_card(jack_clubs);
	string trump;
	bool orderup = three->make_trump(
		queen_clubs,    //upcard
		true,           //is also the dealer
		1,              //first round
		trump           //suit ordered up (if any)
	);
	ASSERT_TRUE(orderup);
	ASSERT_EQUAL(trump, Card::SUIT_CLUBS);
	three->add_and_discard(queen_clubs);

	Card card_led = three->lead_card(Card::SUIT_CLUBS);
	ASSERT_EQUAL(card_led, ace_diamonds);

	delete three;
}

TEST(whole5) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * player1 = Player_factory("player1", "Simple");
	Player * player2 = Player_factory("player2", "Simple");
	Player * player3 = Player_factory("player3", "Simple");

	player1->add_card(king_clubs);
	player1->add_card(ace_diamonds);
	player1->add_card(jack_clubs);
	player1->add_card(nine_hearts);
	player1->add_card(ace_spades);
	player2->add_card(nine_clubs);
	player2->add_card(nine_diamonds);
	player2->add_card(ten_hearts);
	player2->add_card(king_diamonds);
	player2->add_card(ten_clubs);
	player3->add_card(ten_diamonds);
	player3->add_card(queen_spades);
	player3->add_card(nine_spades);
	player3->add_card(jack_spades);
	player3->add_card(ten_spades);
	string trump;
	bool orderup = player1->make_trump(
		king_hearts,//upcard
		false,//is the dealer dont forget to test if it is dealer!
		2,//second round (the right and left bowers, and Q, K, A of the order-up suit) ????
		trump);//suit ordered up (if any)
	ASSERT_TRUE(orderup);
	ASSERT_EQUAL(trump, Card::SUIT_DIAMONDS);

	player1->add_and_discard(king_hearts);

	Card card_led = player1->lead_card(Card::SUIT_DIAMONDS);
	ASSERT_EQUAL(card_led, ace_spades);
	Card card_play2 = player2->play_card(ace_spades, Card::SUIT_DIAMONDS);
	ASSERT_EQUAL(card_play2, nine_clubs);
	//cout <<card_play2;
	Card card_play3 = player3->play_card(ace_spades, Card::SUIT_DIAMONDS);
	ASSERT_EQUAL(card_play3, queen_spades);

	//cout <<card_led1;

	delete player1;
	delete player2;
	delete player3;
}

TEST(whole6) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * five = Player_factory("five", "Simple");
	Player * player = Player_factory("player", "Simple");
	five->add_card(ten_spades);
	five->add_card(queen_spades);
	five->add_card(king_spades);
	five->add_card(jack_spades);
	five->add_card(nine_spades);
	player->add_card(king_diamonds);
	player->add_card(queen_diamonds);
	player->add_card(ace_clubs);
	player->add_card(queen_hearts);
	player->add_card(jack_hearts);


	string trump = Card::SUIT_DIAMONDS;

	bool orderup1 = five->make_trump(
		ace_diamonds,//upcard
		false,// is the dealer
		1,//first round
		trump);//suit ordered up (if any)
			   //ASSERT_EQUAL(trump, Card::SUIT_SPADES);
	bool orderup2 = five->make_trump(
		ace_hearts,
		true, //dealer?
		2,
		trump);
	ASSERT_FALSE(orderup1);
	ASSERT_TRUE(orderup2);
	ASSERT_EQUAL(trump, Card::SUIT_DIAMONDS);
	//ASSERT_EQUAL(trump, Card::SUIT_SPADES);
	ASSERT_EQUAL(five->lead_card(trump), king_spades);
	//cout << five->lead_card(trump);
	Card player_playcard = player->play_card(king_spades, trump);
	ASSERT_EQUAL(player_playcard, queen_hearts);
	//ASSERT_EQUAL(player1->play_card(king_spades, trump), jack_hearts);
	// cout << five->play_card(ten_spades, trump);

	delete five;
	delete player;

}

TEST(whole7) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * six = Player_factory("six", "Simple");
	Player * player6 = Player_factory("player6", "Simple");
	six->add_card(ten_spades);
	six->add_card(queen_spades);
	six->add_card(king_spades);
	six->add_card(jack_hearts);
	six->add_card(nine_spades);
	player6->add_card(king_diamonds);
	player6->add_card(queen_diamonds);
	player6->add_card(ace_clubs);
	player6->add_card(ace_spades);
	player6->add_card(jack_clubs);


	string trump = Card::SUIT_DIAMONDS;

	bool orderup = six->make_trump(
		ace_spades,//ace_diamonds,//upcard
		false,// is the dealer
		1,//first round
		trump);//suit ordered up (if any)
	ASSERT_EQUAL(trump, Card::SUIT_SPADES);

	ASSERT_TRUE(orderup);
	//ASSERT_EQUAL(trump, Card::SUIT_DIAMONDS);
	/*
	six->add_card(ten_spades);
	six->add_card(queen_spades);
	six->add_card(king_spades);
	six->add_card(jack_hearts);
	six->add_card(nine_spades);
	*/
	ASSERT_EQUAL(six->lead_card(trump), jack_hearts);
	Card player6_card = player6->play_card(jack_hearts, trump);
	ASSERT_EQUAL(player6_card, queen_diamonds);//ASSERT_EQUAL(player6->play_card(jack_hearts, trump), jack_clubs);

	delete six;
	delete player6;

}


TEST(whole8) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * twelve = Player_factory("twelve", "Simple");
	twelve->add_card(king_diamonds);
	twelve->add_card(queen_diamonds);
	twelve->add_card(ace_clubs);
	twelve->add_card(queen_spades);
	twelve->add_card(jack_spades);
	string trump;
	bool orderup = twelve->make_trump(
		king_spades,//upcard
		false,//is the dealer dont forget to test if it is dealer!
		1,//second round (the right and left bowers, and Q, K, A of the order-up suit) ????
		trump);//suit ordered up (if any)
	ASSERT_TRUE(orderup);
	ASSERT_EQUAL(trump, Card::SUIT_SPADES);

	Card card_led = twelve->lead_card(Card::SUIT_SPADES);
	ASSERT_EQUAL(card_led, ace_clubs);

	delete twelve;

}

TEST(whole9) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * grace = Player_factory("grace", "Simple");
	Player * jack = Player_factory("jack", "Simple");
	grace->add_card(ace_spades);
	grace->add_card(queen_spades);
	grace->add_card(nine_spades);
	grace->add_card(ten_spades);
	grace->add_card(jack_spades);
	jack->add_card(ten_diamonds);
	jack->add_card(queen_diamonds);
	jack->add_card(ace_clubs);
	jack->add_card(queen_clubs);
	jack->add_card(jack_hearts);
	string trump = Card::SUIT_SPADES;
	bool orderup = grace->make_trump(
		king_hearts,//upcard
		true,//is the dealer dont forget to test if it is dealer!
		1,//second round (the right and left bowers, and Q, K, A of the order-up suit) ????
		trump);//suit ordered up (if any)
	ASSERT_FALSE(orderup);
	ASSERT_EQUAL(trump, Card::SUIT_SPADES);

	Card card_led = grace->lead_card(Card::SUIT_SPADES);
	ASSERT_EQUAL(card_led, jack_spades);
	Card card_play = jack->play_card(jack_spades, Card::SUIT_SPADES);
	ASSERT_EQUAL(card_play, ten_diamonds);
	delete grace;
	delete jack;

}

TEST(whole10) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * ken = Player_factory("ken", "Simple");
	ken->add_card(king_diamonds);
	ken->add_card(queen_diamonds);
	ken->add_card(ace_clubs);
	ken->add_card(queen_spades);
	ken->add_card(queen_clubs);
	string trump;
	bool orderup = ken->make_trump(
		king_clubs,//upcard
		true,//is the dealer dont forget to test if it is dealer!
		1,//second round (the right and left bowers, and Q, K, A of the order-up suit) ????
		trump);//suit ordered up (if any)
	ASSERT_TRUE(orderup);
	ASSERT_EQUAL(trump, Card::SUIT_CLUBS);
	ken->add_and_discard(ace_diamonds);
	Card card_led = ken->lead_card(Card::SUIT_CLUBS);
	ASSERT_EQUAL(card_led, ace_diamonds);

	delete ken;

}

TEST(whole11) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * amy = Player_factory("amy", "Simple");
	amy->add_card(king_clubs);
	amy->add_card(queen_clubs);
	amy->add_card(ace_clubs);
	amy->add_card(nine_clubs);
	amy->add_card(ten_clubs);
	string trump;
	bool orderup = amy->make_trump(
		jack_clubs,//upcard
		true,//is the dealer dont forget to test if it is dealer!
		1,//second round (the right and left bowers, and Q, K, A of the order-up suit) ????
		trump);//suit ordered up (if any)
	ASSERT_TRUE(orderup);
	ASSERT_EQUAL(trump, Card::SUIT_CLUBS);
	amy->add_and_discard(nine_hearts);
	Card card_led = amy->lead_card(Card::SUIT_CLUBS);
	ASSERT_EQUAL(card_led, nine_hearts);

	delete amy;

}

TEST(whole12) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * sam = Player_factory("sam", "Simple");
	sam->add_card(king_spades);
	sam->add_card(queen_spades);
	sam->add_card(ace_spades);
	sam->add_card(nine_spades);
	sam->add_card(jack_diamonds);
	string trump;
	bool orderup1 = sam->make_trump(
		ten_diamonds,//upcard
		false,//is the dealer dont forget to test if it is dealer!
		1,//first round (the right and left bowers, and Q, K, A of the order-up suit)
		trump);//suit ordered up (if any)
	ASSERT_FALSE(orderup1);

	bool orderup2 = sam->make_trump(
		ten_diamonds,//upcard
		false,//is the dealer dont forget to test if it is dealer!
		2,//second round (the right and left bowers, and Q, K, A of the order-up suit) ????
		trump);//suit ordered up (if any)
	ASSERT_TRUE(orderup2);

	delete sam;

}

TEST(whole13) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * alice = Player_factory("alice", "Simple");
	Player * bob = Player_factory("bob", "Simple");
	Player * cathy = Player_factory("cathy", "Simple");
	Player * drew = Player_factory("drew", "Simple");
	alice->add_card(queen_hearts);
	alice->add_card(jack_hearts);

	bob->add_card(king_hearts);
	bob->add_card(ace_hearts);

	cathy->add_card(queen_spades);
	cathy->add_card(ten_clubs);

	drew->add_card(nine_hearts);
	drew->add_card(ten_hearts);

	string trump = Card::SUIT_HEARTS;

	Card card_led = jack_hearts;
	Card card_bob = bob->play_card(jack_hearts, Card::SUIT_HEARTS);
	ASSERT_EQUAL(card_bob, ace_hearts);
	Card card_cathy = cathy->play_card(jack_hearts, Card::SUIT_HEARTS);
	ASSERT_EQUAL(card_cathy, ten_clubs);
	Card card_drew = drew->play_card(jack_hearts, Card::SUIT_HEARTS);
	ASSERT_EQUAL(card_drew, ten_hearts);
	bool result = Card_less(card_led, card_bob, card_led, trump);
	ASSERT_FALSE(result);


	delete alice;
	delete bob;
	delete cathy;
	delete drew;

}

TEST(whole14) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * sam = Player_factory("sam", "Simple");
	sam->add_card(king_spades);
	sam->add_card(queen_spades);
	sam->add_card(ace_spades);
	sam->add_card(jack_hearts);
	sam->add_card(queen_diamonds);
	string trump;
	bool orderup = sam->make_trump(
		ten_diamonds,//upcard
		true,//is the dealer dont forget to test if it is dealer!
		1,//first round (the right and left bowers, and Q, K, A of the order-up suit)
		trump);//suit ordered up (if any)
	ASSERT_TRUE(orderup);


	delete sam;

}

TEST(whole15) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * sam = Player_factory("sam", "Simple");
	sam->add_card(king_spades);
	sam->add_card(queen_spades);
	sam->add_card(ace_spades);
	sam->add_card(nine_hearts);
	sam->add_card(queen_diamonds);
	string trump = Card::SUIT_DIAMONDS;
	Card card_led = jack_hearts;
	Card card_sam = sam->play_card(jack_hearts, Card::SUIT_DIAMONDS);
	//ASSERT_EQUAL(card_sam, queen_diamonds);
	ASSERT_EQUAL(card_sam, queen_diamonds);// do we really need to test this case? Since it will never happen!

	delete sam;

}

TEST(whole16) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * sam = Player_factory("sam", "Simple");
	sam->add_card(king_spades);
	sam->add_card(queen_spades);
	sam->add_card(ace_spades);
	sam->add_card(jack_hearts);
	sam->add_card(queen_diamonds);
	string trump = Card::SUIT_DIAMONDS;
	Card card_led = nine_hearts;
	Card card_sam = sam->play_card(card_led, Card::SUIT_DIAMONDS);
	//ASSERT_EQUAL(card_sam, queen_diamonds);
	ASSERT_EQUAL(card_sam, queen_spades);// do we really need to test this case? Since it will never happen!

	delete sam;

}

TEST(whole17) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * sam = Player_factory("sam", "Simple");
	sam->add_card(nine_spades);
	sam->add_card(ten_spades);
	sam->add_card(ace_clubs);
	sam->add_card(jack_hearts);
	sam->add_card(queen_diamonds);
	string trump = Card::SUIT_DIAMONDS;
	bool orderup = sam->make_trump(
		ace_spades,//upcard
		true,//is the dealer dont forget to test if it is dealer!
		1,//first round (the right and left bowers, and Q, K, A of the order-up suit)
		trump);//suit ordered up (if any)
	ASSERT_FALSE(orderup);
	ASSERT_EQUAL(trump, Card::SUIT_DIAMONDS);

	delete sam;
}

TEST(whole18) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * sam = Player_factory("sam", "Simple");
	sam->add_card(nine_spades);
	sam->add_card(king_spades);
	sam->add_card(ace_clubs);
	sam->add_card(jack_hearts);
	sam->add_card(queen_diamonds);
	string trump = Card::SUIT_DIAMONDS;
	bool orderup = sam->make_trump(
		ace_spades,//upcard
		true,//is the dealer dont forget to test if it is dealer!
		1,//first round (the right and left bowers, and Q, K, A of the order-up suit)
		trump);//suit ordered up (if any)
	ASSERT_FALSE(orderup);
	ASSERT_EQUAL(trump, Card::SUIT_DIAMONDS);

	delete sam;
}

TEST(whole19) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * sam = Player_factory("sam", "Simple");
	sam->add_card(king_spades);
	sam->add_card(jack_clubs);
	sam->add_card(ace_clubs);
	sam->add_card(jack_hearts);
	sam->add_card(queen_diamonds);
	string trump = Card::SUIT_DIAMONDS;
	bool orderup = sam->make_trump(
		ace_spades,//upcard
		true,//is the dealer dont forget to test if it is dealer!
		1,//first round (the right and left bowers, and Q, K, A of the order-up suit)
		trump);//suit ordered up (if any)
	ASSERT_TRUE(orderup);
	ASSERT_EQUAL(trump, Card::SUIT_SPADES);

	delete sam;
}

TEST(whole20) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * sam = Player_factory("sam", "Simple");
	sam->add_card(king_hearts);
	sam->add_card(jack_clubs);
	sam->add_card(ace_clubs);
	sam->add_card(jack_hearts);
	sam->add_card(queen_diamonds);
	string trump = Card::SUIT_DIAMONDS;
	bool orderup = sam->make_trump(
		ace_spades,//upcard
		true,//is the dealer dont forget to test if it is dealer!
		1,//first round (the right and left bowers, and Q, K, A of the order-up suit)
		trump);//suit ordered up (if any)
	ASSERT_FALSE(orderup);
	ASSERT_EQUAL(trump, Card::SUIT_DIAMONDS);

	delete sam;
}

TEST(whole21) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * sam = Player_factory("sam", "Simple");
	sam->add_card(king_spades);
	sam->add_card(jack_clubs);
	sam->add_card(ace_spades);
	sam->add_card(jack_hearts);
	sam->add_card(queen_diamonds);
	string trump = Card::SUIT_DIAMONDS;
	bool orderup = sam->make_trump(
		ace_spades,//upcard
		true,//is the dealer dont forget to test if it is dealer!
		1,//first round (the right and left bowers, and Q, K, A of the order-up suit)
		trump);//suit ordered up (if any)
	ASSERT_TRUE(orderup);
	ASSERT_EQUAL(trump, Card::SUIT_SPADES);

	delete sam;
}

TEST(whole22) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * sam = Player_factory("sam", "Simple");
	sam->add_card(king_clubs);
	sam->add_card(jack_diamonds);
	sam->add_card(ace_diamonds);
	sam->add_card(ten_diamonds);
	sam->add_card(queen_diamonds);
	string trump = Card::SUIT_DIAMONDS;
	bool orderup = sam->make_trump(
		ace_spades,//upcard
		true,//is the dealer dont forget to test if it is dealer!
		2,//first round (the right and left bowers, and Q, K, A of the order-up suit)
		trump);//suit ordered up (if any)
	ASSERT_TRUE(orderup);
	ASSERT_EQUAL(trump, Card::SUIT_CLUBS);

	delete sam;
}

TEST(whole23) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * sam = Player_factory("sam", "Simple");
	sam->add_card(ten_clubs);
	sam->add_card(jack_diamonds);
	sam->add_card(ace_diamonds);
	sam->add_card(ten_diamonds);
	sam->add_card(queen_diamonds);
	string trump = Card::SUIT_DIAMONDS;
	bool orderup = sam->make_trump(
		ace_spades,//upcard
		true,//is the dealer dont forget to test if it is dealer!
		2,//first round (the right and left bowers, and Q, K, A of the order-up suit)
		trump);//suit ordered up (if any)
	ASSERT_TRUE(orderup);
	ASSERT_EQUAL(trump, Card::SUIT_CLUBS);

	delete sam;
}

TEST(whole24) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * sam = Player_factory("sam", "Simple");
	sam->add_card(king_clubs);
	sam->add_card(jack_diamonds);
	sam->add_card(ace_diamonds);
	sam->add_card(ten_diamonds);
	sam->add_card(queen_diamonds);
	string trump = Card::SUIT_DIAMONDS;
	bool orderup = sam->make_trump(
		ace_spades,//upcard
		false,//is the dealer dont forget to test if it is dealer!
		2,//first round (the right and left bowers, and Q, K, A of the order-up suit)
		trump);//suit ordered up (if any)
	ASSERT_TRUE(orderup);
	ASSERT_EQUAL(trump, Card::SUIT_CLUBS);

	delete sam;
}

TEST(whole25) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * sam = Player_factory("sam", "Simple");
	sam->add_card(ten_clubs);
	sam->add_card(jack_diamonds);
	sam->add_card(ace_diamonds);
	sam->add_card(ten_diamonds);
	sam->add_card(queen_diamonds);
	string trump = Card::SUIT_DIAMONDS;
	bool orderup = sam->make_trump(
		ace_spades,//upcard
		false,//is the dealer dont forget to test if it is dealer!
		2,//first round (the right and left bowers, and Q, K, A of the order-up suit)
		trump);//suit ordered up (if any)
	ASSERT_FALSE(orderup);
	ASSERT_EQUAL(trump, Card::SUIT_DIAMONDS);

	delete sam;
}

TEST(whole26) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * sam = Player_factory("sam", "Simple");
	sam->add_card(jack_diamonds);
	sam->add_card(king_hearts);
	sam->add_card(ace_diamonds);
	sam->add_card(ten_diamonds);
	sam->add_card(queen_diamonds);
	string trump = Card::SUIT_DIAMONDS;
	bool orderup = sam->make_trump(
		jack_hearts,//upcard
		false,//is the dealer dont forget to test if it is dealer!
		1,//first round (the right and left bowers, and Q, K, A of the order-up suit)
		trump);//suit ordered up (if any)
	ASSERT_TRUE(orderup);
	ASSERT_EQUAL(trump, Card::SUIT_HEARTS);

	delete sam;
}

TEST(whole27) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * sam = Player_factory("sam", "Simple");
	sam->add_card(jack_diamonds);
	sam->add_card(king_diamonds);
	sam->add_card(ace_diamonds);
	sam->add_card(ten_diamonds);
	sam->add_card(queen_diamonds);
	string trump = Card::SUIT_SPADES;
	ASSERT_EQUAL(sam->play_card(nine_diamonds, trump), ace_diamonds);
	ASSERT_EQUAL(sam->play_card(nine_diamonds, trump), king_diamonds);
	ASSERT_EQUAL(sam->play_card(nine_diamonds, trump), queen_diamonds);
	ASSERT_EQUAL(sam->play_card(nine_diamonds, trump), jack_diamonds);
	ASSERT_EQUAL(sam->play_card(nine_diamonds, trump), ten_diamonds);

	delete sam;
}

TEST(whole28) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * sam = Player_factory("sam", "Simple");
	sam->add_card(jack_diamonds);
	sam->add_card(king_diamonds);
	sam->add_card(ace_diamonds);
	sam->add_card(jack_spades);
	sam->add_card(queen_diamonds);
	string trump = Card::SUIT_SPADES;
	ASSERT_EQUAL(sam->play_card(nine_diamonds, trump), ace_diamonds);
	ASSERT_EQUAL(sam->play_card(nine_diamonds, trump), king_diamonds);
	ASSERT_EQUAL(sam->play_card(nine_diamonds, trump), queen_diamonds);
	ASSERT_EQUAL(sam->play_card(nine_diamonds, trump), jack_diamonds);
	ASSERT_EQUAL(sam->play_card(nine_diamonds, trump), jack_spades);

	delete sam;
}

TEST(whole29) {
	Card ace_spades = Card(Card::RANK_ACE, Card::SUIT_SPADES);
	Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
	Card ten_spades = Card(Card::RANK_TEN, Card::SUIT_SPADES);
	Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
	Card queen_spades = Card(Card::RANK_QUEEN, Card::SUIT_SPADES);
	Card king_spades = Card(Card::RANK_KING, Card::SUIT_SPADES);

	Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
	Card nine_clubs = Card(Card::RANK_NINE, Card::SUIT_CLUBS);
	Card ten_clubs = Card(Card::RANK_TEN, Card::SUIT_CLUBS);
	Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
	Card queen_clubs = Card(Card::RANK_QUEEN, Card::SUIT_CLUBS);
	Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);

	Card ace_diamonds = Card(Card::RANK_ACE, Card::SUIT_DIAMONDS);
	Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
	Card ten_diamonds = Card(Card::RANK_TEN, Card::SUIT_DIAMONDS);
	Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
	Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
	Card king_diamonds = Card(Card::RANK_KING, Card::SUIT_DIAMONDS);

	Card ace_hearts = Card(Card::RANK_ACE, Card::SUIT_HEARTS);
	Card nine_hearts = Card(Card::RANK_NINE, Card::SUIT_HEARTS);
	Card ten_hearts = Card(Card::RANK_TEN, Card::SUIT_HEARTS);
	Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
	Card queen_hearts = Card(Card::RANK_QUEEN, Card::SUIT_HEARTS);
	Card king_hearts = Card(Card::RANK_KING, Card::SUIT_HEARTS);
	Player * sam = Player_factory("sam", "Simple");
	sam->add_card(nine_diamonds);
	sam->add_card(ten_diamonds);
	sam->add_card(ace_diamonds);
	sam->add_card(jack_diamonds);
	sam->add_card(queen_diamonds);
	string trump = Card::SUIT_HEARTS;
	ASSERT_EQUAL(sam->play_card(nine_clubs, trump), nine_diamonds);
	ASSERT_EQUAL(sam->play_card(nine_clubs, trump), ten_diamonds);
	ASSERT_EQUAL(sam->play_card(nine_clubs, trump), queen_diamonds);
	ASSERT_EQUAL(sam->play_card(nine_clubs, trump), ace_diamonds);
	ASSERT_EQUAL(sam->play_card(nine_clubs, trump), jack_diamonds);

	delete sam;
}

TEST_MAIN()



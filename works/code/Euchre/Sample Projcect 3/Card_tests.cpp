#include "Card.h"
#include "unit_test_framework.h"
#include <iostream>
#include <string>

using namespace std;
TEST(test_card_suit_and_rank) {
    Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
    ASSERT_EQUAL(queen_diamonds.get_rank(), Card::RANK_QUEEN);
    ASSERT_EQUAL(queen_diamonds.get_suit(), Card::SUIT_DIAMONDS);
    ASSERT_EQUAL(queen_diamonds.get_suit(Card::SUIT_DIAMONDS), Card::SUIT_DIAMONDS);
}

TEST(test_card_type) {
    Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
    ASSERT_TRUE(jack_clubs.is_face());
    ASSERT_TRUE(jack_clubs.is_right_bower(Card::SUIT_CLUBS));
    ASSERT_FALSE(jack_clubs.is_left_bower(Card::SUIT_DIAMONDS));
    ASSERT_TRUE(jack_clubs.is_trump(Card::SUIT_CLUBS));
    Card jack_hearts = Card(Card::RANK_JACK, Card::SUIT_HEARTS);
    ASSERT_TRUE(jack_hearts.is_face());
    ASSERT_TRUE(jack_hearts.is_right_bower(Card::SUIT_HEARTS));
    ASSERT_FALSE(jack_hearts.is_left_bower(Card::SUIT_SPADES));
    ASSERT_TRUE(jack_hearts.is_trump(Card::SUIT_HEARTS));
    Card jack_diamonds = Card(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    ASSERT_TRUE(jack_diamonds.is_face());
    ASSERT_TRUE(jack_diamonds.is_right_bower(Card::SUIT_DIAMONDS));
    ASSERT_FALSE(jack_diamonds.is_left_bower(Card::SUIT_CLUBS));
    ASSERT_TRUE(jack_diamonds.is_trump(Card::SUIT_DIAMONDS));
    Card jack_spades = Card(Card::RANK_JACK, Card::SUIT_SPADES);
    ASSERT_TRUE(jack_spades.is_face());
    ASSERT_TRUE(jack_spades.is_right_bower(Card::SUIT_SPADES));
    ASSERT_FALSE(jack_spades.is_left_bower(Card::SUIT_HEARTS));
    ASSERT_TRUE(jack_spades.is_trump(Card::SUIT_SPADES));
}

TEST(test_card_self_comparison) {
    Card jack_clubs = Card(Card::RANK_JACK, Card::SUIT_CLUBS);
    Card nine_spades = Card(Card::RANK_NINE, Card::SUIT_SPADES);
    Card ace_clubs = Card(Card::RANK_ACE, Card::SUIT_CLUBS);
    Card king_clubs = Card(Card::RANK_KING, Card::SUIT_CLUBS);
    Card queen_diamonds = Card(Card::RANK_QUEEN, Card::SUIT_DIAMONDS);
    Card nine_diamonds = Card(Card::RANK_NINE, Card::SUIT_DIAMONDS);
    ASSERT_TRUE(jack_clubs > nine_spades);
    ASSERT_TRUE(jack_clubs < ace_clubs);
    ASSERT_TRUE(jack_clubs < king_clubs);
    ASSERT_TRUE(jack_clubs < queen_diamonds);
    ASSERT_FALSE(jack_clubs < nine_diamonds);
    ASSERT_TRUE(jack_clubs == jack_clubs);
    ASSERT_TRUE(jack_clubs != nine_spades);
    ASSERT_TRUE(jack_clubs > nine_spades);
}

TEST(test_Suit_next) {
    ASSERT_EQUAL(Suit_next(Card::SUIT_CLUBS), Card::SUIT_SPADES);
    ASSERT_EQUAL(Suit_next(Card::SUIT_HEARTS), Card::SUIT_DIAMONDS);
    ASSERT_EQUAL(Suit_next(Card::SUIT_SPADES), Card::SUIT_CLUBS);
    ASSERT_EQUAL(Suit_next(Card::SUIT_DIAMONDS), Card::SUIT_HEARTS);
}

TEST(test_Card_less_self) {
    Card three_spades = Card(Card::RANK_THREE, Card::SUIT_SPADES);
    ASSERT_FALSE(Card_less(three_spades, three_spades, Card::SUIT_CLUBS));
    ASSERT_FALSE(Card_less(three_spades, three_spades, three_spades,
                           Card::SUIT_CLUBS));
}

TEST(test_card_insertion) {
    Card three_spades = Card(Card::RANK_THREE, Card::SUIT_SPADES);
    ostringstream oss;
    oss << three_spades;
    ASSERT_EQUAL(oss.str(), "Three of Spades");
}

TEST(test_card_ctor) {
    Card c(Card::RANK_ACE, Card::SUIT_HEARTS);
    ASSERT_EQUAL(Card::RANK_ACE, c.get_rank());
    ASSERT_EQUAL(Card::SUIT_HEARTS, c.get_suit());
}

TEST(test_card_less) {
    string trump = Card::SUIT_HEARTS;
    Card c(Card::RANK_ACE, Card::SUIT_SPADES);
    Card c1(Card::RANK_NINE, Card::SUIT_CLUBS);
    Card c2(Card::RANK_KING, Card::SUIT_HEARTS);//right-bower
    Card c3(Card::RANK_JACK, Card::SUIT_DIAMONDS);//left-bower
    ASSERT_TRUE(Card_less(c1, c,trump));
    ASSERT_TRUE(Card_less(c1, c2, c, trump));
    ASSERT_FALSE(Card_less(c3, c2, c, trump));
    ASSERT_TRUE(Card_less(c1, c3, trump));
    string trump2 = Card::SUIT_SPADES;
    Card a(Card::RANK_ACE, Card::SUIT_SPADES);
    Card a1(Card::RANK_NINE, Card::SUIT_CLUBS);
    Card a2(Card::RANK_QUEEN, Card::SUIT_HEARTS);//right-bower
    Card a3(Card::RANK_JACK, Card::SUIT_DIAMONDS);//left-bower
    ASSERT_TRUE(Card_less(a1, a,trump2));
    ASSERT_TRUE(Card_less(a1, a2, a, trump2));
    ASSERT_TRUE(Card_less(a3, a2, a, trump2));
    ASSERT_TRUE(Card_less(a1, a3, trump2));
    string trump3 = Card::SUIT_DIAMONDS;
    Card b(Card::RANK_KING, Card::SUIT_SPADES);
    Card b1(Card::RANK_NINE, Card::SUIT_CLUBS);
    Card b2(Card::RANK_QUEEN, Card::SUIT_HEARTS);//right-bower
    Card b3(Card::RANK_JACK, Card::SUIT_DIAMONDS);//left-bower
    ASSERT_TRUE(Card_less(b1, b,trump3));
    ASSERT_TRUE(Card_less(b1, b2, b, trump3));
    ASSERT_FALSE(Card_less(b3, b2, b, trump3));
    ASSERT_TRUE(Card_less(b1, b3, trump3));
}
TEST(test_card_less_led) {
    string trump = Card::SUIT_HEARTS;
    Card c(Card::RANK_JACK, Card::SUIT_SPADES);
    Card c1(Card::RANK_NINE, Card::SUIT_CLUBS);
    Card c2(Card::RANK_JACK, Card::SUIT_HEARTS);//right-bower
    Card c3(Card::RANK_JACK, Card::SUIT_DIAMONDS);//left-bower
    Card c4(Card::RANK_ACE, Card::SUIT_SPADES);
    ASSERT_TRUE(Card_less(c1,c,c,trump));
    ASSERT_TRUE(Card_less(c4, c3, c, trump));
    ASSERT_TRUE(Card_less(c4, c2, c, trump));
    ASSERT_TRUE(Card_less(c3, c2, c, trump));
    ASSERT_TRUE(Card_less(c1, c3, c, trump));
    string trump2 = Card::SUIT_CLUBS;
    Card a(Card::RANK_JACK, Card::SUIT_SPADES);
    Card a1(Card::RANK_NINE, Card::SUIT_CLUBS);
    Card a2(Card::RANK_ACE, Card::SUIT_HEARTS);//right-bower
    Card a3(Card::RANK_JACK, Card::SUIT_DIAMONDS);//left-bower
    Card a4(Card::RANK_ACE, Card::SUIT_SPADES);
    ASSERT_TRUE(Card_less(a1,a,a,trump2));
    ASSERT_FALSE(Card_less(a4, a3, a, trump2));
    ASSERT_FALSE(Card_less(a4, a2, a, trump2));
    ASSERT_TRUE(Card_less(a3, a2, a, trump2));
    ASSERT_FALSE(Card_less(a1, a3, a, trump2));
    
}
TEST(test_is_trump) {
    string trump = Card::SUIT_HEARTS;
    Card c(Card::RANK_JACK, Card::SUIT_SPADES);
    Card c1(Card::RANK_NINE, Card::SUIT_CLUBS);
    Card c2(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card c3(Card::RANK_JACK, Card::SUIT_SPADES);
    ASSERT_FALSE(c.is_trump(trump));
    ASSERT_FALSE(c1.is_trump(trump));
    ASSERT_FALSE(!c2.is_trump(trump));
    ASSERT_FALSE(c3.is_trump(trump));
    
}
TEST(test_operator){
    Card c(Card::RANK_JACK, Card::SUIT_SPADES);
    Card c1(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card c2(Card::RANK_KING, Card::SUIT_SPADES);
    ASSERT_TRUE(c<c1);
    ASSERT_TRUE(c<c2);
    ASSERT_TRUE(c1<c2);
    ASSERT_TRUE(c1>c);
    ASSERT_TRUE(c2>c);
    ASSERT_TRUE(c2>c1);
    
}
// Add more test cases here

TEST_MAIN()


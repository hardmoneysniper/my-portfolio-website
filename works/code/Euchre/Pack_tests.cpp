#include "Pack.h"
#include "Card.h"
#include "unit_test_framework.h"
#include <cassert>
#include <iostream>
#include <fstream>
using namespace std;

using namespace std;
static const int PACK_SIZE = 24;

TEST(test_pack_default_ctor) {
	Pack pack;
	Card first = pack.deal_one();
	ASSERT_EQUAL(Card::RANK_NINE, first.get_rank());
	ASSERT_EQUAL(Card::SUIT_SPADES, first.get_suit());
	Card second = pack.deal_one();
	ASSERT_EQUAL(Card::RANK_TEN, second.get_rank());
	ASSERT_EQUAL(Card::SUIT_SPADES, second.get_suit());
	Card third = pack.deal_one();
	ASSERT_EQUAL(Card::RANK_JACK, third.get_rank());
	ASSERT_EQUAL(Card::SUIT_SPADES, third.get_suit());
	Card fourth = pack.deal_one();
	ASSERT_EQUAL(Card::RANK_QUEEN, fourth.get_rank());
	ASSERT_EQUAL(Card::SUIT_SPADES, fourth.get_suit());
	Card fifth = pack.deal_one();
	ASSERT_EQUAL(Card::RANK_KING, fifth.get_rank());
	ASSERT_EQUAL(Card::SUIT_SPADES, fifth.get_suit());
	Card sixth = pack.deal_one();
	ASSERT_EQUAL(Card::RANK_ACE, sixth.get_rank());
	ASSERT_EQUAL(Card::SUIT_SPADES, sixth.get_suit());
	Card seventh = pack.deal_one();
	ASSERT_EQUAL(Card::RANK_NINE, seventh.get_rank());
	ASSERT_EQUAL(Card::SUIT_HEARTS, seventh.get_suit());
}


TEST(test_pack_istream_ctor) {
	const string filename = "pack.in";
	ifstream ifs(filename);
	assert(ifs.is_open());
	Pack pack(ifs);
	Card first_card = pack.deal_one();
	ASSERT_EQUAL(first_card, Card(Card::RANK_NINE, Card::SUIT_SPADES));
}

TEST(test_pack_reset) {
	Pack pack;
	pack.deal_one();
	pack.reset();
	Card first_card = pack.deal_one();
	ASSERT_EQUAL(first_card, Card(Card::RANK_NINE, Card::SUIT_SPADES));
	Card second_card = pack.deal_one();
	ASSERT_EQUAL(second_card, Card(Card::RANK_TEN, Card::SUIT_SPADES));
	Card third_card = pack.deal_one();
	ASSERT_EQUAL(third_card, Card(Card::RANK_JACK, Card::SUIT_SPADES));
	Card fourth_card = pack.deal_one();
	ASSERT_EQUAL(fourth_card, Card(Card::RANK_QUEEN, Card::SUIT_SPADES));
	Card fifth_card = pack.deal_one();
	ASSERT_EQUAL(fifth_card, Card(Card::RANK_KING, Card::SUIT_SPADES));
	Card sixth_card = pack.deal_one();
	ASSERT_EQUAL(sixth_card, Card(Card::RANK_ACE, Card::SUIT_SPADES));
	Card seventh_card = pack.deal_one();
	ASSERT_EQUAL(seventh_card, Card(Card::RANK_NINE, Card::SUIT_HEARTS));
}

TEST(test_pack_empty) {
	Pack pack;
	for (int i = 0; i < PACK_SIZE - 1; ++i) {
		pack.deal_one();
		ASSERT_FALSE(pack.empty());
	}
	pack.deal_one();
	ASSERT_TRUE(pack.empty());
}

TEST(test_pack_shuffle) {
	Pack pack;
	pack.shuffle();
	Card first_card = pack.deal_one();
	ASSERT_EQUAL(first_card, Card(Card::RANK_KING, Card::SUIT_CLUBS));
	Card second_card = pack.deal_one();
	ASSERT_EQUAL(second_card, Card(Card::RANK_JACK, Card::SUIT_HEARTS));
	Card third_card = pack.deal_one();
	ASSERT_EQUAL(third_card, Card(Card::RANK_NINE, Card::SUIT_SPADES));
	Card fourth_card = pack.deal_one();
	ASSERT_EQUAL(fourth_card, Card(Card::RANK_ACE, Card::SUIT_CLUBS));
}

TEST_MAIN()


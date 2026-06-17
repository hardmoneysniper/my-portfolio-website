#include "Card.h"
#include <array>
#include <string>
#include "Pack.h"
#include <cmath>

using namespace std;
Pack::Pack() {
	int k = 0;
	for (int i = 0; i < NUM_SUITS; ++i) {
		for (int j = 7; j < NUM_RANKS; ++j) {
			cards[k]=Card(RANK_NAMES_BY_WEIGHT[j], SUIT_NAMES_BY_WEIGHT[i]);
			k = k + 1;
		}
	}
	next = 0; 
}

Pack::Pack(std::istream& pack_input) {
	next = 0;
	int i = 0;
	string rank_in;
	string of;
	string suit_in;
	while (pack_input >> rank_in >> of >> suit_in) {
		cards[i] = Card(rank_in, suit_in);// will this creat a new card? what about do card c first?
		i = i + 1;//what is istream actually mean??? How to use it????
	}
}

Card Pack::deal_one() {
	next = next + 1;
	return cards[next - 1];
}

void Pack::reset() {
	next = 0;
}

void Pack::shuffle() {
	/*int shuffleCount = 0;
	array<Card, PACK_SIZE>tempCards;
	next = 0;
	for (int i = 0; i < PACK_SIZE; ++i) {
		tempCards[i] = cards[i];
	}
	for (int i = 0; i < PACK_SIZE; ++i) {
		cards[(2 * i + 1) % 25] = tempCards[i];
	}
	++shuffleCount;
	*/
	array<Card, PACK_SIZE> cards_copy;
	for (int i = 0; i < 7; ++i) {
		cards_copy = cards;
		for (int k = 0; k < PACK_SIZE; k++) {
			cards[(2 * k + 1) % (25)] = cards_copy[k];
		}
	}
	reset();
	/*array<Card, PACK_SIZE> cards_copy;
	int j = 0;
	for (int i = 0; i < PACK_SIZE; ++i) {
		cards_copy[i] = cards[i];

	}
	for (int i = 0; i < 23; i=i+2) {
		cards[i] = cards_copy[j+12];
		cards[i+1] = cards_copy[j];
		j = j + 1;
	}
	reset();
	*/
}

bool Pack::empty()const {
	return (next == PACK_SIZE);
}                       
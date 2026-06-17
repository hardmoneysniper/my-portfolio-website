#include <cassert>
#include "Card.h"
#include <iostream>
#include <string>

Card::Card()
	:rank(RANK_TWO),suit(SUIT_SPADES){}
Card::Card(const std::string &rank_in,const std::string &suit_in)
	:rank(rank_in),suit(suit_in){}
std::string Card::get_rank() const {
	return rank;
}
std::string Card::get_suit()const {
	return suit;
}
std::string Card::get_suit(const std::string &trump) const {
	if (rank == RANK_JACK) {
		if (trump == SUIT_CLUBS && suit == SUIT_SPADES) {
			return trump;
		}
		else if (trump == SUIT_SPADES && suit == SUIT_CLUBS) {
			return trump;
		}
		else if (trump == SUIT_DIAMONDS && suit == SUIT_HEARTS) {
			return trump;
		}
		else if (trump == SUIT_HEARTS && suit == SUIT_DIAMONDS) {
			return trump;
		}
		else {
			return suit;
		}
	}
	else {
		return suit;
	}
}
bool Card::is_face() const {
	if (rank == RANK_ACE || rank == RANK_JACK || rank == RANK_QUEEN || rank == RANK_KING) {
		return true;
	}
	else {
		return false;
	}
}
bool Card::is_right_bower(const std::string &trump) const {
	if (rank == RANK_JACK && suit == trump) {
		return true;
	}
	else {
		return false;
	}
}
bool Card::is_left_bower(const std::string &trump) const {
	if (rank == RANK_JACK) {
		if (trump == SUIT_CLUBS && suit == SUIT_SPADES) {
			return true;
		}
		else if (trump == SUIT_SPADES && suit == SUIT_CLUBS) {
			return true;
		}
		else if (trump == SUIT_DIAMONDS && suit == SUIT_HEARTS) {
			return true;
		}
		else if (trump == SUIT_HEARTS && suit == SUIT_DIAMONDS) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}
bool Card::is_trump(const std::string &trump) const {
	if (suit == trump) {
		return true;
	}else if (is_left_bower(trump)){//why this will work???
		return true;
	}
	else {
		return false;
	}
}

bool operator<(const Card &lhs, const Card &rhs) {
	int rank_lhs = 0;
	int rank_rhs = 0;
	int suit_lhs = 0;
	int suit_rhs = 0;
	for (int i = 0; i < NUM_RANKS; ++i) {
		if (RANK_NAMES_BY_WEIGHT[i] == lhs.get_rank()) {
			rank_lhs = i;
		}
		if (RANK_NAMES_BY_WEIGHT[i] == rhs.get_rank()) {
			rank_rhs = i;
		}
	}
	for (int i = 0; i < NUM_SUITS; ++i) {
		if (SUIT_NAMES_BY_WEIGHT[i] == lhs.get_suit()) {
			suit_lhs = i;
		}
		if (SUIT_NAMES_BY_WEIGHT[i] == rhs.get_suit()) {
			suit_rhs = i;
		}
	}
	if (rank_rhs > rank_lhs) {
		return true;
	}
	else if (rank_rhs == rank_lhs && suit_rhs > suit_lhs) {
		return true;
	}
	else {
		return false;
	}
}
bool operator>(const Card &lhs, const Card &rhs) {
	int rank_lhs = 0;
	int rank_rhs = 0;
	int suit_lhs = 0;
	int suit_rhs = 0;
	for (int i = 0; i < NUM_RANKS; ++i) {
		if (RANK_NAMES_BY_WEIGHT[i] == lhs.get_rank()) {
			rank_lhs = i;
		}
		if (RANK_NAMES_BY_WEIGHT[i] == rhs.get_rank()) {
			rank_rhs = i;
		}
	}
	for (int i = 0; i < NUM_SUITS; ++i) {
		if (SUIT_NAMES_BY_WEIGHT[i] == lhs.get_suit()) {
			suit_lhs = i;
		}
		if (SUIT_NAMES_BY_WEIGHT[i] == rhs.get_suit()) {
			suit_rhs = i;
		}
	}
	if (rank_rhs < rank_lhs) {
		return true;
	}
	else if (rank_rhs == rank_lhs && suit_rhs < suit_lhs) {
		return true;
	}
	else {
		return false;
	}
}
bool operator==(const Card &lhs, const Card &rhs) {
	return(lhs.get_rank() == rhs.get_rank() && lhs.get_suit() == rhs.get_suit());
}
bool operator!=(const Card &lhs, const Card &rhs) {
	return !(lhs == rhs);
}
std::string Suit_next(const std::string &suit) {
	if (suit == Card::SUIT_CLUBS) {
		return Card::SUIT_SPADES;
	}
	else if (suit == Card::SUIT_SPADES) {
		return Card::SUIT_CLUBS;
	}
	else if (suit == Card::SUIT_DIAMONDS) {
		return Card::SUIT_HEARTS;
	}
	else {
		return Card::SUIT_DIAMONDS;
	}

}
std::ostream & operator<<(std::ostream &os, const Card &card) {
	os << card.get_rank() << " of " << card.get_suit();
	return os;
}

bool Card_less(const Card &a, const Card &b, const std::string &trump) {
	if (a!=b) {
		if (a.is_trump(trump) && b.is_trump(trump)) {
			if (!a.is_right_bower(trump) && !b.is_right_bower(trump)) {
				if (a.is_left_bower(trump)) {
					return false;
				}
				else if (b.is_left_bower(trump)) {
					return true;
				}
				else {
					return (a<b);
				}
			}
			else if (a.is_right_bower(trump)) {
				return false;
			}
			else {
				return (b.is_right_bower(trump));
			}
		}
		else if (a.is_trump(trump) && !b.is_trump(trump)) {
			return false;
		}
		else if (!a.is_trump(trump) && b.is_trump(trump)) {
			return true;
		}
		else {
			return (a<b);
		}
	}
	else {
		return false;
	}
}

bool Card_less(const Card &a, const Card &b, const Card &led_card, const std::string &trump) {
	if (a!=b) {
		if (a.is_trump(trump) && b.is_trump(trump)) {
			return Card_less(a, b, trump);
		}
		else if (a.is_trump(trump)&&!b.is_trump(trump)) {
			return false;
		}
		else if(!a.is_trump(trump)&& b.is_trump(trump)){
			return true;
		}
		else
		{
			if (a.get_suit(trump) == led_card.get_suit(trump) && b.get_suit(trump) == led_card.get_suit(trump)) {
				return (a < b);
			}
			else if (a.get_suit(trump) != led_card.get_suit(trump) && b.get_suit(trump) == led_card.get_suit(trump)) {
				return true;
			}
			else if (a.get_suit(trump) == led_card.get_suit(trump) && b.get_suit(trump) != led_card.get_suit(trump)) {
				return false;
			}
			else {
				return (a < b);
			}
		}
	}
	else {
		return false;
	}
}

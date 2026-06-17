#include "Card.h"
#include <string>
#include <vector>
#include "Player.h"
#include <cassert>
#include <iostream>
#include <algorithm>
using namespace std;

class Simple:public Player{
private:
	string name1;
	vector<Card>cardAtHand;
public:
	Simple(string name_in)
		:name1(name_in){}
	virtual const std::string & get_name() const override {
		return name1;
	}
	virtual void add_card(const Card &c)override {
		cardAtHand.push_back(c);
	}
	virtual bool make_trump(const Card &upcard, bool is_dealer,
		int round, std::string &order_up_suit) const override{
		if (round == 1) {
			int count = 0;
			for (int i = 0; i < 5; i++) {
				if (cardAtHand[i].is_face() &&
					cardAtHand[i].is_trump(upcard.get_suit())) {
				//cardAtHand[i].get_suit()==upcard.get_suit()) {
					count = count + 1;
				}
			}
			if (count >= 2) {
				order_up_suit = upcard.get_suit();
				return true;
			}
			else {
				return false;
			}
		}
		else {
			int count = 0;
			if (is_dealer ) {
				order_up_suit = Suit_next(upcard.get_suit());
				return true;
			}
			else {
				for (int i = 0; i < 5; i++) {
					if (cardAtHand[i].is_face() 
						&& cardAtHand[i].is_trump(Suit_next(upcard.get_suit()))) {
						count = count + 1;
					}
				}
				if (count >= 1) {
					order_up_suit = Suit_next(upcard.get_suit());
					return true;
				}
				else {
					return false;
				}
			}
		}
	}

	virtual void add_and_discard(const Card &upcard)override {
		int min = 0;
		for (int i = 1; i < int(cardAtHand.size()); i++) {
				if (Card_less(cardAtHand[i], cardAtHand[min], upcard.get_suit())) {
					min = i;
				}
		}
		if (Card_less(cardAtHand[min], upcard, upcard.get_suit())) {
			cardAtHand[min] = upcard;
		}
	}
	virtual Card lead_card(const std::string &trump) override{
		vector<Card>cardAtHand_copy;
		auto ptr = cardAtHand.begin();
		int count = 0;
		for (int i = 0; i < int(cardAtHand.size()); i++) {
			if (cardAtHand[i].is_trump(trump)) {
				count = count + 1;
			}
		}
		if (count == int(cardAtHand.size())) {
			int max = 0;
			for (int i = 1; i < int(cardAtHand.size()); ++i) {
				if (Card_less( cardAtHand[max], cardAtHand[i], trump)) {
					max = i;
				}
			}
			cardAtHand_copy = cardAtHand;
			cardAtHand.erase(ptr + max);
			return cardAtHand_copy[max];
		}
		else {
			int max = 0;
			for (int j = 0; j < int(cardAtHand.size()); j++) {
				if (!cardAtHand[j].is_trump(trump)) {
					max= j;
				}
			}
			for (int i = 0; i < int(cardAtHand.size()); ++i) {
				if (Card_less( cardAtHand[max], cardAtHand[i], trump) 
					&& !cardAtHand[i].is_trump(trump)) {
					max = i;
				}
			}
			cardAtHand_copy = cardAtHand;
			cardAtHand.erase(ptr + max);
			return cardAtHand_copy[max];
		}
	}
	virtual Card play_card(const Card &led_card, const std::string &trump) override{
		vector<Card>cardAtHand_copy;
		auto ptr = cardAtHand.begin();
		//find a lead card and the number of led card at hand
		int lead_card = 0;
		int count = 0;
		for (int i = 0; i < int(cardAtHand.size()); ++i) {
			if (led_card.is_left_bower(trump)) {
				if (cardAtHand[i].is_trump(trump)) {
					lead_card = i;
					count = count + 1;
				}
			}
			else if (cardAtHand[i].get_suit()==led_card.get_suit()&& !(cardAtHand[i].is_left_bower(trump))) {
				lead_card = i;
				count = count + 1;
			}
			else if (cardAtHand[i].is_left_bower(trump)) {
				if (trump == led_card.get_suit()) {
					lead_card = i;
					count = count + 1;
				}
			}
		}
		if (count == 0) {
			int min = 0;
			for (int j = 0; j < int(cardAtHand.size());++j){
				if (Card_less(cardAtHand[j], cardAtHand[min], led_card,trump)) {
					min = j;
				}
			}
			cardAtHand_copy = cardAtHand;
			cardAtHand.erase(ptr + min);
			return cardAtHand_copy[min];
		}
		else {
			int max = lead_card;
			for (int j = 0; j < int(cardAtHand.size()); ++j) {
				if (Card_less(cardAtHand[max], cardAtHand[j], led_card, trump) && cardAtHand[j].get_suit() == led_card.get_suit()&&!cardAtHand[j].is_left_bower(trump) ) {
					max = j;
				}
			}
			cardAtHand_copy = cardAtHand;
			cardAtHand.erase(ptr + max);
			cardAtHand_copy[max];
			return cardAtHand_copy[max];
		}
	}
};

class Human :public Player {
private:
	string name2;
	vector<Card>cardHand;
public:
	Human(string name_in)
		:name2(name_in) {}
	virtual const std::string & get_name() const override {
		return name2;
	}
	virtual void add_card(const Card &c) override {
		cardHand.push_back(c);
	}
	virtual bool make_trump(const Card &upcard, bool is_dealer,
		int round, std::string &order_up_suit) const override {
		//std::sort(cardHand.begin(), cardHand.end()); cannot using sort to change a constant object.
		vector<Card>cardHand_copy;
		cardHand_copy = cardHand;
		sort(cardHand_copy.begin(), cardHand_copy.end());
		for (int i = 0; i < 5; ++i) {
			cout << "Human player " << name2 << "'s hand: " << "[" << i
				<< "] " << cardHand_copy[i] << endl;
		}
		cout << "Human player " << name2 << ", please enter a suit, or \"pass\":"
			<< endl;
		string s;
		cin >> s;
		if (s == "pass") {
			//cout << name2 << "passes" << endl;
			return false;
		}
		else if (s == "Diamonds" || s == "Spades" || s == "Hearts" || s == "Clubs") {
			//cout << name2 << " orders up " << s << endl;
			order_up_suit = s;
			return true;
		}
		else {
			//cout << "passes" << endl;
			return false;
		}
	}
	virtual void add_and_discard(const Card &upcard) override {
		auto ptr = cardHand.begin();
		for (int i = 0; i < int(cardHand.size()); i++) {
			cout << "Human player " << name2 << "'s hand: " << "[" << i
				<< "] " << cardHand[i] << endl;
		}
		cout << "Dicard upcard: [-1]" << endl;
		cout << "Human player " << name2 << ", please select a card to dicard:" << endl;
		int a = 0;
		cin >> a;
		cardHand.erase(ptr + a);
		cardHand.push_back(upcard);
		cout << endl;
	}
	virtual Card lead_card(const std::string &trump) override {
		std::sort(cardHand.begin(), cardHand.end());
		auto ptr = cardHand.begin();
		for (int i = 0; i < int(cardHand.size()); i++) {
			cout << "Human player " << name2 << "'s hand: " << "[" << i
				<< "] " << cardHand[i] << endl;
		}
		cout << "Human player " << name2 << ", please select a card:" << endl;
		int a = 0;
		cin >> a;
		Card lead_card = cardHand[a];
		cardHand.erase(ptr + a);
		return lead_card;
	}
	virtual Card play_card(const Card &led_card, const std::string &trump) override {
		std::sort(cardHand.begin(), cardHand.end());
		auto ptr = cardHand.begin();
		for (int i = 0; i < int(cardHand.size()); i++) {
			cout << "Human player " << name2 << "'s hand: " << "[" << i
				<< "] " << cardHand[i] << endl;
		}
		cout << "Human player " << name2 << ", please select a card:" << endl;
		int a = 0;
		cin >> a;
		Card play_card = cardHand[a];
		//Card *play_card = &cardHand[a];
		cardHand.erase(ptr + a);
		return play_card;
		//delete play_card;
		//return *play_card;
	}
};
Player * Player_factory(const std::string &name, const std::string &strategy) {
	if (strategy == "Simple") {
		return new Simple(name);
		//how to delete?
	}
	if (strategy == "Human") {
		return new Human(name);
	}
	assert(false);
	return nullptr;
}
ostream & operator<<(ostream &os, const Player &p) {
	os << p.get_name();
	return os;
}

		
	
	
	
	
	
	
	


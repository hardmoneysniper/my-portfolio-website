#include <iostream>
#include <string>
#include "Player.h"
#include "Card.h"
#include "Pack.h"
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;


class Simple : public Player {
private:
    string name1;
    vector<Card> cardAtHand;
public:
    Simple(string name_in){
    name1 = name_in;
}
//EFFECTS returns player's name
virtual const std::string & get_name() const {
    return name1;
}

//REQUIRES player has less than MAX_HAND_SIZE cards
//EFFECTS  adds Card c to Player's hand
    virtual void add_card(const Card &c) {
        cardAtHand.push_back(c);
    }

//REQUIRES round is 1 or 2
//MODIFIES order_up_suit
//EFFECTS If Player wishes to order up a trump suit then return true and
//  change order_up_suit to desired suit.  If Player wishes to pass, then do
//  not modify order_up_suit and return false.
virtual bool make_trump(const Card &upcard, bool is_dealer,
                        int round, std::string &order_up_suit) const {
    if (round == 1) {
        int count = 0;
        for (int i = 0; i < 5; i++) {
        if (cardAtHand[i].is_right_bower(upcard.get_suit()) ||
            cardAtHand[i].is_left_bower(upcard.get_suit())) {
            count++;
        }
            else if (cardAtHand[i].is_face() && (cardAtHand[i].get_suit()
                     == upcard.get_suit())) {
                count++;
            }
    }
        if (count > 1) {
            order_up_suit = upcard.get_suit();
            return true;
        }
    }
        
        if (round == 2) {
            int count = 0;
            for (int j = 0; j < 5; j++) {
                if (cardAtHand[j].get_suit() == Suit_next(upcard.get_suit())
                    && cardAtHand[j].is_face()) {
                    count++;
                }
            }
            if (count > 0) {
                order_up_suit = Suit_next(upcard.get_suit());
                return true;
            }
            if (is_dealer == 1) {
                order_up_suit = Suit_next(upcard.get_suit());
                return true;
            }
        }
        return false;
}

//REQUIRES Player has at least one card
//EFFECTS  Player adds one card to hand and removes one card from hand.
    virtual void add_and_discard(const Card &upcard) {
        //find smallest card at hand
        Card min = cardAtHand[0];
        Card lead = lead_card(upcard.get_suit());
        int num = 0;
        for (int i = 0; i < int(cardAtHand.size()); i++) {
            if (Card_less(cardAtHand[i], min, lead, upcard.get_suit())) {
                min = cardAtHand[i];
                num = i;
            }
        }
        //discard the smallest card
        if (Card_less(min, upcard, lead, upcard.get_suit())) {
            cardAtHand[num] = upcard;
        }
    }
    

//REQUIRES Player has at least one card, trump is a valid suit
//EFFECTS  Leads one Card from Player's hand according to their strategy
//  "Lead" means to play the first Card in a trick.  The card
//  is removed the player's hand.
    virtual Card lead_card(const std::string &trump) {
        //sort cards by trump
        Card a;
        for (int i = 1; i < int(cardAtHand.size()); i++) {
            for (int j = 0; j < int(cardAtHand.size()) - i; j++) {
                if (Card_less(cardAtHand[j + 1], cardAtHand[j], trump)) {
                    a = cardAtHand[j + 1];
                    cardAtHand[j + 1] = cardAtHand[j];
                    cardAtHand[j] = a;
                    
                }
            }
        }
        //calculate the number of trump
        int count = 0;
        for (int k = 1; k < int(cardAtHand.size()); k++) {
            if (cardAtHand[k].is_trump(trump)) {
                count++;
            }
        }
        if (count == 0){
            return cardAtHand[int(cardAtHand.size()) - 1];
        }
        if (count == int(cardAtHand.size())) {
            return cardAtHand[int(cardAtHand.size()) - 1];
        }
        Card lead = cardAtHand[count - 1];
        int s = 0;
        while (s < cardAtHand.size() - count ) {
            cardAtHand[count - 1 + s] = cardAtHand[count + s];
            s++;
        }
        cardAtHand.erase(cardAtHand.begin() + cardAtHand.size() - 1);
        return lead;
    }

//REQUIRES Player has at least one card, trump is a valid suit
//EFFECTS  Plays one Card from Player's hand according to their strategy.
//  The card is removed from the player's hand.
    virtual Card play_card(const Card &led_card, const std::string &trump) {
        //sort cards by lead and trump
        Card a;
        Card play;
        for (int i = 1; i < int(cardAtHand.size()); i++) {
            for (int j = 0; j < int(cardAtHand.size()) - i; j++) {
                if (Card_less(cardAtHand[j + 1], cardAtHand[j],
                              led_card, trump)){
                    a = cardAtHand[j + 1];
                    cardAtHand[j + 1] = cardAtHand[j];
                    cardAtHand[j] = a;
                    
                }
            }
        }
        for (int i = 0; i < cardAtHand.size(); i++) {
            if (cardAtHand[i].get_suit(trump) == led_card.get_suit(trump)) {
                play = cardAtHand[i];
                cardAtHand[i] = cardAtHand[cardAtHand.size() - 1];
                return play;
            }
        }
        play =  cardAtHand[0];
        int s = 0;
        while (s < cardAtHand.size() - 1) {
            cardAtHand[s] = cardAtHand[1 + s];
            s++;
        }
        cardAtHand.erase(cardAtHand.begin() + cardAtHand.size() - 1);
         //cardAtHand[0] = cardAtHand[cardAtHand.size() - 1];
        return play;
    }
    };


    class Human : public Player {
    private:
        string name2;
        vector<Card> cardHand;

    public:
        Human(string name_in) {
            name2 = name_in;
        }
        
        virtual const string & get_name() const {
            return name2;
        }
        
        virtual void add_card(const Card &c) {
            cardHand.push_back(c);
            std::sort(cardHand.begin(), cardHand.end());
        }
        
        virtual bool make_trump(const Card &upcard, bool is_dealer,
                                int round, string &order_up_suit) const {
            for (int i = 0; i < 5; i++) {
                cout << "Human player " << name2 << "'s hand: " << "[" << i <<
                "]" << " " << cardHand[i] << endl;
            }
            
            cout << "Human player " << name2
            << ", please enter a suit, or \"pass\":" << endl;
            string s;
            cin >> s;
            if (s == "Diamonds" || s == "Spades" || s == "Hearts" || s == "Clubs"){
                return true;
            }
            else if (s == "pass") {
                return false;
            }
            return false;
        }
        
        
        virtual void add_and_discard(const Card &upcard) {
            sort(cardHand.begin(), cardHand.end());
            for (int i = 0; i < 5; i++) {
                cout << "Human player " << name2 << "'s hand: " << "[" << i
                << "]" << " " << cardHand[i] << endl;
            }
            cout << "Discard upcard: [-1] " << endl;
            cout << "Human player " <<
            name2 << ", please select a card to discard: " << endl;
            int a = 0;
            cin >> a;
            
            cardHand.push_back(upcard);
            cardHand.erase(cardHand.begin() + a);
        }
        
        virtual Card lead_card(const std::string &trump) {
            sort(cardHand.begin(), cardHand.end());
            for (int i = 0; i < int(cardHand.size()); i++) {
                cout << "Human player " << name2 << "'s hand: " << "[" << i
                << "]" << " " << cardHand[i] << endl;
            }
            cout << "Discard upcard: [-1] " << endl;
            cout << "Human player " <<
            name2 << ", please select a card to discard: " << endl;
            int a = 0;
            cin >> a;
            
            Card lead = cardHand[a];
            cardHand.erase(cardHand.begin() + a);
            return lead;
            
        }
        
        virtual Card play_card(const Card &led_card, const std::string &trump) {
            sort(cardHand.begin(), cardHand.end());
            for (int i = 0; i < int(cardHand.size()); i++) {
                cout << "Human player " << name2 << "'s hand: " << "[" << i
                << "]" << " " << cardHand[i] << endl;
            }
            cout << "Discard upcard: [-1] " << endl;
            cout << "Human player " <<
            name2 << ", please select a card to discard: " << endl;
            int a = 0;
            cin >> a;
            Card play = cardHand[a];
            cardHand.erase(cardHand.begin() + a);
            return play;
        }
};
    
        Player * Player_factory(const std::string &name, const std::string &strategy) {
            
            // We need to check the value of strategy and return
            // the corresponding player type.
            if (strategy == "Simple") {
                
                // The "new" keyword dynamically allocates an object.
                return new Simple(name);
            }
            
            // Repeat for each other type of Player
            if (strategy == "Human") {
                
                // The "new" keyword dynamically allocates an object.
                return new Human(name);
            }
            // Invalid strategy if we get here
            assert(false);
            return nullptr;
        }


ostream & operator<<(ostream &os, const Player &p) {
    os << p.get_name();
    return os;
}

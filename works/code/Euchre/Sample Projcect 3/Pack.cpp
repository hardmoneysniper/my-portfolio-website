#include "Pack.h"
#include "Card.h"
#include <array>
#include <string>


using namespace std;



Pack::Pack() : next(0) {

    int index = 0;
    //initialize each card
    for (int j = 0; j < NUM_SUITS; j++) {
        for (int i = 7; i < NUM_RANKS; i++) {
            cards[index] = Card(RANK_NAMES_BY_WEIGHT[i], 
                SUIT_NAMES_BY_WEIGHT[j]);
            index++;
        }
    }

}

Pack::Pack(std::istream& pack_input) {

    next = 0;

    int index = 0;
    string rank, suit, trash;
    //read from pack_input
    while(pack_input >> rank >> trash >> suit) {
        cards[index] = Card(rank, suit);
        index++;
    }


}

Card Pack::deal_one() {
    next++;
    return cards[next - 1];

}

void Pack::reset() {
    next = 0;

}

void Pack::shuffle() {
    Card card_temp[7];
    for (int count = 0; count < 3; count++){
        for (int i = 0; i < 7; i++) {
            card_temp[i] = cards[17 + i];
        }

        for (int j = 16; j >= 0; j--) {
            cards[j + 7] = cards[j]; 
        }

        for (int k = 0; k < 7; k++) {
            cards[k] = card_temp[k];
        }

    }
    next = 0;
}

bool Pack::empty() const {
    return (next == PACK_SIZE);
}
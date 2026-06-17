#include "Card.h"
#include "Pack.h"
#include "Player.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <fstream>

using namespace std;


class Game {
public:
	Game() {};
	void read_pack(string filename) {
		ifstream fin;
		fin.open(filename);
		if (!fin.is_open()) {
			cout << "Error opening file: " << filename << endl;
			exit(1);
		}
		Pack Pack_in(fin);// why I cant do without Pack?
		fin.close();
	}
	/*void shuffle_card(string shuffle_in) {
	if (shuffle_in == "shuffle") {
	Pack_in.shuffle();
	}
	else if (shuffle_in == "noshuffle") {
	Pack_in.reset();
	}
	}
	*/
	void shufle_card() {
		Pack_in.shuffle();
	}
	void noshuffle_card() {
		Pack_in.reset();
	}
	void game_set(int winpoint_in, Player* player1, Player*player2,
		Player*player3, Player*player4) {
		winpoint = winpoint_in;
		players.push_back(player1);
		players.push_back(player2);
		players.push_back(player3);
		players.push_back(player4);

	}
	void game_reset() {
		Pack_in.reset();
		ordered_player = dealer_number % 4;
	}
	void deal() {
		int deal = dealer_number % 4;
		for (int i = 0; i < 3; ++i) {
			players[(deal + 1) % 4]->add_card(Pack_in.deal_one());
		}
		for (int i = 0; i < 2; ++i) {
			players[(deal + 2) % 4]->add_card(Pack_in.deal_one());
		}
		for (int i = 0; i < 3; ++i) {
			players[(deal + 3) % 4]->add_card(Pack_in.deal_one());
		}
		for (int i = 0; i < 2; ++i) {
			players[(deal + 4) % 4]->add_card(Pack_in.deal_one());
		}
		for (int i = 0; i < 2; ++i) {
			players[(deal + 5) % 4]->add_card(Pack_in.deal_one());
		}
		for (int i = 0; i < 3; ++i) {
			players[(deal + 6) % 4]->add_card(Pack_in.deal_one());
		}
		for (int i = 0; i < 2; ++i) {
			players[(deal + 7) % 4]->add_card(Pack_in.deal_one());
		}
		for (int i = 0; i < 3; ++i) {
			players[(deal + 8) % 4]->add_card(Pack_in.deal_one());
		}
	}

	void announce_hand() {
		pack_upcard = Pack_in.deal_one();
		cout << "Hand " << dealer_number << endl;
		cout << players[(dealer_number % 4)]->get_name() << " deals" << endl;
		cout << pack_upcard << " turned up" << endl;
	}
	void make_trump() {
		int dealer = dealer_number % 4;
		Card upcard = pack_upcard;
		for (int j = 1; j < 3; ++j) {
			for (int i = 1; i < 5; i++) {
				if (players[(dealer + i) % 4]->make_trump(upcard, i == 4, j, order_up_suit)) {
					cout << players[(dealer + i) % 4]->get_name() << " orders up "
						<< order_up_suit << endl;
					trump = order_up_suit;
					ordered_player = (dealer + i) % 4;
					cout << endl;
					if (j == 1) {
						players[dealer]->add_and_discard(upcard);
					}
					return;
				}
				else {
					cout << players[(dealer + i) % 4]->get_name() << " passes" << endl;
				}
			}
		}
	}
	void play_card() {
		int trick_point_02 = 0;
		int trick_point_13 = 0;
		int dealer = dealer_number % 4;
		int winner = (dealer + 1) % 4;//dealer
		for (int j = 0; j < 5; ++j) {//each trick
			int max = 0;
			led_card = players[winner]->lead_card(trump);
			cout << led_card << " led by " << players[winner]->get_name()
				<< endl;
			Card max_card = led_card;
			for (int i = 1; i <4; ++i) {//each player
				Card played_card = players[(winner + i) % 4]->play_card(led_card, trump);
				cout << played_card << " played by " << players[(winner + i) % 4]->get_name() << endl;
				if (Card_less(max_card, played_card, led_card, trump)) {
					max = i;
					max_card = played_card;
				}
			}
			winner = (winner + max) % 4;
			//winner = max;
			cout << players[winner]->get_name() << " takes the trick" << endl;
			cout << endl;
			if (winner == 0 || winner == 2) {
				trick_point_02 = trick_point_02 + 1;
			}
			else {
				trick_point_13 = trick_point_13 + 1;
			}
		}
		if (ordered_player == 0 || ordered_player == 2) {
			if (trick_point_02 > trick_point_13 && trick_point_02 != 5) {
				++player02_point;
				cout << players[0]->get_name() << " and " << players[2]->get_name()
					<< " win the hand" << endl;
			}
			else if (trick_point_02 > trick_point_13 && trick_point_02 == 5) {
				player02_point = player02_point + 2;
				cout << players[0]->get_name() << " and " << players[2]->get_name()
					<< " win the hand" << endl;
				cout << "march!" << endl;
			}
			else if (trick_point_02<trick_point_13) {
				player13_point = player13_point + 2;
				cout << players[1]->get_name() << " and " << players[3]->get_name()
					<< " win the hand" << endl;
				cout << "euchred!" << endl;
			}
		}
		else {
			if (trick_point_02 < trick_point_13 && trick_point_13 != 5) {
				++player13_point;
				cout << players[1]->get_name() << " and " << players[3]->get_name()
					<< " win the hand" << endl;
				cout << "march!" << endl;
			}
			else if ((trick_point_02 < trick_point_13 && trick_point_13 == 5)) {
				player13_point = player13_point + 2;
				cout << players[1]->get_name() << " and " << players[3]->get_name()
					<< " win!" << endl;
				cout << "march!" << endl;
			}
			else {
				player02_point = player02_point + 2;
				cout << players[0]->get_name() << " and " << players[2]->get_name()
					<< " win the hand" << endl;
				cout << "euchred!" << endl;
			}
		}
		++dealer_number;
	}
	void print_points() {
		cout << players[0]->get_name() << " and " << players[2]->get_name()
			<< " have " << player02_point << " points" << endl;
		cout << players[1]->get_name() << " and " << players[3]->get_name()
			<< " have " << player13_point << " points" << endl;
	}
	void print_winner() {
		if (player02_point >= winpoint) {
			cout << endl;
			cout << players[0]->get_name() << " and " << players[2]->get_name()
				<< " win!" << endl;
		}
		else if (player13_point >= winpoint) {
			cout << endl;
			cout << players[1]->get_name() << " and " << players[3]->get_name()
				<< " win!" << endl;
		}
	}
	int get_point_02() {
		return player02_point;
	}
	int get_point_13() {
		return player13_point;
	}
	int get_winpoint() {
		return winpoint;
	}
	void delete_variables() {
		for (int i = 0; i < int(players.size()); ++i) {
			delete players[i];
		}
	}
private:
	int ordered_player = 0;
	int player02_point = 0;
	int player13_point = 0;
	Pack Pack_in;
	string trump;
	string led;//mei yong dao
	vector<Player*>players;
	Card led_card;
	int winpoint = 0;
	int dealer_number = 0;// a little bit shaky here, not sure whether it will work.
	string order_up_suit;
	Card pack_upcard;
};


int main(int argc, char **argv) {
	for (int i = 0; i < argc; ++i) {
		cout << argv[i] << " ";
	}
	cout << endl;
	string str = argv[2];
	if (argc != 12) {
		cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
			<< "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
			<< "NAME4 TYPE4" << endl;
		return 1;
	}
	if (atoi(argv[3]) > 100 && atoi(argv[3]) < 1) {
		cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
			<< "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
			<< "NAME4 TYPE4" << endl;
		return 1;
	}
	if (str != "shuffle" && str != "noshuffle") {
		cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
			<< "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
			<< "NAME4 TYPE4" << endl;
		return 1;
	}
	if ((string(argv[5]) != "Simple" && string(argv[5]) != "Human") ||
		(string(argv[7]) != "Simple" && string(argv[7]) != "Human") ||
		(string(argv[9]) != "Simple" && string(argv[9]) != "Human") ||
		(string(argv[11]) != "Simple" && string(argv[11]) != "Human")) {
		cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
			<< "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
			<< "NAME4 TYPE4" << endl;
		return 1;
	}
	Game game;
	game.read_pack(string(argv[1]));
	bool shuffle = (string(argv[2]) == "shuffle");
	if (shuffle) {
		game.shufle_card();
	}
	else {
		game.noshuffle_card();
	}
	Player*player1 = Player_factory(string(argv[4]), string(argv[5]));
	Player*player2 = Player_factory(string(argv[6]), string(argv[7]));
	Player*player3 = Player_factory(string(argv[8]), string(argv[9]));
	Player*player4 = Player_factory(string(argv[10]), string(argv[11]));
	game.game_set(atoi(argv[3]), player1, player2, player3, player4);
	game.deal();
	game.announce_hand();
	game.make_trump();
	game.play_card();
	game.print_points();
	while (game.get_point_02()<game.get_winpoint() && game.get_point_13()<game.get_winpoint()) {
		cout << endl;
		if (shuffle) {
			game.shufle_card();
		}
		else {
			game.noshuffle_card();
		}
		game.game_reset();
		game.deal();
		game.announce_hand();
		game.make_trump();
		game.play_card();
		game.print_points();
	}
	game.print_winner();
	game.delete_variables();
	return 0;
}

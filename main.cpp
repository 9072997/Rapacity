/*
 * main.cpp
 *
 *  Created on: Mar 27, 2015
 *
 *  Copyright (C) 2015  Jon Penn
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <string>
#include <tuple>
#include <vector>

#include "AI.h"
#include "Board.h"
#include "customIO.h"
#include "Ship.h"
#include "Tile.h"
#include "ui.h"

using namespace ui;
using namespace std;
using namespace customIO;

namespace {
	typedef unsigned int uint;
}
int main() {
	srand(time(NULL));
	clear();

	tuple<uint, uint> boardSize;
	switch(menu("Game Size", {"Small (50 tiles)", "Medium (200 tiles)", "Large (450 tiles)"})) {
		case 0:
			boardSize = make_tuple(5, 10);
			break;
		case 1:
			boardSize = make_tuple(10, 20);
			break;
		case 2:
			boardSize = make_tuple(15, 30);
			break;
	}
	Board board(boardSize);

	/////////////////////
	// Tile Population //
	/////////////////////
	vector<Tile*> planits;
	planits.push_back(new Tile("Mercury", 'H', false));
	planits.push_back(new Tile("Venus", 'V', false));
	planits.push_back(new Tile("Earth", 'E', false));
	planits.push_back(new Tile("Mars", 'M', false));
	planits.push_back(new Tile("Jupiter", 'J', false));
	planits.push_back(new Tile("Saturn", 'S', false));
	planits.push_back(new Tile("Uranius", 'U', true));
	planits.push_back(new Tile("Neptune", 'N', true));
	planits.push_back(new Tile("Pluto", 'P', true));
	for(uint numTile = 0; numTile < board.get1DLength(); numTile++) {
		uint range = planits.size() * numTile / board.get1DLength();
		if(range < 1) range = 1; // so we start with some variaty
		Tile* tile = planits.at(rand() % (range+1));
		board.putTile(tile, numTile);
	}

	/////////////
	// Players //
	/////////////
	vector<Player*> players;
	{
		vector<char> avitars = {'@', '#', '$', '%', '&', '~'};
		vector<Player*> tmpPlayers;
		uint numHumanPlayers, numAIPlayers;
		clear();
		for(;;) { // we break out
			numHumanPlayers = getUserUInt("How Many Human Players");
			numAIPlayers = 0;
			if(numHumanPlayers + numAIPlayers > 6) {
				clear();
				cout << "There may be no mode than 6 total players." << endl;
				continue;
			}

			if(numHumanPlayers < 6) {
				numAIPlayers = getUserUInt("How Many AI Players");
			} // else we are out of avitars
			if(numHumanPlayers + numAIPlayers > 6) {
				clear();
				cout << "There may be no mode than 6 total players." << endl;
				continue;
			} else if(numHumanPlayers + numAIPlayers <= 0) {
				clear();
				cout << "There must be at least 1 player." << endl;
				continue;
			} else {
				break;
			}
		}

		for(uint numHumanPlayer = 0; numHumanPlayer < numHumanPlayers; numHumanPlayer++) { // get new players
			clear();
			tmpPlayers.push_back(new Player(&avitars, 4000));
		}
		if(numAIPlayers > 0) {
			clear();
			for(uint numAIPlayer = 0; numAIPlayer < numAIPlayers; numAIPlayer++) { // get new players
				tmpPlayers.push_back(new AI(&avitars, 4000));
			}
			waitForUser();
		}


		while(tmpPlayers.size()) { // while we have players
			uint index = rand() % tmpPlayers.size(); // pick a random player
			players.push_back(tmpPlayers.at(index)); // push him on the end
			tmpPlayers.erase(tmpPlayers.begin() + index); // remove him from the roster
		} // this shuffles the players
	}


	// print player order
	clear();
	cout << "> " << players.at(0)->getName() << " (" << players.at(0)->getChar() << ')' << endl;
	for(vector<Player*>::iterator player = players.begin() + 1; player < players.end(); player++) {
		cout << "  " << (*player)->getName() << " (" << (*player)->getChar() << ')' << endl;
	}
	waitForUser();

	{ // turn loop
		bool firstTurn = true;
		vector<Player*>::iterator playerIterator = players.begin();
		bool gameIsOver = false;
		do {
			// world
			board.shiftMarket(); // shift global economy
			for(vector<Tile*>::iterator planit = planits.begin(); planit < planits.end(); planit++) { // shift planit economy
				(*planit)->shiftMarket();
				(*planit)->manageInventory();
			}

			// turn
			Player* player = (*playerIterator);
			if(players.size() > 1 && !dynamic_cast<AI*>(player)) { // banner when switching players and not AI
				clear();
				cout << " /$$$$$$$                                          /$$   /$$              " << endl;
				cout << "| $$__  $$                                        |__/  | $$              " << endl;
				cout << "| $$  \\ $$  /$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$$ /$$ /$$$$$$   /$$   /$$" << endl;
				cout << "| $$$$$$$/ |____  $$ /$$__  $$ |____  $$ /$$_____/| $$|_  $$_/  | $$  | $$" << endl;
				cout << "| $$__  $$  /$$$$$$$| $$  \\ $$  /$$$$$$$| $$      | $$  | $$    | $$  | $$" << endl;
				cout << "| $$  \\ $$ /$$__  $$| $$  | $$ /$$__  $$| $$      | $$  | $$ /$$| $$  | $$" << endl;
				cout << "| $$  | $$|  $$$$$$$| $$$$$$$/|  $$$$$$$|  $$$$$$$| $$  |  $$$$/|  $$$$$$$" << endl;
				cout << "|__/  |__/ \\_______/| $$____/  \\_______/ \\_______/|__/   \\___/   \\____  $$" << endl;
				cout << "                    | $$                                         /$$  | $$" << endl;
				cout << "                    | $$                                        |  $$$$$$/" << endl;
				cout << "                    |__/                                         \\______/ " << endl;
				cout << endl;
				cout << "You're up, " << player->getName() << "." << endl;
				waitForUser();
			}
			if(firstTurn) {
				// back story on player 1
				clear();
				cout << "         ______________________" << endl;
				cout << "        /                      \\" << endl;
				cout << "        | So how should I go   |" << endl;
				cout << "        | about doing a thing  |" << endl;
				cout << "        | like that?           |" << endl;
				cout << "        \\___________________  _/" << endl;
				cout << "                            \\|" << endl;
				cout << "   ______________________" << endl;
				cout << "  /                      \\" << endl;
				cout << "  | What? How should I   |" << endl;
				cout << "  | know? I just tend    |" << endl;
				cout << "  | bar, but I guess if  |" << endl;
				cout << "  | you want to be a     |" << endl;
				cout << "  | merchant you should  |" << endl;
				cout << "  | probably get a ship  |" << endl;
				cout << "  | and then buy some... |" << endl;
				cout << "  | stuff? I don't know. |" << endl;
				cout << "  \\_  ___________________/" << endl;
				cout << "    |/" << endl;
				cout << "   O" << endl;
				cout << "  /|\\   cU" << endl;
				cout << "=============" << endl;
				cout << "            H" << endl;
				cout << "            H" << endl;
				cout << endl;
				waitForUser();

				clear();
				cout << "   ______________________" << endl;
				cout << "  /                      \\" << endl;
				cout << "  | And you have to pay  |" << endl;
				cout << "  | for that!            |" << endl;
				cout << "  \\_  ___________________/" << endl;
				cout << "    |/" << endl;
				cout << endl;
				waitForUser();

				players.at(0)->transaction(-5); // pay for beer, the cost of being first
				firstTurn = false;
			}
			player->takeTurn(&board, players);

			if(player->getPosition() >= board.get1DLength()) {
				clear();
				cout << "You made it (to forced retirement)." << endl;
				cout << "Your kids will now take and sell all your stuff." << endl;
				uint price = 0;
				price += get<0>(board.getPrices());
				price += get<1>(board.getPrices());
				price += get<2>(board.getPrices());
				price += get<3>(board.getPrices());
				price += get<4>(board.getPrices());
				price += 50 * player->getShip()->getSize();
				player->transaction(price);
				cout << "It went for " << price << "cr." << endl;
				waitForUser();
				gameIsOver = true;
			}

			if(playerIterator < players.end() - 1) {
				playerIterator++;
			} else {
				playerIterator = players.begin();
			}


		} while(!gameIsOver);

		// victors
		clear();
		sort(players.begin(), players.end(), [](Player* a, Player* b) -> bool {
			return a->getMoney() > b->getMoney();
		});
		for(vector<Player*>::iterator player = players.begin(); player < players.end(); player++) {
			uint rank = distance(players.begin(), player) + 1;
			cout << rank << ": " << (*player)->getName() << " (" << (*player)->getChar() << ") " << (*player)->getMoney() << "cr" << endl;
		}

		cout << endl;
		cout << "        |" << endl;
		cout << "       / \\" << endl;
		cout << "      / _ \\   Thanks to l42 of" << endl;
		cout << "     |.o '.|  http://ascii.gallery/art/J2Jz7aDnHHZ9_2Nr50l8VgeF" << endl;
		cout << "     |'._.'|  for the rocket." << endl;
		cout << "     |     |" << endl;
		cout << "   ,'|  |  |`." << endl;
		cout << "  /  |  |  |  \\" << endl;
		cout << "  |,-'--|--'-.|" << endl;
		cout << endl;
		cout << "btw this game is GPLv3." << endl;

		waitForUser();
	}


	return 0;
}

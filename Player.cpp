/*
 * Player.cpp
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

#include "Player.h"

#include <algorithm> /* min */
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>
#include <tuple>
#include <vector>

#include "Board.h"
#include "customIO.h"
#include "Ship.h"
#include "Tile.h"
#include "ui.h"


using namespace std;
using namespace ui;
using namespace customIO;

Player::Player(vector<char>* avalibleAvitars, uint startingMoney) {
	name = getUserString("Name");
	vector<string> menuOptions;
	for(vector<char>::iterator avalibleAvitar = avalibleAvitars->begin(); avalibleAvitar < avalibleAvitars->end(); avalibleAvitar++) {
		menuOptions.push_back(string(1,*avalibleAvitar));
	}
	avitar = avalibleAvitars->at(menu(menuOptions));
	for(vector<char>::iterator avalibleAvitar = avalibleAvitars->begin(); avalibleAvitar < avalibleAvitars->end(); avalibleAvitar++) {
		if(avitar == *avalibleAvitar) {
			avalibleAvitars->erase(avalibleAvitar);
			break; // for speed
		}
	}
	money = startingMoney;
	position = 0;
	ship = NULL;
}
unsigned int Player::getMoney() {
	return money;
}
string Player::getName() {
	return name;
}
unsigned int Player::getPosition() {
	return position;
}
char Player::getChar() {
	return avitar;
}
Ship* Player::getShip() {
	return ship;
}
bool Player::transaction(int transAmt) {
	if(int(money) + transAmt >= 0) {
		money += transAmt;
		return true;
	} else {
		return false;
	}
}
void Player::takeTurn(Board* board, vector<Player*> players) {
	Tile* tile = board->getTile(getPosition());
	bool turnDone = false;
	while(!turnDone) {
		if(tile->getWanted() == this) { // we are on the run
			uint visibility;
			if(getShip()) {
				visibility = getShip()->getSize();
			} else {
				visibility = 1;
			}
			uint threshold = rand() % 65;
			if(visibility >= threshold) { // you got caught
				clear();
				if(menu("The police are coming", {"Play Nice", "Fight"}) == 0) { // play nice
					clear();
					uint fine = 2000;
					if(getShip()) {
						uint munitions = get<3>(getShip()->getResources());
						if(munitions > 0) {
							fine += 500 * munitions;
							getShip()->changeResource(3, -munitions);
							cout << "You were illegally transporting munitions." << endl;
							cout << "They have bean confiscated." << endl;
						}
					}
					if(!transaction(-fine)) {
						// if player dosen't have enough money, just take all of it
						money = 0;
					}
					cout << "You were fined " << fine << "cr." << endl;
					tile->setWanted(NULL);
				} else { // fight
					clear();
					uint damage = rand()%10;
					cout << "You took " << damage << " damage." << endl;
					if(!getShip()) { // no ship
						cout << "That was dumb, you don't even have a ship." << endl;
						cout << "You were sent to jail." << endl;
						position = 0;
						tile->setWanted(NULL);
					} else if(!getShip()->takeDamage(damage)) { // if ship is destroied
						delete getShip();
						ship = NULL;
						cout << "Your ship was destroyed." << endl;
						cout << "You were sent to jail." << endl;
						position = 0;
						tile->setWanted(NULL);
					}
				}
				waitForUser();
			}
		}

		clear();
		printUserStatus(*this, *board);
		cout << endl;

		uint menuSelection;
		if(getShip()) {
			menuSelection = menu("What would you like to do", {
				"Buy Stuff",
				"Sell Stuff",
				"View Map",
				"Pillage",
				"Sell a Ship",
				"Rest",
				"Move On"
			});
		} else {
			menuSelection = menu("What would you like to do", {
				"View Map",
				"Pillage",
				"Buy a Ship",
				"Rest"
			}) + 2; // I removed 2 options from the begining
		}

		switch(menuSelection) {
			case 0: {// buy
				clear();
				printUserStatus(*this, *board);
				cout << endl;

				uint resource = menu("Resource", {"Uranium", "Gold", "Ore", "Munitions", "Fuel"});
				uint amount = getUserUInt("Amount");
				uint price;
				switch(resource) {
					case 0: price = get<0>(tile->getMarkupPrices(board->getPrices())); break;
					case 1: price = get<1>(tile->getMarkupPrices(board->getPrices())); break;
					case 2: price = get<2>(tile->getMarkupPrices(board->getPrices())); break;
					case 3: price = get<3>(tile->getMarkupPrices(board->getPrices())); break;
					case 4: price = get<4>(tile->getMarkupPrices(board->getPrices())); break;
				}
				if(price <= getMoney()) {
					if(getShip()->changeResource(resource, amount)) {
						transaction(-price);
						clear();
						printUserStatus(*this, *board);
						cout << endl;
						cout << "Nice doing business." << endl;
					} else {
						cout << "You can't fit that in your ship." << endl;
					}
				} else {
					cout << "Come back when you have real money. Hemph." << endl;
				}
				waitForUser();
			} break;
			case 1: {// sell
				clear();
				printUserStatus(*this, *board);
				cout << endl;

				uint resource = menu("Resource", {"Uranium", "Gold", "Ore", "Munitions", "Fuel"});
				uint amount = getUserUInt("Amount");
				uint price;
				switch(resource) {
					case 0: price = get<0>(tile->getPrices(board->getPrices())); break;
					case 1: price = get<1>(tile->getPrices(board->getPrices())); break;
					case 2: price = get<2>(tile->getPrices(board->getPrices())); break;
					case 3: price = get<3>(tile->getPrices(board->getPrices())); break;
					case 4: price = get<4>(tile->getPrices(board->getPrices())); break;
				}
				if(getShip()->changeResource(resource, -amount)) {
					transaction(price);
					clear();
					printUserStatus(*this, *board);
					cout << endl;
					cout << "Nice doing business." << endl;
				} else {
					cout << "I won't be paying for what you don't have." << endl;
				}
				waitForUser();
			} break;
			case 2: {
				clear();
				board->print(players);
				cout << endl;
				waitForUser();
			} break;
			case 3: { // pillage
				uint spoils = 200 + rand()%801;
				transaction(spoils);
				cout << "You got " << spoils << "cr." << endl;
				tile->setWanted(this);
				cout << "You are also now a fugitive." << endl;
				waitForUser();
			} break;
			case 4: { // shipyard
				clear();
				if(getShip()) { // sell
					uint value = 50 * getShip()->getSize();
					cout << "I'll give you " << value << "cr for the scrap." << endl;
					if(getUserBool("Deal?")) {
						transaction(value);
						delete getShip();
						ship = NULL;
					}
				} else { // buy
					tile->printShips();
					cout << endl;
					uint numShip = getUserMenu("What'll it be", tile->getNumShips()) - 1;
					if(transaction(-tile->costOfShip(numShip))) {
						ship = tile->buyShip(numShip);
						cout << "Congratulations. You are the new owner of this fine vehicle." << endl;
					} else {
						cout << "Come back when you have real money. Hemph." << endl;
					}
				}
				waitForUser();
			} break;
			case 5: { // rest
				if(getShip()) {
					getShip()->rest();
				}
				turnDone = true;
			} break;
			case 6: {// move on
				clear();
				if(ship->getFuel() > 0) {
					board->print(players);
					cout << endl;
					uint gear = menu("Speed", {"Impulse Power Only", "Warp", "Transwarp"}) + 1;
					uint roll = getShip()->roll(gear);
					uint dist = min(roll, ship->getFuel());
					getShip()->changeResource(4, -dist);
					position += dist;

					clear();
					board->print(players);
					cout << endl;
					cout << "You advance " << dist << " spaces." << endl;
					turnDone = true;
				} else {
					printUserStatus(*this, *board);
					cout << endl;
					cout << "Sorry, moving takes fuel." << endl;
				}

				waitForUser();
			} break;
		}
	};
}

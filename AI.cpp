/*
 * AI.cpp
 *
 *  Created on: Apr 4, 2015
 *      Author: jon
 */

#include "AI.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>
#include <tuple>
#include <vector>

#include "Board.h"
#include "customIO.h"
#include "Player.h"
#include "Ship.h"
#include "Tile.h"
#include "ui.h"

using namespace std;
using namespace ui;
using namespace customIO;

std::vector<std::string> AI::names = {
	"The Machine",
	"HAL-9000",
	"GLaDOS",
	"WOPR",
	"Mr. Norman",
	"Deep Blue"
};

AI::AI(vector<char>* avalibleAvitars, uint startingMoney) {
	uint numName = rand()%names.size();
	name = names.at(numName);
	names.erase(names.begin() + numName);

	avitar = avalibleAvitars->at(0); // just take the first avalible avitar
	avalibleAvitars->erase(avalibleAvitars->begin()); // basically pop_front

	money = startingMoney;
	position = 0;
	ship = NULL;

	cout << name << " (" << getChar() << ") is online." << endl;
}

AI::~AI() {}

void AI::randomCheck(Tile* tile) {
	if(tile->getWanted() == this) { // we are on the run
		uint visibility;
		if(getShip()) {
			visibility = getShip()->getSize();
		} else {
			visibility = 1;
		}
		uint threshold = rand() % 65;
		if(visibility >= threshold) { // you got caught
			cout << "The police are coming." << endl;
			if(rand()%2 == 0) { // play nice
				cout << "We are playing nice." << endl;
				uint fine = 2000;
				if(getShip()) {
					uint munitions = get<3>(getShip()->getResources());
					if(munitions > 0) {
						fine += 500 * munitions;
						getShip()->changeResource(3, -munitions);
						cout << "We were illegally transporting munitions." << endl;
						cout << "They have bean confiscated." << endl;
					}
				}
				if(!transaction(-fine)) {
					// if player dosen't have enough money, just take all of it
					money = 0;
				}
				cout << "We were fined " << fine << "cr." << endl;
				tile->setWanted(NULL);
			} else { // fight
				cout << "We are fighting." << endl;
				uint damage = rand()%10;
				cout << "We took " << damage << " damage." << endl;
				if(!getShip()) { // no ship
					cout << "That was dumb, we don't even have a ship." << endl;
					cout << "I was sent to jail." << endl;
					position = 0;
					tile->setWanted(NULL);
				} else if(!getShip()->takeDamage(damage)) { // if ship is destroied
					delete getShip();
					ship = NULL;
					cout << "Our ship was destroyed." << endl;
					cout << "We were sent to jail." << endl;
					position = 0;
					tile->setWanted(NULL);
				}
			}
		}
	}
}

void AI::takeTurn(Board* board, vector<Player*> players) {
	clear();
	Tile* tile = board->getTile(getPosition());

	while(!getShip()) {
		randomCheck(tile);
		cout << "Let's buy a ship." << endl;
		uint numShip = rand()%(tile->getNumShips());
		if(transaction(-tile->costOfShip(numShip))) {
			ship = tile->buyShip(numShip);
		} else {
			cout << "We don't have enough money." << endl;
			uint spoils = 200 + rand()%801; // pillage
			transaction(spoils);
			cout << "We got " << spoils << "cr from pillageing." << endl;
			tile->setWanted(this);
		}

	}

	bool turnDone = false;
	while(!turnDone) {
		randomCheck(tile);

		switch(rand()%6) { // sometimes I use double CASE statements to alter probability
			case 0: case 1: {// buy
				uint resource; // order of prefrence
				if(getShip()->getFuel() < getShip()->getRange() - 10) resource = 4;
				else if(get<0>(tile->getMarkupPrices(board->getPrices())) <= get<0>(board->getPrices())) resource = 0;
				else if(get<1>(tile->getMarkupPrices(board->getPrices())) <= get<1>(board->getPrices())) resource = 1;
				else if(get<2>(tile->getMarkupPrices(board->getPrices())) <= get<2>(board->getPrices())) resource = 2;
				else if(get<3>(tile->getMarkupPrices(board->getPrices())) <= get<3>(board->getPrices())) resource = 3;
				else break; // not a good time to buy

				uint price, amount;
				switch(resource) {
					case 0:
						amount = 1 + rand()%4;
						price = get<0>(tile->getMarkupPrices(board->getPrices()));
						cout << "Let's buy " << amount << " Uranium." << endl;
						break;
					case 1:
						amount = 1 + rand()%4;
						price = get<1>(tile->getMarkupPrices(board->getPrices()));
						cout << "Let's buy " << amount << " Gold." << endl;
						break;
					case 2:
						amount = 1 + rand()%4;
						price = get<2>(tile->getMarkupPrices(board->getPrices()));
						cout << "Let's buy " << amount << " Ore." << endl;
						break;
					case 3:
						amount = 1 + rand()%4;
						price = get<3>(tile->getMarkupPrices(board->getPrices()));
						cout << "Let's buy " << amount << " Munitions." << endl;
						break;
					case 4:
						amount = rand() % (getShip()->getRange() - getShip()->getFuel());
						price = get<4>(tile->getMarkupPrices(board->getPrices()));
						cout << "Let's buy " << amount << " Fuel." << endl;
						break;
				}
				price *= amount;
				if(price <= getMoney()) {
					if(getShip()->changeResource(resource, amount)) {
						transaction(-price);
					} else {
						cout << "We can't fit it in the ship." << endl;
					}
				} else {
					cout << "We can't afford it." << endl;
				}
			} break;
			case 2: case 3: {// sell
				uint resource = rand()%4; // order of prefrence
				if(get<3>(tile->getPrices(board->getPrices())) >= get<3>(board->getPrices()) && get<3>(getShip()->getResources()) > 0) resource = 3;
				else if(get<2>(tile->getPrices(board->getPrices())) >= get<2>(board->getPrices()) && get<2>(getShip()->getResources()) > 0) resource = 2;
				else if(get<1>(tile->getPrices(board->getPrices())) >= get<1>(board->getPrices()) && get<1>(getShip()->getResources()) > 0) resource = 1;
				else if(get<0>(tile->getPrices(board->getPrices())) >= get<0>(board->getPrices()) && get<0>(getShip()->getResources()) > 0) resource = 0;
				else break; // not a good time to sell

				uint price, amount;
				switch(resource) {
					case 0:
						price = get<0>(tile->getPrices(board->getPrices()));
						amount = get<0>(getShip()->getResources());
						cout << "Let's sell " << amount << " Uranium." << endl;
						break;
					case 1:
						price = get<1>(tile->getPrices(board->getPrices()));
						amount = get<1>(getShip()->getResources());
						cout << "Let's sell " << amount << " Gold." << endl;

						break;
					case 2:
						price = get<2>(tile->getPrices(board->getPrices()));
						amount = get<2>(getShip()->getResources());
						cout << "Let's sell " << amount << " Ore." << endl;

						break;
					case 3:
						price = get<3>(tile->getPrices(board->getPrices()));
						amount = get<3>(getShip()->getResources());
						cout << "Let's sell " << amount << " Munitions." << endl;

						break;
				}
				price *= amount;
				if(getShip()->changeResource(resource, -amount)) {
					transaction(price);
				} else {
					cout << "Ops, we don't really have that." << endl;
				}
			} break;
			case 4: { // rest
				if(getShip()->getHealth() < getShip()->getDefense() || ship->getFuel() <= 0) { // only rest of we are hurt or cant move
					cout << "Let's rest." << endl;
					if(getShip()) {
						getShip()->rest();
					}
					turnDone = true;
				}
			} break;
			case 5: {// move on
				cout << "Let's move on." << endl;
				if(ship->getFuel() > 0) {
					uint gear = rand()%3 + 1;
					uint roll = getShip()->roll(gear);
					uint dist = min(roll, ship->getFuel());
					getShip()->changeResource(4, -dist);
					position += dist;

					cout << "We advance " << dist << " spaces." << endl;
					turnDone = true;
				} else {
					cout << "We are out of fuel." << endl;
				}
			} break;
		}
	}

	if(getPosition() < board->get1DLength()) {
		cout << endl;
		printUserStatus(*this, *board);
	} // this is nesisary b.c. this is a post movement status
	cout << endl;
	waitForUser();
}

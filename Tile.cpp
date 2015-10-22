/*
 * Tile.cpp
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

#include "Tile.h"

#include <algorithm> /* max */
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>
#include <tuple>
#include <vector>

#include "Player.h"
#include "Ship.h"


using namespace std;

Tile::Tile(string newName, char newAvitar, bool isPrimitive) {
	uint numShips;
	if(isPrimitive) {
		get<0>(marketMul) = float(300 + rand()%201) / 1000; // uranium
		get<1>(marketMul) = float(1400 + rand()%201) / 1000; // gold
		get<2>(marketMul) = float(500 + rand()%201) / 1000; // ore
		get<3>(marketMul) = float(1000 + rand()%201) / 1000; // munitions
		get<4>(marketMul) = float(1900 + rand()%201) / 1000; // fuel

		get<0>(markup) = float(9900 + rand()%101) / 1000; // uranium
		get<1>(markup) = float(1400 + rand()%101) / 1000; // gold
		get<2>(markup) = float(1000 + rand()%101) / 1000; // ore
		get<3>(markup) = float(1000 + rand()%101) / 1000; // munitions
		get<4>(markup) = float(1700 + rand()%101) / 1000; // fuel

		numShips = 2;
	} else {
		get<0>(marketMul) = float(900 + rand()%201) / 1000; // uranium
		get<1>(marketMul) = float(400 + rand()%201) / 1000; // gold
		get<2>(marketMul) = float(1400 + rand()%201) / 1000; // ore
		get<3>(marketMul) = float(900 + rand()%201) / 1000; // munitions
		get<4>(marketMul) = float(900 + rand()%201) / 1000; // fuel

		get<0>(markup) = float(1000 + rand()%101) / 1000; // uranium
		get<1>(markup) = float(1400 + rand()%101) / 1000; // gold
		get<2>(markup) = float(1900 + rand()%101) / 1000; // ore
		get<3>(markup) = float(2900 + rand()%101) / 1000; // munitions
		get<4>(markup) = float(1000 + rand()%101) / 1000; // fuel

		numShips = 4;
	}
	name = newName;
	avitar = newAvitar;
	wanted = NULL;

	for(uint numShip = 0; numShip < numShips; numShip++) {
		ships.push_back(new Ship());
	}
}

float Tile::positiveSum(float origional, float addend) {
	if(origional + addend > 0) return origional + addend;
	else return origional;
}

void Tile::shiftMarket() {
	get<0>(marketMul) = positiveSum(get<0>(marketMul), float(10 - rand()%21)/1000); // uranium
	get<1>(marketMul) = positiveSum(get<1>(marketMul), float(10 - rand()%21)/1000); // gold
	get<2>(marketMul) = positiveSum(get<2>(marketMul), float(10 - rand()%21)/1000); // ore
	get<3>(marketMul) = positiveSum(get<3>(marketMul), float(10 - rand()%21)/1000); // munitions
	get<4>(marketMul) = positiveSum(get<4>(marketMul), float(10 - rand()%21)/1000); // fuel

	get<0>(markup) = max(get<0>(markup) + float(10 - rand()%21)/1000, float(1)); // uranium
	get<1>(markup) = max(get<1>(markup) + float(10 - rand()%21)/1000, float(1)); // gold
	get<2>(markup) = max(get<2>(markup) + float(10 - rand()%21)/1000, float(1)); // ore
	get<3>(markup) = max(get<3>(markup) + float(10 - rand()%21)/1000, float(1)); // munitions
	get<4>(markup) = max(get<4>(markup) + float(10 - rand()%21)/1000, float(1)); // fuel
}

string Tile::getName() {
	return name;
}

tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int> Tile::getPrices(prices_t prices) {
	return make_tuple(
			(uint)ceil(get<0>(prices) * get<0>(marketMul)),
			(uint)ceil(get<1>(prices) * get<1>(marketMul)),
			(uint)ceil(get<2>(prices) * get<2>(marketMul)),
			(uint)ceil(get<3>(prices) * get<3>(marketMul)),
			(uint)ceil(get<4>(prices) * get<4>(marketMul))
	);
}
tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int> Tile::getMarkupPrices(prices_t prices) {
	return make_tuple(
			(uint)ceil(get<0>(prices) * get<0>(marketMul) * get<0>(markup)),
			(uint)ceil(get<1>(prices) * get<1>(marketMul) * get<1>(markup)),
			(uint)ceil(get<2>(prices) * get<2>(marketMul) * get<2>(markup)),
			(uint)ceil(get<3>(prices) * get<3>(marketMul) * get<3>(markup)),
			(uint)ceil(get<4>(prices) * get<4>(marketMul) * get<4>(markup))
	);
}
Player* Tile::getWanted() {
	return wanted;
}
void Tile::setWanted(Player* player) {
	wanted = player;
}
unsigned int Tile::costOfShip(uint numShip) {
	Ship* ship = ships.at(numShip);
	uint size = ship->getSize();
	uint defense = ship->getDefense();
	uint speed = ship->getSpeed();
	uint range = ship->getRange();
	uint cost = (size + defense + speed + range) * 50;
	return cost;
}
Ship* Tile::buyShip(uint numShip) {
	Ship* ship = ships.at(numShip);
	ships.erase(ships.begin() + numShip);
	return ship;
}
unsigned int Tile::getNumShips() {
	return ships.size();
}
void Tile::manageInventory() {
	while(ships.size() < 2) {
		ships.push_back(new Ship());
	}
}
void Tile::printShips() {
	if(ships.size() > 1) {
		cout << "   ______________________" << endl;
		cout << "  /                      \\" << endl;
		cout << "  | You have the pick of |" << endl;
		cout << "  | the lot, if you can  |" << endl;
		cout << "  | pay that is.         |" << endl;
		cout << "  \\_  ___________________/" << endl;
		cout << "    |/" << endl;
		cout << "   O" << endl;
		cout << "  /|\\" << endl;
		cout << "   |" << endl;
		cout << "  / \\" << endl;
		cout << endl;
	} else if(ships.size() > 0) {
		cout << "   ______________________" << endl;
		cout << "  /                      \\" << endl;
		cout << "  | We should be getting |" << endl;
		cout << "  | more in next turn.   |" << endl;
		cout << "  \\_  ___________________/" << endl;
		cout << "    |/" << endl;
		cout << "   O" << endl;
		cout << "  /|\\" << endl;
		cout << "   |" << endl;
		cout << "  / \\" << endl;
		cout << endl;
	} else {
		cout << "   ______________________" << endl;
		cout << "  /                      \\" << endl;
		cout << "  | Sorry, but we're     |" << endl;
		cout << "  | all sold out. We     |" << endl;
		cout << "  | should be getting    |" << endl;
		cout << "  | more shortly though. |" << endl;
		cout << "  \\_  ___________________/" << endl;
		cout << "    |/" << endl;
		cout << "   O" << endl;
		cout << "  /|\\" << endl;
		cout << "   |" << endl;
		cout << "  / \\" << endl;
		cout << endl;
	}
	for(vector<Ship*>::iterator ship = ships.begin(); ship < ships.end(); ship++) {
		uint numShip = distance(ships.begin(), ship) + 1;
		uint size = (*ship)->getSize();
		uint defense = (*ship)->getDefense();
		uint speed = (*ship)->getSpeed();
		uint range = (*ship)->getRange();
		uint cost = (size + defense + speed + range) * 50;
		cout << endl;
		cout << "        |     #" << numShip << endl;
		cout << "       / \\    Cost    " << cost << endl;
		cout << "      / _ \\   Defense " << defense << endl;
		cout << "     |.o '.|  Range   " << range << endl;
		cout << "     |'._.'|  Size    " << size << endl;
		cout << "     |     |  Speed   " << speed << endl;
		cout << "   ,'|  |  |`." << endl;
		cout << "  /  |  |  |  \\" << endl;
		cout << "  |,-'--|--'-.|" << endl;
	}
}
char Tile::getChar() {
	return avitar;
}

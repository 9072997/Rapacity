/*
 * Ship.cpp
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

#include "Ship.h"

#include <cstdlib>
#include <iostream>
#include <tuple>

using namespace std;
Ship::Ship() {
	size = 10 + rand()%21; // 10-20
	defense = size/3 + 2 + rand()%4;

	if(size < 12 + rand()%4) { // 12-15
		dice = make_tuple(7, 5); // note major, minor
	} else if(size < 20 + rand()%5) { //20-24
		dice = make_tuple(6, 4);
	} else {
		dice = make_tuple(4, 2);
	}

	range = getSpeed() * (20 + rand()%11) / 10;

	fuel = range; // perks of the dealership
	health = defense; // it's new
	resources = make_tuple(0, 0, 0, 0); // not a trade in
}
unsigned int Ship::getDefense() {
	return defense;
}

unsigned int Ship::getFuel() {
	return fuel;
}

unsigned int Ship::getHealth() {
	return health;
}

unsigned int Ship::getSpeed() {
	return get<0>(dice) + get<1>(dice);
}
unsigned int Ship::getRange() {
	return range;
}
unsigned int Ship::getSize() {
	return size;
}

tuple<unsigned int, unsigned int, unsigned int, unsigned int> Ship::getResources() {
	return resources;
}
unsigned int Ship::amtStuff() {
	return get<0>(resources) + get<1>(resources) + get<2>(resources) + get<3>(resources);
}
bool Ship::changeResource(uint resource, int by) {
	switch(resource) {
		case 0:
			if(amtStuff() + by <= getSize() && get<0>(resources) + by >= 0) {
				get<0>(resources) += by;
				return true;
			} else {
				return false;
			}
		case 1:
			if(amtStuff() + by <= getSize() && get<1>(resources) + by >= 0) {
				get<1>(resources) += by;
				return true;
			} else {
				return false;
			}
		case 2:
			if(amtStuff() + by <= getSize() && get<2>(resources) + by >= 0) {
				get<2>(resources) += by;
				return true;
			} else {
				return false;
			}
		case 3:
			if(amtStuff() + by <= getSize() && get<3>(resources) + by >= 0) {
				get<3>(resources) += by;
				return true;
			} else {
				return false;
			}
		case 4:
			if(fuel + by <= getRange() && fuel + by >= 0) {
				fuel += by;
				return true;
			} else {
				return false;
			}
		default:
			return false;
	}
}
bool Ship::takeDamage(uint damage) {
	if(health > damage) {
		health -= damage;
		return true;
	} else {
		return false;
	}
}
void Ship::rest() {
	if(rand()%3 == 0 && health < defense) {
		health++;
	}
}

unsigned int Ship::roll(uint throtle) {
	switch(throtle) {
		case 1:
			return rand()%get<1>(dice) + 1;
			break;
		case 2:
			return rand()%get<0>(dice) + 1;
			break;
		case 3:
			return rand()%get<0>(dice) + rand()%get<1>(dice) + 2;
			break;
		default:
			return 0;
	}
}

void Ship::printCargo() {
	for(uint numBay = 0; numBay < getSize(); numBay++) {
		cout << " _";
	}
	cout << endl << '/';
	for(uint numBay = 0; numBay < getSize() - 1; numBay++) {
		cout << " V";
	}
	cout << " \\" << endl;


	uint numBay = 0;
	for(uint numResource = 0; numResource < get<0>(getResources()); numResource++, numBay++) { // uranium
		cout << "|U";
	}
	for(uint numResource = 0; numResource < get<1>(getResources()); numResource++, numBay++) { // gold
		cout << "|G";
	}
	for(uint numResource = 0; numResource < get<2>(getResources()); numResource++, numBay++) { // ore
		cout << "|O";
	}
	for(uint numResource = 0; numResource < get<3>(getResources()); numResource++, numBay++) { // munitions
		cout << "|M";
	}
	for(; numBay < getSize(); numBay++) {
		cout << "| ";
	}
	cout << '|' << endl;



	cout << '\\';
	for(uint numBay = 0; numBay < getSize() - 1; numBay++) {
		cout << "_A";
	}
	cout << "_/" << endl;
}

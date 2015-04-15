/*
 * Tile.h
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

#ifndef TILE_H_
#define TILE_H_

#include <string>
#include <tuple>
#include <vector>

#include "Player.h"
#include "Ship.h"
#include "Rapacity.h"

class Tile {
	protected:
		typedef unsigned int uint;
		typedef std::tuple<float, float, float, float, float> mul_t;
		typedef std::tuple<uint, uint, uint, uint, uint> prices_t;
		std::string name;
		char avitar;
		mul_t marketMul;
		mul_t markup;
		Player* wanted;
		float positiveSum(float origional, float addend);
		std::vector<Ship*> ships;
	public:
		Tile(std::string name, char avitar, bool isPrimitive);
		void shiftMarket();
		std::string getName();
		prices_t getPrices(prices_t prices);
		prices_t getMarkupPrices(prices_t prices);
		Player* getWanted();
		void setWanted(Player* player);
		void printShips();
		uint costOfShip(uint numShip);
		Ship* buyShip(uint numShip);
		uint getNumShips();
		char getChar();
		void manageInventory();
};

#endif /* TILE_H_ */

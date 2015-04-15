/*
 * Ship.h
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

#ifndef SHIP_H_
#define SHIP_H_

#include <tuple>


class Ship {
	protected:
		typedef unsigned int uint;
		typedef std::tuple<uint, uint, uint, uint> resources_t;
		typedef std::tuple<uint, uint> dice_t;
		dice_t dice;
		uint fuel;
		uint range;
		uint health;
		uint defense;
		uint size;
		resources_t resources;
	public:
		Ship();

		uint getDefense();
		uint getFuel();
		uint getHealth();
		uint getRange();
		uint getSpeed();
		uint getSize();
		resources_t getResources();
		bool changeResource(uint resource, int by);
		uint amtStuff();
		bool takeDamage(uint damage);
		void rest();
		uint roll(uint throtle);
		void printCargo();
};

#endif /* SHIP_H_ */

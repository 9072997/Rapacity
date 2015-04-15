/*
 * Player.h
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

#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <vector>

#include "Board.h"
#include "Ship.h"
#include "Rapacity.h"

class Player {
	protected:
		typedef unsigned int uint;
		char avitar;
		uint position;
		uint money;
		std::string name;
		Ship* ship;
	public:
		Player(std::string name, char avitar, uint money);
		Player(std::vector<char>* avalibleAvitars, uint money);
		uint getPosition();
		void takeTurn(Board* board, std::vector<Player*>);
		char getChar();
		std::string getName();
		uint getMoney();
		bool transaction(int money);
		Ship* getShip();
};


#endif /* PLAYER_H_ */

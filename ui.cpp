/*
 * ui.cpp
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

#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "Board.h"
#include "customIO.h"
#include "Player.h"

using namespace std;
using namespace customIO;
namespace {
	typedef unsigned int uint;
	typedef tuple<uint, uint, uint, uint, uint> prices_t;
}

namespace ui {
	void clear() {
		//cout << endl << "-------------------------------" << endl;
		cout << endl << "\033[2J"; // reset screen, the endl makes file output readable
	}
	unsigned int menu(string prompt, vector<string> options) {
		uint numOption = 0;
		for(vector<string>::iterator option = options.begin(); option < options.end(); option++) {
			numOption++;
			cout << '[' << numOption << "] " << *option << endl;
		}
		return getUserMenu(prompt, numOption)-1;
	}
	unsigned int menu(vector<string> options) {
		uint numOption = 0;
		for(vector<string>::iterator option = options.begin(); option < options.end(); option++) {
			numOption++;
			cout << '[' << numOption << "] " << *option << endl;
		}
		return getUserMenu(numOption)-1;
	}
	void printUserStatus(Player player, Board board) {
		Tile tile = *(board.get1DBoard().at(player.getPosition()));

		prices_t marketPrices = board.getPrices();
		uint marketPriceUranium = get<0>(marketPrices);
		uint marketPriceGold = get<1>(marketPrices);
		uint marketPriceOre = get<2>(marketPrices);
		uint marketPriceMunitions = get<3>(marketPrices);
		uint marketPriceFuel = get<4>(marketPrices);

		prices_t localPrices = tile.getPrices(marketPrices);
		uint localPriceUranium = get<0>(localPrices);
		uint localPriceGold = get<1>(localPrices);
		uint localPriceOre = get<2>(localPrices);
		uint localPriceMunitions = get<3>(localPrices);
		uint localPriceFuel = get<4>(localPrices);

		prices_t markupPrices = tile.getMarkupPrices(marketPrices);
		uint markupPriceUranium = get<0>(markupPrices);
		uint markupPriceGold = get<1>(markupPrices);
		uint markupPriceOre = get<2>(markupPrices);
		uint markupPriceMunitions = get<3>(markupPrices);
		uint markupPriceFuel = get<4>(markupPrices);

		uint money = player.getMoney();
		char wanted;
		if(tile.getWanted()) {
			wanted = tile.getWanted()->getChar();
		} else { // none of our players are fugitives
			wanted = 'O'; // generic head;
		}

		if(player.getShip()) { // if the player has a ship
			Ship ship = *(player.getShip());
			uint health = ship.getHealth();
			uint defense = ship.getDefense();
			uint fuel = ship.getFuel();
			uint range = ship.getRange();

			cout << "            Welcome to " << tile.getName() << ", " << player.getName() << " (" << player.getChar() << ")" << endl;
            printf("        |                     +---------------+--------+------------+\n       / \\    Money %7d   |     Prices    | Market |    Local   |\n      / _ \\   Sheilds %2d/%2d   +---------------+--------+------------+\n     |.o '.|  Fuel    %2d/%2d   | Fuel          | %3d Cr | %3d/%3d Cr |\n     |'._.'|                  +---------------+--------+------------+\n     |     |                  | Uranium (U)   | %3d Cr | %3d/%3d Cr |\n   ,'|  |  |`.     +------+   +---------------+--------+------------+\n  /  |  |  |  \\    |WANTED|   | Gold (G)      | %3d Cr | %3d/%3d Cr |\n  |,-'--|--'-.|    +------+   +---------------+--------+------------+\n      \\   /        |  %c   |   | Raw Ore (R)   | %3d Cr | %3d/%3d Cr |\n       \\ /         | /|\\  |   +---------------+--------+------------+\n        V          +----- +   | Munitions (M) | %3d Cr | %3d/%3d Cr |\n                              +---------------+--------+------------+\n",
				money, health, defense, fuel, range, marketPriceFuel, localPriceFuel, markupPriceFuel,
				marketPriceUranium, localPriceUranium, markupPriceUranium, marketPriceGold, localPriceGold, markupPriceGold,
				wanted, marketPriceOre, localPriceOre, markupPriceOre, marketPriceMunitions, localPriceMunitions, markupPriceMunitions);
            player.getShip()->printCargo();
		} else { //no ship
			cout << "     Welcome to " << tile.getName() << ", " << player.getName() << " (" << player.getChar() << ")" << endl;
			printf("                  +---------------+--------+------------+\n                  |     Prices    | Market |    Local   |\n  Money %7d   +---------------+--------+------------+\n                  | Fuel          | %3d Cr | %3d/%3d Cr |\n   O              +---------------+--------+------------+\n  /|\\             | Uranium (U)   | %3d Cr | %3d/%3d Cr |\n   |    +------+  +---------------+--------+------------+\n  / \\   |WANTED|  | Gold (G)      | %3d Cr | %3d/%3d Cr |\n        +------+  +---------------+--------+------------+\n        |  %c   |  | Raw Ore (R)   | %3d Cr | %3d/%3d Cr |\n        | /|\\  |  +---------------+--------+------------+\n        +----- +  | Munitions (M) | %3d Cr | %3d/%3d Cr |\n                  +---------------+--------+------------+\n",
				money, marketPriceFuel, localPriceFuel, markupPriceFuel, marketPriceUranium, localPriceUranium, markupPriceUranium,
				marketPriceGold, localPriceGold, markupPriceGold, wanted, marketPriceOre, localPriceOre, markupPriceOre,
				marketPriceMunitions, localPriceMunitions, markupPriceMunitions);
		}
	}
}

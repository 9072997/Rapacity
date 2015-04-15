/*
 * Board.h
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

#ifndef BOARD_H_
#define BOARD_H_

#include <tuple>
#include <vector>

#include "Player.h"
#include "Tile.h"
#include "Rapacity.h"

class Board {
	protected:
		typedef std::vector<Tile*> tileList1D_t;
		typedef std::vector<tileList1D_t> tileList2D_t;
		typedef unsigned int uint;
		typedef std::tuple<uint, uint> pair_t;
		typedef std::tuple<uint, uint, uint, uint, uint> prices_t;
		prices_t marketPrices;
		tileList1D_t tiles;
		uint minorLength;
		static uint getMajor(pair_t pair);
		static uint getMinor(pair_t pair);
		void printVDiv(uint numPlayers);
	public:
		Tile* getTile(uint pos);
		void putTile(Tile* tile, uint pos);

		tileList1D_t get1DBoard();
		tileList2D_t get2DBoard();

		uint get1DLength();
		pair_t get2DLength();

		prices_t getPrices();
		void shiftMarket();

		void print(std::vector<Player*> players);
		pair_t getDisplaySize(uint numPlayers);

		Board(pair_t size);
};

#endif /* BOARD_H_ */

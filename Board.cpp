/*
 * Board.cpp
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

#include "Board.h"

#include <sys/types.h>
#include <iostream>
#include <iterator>
#include <tuple>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <algorithm> /* max */

#include "Player.h"
#include "Tile.h"

using namespace std;


uint Board::getMajor(pair_t pair) {
	return get<0>(pair);
}
uint Board::getMinor(pair_t pair) {
	return get<1>(pair);
}

Tile* Board::getTile(uint numTile) {
	return tiles[numTile];
}

void Board::putTile(Tile* tile, uint pos) {
	tiles.at(pos) = tile;
}

vector<Tile*> Board::get1DBoard() {
	return tiles;
}
vector<vector<Tile*>> Board::get2DBoard() {
	uint linearLength = get1DLength();
	uint minorLength = getMinor(get2DLength());
	tileList2D_t majorVector;
	for(uint linearPos = 0; linearPos < linearLength;) {
		tileList1D_t minorVector;
		for(uint minorPos = 0; minorPos < minorLength; minorPos++, linearPos++) {
			minorVector.push_back(getTile(linearPos));
		}
		majorVector.push_back(minorVector);
	}
	return majorVector;
}

uint Board::get1DLength() {
	return tiles.size();
}
tuple<unsigned int, unsigned int> Board::get2DLength() {
	uint linearLength = get1DLength();
	uint majorLength = linearLength / minorLength;

	return make_tuple(majorLength, minorLength);
}

void Board::printVDiv(uint numPlayers) {
	uint minorLength = getMinor(get2DLength());
	for(uint minorPos = 0; minorPos < minorLength; minorPos++) {
		cout << '+';
		for(uint numPlayer = 0; numPlayer < numPlayers; numPlayer++) {
			cout << '-';
		}
	}
	cout << "+" << endl;
}
void Board::print(vector<Player*> players) { // non for iterator
	tileList2D_t majorVector = get2DBoard();
	uint boardPosition = 0;
	uint numPlayers = players.size();
	for(tileList2D_t::iterator majorIterator = majorVector.begin(); majorIterator < majorVector.end(); majorIterator++) {
		printVDiv(numPlayers);

		tileList1D_t minorVector = *majorIterator;
		for(tileList1D_t::iterator minorIterator = minorVector.begin(); minorIterator < minorVector.end(); minorIterator++) {
			cout << '|' << (*minorIterator)->getChar();
			for(uint numPlayer = 1; numPlayer < numPlayers; numPlayer++) {
				cout << ' ';
			}
		}
		cout << '|' << endl;
		for(uint rowPosition = 0; rowPosition < minorVector.size(); rowPosition++, boardPosition++) {
			cout << '|';
			for(vector<Player*>::iterator playerIterator = players.begin(); playerIterator < players.end(); playerIterator++) {
				Player player = **playerIterator;
				if(player.getPosition() == boardPosition) {
					cout << player.getChar();
				} else {
					cout << ' ';
				}
			}
		}
		cout << '|' << endl;
	}
	printVDiv(numPlayers);
}
tuple<unsigned int, unsigned int> Board::getDisplaySize(uint numPlayers) {
	uint width = (numPlayers+1) * getMinor(get2DLength()) + 1;
	uint height = 2*getMajor(get2DLength()) + 1;
	return make_tuple(height, width); // backwards to be major, minor consistant
}

Board::Board(pair_t size) {
	minorLength = getMinor(size); // object var
	uint majorLength = getMajor(size);

	tiles.resize(majorLength*minorLength);


	get<0>(marketPrices) = 120 + rand()%40; // uranium
	get<1>(marketPrices) = 80 + rand()%20; // gold
	get<2>(marketPrices) = 20 + rand()%10; // ore
	get<3>(marketPrices) = 40 + rand()%10; // munitions
	get<4>(marketPrices) = 2;             // fuel

}

std::tuple<uint, uint, uint, uint, uint> Board::getPrices() {
	return marketPrices;
}

void Board::shiftMarket() {
	get<0>(marketPrices) = max(int(get<0>(marketPrices)) + rand()%7 - 3, 1); // uranium
	get<1>(marketPrices) = max(int(get<1>(marketPrices)) + rand()%7 - 3, 1); // gold
	get<2>(marketPrices) = max(int(get<2>(marketPrices)) + rand()%5 - 2, 1); // ore
	get<3>(marketPrices) = max(int(get<3>(marketPrices)) + rand()%5 - 2, 1); // munitions
	get<4>(marketPrices) = max(int(get<4>(marketPrices)) + rand()%3 - 1, 1); // fuel

}


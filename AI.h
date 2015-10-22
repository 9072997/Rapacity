/*
 * AI.h
 *
 *  Created on: Apr 4, 2015
 *      Author: jon
 */

#ifndef AI_H_
#define AI_H_

#include <string>
#include <vector>

#include "Board.h"
#include "Player.h"
#include "Tile.h"

class AI: public Player {
	protected:
		static std::vector<std::string> names;
		void randomCheck(Tile* tile);
	public:
		AI(std::vector<char>* avalibleAvitars, uint money);
		virtual ~AI();
		void takeTurn(Board* board, std::vector<Player*>);
};

#endif /* AI_H_ */

/*
 * ui.h
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

#ifndef UI_H_
#define UI_H_


#include <string>
#include <vector>

namespace ui {
	void clear();
	unsigned int menu(std::vector<std::string> options);
	unsigned int menu(std::string prompt, std::vector<std::string> options);
	void printUserStatus(Player player, Board board);
}



#endif /* UI_H_ */

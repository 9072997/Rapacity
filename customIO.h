/*
 * customIO.h
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

#ifndef CUSTOMIO_H_
#define CUSTOMIO_H_

#include <string>

namespace customIO {
	/////////////////////
	// INPUT FUNCTIONS //
	/////////////////////
	std::string getUserString(std::string prompt);
	std::string getUserString(void);

	bool isNumberPart(char);

	int getUserInt(std::string prompt);
	int getUserInt(void);

	float getUserFloat(std::string prompt);
	float getUserFloat(void);

	unsigned int getUserMenu(std::string prompt, unsigned int max);
	unsigned int getUserMenu(unsigned int max);

	bool getUserBool(std::string prompt);
	bool getUserBool(void);

	int getUserUInt(std::string prompt);
	int getUserUInt(void);
	float getUserUFloat(std::string prompt);
	float getUserUFloat(void);

	void waitForUser(std::string prompt);
	void waitForUser(void);
}


#endif /* CUSTOMIO_H_ */

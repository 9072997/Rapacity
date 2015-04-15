/*
 * customIO.cpp
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

#include "customIO.h"

#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

namespace customIO {
	/////////////////////
	// INPUT FUNCTIONS //
	/////////////////////
	string getUserString(string prompt) { // I want spaces in team names
		string input = "";

		cout << prompt  << ": "; // tell the user what we want
		for(;;) { // append loop
			char terminator;
			string word;
			cin >> word;
			input = input + word;

			terminator = getchar();
			if(terminator == ' ') {
				input = input + ' ';
			} else {
				break; // we brobably read a '\n'
			}
		}

		return input;
	}
	string getUserString(void) {
		return getUserString("Enter any value");
	}

	bool isNumberPart(char charicter) { // is digit or '-'
		// note we throw away leading '+' as we assume positive anyway
		return ('0' <= charicter && charicter <= '9') || charicter == '-';
	}

	int getUserInt(string prompt) { // I don't want spaces in numbers
		string input;
		int value = 0;
		bool isNegitive = false;
		int numChar = 0;
		unsigned int placeValue = 1;
		input = getUserString(prompt);

		//                                     \/ this signals the end of string
		while(!isNumberPart(input[numChar]) && input[numChar] != 0) numChar++; // throw away charicters untill we hit a number
		if(input[numChar] == '-') {
			isNegitive = true;
			numChar++; // we are done with this char
		}

		// note null terminator is not a number part
		while(isNumberPart(input[numChar]) && input[numChar] != 0) numChar++; // now go past the number bit

		// and then read the number backwards
		//          \/ not great form, but needs to be before while compare
		while(numChar-- > 0 && isNumberPart(input[numChar]) && input[numChar] != '-') {
			value += (input[numChar] - '0') * placeValue;
			placeValue *= 10;
		}

		if(isNegitive) value *= -1;

		return value;
	}
	int getUserInt(void) {
		return getUserInt("Enter a whole number");
	}

	float getUserFloat(string prompt) { // I don't want spaces in numbers
		string input;
		float value = 0;
		bool isNegitive = false;
		int numChar = 0;
		float placeValue = 1;
		input = getUserString(prompt);

		//                                     \/ this signals the end of string
		while(!isNumberPart(input[numChar]) && input[numChar] != 0) numChar++; // throw away charicters untill we hit a number
		if(input[numChar] == '-') {
			isNegitive = true;
			numChar++; // we are done with this char
		}

		// note null terminator is not a number part
		while(isNumberPart(input[numChar]) && input[numChar] != 0) numChar++; // now go past the number bit

		// and then read the number backwards
		//         \/ not great form, but needs to be before while compare
		while(numChar-- > 0 && isNumberPart(input[numChar]) && input[numChar] != '-') {
			value += (input[numChar] - '0') * placeValue;
			placeValue *= 10;
		}

		// now back forward to the decimle or end
		while(isNumberPart(input[++numChar]) && input[numChar] != 0);
		if(input[numChar++] == '.') { // consume decimle
			placeValue = 0.1;
			while(isNumberPart(input[numChar]) && input[numChar] != '-' && input[numChar] != 0) {
				value += (input[numChar] - '0') * placeValue;
				placeValue /= 10;
				numChar++;
			}
		}

		if(isNegitive) value *= -1;

		return value;
	}
	float getUserFloat(void) {
		return getUserFloat("Enter any number");
	}


	unsigned int getUserMenu(string prompt, unsigned int max) {
		unsigned int selection;
		for(;;) {
			selection = getUserInt(prompt);
			if(selection > 0 && selection <= max) return selection;
			cout << "please enter a number from 1 to " << max << endl;
		}
	}
	unsigned int getUserMenu(unsigned int max) {
		return getUserMenu("Enter a number from the menu", max);
	}

	bool getUserBool(string prompt) {
		for(;;) {
			char response;
			cout << prompt;
			response = getUserString(" [Y/N]")[0];
			switch(response) { // based on user keypress
				case 'y': case 'Y': return true;
				case 'n': case 'N': return false;
			}
			cout << "Invalid response. Please enter Y or N." << endl;
		}
	}
	bool getUserBool(void) {
		return getUserBool("Is this correct?");
	}


	int getUserUInt(string prompt) {
		int value;
		for(;;) {
			value = getUserInt(prompt);
			if(value >= 0) return value;
			cout << "Invalid response. Negitive numbers are not allowed." << endl;
		}
	}
	int getUserUInt(void) {
		int value;
		for(;;) {
			value = getUserInt();
			if(value >= 0) return value;
			cout << "Invalid response. Negitive numbers are not allowed." << endl;
		}
	}
	float getUserUFloat(string prompt) {
		float value;
		for(;;) {
			value = getUserFloat(prompt);
			if(value >= 0) return value;
			cout << "Invalid response. Negitive numbers are not allowed." << endl;
		}
	}
	float getUserUFloat(void) {
		float value;
		for(;;) {
			value = getUserFloat();
			if(value >= 0) return value;
			cout << "Invalid response. Negitive numbers are not allowed." << endl;
		}
	}

	void waitForUser(string prompt) {
		cout << prompt;
		while(getchar() != '\n'); // consume chars (and have the OS stop the program) untill we get a return
	}
	void waitForUser(void) {
		waitForUser("Press enter to continue");
	}
}




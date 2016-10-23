#include "universal.h"
//#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

/*
std::vector<int> getReversePermutation(std::vector<int> a) {
	static int b[12];
	for (int i = 0; i < M; ++i)
		b[a[i]] = i;
	std::vector<int> ra;
	for (int i = 0; i < M; ++i)
		ra.push_back(b[i]);
	return ra;
}
*/

std::string numberToColor(int c) {
	if (c == 0)
		return "WHITE ";
	if (c == 1)
		return "BLUE  ";
	if (c == 2)
		return "ORANGE";
	if (c == 3)
		return "RED   ";
	if (c == 4)
		return "GREEN ";
	if (c == 5)
		return "YELLOW";
	return "Wrong color number!";
}

int colorToNumber(char c) {
	if (c == 'W')
		return 0;
	if (c == 'B')
		return 1;
	if (c == 'O')
		return 2;
	if (c == 'R')
		return 3;
	if (c == 'G')
		return 4;
	if (c == 'Y')
		return 5;
	return -1;
}

int getRandom() {
	//srand(time(NULL));
	return rand() % M;
}

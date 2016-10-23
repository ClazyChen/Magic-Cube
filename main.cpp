#include "universal.h"
#include "surface.h"
#include "magiccube.h"
#include <cstdio>
#include <string>
#include <iostream>

int main() {
	srand(time(NULL));
	for (int i = 0; i < 6; ++i)
		std::cout << i << " -> " << numberToColor(i) << std::endl;
	MagicCube *myMagicCube = new MagicCube();
	myMagicCube->readStatus();
	for (int i = 0; i < 30; ++i)
		myMagicCube->randomRotate();
	myMagicCube->printPath();
	myMagicCube->printStatus();
	return 0;
}


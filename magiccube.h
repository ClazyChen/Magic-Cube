#ifndef _MAGICCUBE_H
#define _MAGICCUBE_H

#include "universal.h"
#include "surface.h"
#include <vector>
#include <string>

class MagicCube {

public:
	MagicCube();
	void readStatus();
	void leftRoll();
	void rightRoll();
	void frontRoll();
	void backRoll();
	void leftRotate();
	void rightRotate();
	void L();
	void LL();
	void R();
	void RR();
	void U();
	void UU();
	void D();
	void DD();
	void F();
	void FF();
	void B();
	void BB();
	void randomRotate();
	void printPath();
	void printStatus();
private:
	Surface front, back, left, right, up, down;
	//std::vector<int> permutation;
	std::vector<std::string> path;
};
#endif

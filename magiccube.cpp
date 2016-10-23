#include "magiccube.h"
#include "universal.h"
#include "surface.h"
#include <cstdio>
#include <vector>
#include <string>
#include <iostream>

MagicCube::MagicCube() {
	/*
	permutation.clear();
	for (int i = 0; i < M; ++i)
		permutation.push_back(i);
	*/
	path.clear();
	
	up.fill(WHITE);
	down.fill(YELLOW);
	front.fill(BLUE);
	back.fill(GREEN);
	left.fill(RED);
	right.fill(ORANGE);
}

void MagicCube::readStatus() {
	//front, back, left, right, up, down
	path.clear();
	puts("The front surface:");
	front.read();
	puts("The back surface:");
	back.read();
	puts("The left surface:");
	left.read();
	puts("The right surface:");
	right.read();
	puts("The up surface:");
	up.read();
	puts("The down surface:");
	down.read();
}

void MagicCube::leftRoll() {
	static Surface temp;
	temp = up;
	up = right;
	right = down;
	down = left;
	left = temp;
	up.leftRotate();
	left.leftRotate();
	down.leftRotate();
	right.leftRotate();
	front.leftRotate();
	back.rightRotate();
	path.push_back(std::string("LR"));
}

void MagicCube::rightRoll() {
	static Surface temp;
	temp = up;
	up = left;
	left = down;
	down = right;
	right = temp;
	up.rightRotate();
	right.rightRotate();
	down.rightRotate();
	left.rightRotate();
	front.rightRotate();
	back.leftRotate();
	path.push_back(std::string("RR"));
}

void MagicCube::frontRoll() {
	static Surface temp;
	temp = up;
	up = back;
	up.rightRotate(), up.rightRotate();
	back = down;
	back.rightRotate(), back.rightRotate();
	down = front;
	front = temp;
	left.rightRotate();
	right.leftRotate();
	path.push_back(std::string("FR"));
}

void MagicCube::backRoll() {
	static Surface temp;
	temp = up;
	up = front;
	front = down;
	down = back;
	down.rightRotate(), down.rightRotate();
	back = temp;
	back.rightRotate(), back.rightRotate();
	left.leftRotate();
	right.rightRotate();
	path.push_back(std::string("BR"));
}

void MagicCube::L() {
	static int x[4][3] = {{0, 1, 2}, {0, 1, 2}, {0, 1, 2}, {2, 1, 0}};
	static int y[4][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {2, 2, 2}};
	circleTransition(up, front, down, back, x, y);
	left.rightRotate();	
	path.push_back(std::string("L"));
}

void MagicCube::LL() {
	static int x[4][3] = {{2, 1, 0}, {0, 1, 2}, {2, 1, 0}, {2, 1, 0}};
	static int y[4][3] = {{0, 0, 0}, {2, 2, 2}, {0, 0, 0}, {0, 0, 0}};
	circleTransition(up, back, down, front, x, y);
	left.leftRotate();
	path.push_back(std::string("L'"));
}

void MagicCube::R() {
	static int x[4][3] = {{2, 1, 0}, {0, 1, 2}, {2, 1, 0}, {2, 1, 0}};
	static int y[4][3] = {{2, 2, 2}, {0, 0, 0}, {2, 2, 2}, {2, 2, 2}};
	circleTransition(up, back, down, front, x, y);
	right.rightRotate();
	path.push_back(std::string("R"));
}

void MagicCube::RR() {
	static int x[4][3] = {{0, 1, 2}, {0, 1, 2}, {0, 1, 2}, {2, 1, 0}};
	static int y[4][3] = {{2, 2, 2}, {2, 2, 2}, {2, 2, 2}, {0, 0, 0}};
	circleTransition(up, front, down, back, x, y);
	right.leftRotate();
	path.push_back(std::string("R'"));
}

void MagicCube::U() {
	static int x[4][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
	static int y[4][3] = {{2, 1, 0}, {2, 1, 0}, {2, 1, 0}, {2, 1, 0}};
	circleTransition(back, right, front, left, x, y);
	up.rightRotate();
	path.push_back(std::string("U"));
}

void MagicCube::UU() {
	static int x[4][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
	static int y[4][3] = {{0, 1, 2}, {0, 1, 2}, {0, 1, 2}, {0, 1, 2}};
	circleTransition(back, left, front, right, x, y);
	up.leftRotate();
	path.push_back(std::string("U'"));
}

void MagicCube::D() {
	static int x[4][3] = {{2, 2, 2}, {2, 2, 2}, {2, 2, 2}, {2, 2, 2}};
	static int y[4][3] = {{0, 1, 2}, {0, 1, 2}, {0, 1, 2}, {0, 1, 2}};
	circleTransition(front, right, back, left, x, y);
	down.rightRotate();
	path.push_back(std::string("D"));
}

void MagicCube::DD() {
	static int x[4][3] = {{2, 2, 2}, {2, 2, 2}, {2, 2, 2}, {2, 2, 2}};
	static int y[4][3] = {{2, 1, 0}, {2, 1, 0}, {2, 1, 0}, {2, 1, 0}};
	circleTransition(front, left, back, right, x, y);
	down.leftRotate();
	path.push_back(std::string("D'"));
}

void MagicCube::F() {
	static int x[4][3] = {{2, 2, 2}, {0, 1, 2}, {0, 0, 0}, {2, 1, 0}};
	static int y[4][3] = {{0, 1, 2}, {0, 0, 0}, {2, 1, 0}, {2, 2, 2}};
	circleTransition(up, right, down, left, x, y);
	front.rightRotate();
	path.push_back(std::string("F"));
}

void MagicCube::FF() {
	static int x[4][3] = {{2, 2, 2}, {0, 1, 2}, {0, 0, 0}, {2, 1, 0}};
	static int y[4][3] = {{2, 1, 0}, {2, 2, 2}, {0, 1, 2}, {0, 0, 0}};
	circleTransition(up, left, down, right, x, y);
	front.leftRotate();
	path.push_back(std::string("F'"));
}

void MagicCube::B() {
	static int x[4][3] = {{0, 0, 0}, {0, 1, 2}, {2, 2, 2}, {2, 1, 0}};
	static int y[4][3] = {{2, 1, 0}, {0, 0, 0}, {0, 1, 2}, {2, 2, 2}};
	circleTransition(up, left, down ,right, x, y);
	back.rightRotate();
	path.push_back(std::string("B"));
}

void MagicCube::BB() {
	static int x[4][3] = {{0, 0, 0}, {0, 1, 2}, {2, 2, 2}, {2, 1, 0}};
	static int y[4][3] = {{0, 1, 2}, {2, 2, 2}, {2, 1, 0}, {0, 0, 0}};
	circleTransition(up, right, down, left, x, y);
	back.leftRotate();
	path.push_back(std::string("B'"));
}

void MagicCube::randomRotate() {
	int r = getRandom();
	if (r == 0)
		L();
	if (r == 1)
		LL();
	if (r == 2)
		R();
	if (r == 3)
		RR();
	if (r == 4)
		U();
	if (r == 5)
		UU();
	if (r == 6)
		D();
	if (r == 7)
		DD();
	if (r == 8)
		F();
	if (r == 9)
		FF();
	if (r == 10)
		B();
	if (r == 11)
		BB();
	if (r == 12)
		leftRoll();
	if (r == 13)
		rightRoll();
	if (r == 14)
		backRoll();
	if (r == 15)
		frontRoll();
}

void MagicCube::printPath() {
	printf("Total %d operates.\n", (int)path.size());
	for (int i = 0; i < (int)path.size(); ++i)
		std::cout << path[i] << " ";
	std::cout << std::endl;
}

void MagicCube::printStatus() {
	printf("The front surface:\n");
	front.print();
	printf("The back surface:\n");
	back.print();
	printf("The left surface:\n");
	left.print();
	printf("The right surface:\n");
	right.print();
	printf("The up surface:\n");
	up.print();
	printf("The down surface:\n");
	down.print();
}



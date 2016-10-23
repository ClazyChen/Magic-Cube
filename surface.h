#ifndef _SURFACE_H
#define _SURFACE_H

#include "universal.h"

class Surface {
public:
	int get(int i, int j);
	void set(int i, int j, int x);
	void fill(int x);
	void leftRotate();
	void rightRotate();
	void print();
	void read();
private:
	int c[N][N];
};

void circleTransition(Surface &s0, Surface &s1, Surface &s2, Surface &s3, int x[4][3], int y[4][3]); 
#endif

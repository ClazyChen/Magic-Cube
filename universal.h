#ifndef _UNIVERSAL_H
#define _UNIVERSAL_H

//#include <vector>
#include <string>

static const int N = 3;
static const int M = 16;
static const int WHITE   = 0;
static const int BLUE    = 1;
static const int ORANGE  = 2;
static const int RED     = 3;
static const int GREEN   = 4;
static const int YELLOW  = 5;

//std::vector<int> getReversePermutation(std::vector<int> a);

std::string numberToColor(int c);

int colorToNumber(char c);

int getRandom();
#endif

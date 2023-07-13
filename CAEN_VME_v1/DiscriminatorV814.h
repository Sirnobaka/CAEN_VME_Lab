#ifndef V814_H
#define V814_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "CAENVME.h"
#include <stdlib.h>
#include <bitset>

#define V418_THR_CH0                 0x00
#define V418_THR_CH15                0x1E
#define V418_OUTPUT_WIDTH_CH_0_7     0x40
#define V418_OUTPUT_WIDTH_CH_8_15    0x42
#define V418_MAJORITY_THR            0x48
#define V418_PATTERN_INHIBIT         0x4A
#define V418_FIXED_CODE              0xFA
#define V418_MAN_MOD_TYPE            0xFC
#define V418_VER_SERIAL              0xFE



class DiscriminatorV814
{
private:
	int addr;
	CAENVME* vme;

public:
	// constructor
	DiscriminatorV814();
	DiscriminatorV814(unsigned int Address, CAENVME* vme); // Address = 0xEE00
	// destructor
	~DiscriminatorV814();

	void SetThresholds(int thresholds);
	void SetOutputWidth(int width);
	void SetPatternInhibit(unsigned long pattern);
	void GetInfo();

};
#endif


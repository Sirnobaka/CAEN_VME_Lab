#ifndef VME_H
#define VME_H

#include <iostream>
#include <string>
#include <sys/timeb.h>
#include <vector>
#include <map>
#include <signal.h>
#include <cmath>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <windows.h>

#include "CAENVMElib.h"
#include "CAENVMEtypes.h"

#define DEBUG true

class CAENVME
{
private:

	bool    inuse;
	int32_t handle;

public:

	CVAddressModifier cvAM;
	CVDataWidth cvDW;

public:
	// constructor
	CAENVME() { cvAM = cvA32_U_DATA; cvDW = cvD16; }; // cvD32
	CAENVME(CVAddressModifier CvAM, CVDataWidth CvDW) { cvAM = CvAM; cvDW = CvDW; };
	// destructor
	~CAENVME();

	int32_t      GetHandle() { return handle; };
	bool         GetInUse() { return inuse; };
	CVErrorCodes initVME(int link);
	CVErrorCodes closeVME();

	CVErrorCodes readVMEdata(unsigned int boardAdd, unsigned int registerAdd, void* data, CVAddressModifier CvAM, CVDataWidth CvDW);
	CVErrorCodes writeVMEdata(unsigned int boardAdd, unsigned int registerAdd, void* data, CVAddressModifier CvAM, CVDataWidth CvDW);
	CVErrorCodes readVMEdata(unsigned int boardAdd, unsigned int registerAdd, void* data);
	CVErrorCodes writeVMEdata(unsigned int boardAdd, unsigned int registerAdd, void* data);
};

#endif


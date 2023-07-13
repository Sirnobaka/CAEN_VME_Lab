#include <iostream>
#include <string>
#include <sys/timeb.h>
#include <vector>
#include <map>
#include <signal.h>
#include <cmath>

#include <sstream>
#include <iomanip>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>

//#include "VME.h"
#include "CAENVMElib.h"
//#include "CAENVMEtypes.h"


int main_vx1742()
{

    std::cout << "Now we are trying to talk with CAEN VME!\n";

    // Initialize the CAEN VME
    CVBoardTypes BoardType = cvUSB_V4718;  // cvV2718 or cvV1718
    uint32_t LinkNumber = 21436; // PID
    uint32_t ConetNode = 0;
    int32_t BHandle;
    CVErrorCodes Result = CAENVME_Init2(BoardType, &LinkNumber, ConetNode, &BHandle);
    if (Result != cvSuccess) {
        printf("Error initializing the VME library: %s\n", CAENVME_DecodeError(Result));
        return 1;
    }
    else {
        printf("Connection OK\n");
    }

    unsigned int BoardAddress = 0x32100000; // Address of ADC VX1742
    unsigned int RegisterAdd = 0x8104; // address to read Aqusition status
    unsigned int address = BoardAddress + RegisterAdd;

    uint32_t data; // data container

    CVAddressModifier CvAM = cvA32_S_PGM; // Address modifier
    CVDataWidth CvDW = cvD16; // Data width

    std::cout << "\n-----------------------------" << std::endl;
    std::cout << "Input:" << std::endl << "-----------------------------" << std::endl;
    std::cout << "address (hex) = " << std::hex << address << std::endl;
    printf("address (unsigned int) = %u\n", address);
    printf("address (signed int) = %d\n", address);
    std::cout << "-----------------------------" << std::endl;

    CVErrorCodes retval = CAENVME_ReadCycle(BHandle, address, &data, CvAM, CvDW);

    std::cout << "Output:" << std::endl << "-----------------------------" << std::endl;
    printf("Error code = %d\n", retval);
    std::cout << "data (hex) = " << std::hex << data << std::endl;
    printf("data = %d\n", data);
    printf("data (unsigned) = %u\n", data);
    std::cout << "-----------------------------" << std::endl;

    CAENVME_End(BHandle);

    return 0;	
}
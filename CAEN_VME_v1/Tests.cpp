// CAEN_VME_v1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


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
#include <bitset>

// Including them here causes conflict
//#include "CAENVMElib.h"
//#include "CAENVMEtypes.h"
#include "CAENDigitizer.h"
#include "CAENDigitizerType.h"

#include "CAENVME.h"
#include "DiscriminatorV814.h"


int run_v814_test()
{
    std::cout << "V814\n";

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

    unsigned int BoardAddress = 0xEE000000; // Address of discriminator V814
    unsigned int RegisterAdd = 0xFA; // address to read Fixed code
    //unsigned int address = (BoardAddress + RegisterAdd); // Full address

    uint32_t data; // data container
    /*
    The address modifiers codes recognized by the v814 module:
    AM = %39 Standard user data access (cvA24_U_DATA)
    AM = %3D Standard supervisor data access (cvA24_S_DATA)
    AM = %09 Extended user data access (cvA32_U_DATA)
    AM = %0D Extended supervisor data access (cvA32_S_DATA)
    */
    CVAddressModifier CvAM = cvA32_S_PGM; // Address modifier
    CVDataWidth CvDW = cvD16; // Data width

    std::cout << "Read cycles test" << std::endl;

    std::vector<std::pair<unsigned int, std::string>> addresses;
    std::pair <unsigned int, std::string> pair1(0xFA, "Fixed code");
    std::pair <unsigned int, std::string> pair2(0xFC, "Manufacturer & Module type");
    std::pair <unsigned int, std::string> pair3(0xFE, "Version & Serial number");
    addresses.push_back(pair1);
    addresses.push_back(pair2);
    addresses.push_back(pair3);

    for (unsigned int i = 0; i < addresses.size(); i++) {
        std::cout << std::endl << i + 1 << "." << std::endl;
        unsigned int address = (BoardAddress + addresses[i].first);

        std::cout << "-----------------------------" << std::endl;
        std::cout << "Input:" << std::endl << "-----------------------------" << std::endl;
        std::cout << addresses[i].second << "(%" << std::hex << addresses[i].first << ")" << std::endl;
        std::cout << "address (hex) = " << std::hex << address << std::endl;
        printf("address (unsigned int) = %u\n", address);
        printf("address (signed int) = %d\n", address);
        std::cout << "-----------------------------" << std::endl;

        CVErrorCodes retval = CAENVME_ReadCycle(BHandle, address, &data, CvAM, CvDW);

        // What is this???
        unsigned int data_short = data - 0xCCCC0000;

        std::cout << "Output:" << std::endl << "-----------------------------" << std::endl;
        printf("Error code = %d\n", retval);
        std::cout << "data (hex) = " << std::hex << data_short << std::endl;
        printf("data = %d\n", data_short);
        printf("data (unsigned) = %u\n", data_short);
        std::cout << "-----------------------------" << std::endl;

    }

    CAENVME_End(BHandle);

    return 0;
}

// Board adresses:
// 0xEE000000 for v814
// 0x0E000000 for v812

int set_discriminator_thresholds(unsigned int BoardAddress, CVAddressModifier CvAM, CVDataWidth CvDW, const unsigned int channel_thr) {

    std::cout << "V814 set thresholds\n";

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

    //unsigned int BoardAddress = 0xEE000000; // Address of discriminator V814
    /*
    The address modifiers codes recognized by the v814 module:
    AM = %39 Standard user data access (cvA24_U_DATA)
    AM = %3D Standard supervisor data access (cvA24_S_DATA)
    AM = %09 Extended user data access (cvA32_U_DATA)
    AM = %0D Extended supervisor data access (cvA32_S_DATA)
    */
    //CVAddressModifier CvAM = cvA32_S_PGM; // Address modifier
    //CVDataWidth CvDW = cvD16; // Data width

    std::string thr_string = "Threshold register Ch. ";
    std::vector<std::pair<unsigned int, std::string>> addresses_thr;

    for (unsigned int i = 0; i < 16; i++) {
        std::pair<unsigned int, std::string> pair_ch(0 + i * 2, "Threshold register Ch. " + std::to_string(i));
        addresses_thr.push_back(pair_ch);
    }

    for (unsigned int i = 0; i < addresses_thr.size(); i++) {
        std::cout << addresses_thr[i].first << " | " << addresses_thr[i].second << std::endl;
        unsigned int address = (BoardAddress + addresses_thr[i].first);
        //const unsigned int channel_thr = 100; // channel threshold in mV
        std::cout << "Set discriminator threshold to " << channel_thr << " mV (hex)" << std::endl;
        printf("Set discriminator threshold to %d mV\n", channel_thr);
        CVErrorCodes retval = CAENVME_WriteCycle(BHandle, address, &channel_thr, CvAM, CvDW);
        std::cout << "Error code = " << retval << std::endl;
        //if (i > 1) break;
    }

    CAENVME_End(BHandle);

    return 0;
}

int discriminator_simple_test(unsigned int BoardAddress, CVAddressModifier CvAM, CVDataWidth CvDW) {
    std::cout << "test discriminator " << std::hex << BoardAddress << std::endl;

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

    std::cout << "BHandle = " << BHandle << std::hex << std::endl;

    //unsigned int BoardAddress = 0xEE000000; // Address of discriminator V814
    unsigned int RegisterAdd = 0xFA; // address to read Fixed code
    //unsigned int address = (BoardAddress + RegisterAdd); // Full address

    uint32_t data; // data container
    /*
    The address modifiers codes recognized by the v814 module:
    AM = %39 Standard user data access (cvA24_U_DATA)
    AM = %3D Standard supervisor data access (cvA24_S_DATA)
    AM = %09 Extended user data access (cvA32_U_DATA)
    AM = %0D Extended supervisor data access (cvA32_S_DATA)
    */
    //CVAddressModifier CvAM = cvA32_S_PGM; // Address modifier
    //CVDataWidth CvDW = cvD16; // Data width

    std::cout << "Read cycle test" << std::endl;
    unsigned int address = (BoardAddress + RegisterAdd);

    std::cout << "-----------------------------" << std::endl;
    std::cout << "Input:" << std::endl << "-----------------------------" << std::endl;
    std::cout << "address (hex) = " << std::hex << address << std::endl;
    printf("address (unsigned int) = %u\n", address);
    printf("address (signed int) = %d\n", address);
    std::cout << "-----------------------------" << std::endl;

    CVErrorCodes retval = CAENVME_ReadCycle(BHandle, address, &data, CvAM, CvDW);

    // What is this???
    unsigned int data_short = data - 0xCCCC0000;

    std::cout << "Output:" << std::endl << "-----------------------------" << std::endl;
    printf("Error code = %d\n", retval);
    std::cout << "error: " << CAENVME_DecodeError(retval) << std::endl;
    std::cout << "data (hex) = " << std::hex << data_short << std::endl;
    printf("data = %d\n", data_short);
    printf("data (unsigned) = %u\n", data_short);
    std::cout << "-----------------------------" << std::endl;

    CAENVME_End(BHandle);

    return 0;
}

int set_discriminator_output_width(unsigned int BoardAddress, CVAddressModifier CvAM, CVDataWidth CvDW, const unsigned int channel_width) {

    std::cout << "V814 set output width\n";

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

    //unsigned int BoardAddress = 0xEE000000; // Address of discriminator V814
    /*
    The address modifiers codes recognized by the v814 module:
    AM = %39 Standard user data access (cvA24_U_DATA)
    AM = %3D Standard supervisor data access (cvA24_S_DATA)
    AM = %09 Extended user data access (cvA32_U_DATA)
    AM = %0D Extended supervisor data access (cvA32_S_DATA)
    */
    //CVAddressModifier CvAM = cvA32_S_PGM; // Address modifier
    //CVDataWidth CvDW = cvD16; // Data width


    std::cout << "Set output width Ch. 0 to 7" << std::endl;
    unsigned int address_output_ch0to7 = 0x40;
    unsigned int address_output_ch8to15 = 0x42;
    unsigned int address_width = (BoardAddress + address_output_ch0to7);
    //const unsigned int channel_width = 240; // for ~50 ns
    CVErrorCodes retval_width = CAENVME_WriteCycle(BHandle, address_width, &channel_width, CvAM, CvDW);
    std::cout << "Error code 0 to 7 = " << retval_width << std::endl;
    address_width = (BoardAddress + address_output_ch8to15);
    retval_width = CAENVME_WriteCycle(BHandle, address_width, &channel_width, CvAM, CvDW);
    std::cout << "Error code 8 to 15 = " << retval_width << std::endl;
    printf("Width -> %d (~50 ns)\n", channel_width);

    CAENVME_End(BHandle);

    return 0;
}

int set_discriminator_pattern_inhibit(unsigned int BoardAddress, CVAddressModifier CvAM, CVDataWidth CvDW, unsigned int pattern_inhibit) {

    std::cout << "V814 set pattern ihibit\n";

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

    unsigned int address_inhibit = BoardAddress + 0x4A;
    std::bitset<16> b1(pattern_inhibit);
    std::cout << "In binary (" << address_inhibit << "): " << b1 << std::endl;

    CVErrorCodes retval_pattern = CAENVME_WriteCycle(BHandle, address_inhibit, &pattern_inhibit, CvAM, CvDW);
    std::cout << "error code = " << retval_pattern << std::endl;
    std::cout << "error: " << CAENVME_DecodeError(retval_pattern) << std::endl;

    /*
    unsigned int RegisterAdd = 0xFA;
    unsigned int data;
    std::cout << "Read cycle test" << std::endl;
    unsigned int address = (BoardAddress + RegisterAdd);
    CVErrorCodes retval = CAENVME_ReadCycle(BHandle, address, &data, CvAM, CvDW);
    unsigned int data_short = data - 0xCCCC0000;
    printf("error code = %d\n", retval);
    std::cout << "error: " << CAENVME_DecodeError(retval) << std::endl;
    std::cout << "data (hex) = " << std::hex << data_short << std::endl;
    */

    CAENVME_End(BHandle);

    return retval_pattern;
}


int run_vx1742_test()
{

    std::cout << "VX1742\n";

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
    unsigned int RegisterAdd = 0x8140; // address to read Aqusition status
    unsigned int address = BoardAddress + RegisterAdd;

    uint32_t data; // data container

    CVAddressModifier CvAM = cvA32_S_PGM; // Address modifier
    CVDataWidth CvDW = cvD16; // Data width

    unsigned int addresses[] = {0x8104};
    std::vector<std::string> funcs;


    CVErrorCodes retval = CAENVME_ReadCycle(BHandle, address, &data, CvAM, CvDW);
    unsigned int data_short = data - 0xCCCC0000;

    std::cout << "Output:" << std::endl << "-----------------------------" << std::endl;
    printf("Error code = %d\n", retval);
    std::cout << "data (hex) = " << std::hex << data_short << std::endl;
    std::bitset<16> b1(data);
    std::cout << b1 << std::endl;
    std::cout << "-----------------------------" << std::endl;


    //////////////////////////////////////////////////////////////////



    CAENVME_End(BHandle);

    return 0;
}


int main() {

    int LinkNumber = 21436;

    // This the test for V814 discriminator
    CAENVME* VMETest = new CAENVME(cvA32_U_DATA, cvD16);
    VMETest->initVME(LinkNumber);

    unsigned int DiscriminatorBaseAddress = 0xEE00;
    DiscriminatorV814* Discr = new DiscriminatorV814(DiscriminatorBaseAddress, VMETest);
    std::cout << "Setting pattern of inhibit" << std::endl;
    unsigned long pattern_inhibit = 0b1111111111111111;
    Discr->SetPatternInhibit(pattern_inhibit);
    Discr->GetInfo();

    std::cout << "Output width" << std::endl;
    Discr->SetOutputWidth(240);
    Discr->SetThresholds(100);

    delete VMETest;
    //if (NULL != VMETest)  VMETest->closeVME();


    // This the test for V1742 ADC
    int handle;
    CAEN_DGTZ_ErrorCode ret;
    CAEN_DGTZ_BoardInfo_t BoardInfo;
    char* buffer = NULL;
    int c = 0;

    ret = CAEN_DGTZ_OpenDigitizer2(CAEN_DGTZ_USB_V4718, &LinkNumber, 0, 0x32100000, &handle);
    if (ret != CAEN_DGTZ_Success) {
        printf("Can't open digitizer\n");
        goto QuitProgram;
    }
    // Once we have the handler to the digitizer, we use it to call the other functions
    ret = CAEN_DGTZ_GetInfo(handle, &BoardInfo);
    printf("\nConnected to CAEN Digitizer Model %s \n", BoardInfo.ModelName);
    printf("\tROC FPGA Release is %s\n", BoardInfo.ROC_FirmwareRel);
    printf("\tAMC FPGA Release is %s\n", BoardInfo.AMC_FirmwareRel);
    printf("\tSerial Number is %d\n", BoardInfo.SerialNumber);
    printf("\tNumber of channels = %d\n", BoardInfo.Channels);
    printf("\tNumber of bits of the ADC = %d\n", BoardInfo.ADC_NBits);
    printf("\tComm handle = %d\n", BoardInfo.CommHandle);
    printf("\tVME handle = %d\n", BoardInfo.VMEHandle);

    // GetInterruptConfig

    std::cout << "Gitting interrup config" << std::endl;

    CAEN_DGTZ_EnaDis_t state; // Enable or disable
    uint8_t level;            // VME IRQ Level (from 1 to 7). Must be 1 for direct connection through CONET
    uint32_t status_id;       // 32-bit number assigned to the device and returned by the device during the Interrupt Acknowledge
    uint16_t event_number;    // If the number of events ready for the readout is equal to or greater than event_number,
                              // then the digitizer asserts the interrupt request
    CAEN_DGTZ_IRQMode_t mode; // Interrupt release mode: CAEN_DGTZ_IRQ_MODE_RORA (release on register access) or 
                              // CAEN_DGTZ_IRQ_MODE_ROAK (release on acknowledge) // CAEN_DGTZ_IRQMode_t

    ret = CAEN_DGTZ_GetInterruptConfig(handle, &state, &level, &status_id, &event_number, &mode);

    if (ret == CAEN_DGTZ_Success) {
        printf("Success\n");
        printf("\tstate = %d\n", state);
        printf("\tlevel = %d\n", level);
        printf("\tstatus_id = %d\n", status_id);
        printf("\tevent_number = %d\n", event_number);
        printf("\tmode = %d\n", mode);
    }





    goto QuitProgram;


    //
    QuitProgram:
        // Free the buffers and close the digitizers
        ret = CAEN_DGTZ_FreeReadoutBuffer(&buffer);
        ret = CAEN_DGTZ_CloseDigitizer(handle);
        printf("Press 'Enter' key to exit\n");
        c = getchar();
        return 0;
    return 0;
}
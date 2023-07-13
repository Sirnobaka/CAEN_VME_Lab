#include "CAENVME.h"

CAENVME::~CAENVME() {
	closeVME();
}

CVErrorCodes CAENVME::initVME(int link) {
	CVErrorCodes retval = CAENVME_Init2(cvUSB_V4718, &link, 0, &handle);
	inuse = false;
	return retval;
}

CVErrorCodes CAENVME::closeVME() {
	//unlockVME();
	std::cout << "Close CAENVME" << std::endl;
	CVErrorCodes retval = CAENVME_End(handle);
	std::cout << "Close error: " << CAENVME_DecodeError(retval) << std::endl;
	return retval;
}

CVErrorCodes CAENVME::readVMEdata(unsigned int boardAdd, unsigned int registerAdd, void* data, CVAddressModifier CvAM, CVDataWidth CvDW) {
	unsigned long int address = boardAdd;
	address = (address << 16) + registerAdd;
	inuse = true;
	if (DEBUG)   printf("read in %08x %04X am=%x dw=%x\n", address, data, CvAM, CvDW);
	CVErrorCodes retval = CAENVME_ReadCycle(handle, address, data, CvAM, CvDW);
	inuse = false;
	return retval;
}

CVErrorCodes CAENVME::writeVMEdata(unsigned int boardAdd, unsigned int registerAdd, void* data, CVAddressModifier CvAM, CVDataWidth CvDW) {
	unsigned long int address = boardAdd;
	address = (address << 16) + registerAdd;
	inuse = true;
	if (DEBUG)   printf("write in %08x %04X am=%x dw=%x\n", address, data, CvAM, CvDW);
	CVErrorCodes retval = CAENVME_WriteCycle(handle, address, data, CvAM, CvDW);
	inuse = false;
	return retval;
}

CVErrorCodes CAENVME::readVMEdata(unsigned int boardAdd, unsigned int registerAdd, void* data) {
	unsigned int address = boardAdd;
	address = (address << 16) + registerAdd;
	inuse = true;
	if (DEBUG) printf("from %08x %04X am=%x dw=%x\n", address, *(int*)data, cvAM, cvDW);
	CVErrorCodes retval = CAENVME_ReadCycle(handle, address, data, cvAM, cvDW);
	inuse = false;
	return retval;
}

CVErrorCodes CAENVME::writeVMEdata(unsigned int boardAdd, unsigned int registerAdd, void* data) {
	unsigned long int address = boardAdd;
	address = (address << 16) + registerAdd;
	inuse = true;
	if (DEBUG) printf("from %08x %04X am=%x dw=%x\n", address, *(int*)data, cvAM, cvDW);
	CVErrorCodes retval = CAENVME_WriteCycle(handle, address, data, cvAM, cvDW);
	inuse = false;
	return retval;
}
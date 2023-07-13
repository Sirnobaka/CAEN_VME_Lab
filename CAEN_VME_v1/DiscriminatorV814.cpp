#include "DiscriminatorV814.h"

DiscriminatorV814::DiscriminatorV814() 
{
}

DiscriminatorV814::DiscriminatorV814(unsigned int Address, CAENVME* Vme)
{
	addr = Address;
	vme = Vme;

	std::cout << "Init DiscriminatorV814 object, addr = " << std::hex << addr << std::endl;
}

DiscriminatorV814::~DiscriminatorV814()
{
	if (DEBUG) std::cout << "Delete object DiscriminatorV814" << std::endl;
}

void DiscriminatorV814::SetThresholds(int thresholds)
{
	if (vme) {
		for (unsigned int i = 0; i < 16; i++) {
			CVErrorCodes error = vme->writeVMEdata(this->addr, 0x00 + i, &thresholds, cvA32_U_DATA, cvD16);
			if (DEBUG) std::cout << "Channel " << i << " : " << CAENVME_DecodeError(error) << std::endl;
		}
	}
	else {
		std::cout << "CAENVME object not initialized" << std::endl;
	}
}

void DiscriminatorV814::SetOutputWidth(int width)
{
	if (vme) {
		CVErrorCodes error1 = vme->writeVMEdata(this->addr, V418_OUTPUT_WIDTH_CH_0_7, &width, cvA32_U_DATA, cvD16);
		if (DEBUG) std::cout << "Channels 0-7: " << CAENVME_DecodeError(error1) << std::endl;
		CVErrorCodes error2 = vme->writeVMEdata(this->addr, V418_OUTPUT_WIDTH_CH_8_15, &width, cvA32_U_DATA, cvD16);
		if (DEBUG) std::cout << "Channels 8-15: " << CAENVME_DecodeError(error2) << std::endl;
	}
	else {
		std::cout << "CAENVME object not initialized" << std::endl;
	}
}

void DiscriminatorV814::SetPatternInhibit(unsigned long pattern)
{
	std::bitset<16> b_pattern(pattern);
	if (DEBUG) std::cout << "Pattern if Inhibit: " << b_pattern << std::endl;
	if (vme) {
		CVErrorCodes error = vme->writeVMEdata(this->addr, V418_PATTERN_INHIBIT, &pattern, cvA32_U_DATA, cvD16);
		if (DEBUG) std::cout << "Error code: " << CAENVME_DecodeError(error) << std::endl;
	}
	else {
		std::cout << "CAENVME object not initialized" << std::endl;
	}
}

void DiscriminatorV814::GetInfo()
{
	int data_code;   // Fixed code
	int data_module; // Manufacturer and Module type
	int data_serial; // Version and serial number

	if (vme) {
		CVErrorCodes error1 = vme->readVMEdata(this->addr, V418_FIXED_CODE, &data_code, cvA32_U_DATA, cvD16);
		data_code = data_code - 0xCCCC0000;
		std::cout << "Fixed Code: " << std::hex << data_code << std::endl;
		if (DEBUG) std::cout << "Error code: " << CAENVME_DecodeError(error1) << std::endl;

		CVErrorCodes error2 = vme->readVMEdata(this->addr, V418_MAN_MOD_TYPE, &data_module, cvA32_U_DATA, cvD16);
		data_module = data_module - 0xCCCC0000;
		std::cout << "Manufacturer and Module type: " << std::hex << data_module << std::endl;
		if (DEBUG) std::cout << "Error code: " << CAENVME_DecodeError(error2) << std::endl;

		CVErrorCodes error3 = vme->readVMEdata(this->addr, V418_VER_SERIAL, &data_serial, cvA32_U_DATA, cvD16);
		data_serial = data_serial - 0xCCCC0000;
		std::cout << "Version and serial number: " << std::hex << data_serial << std::endl;
		if (DEBUG) std::cout << "Error code: " << CAENVME_DecodeError(error3) << std::endl;
	}
	else {
		std::cout << "CAENVME object not initialized" << std::endl;
	}
}
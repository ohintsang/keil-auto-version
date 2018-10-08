#include "../inc/Typedef.h" 

#define BUILD_DATE	 "Oct  8 2018" 
///////////////////////////////////////////
#define BUILD_NUMBER	 1   
///////////////////////////////////////////

#define YY	 0x18
#define MM	 0x10
#define DD	 0x08

#define NN	BUILD_NUMBER

const uint8_t MCU_VERSION[4] = {YY, MM, DD, NN}; 

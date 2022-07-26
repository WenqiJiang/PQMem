#pragma once

#define NLIST_MAX 65536 // 256K centroids at max

// #define NLIST 8192
// #define NPROBE 32
// #define QUERY_NUM 10000
// #define SCANNED_ENTRIES_PER_CELL 1000 // every cell contains 1000 vec of PQ codes

#define NBITS 11 // the size of a PQ code, customized number from 4 to 11
#define M 16

#if NBITS == 4
	#define LUT_ENTRY_NUM 16
#elif NBITS == 5
	#define LUT_ENTRY_NUM 32
#elif NBITS == 6
	#define LUT_ENTRY_NUM 64
#elif NBITS == 7
	#define LUT_ENTRY_NUM 128
#elif NBITS == 8
	#define LUT_ENTRY_NUM 256
#elif NBITS == 9
	#define LUT_ENTRY_NUM 512
#elif NBITS == 10
	#define LUT_ENTRY_NUM 1024
#elif NBITS == 11
	#define LUT_ENTRY_NUM 2048
#endif

#define DDR_BANK_NUM 4
// use the 4 * 512 bit interface to store a row, discard the leftover, 
//  e.g., for 9-bit PQ codes and m = 16, ADC_PE_NUM = 14, consumed bits = 14 * 16 * 9 = 2016
#define ADC_PE_NUM (4 * 512 / M / NBITS)

#define LARGE_NUM 99999999 // used to init the heap

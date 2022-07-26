#pragma once

// Variables that can be configured as a parameters
// #define NLIST 8192
// #define NPROBE 32
// #define QUERY_NUM 10000
// #define ITER_NUM_PER_QUERY 10000

// Variables that can be changed & should be set at compile time

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

// Derived & Fixed numbers
#define NLIST_MAX 65536 // 256K centroids at max
#define TOPK 100
#define DDR_BANK_NUM 4
// use the 4 * 512 bit interface to store a row, discard the leftover, 
//  e.g., for 9-bit PQ codes and m = 16, ADC_PE_NUM = 14, consumed bits = 14 * 16 * 9 = 2016
#define ADC_PE_NUM (4 * 512 / M / NBITS)
#define PRIORITY_QUEUE_NUM_L1 (2 * ADC_PE_NUM)
#define PRIORITY_QUEUE_PER_FIRST_THREE_BANK (PRIORITY_QUEUE_NUM_L1 / 4)
#define PRIORITY_QUEUE_LAST_BANK (PRIORITY_QUEUE_NUM_L1 - 3 * PRIORITY_QUEUE_PER_FIRST_THREE_BANK)

#if M == 8 // probablistic approximate priority queue group
    #define PRIORITY_QUEUE_LEN_L1 10
#elif M == 16
    #define PRIORITY_QUEUE_LEN_L1 15
#elif M == 32
    #define PRIORITY_QUEUE_LEN_L1 23
#elif M == 16
    #define PRIORITY_QUEUE_LEN_L1 38
#endif
#define PRIORITY_QUEUE_LEN_L2 TOPK
#define LARGE_NUM 9999999999
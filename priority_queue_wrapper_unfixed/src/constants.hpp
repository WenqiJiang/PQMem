#pragma once
#include <ap_int.h>

// Variables that can be configured as a parameters
#define NLIST 8192
#define NPROBE 32
#define QUERY_NUM 10000
#define ITER_NUM_PER_QUERY 10000

// Variables that can be changed & should be set at compile time
#define M 32
#define LUT_ENTRY_NUM 256

// Derived & Fixed numbers
#define TOPK 100
#define DDR_BANK_NUM 4
#define ADC_PE_PER_CHANNEL (512 / 8 / M) // number of vectors per 512-bit AXI interface
#define ADC_PE_NUM (DDR_BANK_NUM * ADC_PE_PER_CHANNEL)
#define PRIORITY_QUEUE_NUM_L1 (2 * ADC_PE_PER_CHANNEL)
#if M == 8 // probablistic approximate priority queue group
    #define PRIORITY_QUEUE_LEN_L1 8
#elif M == 16
    #define PRIORITY_QUEUE_LEN_L1 11
#elif M == 32
    #define PRIORITY_QUEUE_LEN_L1 17
#elif M == 16
    #define PRIORITY_QUEUE_LEN_L1 26
#endif
#define PRIORITY_QUEUE_LEN_L2 TOPK
#define LARGE_NUM 9999999999
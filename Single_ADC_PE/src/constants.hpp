#pragma once
#include <ap_int.h>

#define NLIST 8192
#define NPROBE 32
#define D 128
#define M 32
#define K 256
#define CENTROID_PARTITIONS 32
#define CENTROIDS_PER_PARTITION (NLIST / CENTROID_PARTITIONS)
#define QUERY_NUM 10000
#define SCANNED_ENTRIES_PER_CELL 193

#define LARGE_NUM 99999999 // used to init the heap

typedef struct {
    // (4 + 16) * 3 * 8 = 480 bit
    int vec_ID_A;
    unsigned char PQ_code_A[M];

    int vec_ID_B;
    unsigned char PQ_code_B[M];

    int vec_ID_C;
    unsigned char PQ_code_C[M];
} PQ_codes;

typedef struct {
    int vec_ID;
    unsigned char PQ_code[M];
} single_PQ;
 
typedef struct {
    // a wrapper for single_PQ
    // used in the ap_uint<480> to 3 x PQ split function
    single_PQ PQ_0;
    single_PQ PQ_1;
    single_PQ PQ_2;
} three_PQ_codes;

typedef struct {
    int vec_ID;
    float dist;
} single_PQ_result; 

typedef struct {
    // each distance LUT has K=256 such row
    // each distance_LUT_PQ16_t is the content of a single row (16 floats)
    float dist[M];
//     float dist_0; 
//     float dist_1; 
//     float dist_2; 
//     float dist_3; 
//     float dist_4; 
//     float dist_5; 
//     float dist_6;
//     float dist_7; 
//     float dist_8; 
//     float dist_9; 
//     float dist_10; 
//     float dist_11; 
//     float dist_12; 
//     float dist_13;
//     float dist_14; 
//     float dist_15;
} distance_LUT_parallel_t;

typedef ap_uint<512> t_axi;


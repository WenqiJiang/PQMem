#pragma once

#define NLIST 8192
#define NPROBE 32
#define D 128
#define M 32
#define K 256
#define VEC_PER_CHANNEL (512 / 8 / M) // number of vectors per 512-bit AXI interface
#define CENTROID_PARTITIONS 32
#define CENTROIDS_PER_PARTITION (NLIST / CENTROID_PARTITIONS)
#define QUERY_NUM 10000
#define SCANNED_ENTRIES_PER_CELL 1000 // every cell contains 1000 vec of PQ codes

#define LARGE_NUM 99999999 // used to init the heap

#include <hls_stream.h>

#include "constants.hpp"

template<const int query_num, const int nprobe, const int scanned_entries_every_cell>
void send_PE_codes(
    hls::stream<single_PQ>& s_single_PQ) {

    single_PQ reg;
    reg.vec_ID = 100;
    for (int i = 0; i < M; i++) {
        reg.PQ_code[i] = 0;
    }

    for (int query_id = 0; query_id < query_num; query_id++) {

        for (int nprobe_id = 0; nprobe_id < nprobe; nprobe_id++) {

            for (int entry_id = 0; entry_id < scanned_entries_every_cell; entry_id++) {
#pragma HLS pipeline II=1
                s_single_PQ.write(reg);
            }
        }
    }
}

template<const int query_num, const int nprobe>
void dummy_last_element_valid_sender(
    hls::stream<int> &s_last_element_valid_PQ_lookup_computation) {

    for (int query_id = 0; query_id < query_num; query_id++) {

        for (int nprobe_id = 0; nprobe_id < nprobe; nprobe_id++) {

            // invalid, padded element
            s_last_element_valid_PQ_lookup_computation.write(0); 
        }
    }
}

template<const int query_num, const int nprobe>
void dummy_scanned_entries_every_cell(
    hls::stream<int> &s_scanned_entries_every_cell) {

    for (int query_id = 0; query_id < query_num; query_id++) {

        for (int nprobe_id = 0; nprobe_id < nprobe; nprobe_id++) {

            s_scanned_entries_every_cell.write(SCANNED_ENTRIES_PER_CELL); 
        }
    }
}

template<const int query_num, const int nprobe>
void dummy_distance_LUT_sender(
    hls::stream<distance_LUT_parallel_t>& s_distance_LUT) {

    distance_LUT_parallel_t dist_row;

    for (int i = 0; i < M; i++) {
#pragma HLS unroll
        dist_row.dist[i] = i;
    }
    // dist_row.dist_0 = 0; 
    // dist_row.dist_1 = 1; 
    // dist_row.dist_2 = 2;
    // dist_row.dist_3 = 3; 
    // dist_row.dist_4 = 4; 
    // dist_row.dist_5 = 5; 
    // dist_row.dist_6 = 6; 
    // dist_row.dist_7 = 7; 
    // dist_row.dist_8 = 8; 
    // dist_row.dist_9 = 9; 
    // dist_row.dist_10 = 10; 
    // dist_row.dist_11 = 11; 
    // dist_row.dist_12 = 12; 
    // dist_row.dist_13 = 13; 
    // dist_row.dist_14 = 14; 
    // dist_row.dist_15 = 15;

    for (int query_id = 0; query_id < query_num; query_id++) {

        for (int nprobe_id = 0; nprobe_id < nprobe; nprobe_id++) {

            for (int row_id = 0; row_id < K; row_id++) {
#pragma HLS pipeline II=1
                s_distance_LUT.write(dist_row);
            }

        }
    }
    
}

template<const int query_num, const int nprobe>
void dummy_distance_LUT_consumer(
    hls::stream<distance_LUT_parallel_t>& s_distance_LUT) {

    distance_LUT_parallel_t dist_row;

    for (int query_id = 0; query_id < query_num; query_id++) {

        for (int nprobe_id = 0; nprobe_id < nprobe; nprobe_id++) {

            for (int row_id = 0; row_id < K; row_id++) {
#pragma HLS pipeline II=1
                dist_row = s_distance_LUT.read();
            }

        }
    }
}

template<const int query_num, const int nprobe, const int scanned_entries_every_cell>
void PQ_lookup_computation(
    // input streams
    hls::stream<distance_LUT_parallel_t>& s_distance_LUT_in,
    hls::stream<single_PQ>& s_single_PQ,
    hls::stream<int>& s_scanned_entries_every_cell_PQ_lookup_computation,
    hls::stream<int>& s_last_element_valid_PQ_lookup_computation, 
    // output streams
    hls::stream<distance_LUT_parallel_t>& s_distance_LUT_out,
    hls::stream<single_PQ_result>& s_single_PQ_result) {

    float distance_LUT[M][256];
#pragma HLS array_partition variable=distance_LUT dim=1
#pragma HLS resource variable=distance_LUT core=RAM_1P_BRAM

    QUERY_LOOP: 
    for (int query_id = 0; query_id < query_num; query_id++) {

        NPROBE_LOOP:
        for (int nprobe_id = 0; nprobe_id < nprobe; nprobe_id++) {

            // Wenqi: for HLS performance report, dont read this
            int tmp_scanned_entries_every_cell = 
                s_scanned_entries_every_cell_PQ_lookup_computation.read();
            int last_element_valid = 
                s_last_element_valid_PQ_lookup_computation.read();

            CP_LUT_LOOP:
            // Stage A: init distance LUT
            for (int row_id = 0; row_id < K; row_id++) {
#pragma HLS pipeline II=1

                // without duplication, HLS cannot achieve II=1
                distance_LUT_parallel_t dist_row = s_distance_LUT_in.read();
                s_distance_LUT_out.write(dist_row);
                
                // col 0 ~ 7
                distance_LUT[0][row_id] = dist_row.dist[0]; 
                distance_LUT[1][row_id] = dist_row.dist[1]; 
                distance_LUT[2][row_id] = dist_row.dist[2];
                distance_LUT[3][row_id] = dist_row.dist[3]; 
                distance_LUT[4][row_id] = dist_row.dist[4]; 
                distance_LUT[5][row_id] = dist_row.dist[5]; 
                distance_LUT[6][row_id] = dist_row.dist[6]; 
                distance_LUT[7][row_id] = dist_row.dist[7]; 
                distance_LUT[8][row_id] = dist_row.dist[8]; 
                distance_LUT[9][row_id] = dist_row.dist[9]; 
                distance_LUT[10][row_id] = dist_row.dist[10]; 
                distance_LUT[11][row_id] = dist_row.dist[11]; 
                distance_LUT[12][row_id] = dist_row.dist[12]; 
                distance_LUT[13][row_id] = dist_row.dist[13]; 
                distance_LUT[14][row_id] = dist_row.dist[14]; 
                distance_LUT[15][row_id] = dist_row.dist[15];
                // distance_LUT[0][row_id] = dist_row.dist_0; 
                // distance_LUT[1][row_id] = dist_row.dist_1; 
                // distance_LUT[2][row_id] = dist_row.dist_2;
                // distance_LUT[3][row_id] = dist_row.dist_3; 
                // distance_LUT[4][row_id] = dist_row.dist_4; 
                // distance_LUT[5][row_id] = dist_row.dist_5; 
                // distance_LUT[6][row_id] = dist_row.dist_6; 
                // distance_LUT[7][row_id] = dist_row.dist_7; 
                // distance_LUT[8][row_id] = dist_row.dist_8; 
                // distance_LUT[9][row_id] = dist_row.dist_9; 
                // distance_LUT[10][row_id] = dist_row.dist_10; 
                // distance_LUT[11][row_id] = dist_row.dist_11; 
                // distance_LUT[12][row_id] = dist_row.dist_12; 
                // distance_LUT[13][row_id] = dist_row.dist_13; 
                // distance_LUT[14][row_id] = dist_row.dist_14; 
                // distance_LUT[15][row_id] = dist_row.dist_15;
            }

            ADC_LOOP:
            // Stage B: compute estimated distance
            for (int entry_id = 0; entry_id < scanned_entries_every_cell; entry_id++) {
#pragma HLS pipeline II=1

                single_PQ PQ_local = s_single_PQ.read();

                single_PQ_result out; 
                out.vec_ID = PQ_local.vec_ID;
                
                for (int b = 0; b < M; b++) {
#pragma HLS unroll
#pragma HLS
                    out.dist += distance_LUT[b][PQ_local.PQ_code[b]];
                }
                // out.dist = 
                //     distance_LUT[0][PQ_local.PQ_code[0]] + 
                //     distance_LUT[1][PQ_local.PQ_code[1]] + 
                //     distance_LUT[2][PQ_local.PQ_code[2]] + 
                //     distance_LUT[3][PQ_local.PQ_code[3]] + 
                //     distance_LUT[4][PQ_local.PQ_code[4]] + 
                //     distance_LUT[5][PQ_local.PQ_code[5]] + 
                //     distance_LUT[6][PQ_local.PQ_code[6]] + 
                //     distance_LUT[7][PQ_local.PQ_code[7]] + 
                //     distance_LUT[8][PQ_local.PQ_code[8]] + 
                //     distance_LUT[9][PQ_local.PQ_code[9]] + 
                //     distance_LUT[10][PQ_local.PQ_code[10]] + 
                //     distance_LUT[11][PQ_local.PQ_code[11]] + 
                //     distance_LUT[12][PQ_local.PQ_code[12]] + 
                //     distance_LUT[13][PQ_local.PQ_code[13]] + 
                //     distance_LUT[14][PQ_local.PQ_code[14]] + 
                //     distance_LUT[15][PQ_local.PQ_code[15]];

                // for padded element, replace its distance by large number
                if ((entry_id == (scanned_entries_every_cell - 1)) && (last_element_valid == 0)) {
                    out.vec_ID = -1;
                    out.dist = LARGE_NUM;
                }
                s_single_PQ_result.write(out);
            }
        }
    }
}

template<const int query_num, const int nprobe, const int scanned_entries_every_cell>
void write_result(
    hls::stream<single_PQ_result>& s_result, ap_uint<64>* results_out) {
    
    single_PQ_result reg;

    for (int query_id = 0; query_id < query_num; query_id++) {

        for (int nprobe_id = 0; nprobe_id < nprobe; nprobe_id++) {

            for (int entry_id = 0; entry_id < scanned_entries_every_cell; entry_id++) {
#pragma HLS pipeline II=1
                reg = s_result.read();
            }
        }
    }

    // only write the last value to out
    int vec_ID = reg.vec_ID;
    float dist = reg.dist;
    ap_uint<32> vec_ID_ap = *((ap_uint<32>*) (&vec_ID));
    ap_uint<32> dist_ap = *((ap_uint<32>*) (&dist));
    results_out[0].range(31, 0) = vec_ID_ap;
    results_out[0].range(63, 32) = dist_ap;
}


extern "C" {


void vadd(  
    const ap_uint<512>* in, 
    ap_uint<64>* out
    )
{
#pragma HLS INTERFACE m_axi port=in offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=out offset=slave bundle=gmem1

#pragma HLS dataflow

    hls::stream<single_PQ> s_PQ_codes;
#pragma HLS stream variable=s_PQ_codes depth=8
#pragma HLS resource variable=s_PQ_codes core=FIFO_SRL

    hls::stream<single_PQ_result> s_single_PQ_result;
#pragma HLS stream variable=s_single_PQ_result depth=8
#pragma HLS resource variable=s_single_PQ_result core=FIFO_SRL

    hls::stream<int> s_scanned_entries_every_cell;
#pragma HLS stream variable=s_scanned_entries_every_cell depth=8
#pragma HLS resource variable=s_scanned_entries_every_cell core=FIFO_SRL

    hls::stream<int> s_last_element_valid_PQ_lookup_computation;
#pragma HLS stream variable=s_last_element_valid_PQ_lookup_computation depth=8
#pragma HLS resource variable=s_last_element_valid_PQ_lookup_computation core=FIFO_SRL

    hls::stream<distance_LUT_parallel_t> s_distance_LUT_in;
#pragma HLS stream variable=s_distance_LUT_in depth=8
#pragma HLS resource variable=s_distance_LUT_in core=FIFO_SRL

    hls::stream<distance_LUT_parallel_t> s_distance_LUT_out;
#pragma HLS stream variable=s_distance_LUT_out depth=8
#pragma HLS resource variable=s_distance_LUT_out core=FIFO_SRL

    send_PE_codes<QUERY_NUM, NPROBE, SCANNED_ENTRIES_PER_CELL>(
        s_PQ_codes);

    dummy_scanned_entries_every_cell<QUERY_NUM, NPROBE>(
        s_scanned_entries_every_cell);

    dummy_last_element_valid_sender<QUERY_NUM,  NPROBE>(
        s_last_element_valid_PQ_lookup_computation);

    dummy_distance_LUT_sender<QUERY_NUM,  NPROBE>(
        s_distance_LUT_in);

////////////////////    Core Function Starts     //////////////////// 

    PQ_lookup_computation<QUERY_NUM, NPROBE, SCANNED_ENTRIES_PER_CELL>(
        // input streams
        s_distance_LUT_in,
        s_PQ_codes, 
        s_scanned_entries_every_cell,
        s_last_element_valid_PQ_lookup_computation,
        // output streams
        s_distance_LUT_out,
        s_single_PQ_result);

////////////////////    Core Function Ends     //////////////////// 

    dummy_distance_LUT_consumer<QUERY_NUM,  NPROBE>(
        s_distance_LUT_out);

    write_result<QUERY_NUM, NPROBE, SCANNED_ENTRIES_PER_CELL>(
        s_single_PQ_result, 
        out);
}

}
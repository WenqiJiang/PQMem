#pragma once

#include "constants.hpp"
#include "hierarchical_priority_queue.hpp"


template<const int stream_num>
void replicate_scanned_entries_per_query_Redirected_sorted_stream(
    const int query_num, 
    hls::stream<int> &s_scanned_entries_per_query_Priority_queue, 
    hls::stream<int> (&s_insertion_per_queue_L1)[stream_num]) {
    
    for (int i = 0; i < query_num; i++) {

        int scanned_entries_per_query = s_scanned_entries_per_query_Priority_queue.read();
        
        for (int s = 0; s < stream_num; s++) {
#pragma HLS UNROLL
            s_insertion_per_queue_L1[s].write(scanned_entries_per_query);
        }
    }
}



void split_single_stream(
    const int query_num, 
    hls::stream<PQ_out_t> &input_stream,
    hls::stream<int> &s_scanned_entries_every_cell,
    hls::stream<int> &s_scanned_entries_every_cell_Out_priority_queue_A, 
    hls::stream<int> &s_scanned_entries_every_cell_Out_priority_queue_B, 
    hls::stream<PQ_out_t> &output_stream_A,
    hls::stream<PQ_out_t> &output_stream_B) {

    for (int query_id = 0; query_id < query_num; query_id++) {

        int scanned_entries_every_cell = s_scanned_entries_every_cell.read();
        int half_scanned_entries_every_cell = scanned_entries_every_cell / 2;

        if ((scanned_entries_every_cell - 2 * half_scanned_entries_every_cell) == 1) {
            s_scanned_entries_every_cell_Out_priority_queue_A.write(half_scanned_entries_every_cell + 1);
            output_stream_A.write(input_stream.read());
        }
        else {
            s_scanned_entries_every_cell_Out_priority_queue_A.write(half_scanned_entries_every_cell); 
        }
        s_scanned_entries_every_cell_Out_priority_queue_B.write(half_scanned_entries_every_cell);

        for (int entry_id = 0; entry_id < half_scanned_entries_every_cell; entry_id++) {
#pragma HLS pipeline II=2
            output_stream_A.write(input_stream.read());
            output_stream_B.write(input_stream.read());
        }
    }
}

template<const int stream_num>
void split_single_PQ_result_wrapper(
    const int query_num, 
    hls::stream<PQ_out_t> (&s_input)[stream_num], 
    hls::stream<int> &s_scanned_entries_per_query_In_Priority_queue,
    hls::stream<int> (&s_scanned_entries_every_cell_Out_priority_queue)[2 * stream_num],
    hls::stream<PQ_out_t> (&s_input_splitted)[2 * stream_num]) {
    
#pragma HLS inline
    // for the top 16 elements, discard the last 6 
    // for the rest 10 elements, split them to 2 streams, since Priority Queue's
    //   insertion takes 2 CC

    hls::stream<int> s_scanned_entries_every_cell_Replicated[stream_num];
#pragma HLS stream variable=s_scanned_entries_every_cell_Replicated depth=8
#pragma HLS array_partition variable=s_scanned_entries_every_cell_Replicated complete
// #pragma HLS RESOURCE variable=s_scanned_entries_every_cell_Replicated core=FIFO_SRL

    replicate_scanned_entries_per_query_Redirected_sorted_stream<stream_num>(
        query_num,
        s_scanned_entries_per_query_In_Priority_queue, 
        s_scanned_entries_every_cell_Replicated);

    for (int i = 0; i < stream_num; i++) {
#pragma HLS UNROLL
        split_single_stream(
            query_num, 
            s_input[i], 
            s_scanned_entries_every_cell_Replicated[i],
            s_scanned_entries_every_cell_Out_priority_queue[2 * i],
            s_scanned_entries_every_cell_Out_priority_queue[2 * i + 1],
            s_input_splitted[2 * i], 
            s_input_splitted[2 * i + 1]);
    }
}

template<const int iter_num_per_query>
void send_iter_num(
    const int query_num,
    hls::stream<int> &s_insertion_per_queue) {

    for (int query_id = 0; query_id < query_num; query_id++) {
        s_insertion_per_queue.write(iter_num_per_query);
    }
}


template<const int priority_queue_len, const int stream_num>
void merge_streams(
    const int query_num, 
    hls::stream<PQ_out_t> (&intermediate_result)[stream_num],
    hls::stream<PQ_out_t> &output_stream) {
    
    for (int query_id = 0; query_id < query_num; query_id++) {
        for (int d = 0; d < priority_queue_len; d++) {
            for (int s = 0; s < stream_num; s++) {
#pragma HLS pipeline II=1
                output_stream.write(intermediate_result[s].read());
            }
        }
    }
}

void stage6_priority_queue_group_L2_wrapper( 
    const int query_num, 
    hls::stream<int> &s_scanned_entries_per_query_Priority_queue,
    hls::stream<PQ_out_t> (&s_input)[ADC_PE_NUM], 
    ap_uint<64>* vec_ID_DDR_0,
    ap_uint<64>* vec_ID_DDR_1,
    ap_uint<64>* vec_ID_DDR_2,
    ap_uint<64>* vec_ID_DDR_3,
    hls::stream<result_t> &output_stream) {
/*
    Hardcode number of memory channels as 4

    Input: 
        the number of outputs per compute PE (s_scanned_entries_per_query_Priority_queue)
        the compute PE's result (s_input)

    Output:
        the topK results of (vec_ID, dist) pairs
    
    Process: 
        per memory channel, gather the compute PE's result by L1 priority queues,
            then lookup the vector ID from memory channels.
        L2 priory queue consumes the per channel results
*/

#pragma HLS inline

    hls::stream<int> s_insertion_per_queue_L1[PRIORITY_QUEUE_NUM_L1];
#pragma HLS stream variable=s_insertion_per_queue_L1 depth=8
#pragma HLS array_partition variable=s_insertion_per_queue_L1 complete
// #pragma HLS RESOURCE variable=s_insertion_per_queue_L1 core=FIFO_SRL

    hls::stream<PQ_out_t> s_input_splitted[PRIORITY_QUEUE_NUM_L1];
#pragma HLS stream variable=s_input_splitted depth=8
#pragma HLS array_partition variable=s_input_splitted complete
// #pragma HLS RESOURCE variable=s_input_splitted core=FIFO_SRL

    hls::stream<PQ_out_t> s_intermediate_result_with_offset[PRIORITY_QUEUE_NUM_L1];
#pragma HLS stream variable=s_intermediate_result_with_offset depth=8
#pragma HLS array_partition variable=s_intermediate_result_with_offset complete
// #pragma HLS RESOURCE variable=s_intermediate_result_with_offset core=FIFO_SRL

    hls::stream<PQ_out_t> s_intermediate_result_with_offset_per_channel[DDR_BANK_NUM];
#pragma HLS stream variable=s_intermediate_result_with_offset depth=8
#pragma HLS array_partition variable=s_intermediate_result_with_offset complete
// #pragma HLS RESOURCE variable=s_intermediate_result_with_offset core=FIFO_SRL

    hls::stream<result_t> s_intermediate_result_with_vec_ID[DDR_BANK_NUM];
#pragma HLS stream variable=s_intermediate_result_with_vec_ID depth=8
#pragma HLS array_partition variable=s_intermediate_result_with_vec_ID complete
// #pragma HLS RESOURCE variable=s_intermediate_result_with_vec_ID core=FIFO_SRL

    // collecting results from multiple sources need deeper FIFO
    const int intermediate_result_depth = PRIORITY_QUEUE_NUM_L1 * PRIORITY_QUEUE_LEN_L1;
    hls::stream<result_t> s_merged_intermediate_result;
#pragma HLS stream variable=s_merged_intermediate_result depth=intermediate_result_depth

    // WENQI: Here, the number of priority queue must be a constant passed by macro,
    //   I have tried to use the template argument, i.e., PRIORITY_QUEUE_NUM_L1, but HLS has bug on it
    Priority_queue_L1<PQ_out_t, PRIORITY_QUEUE_LEN_L1, Collect_smallest> priority_queue_L1[PRIORITY_QUEUE_NUM_L1];
#pragma HLS array_partition variable=priority_queue_L1 complete

    ////////////////////         Priority Queue Level 1          ////////////////////
    split_single_PQ_result_wrapper<ADC_PE_NUM>(
        query_num, 
        s_input, 
        s_scanned_entries_per_query_Priority_queue,
        s_insertion_per_queue_L1,
        s_input_splitted); 

    // 2 CC per insertion
    for (int i = 0; i < PRIORITY_QUEUE_NUM_L1; i++) {
#pragma HLS UNROLL
        // for each individual query, output intermediate_result
        priority_queue_L1[i].insert_wrapper(
            query_num, 
            s_insertion_per_queue_L1[i], 
            s_input_splitted[i], 
            s_intermediate_result_with_offset[i]);
    }

    // TODO: merge per channel results
    for (int c = 0; c < DDR_BANK_NUM; c++) {
#pragma HLS UNROLL
        merge_results_per_channel();
    merge_streams<TOPK, PRIORITY_QUEUE_NUM_L1>(
        query_num, intermediate_result, s_merged_intermediate_result);
    }
    // TODO: lookup vecIDs
    lookup_vec_ID(vec_ID_DDR_0, );
    lookup_vec_ID(vec_ID_DDR_1, );
    lookup_vec_ID(vec_ID_DDR_2, );
    lookup_vec_ID(vec_ID_DDR_3, );


    merge_streams<TOPK, PRIORITY_QUEUE_NUM_L1>(
        query_num, intermediate_result, s_merged_intermediate_result);

    ////////////////////         Priority Queue Level 2          ////////////////////

    hls::stream<int> s_insertion_per_queue_L2;
#pragma HLS stream variable=s_insertion_per_queue_L2 depth=8
// #pragma HLS RESOURCE variable=s_insertion_per_queue_L2 core=FIFO_SRL

    Priority_queue_L2<result_t, TOPK, Collect_smallest> priority_queue_final;

    send_iter_num<PRIORITY_QUEUE_NUM_L1 * PRIORITY_QUEUE_LEN_L1>(
        query_num, 
        s_insertion_per_queue_L2);

    priority_queue_final.insert_wrapper(
        query_num,
        s_insertion_per_queue_L2,
        s_merged_intermediate_result, 
        output_stream); 
}
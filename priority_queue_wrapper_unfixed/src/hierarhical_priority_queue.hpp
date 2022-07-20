#pragma once

#include "constants.hpp"
#include "types.hpp"
#include "priority_queue_distance_results_L1.hpp"
#include "priority_queue_distance_results_L2.hpp"

template<const int stream_num>
void replicate_scanned_entries_per_query_Redirected_sorted_stream;

void split_single_stream(
    const int query_num, 
    hls::stream<PQ_out_t> &input_stream,
    hls::stream<int> &s_scanned_entries_every_cell,
    hls::stream<int> &s_scanned_entries_every_cell_Out_priority_queue_A, 
    hls::stream<int> &s_scanned_entries_every_cell_Out_priority_queue_B, 
    hls::stream<PQ_out_t> &output_stream_A,
    hls::stream<PQ_out_t> &output_stream_B);

template<const int stream_num>
void split_single_PQ_result_wrapper(
    const int query_num, 
    hls::stream<PQ_out_t> (&s_input)[stream_num], 
    hls::stream<int> &s_scanned_entries_per_query_In_Priority_queue,
    hls::stream<int> (&s_scanned_entries_every_cell_Out_priority_queue)[2 * stream_num],
    hls::stream<PQ_out_t> (&s_input_splitted)[2 * stream_num]);

template<const int iter_num_per_query>
void send_iter_num(
    const int query_num,
    hls::stream<int> &s_insertion_per_queue);


template<const int priority_queue_len, const int stream_num>
void merge_streams(
    const int query_num, 
    hls::stream<PQ_out_t> (&intermediate_result)[stream_num],
    hls::stream<PQ_out_t> &output_stream);

template<const int priority_queue_len, const int stream_num>
void convert_vec_ID_offset_to_addr(
    const int query_num, 
    hls::stream<int> &s_nlist_vec_ID_start_addr,
    hls::stream<PQ_out_t> (&s_intermediate_result_with_offset)[stream_num],
    hls::stream<PQ_lookup_t> (&s_intermediate_result_with_addr)[stream_num]);

template<const int priority_queue_len, const int stream_num>
void convert_addr_to_vec_ID(
    const int query_num, 
    ap_uint<64>* vec_ID_DDR_0,
    ap_uint<64>* vec_ID_DDR_1,
    ap_uint<64>* vec_ID_DDR_2,
    ap_uint<64>* vec_ID_DDR_3,
    hls::stream<PQ_lookup_t> (&s_intermediate_result_with_addr)[stream_num],
    hls::stream<result_t> (&s_intermediate_result_with_vec_ID)[stream_num]);

void hierarchical_priority_queue( 
    const int query_num, 
    hls::stream<int> &s_nlist_vec_ID_start_addr,
    hls::stream<int> &s_scanned_entries_per_query_Priority_queue,
    hls::stream<PQ_out_t> (&s_input)[ADC_PE_NUM], 
    ap_uint<64>* vec_ID_DDR_0,
    ap_uint<64>* vec_ID_DDR_1,
    ap_uint<64>* vec_ID_DDR_2,
    ap_uint<64>* vec_ID_DDR_3,
    hls::stream<result_t> &output_stream);
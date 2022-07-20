#pragma once

#include "constants.hpp"
#include "types.hpp"
#include "priority_queue_distance_results_L1.hpp"
#include "priority_queue_distance_results_L2.hpp"

template<const int query_num>
void replicate_scanned_entries_per_query_Redirected_sorted_stream(
        hls::stream<int> &s_scanned_entries_per_query_Priority_queue, 
        hls::stream<int> (&s_scanned_entries_every_cell_Redirected_sorted_stream)[16]);

template<const int query_num>
void consume_single_stream(
    hls::stream<single_PQ_result> &input_stream,
    hls::stream<int> &s_scanned_entries_every_cell);

template<const int query_num>
void split_single_stream(
    hls::stream<single_PQ_result> &input_stream,
    hls::stream<int> &s_scanned_entries_every_cell,
    hls::stream<int> &s_scanned_entries_every_cell_Out_priority_queue_A, 
    hls::stream<int> &s_scanned_entries_every_cell_Out_priority_queue_B, 
    hls::stream<single_PQ_result> &output_stream_A,
    hls::stream<single_PQ_result> &output_stream_B);

template<const int query_num>
void consume_and_redirect_sorted_streams(
    hls::stream<single_PQ_result> (&sorted_stream)[16], 
    hls::stream<int> &s_scanned_entries_per_query_In_Priority_queue,
    hls::stream<int> (&s_scanned_entries_every_cell_Out_priority_queue)[20],
    hls::stream<single_PQ_result> (&redirected_sorted_stream)[20]);

template<const int query_num, const int iter_num_per_query>
void send_iter_num(hls::stream<int> &s_merged_intermediate_result_iter);

template<const int query_num, const int priority_queue_len>
void merge_streams(
    hls::stream<single_PQ_result> (&intermediate_result)[20],
    hls::stream<single_PQ_result> &output_stream);

template<const int query_num>
void stream_redirect_to_priority_queue_wrapper( 
    hls::stream<int> &s_scanned_entries_per_query_Priority_queue,
    hls::stream<single_PQ_result> (&sorted_stream)[16], 
    hls::stream<single_PQ_result> &output_stream);
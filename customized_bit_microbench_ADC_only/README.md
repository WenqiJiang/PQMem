# customized_bit_microbench_ADC_only

The first half of customized_bit_microbench, no priority queues. 

## Performance 

As expected. 

Scan 1M vector of m=16 and nbits = 11

size_t query_num = 1000;

Freq = 160 MHz

num_ADC_PE = 512 * 4 / 16 / 11 = 11 PEs -> 11 vec per CC

expected time = 1000 * (1e6 / 11) / (160 * 1e6) = 0.568 sec

Real time = 581.78 sec

Bandwidth = 1e3 * 1e6 * 16 * 11 / 8 / 0.58178 / 1e9 = 37.81 GB/s
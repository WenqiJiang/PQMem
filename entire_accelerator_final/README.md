# entire_accelerator_final

This is the functionality test of entire accelerator (send query & center vectors from DRAM channels, thus performance is suboptimal). 

This project simulates the situation that query_id, cell_IDs, and cell centroids vectors are streamed in from network, and the results are streamed out to network.

Compare to entire_accelerator_with_LUT_network_simulation, this project allow user to configure double buffering for ADC computation, and fully sort the topK results before returning them.

## Performance

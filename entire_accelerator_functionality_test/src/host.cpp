#include "host.hpp"


#include <algorithm>
#include <unistd.h>
#include <limits>
#include <assert.h>

#include "constants.hpp"
// Wenqi: seems 2022.1 somehow does not support linking ap_uint.h to host?
// #include "ap_uint.h"

char* read_binary_file(const std::string &xclbin_file_name, unsigned &nb) 
{
    std::cout << "INFO: Reading " << xclbin_file_name << std::endl;

	if(access(xclbin_file_name.c_str(), R_OK) != 0) {
		printf("ERROR: %s xclbin not available please build\n", xclbin_file_name.c_str());
		exit(EXIT_FAILURE);
	}
    //Loading XCL Bin into char buffer 
    std::cout << "Loading: '" << xclbin_file_name.c_str() << "'\n";
    std::ifstream bin_file(xclbin_file_name.c_str(), std::ifstream::binary);
    bin_file.seekg (0, bin_file.end);
    nb = bin_file.tellg();
    bin_file.seekg (0, bin_file.beg);
    char *buf = new char [nb];
    bin_file.read(buf, nb);
    return buf;
}

// boost::filesystem does not compile well, so implement this myself
std::string dir_concat(std::string dir1, std::string dir2) {
    if (dir1.back() != '/') {
        dir1 += '/';
    }
    return dir1 + dir2;
}

int main(int argc, char** argv)
{
    cl_int err;
    // Allocate Memory in Host Memory
    // When creating a buffer with user pointer (CL_MEM_USE_HOST_PTR), under the hood user ptr 
    // is used if it is properly aligned. when not aligned, runtime had no choice but to create
    // its own host side buffer. So it is recommended to use this allocator if user wish to
    // create buffer using CL_MEM_USE_HOST_PTR to align user buffer to page boundary. It will 
    // ensure that user buffer is used when user create Buffer/Mem object with CL_MEM_USE_HOST_PTR 


    std::string data_dir_prefix = "/mnt/scratch/wenqi/Faiss_Enzian_U250_index/SIFT100M_IVF8192,PQ16";

    ///////////     get data size from disk     //////////

    // PQ codes
    std::string PQ_codes_DRAM_0_dir_suffix("DDR_bank_0_PQ_raw");
    std::string PQ_codes_DRAM_0_dir = dir_concat(data_dir_prefix, PQ_codes_DRAM_0_dir_suffix);
    std::ifstream PQ_codes_DRAM_0_fstream(
        PQ_codes_DRAM_0_dir, 
        std::ios::in | std::ios::binary);
    PQ_codes_DRAM_0_fstream.seekg(0, PQ_codes_DRAM_0_fstream.end);
    size_t PQ_codes_DRAM_0_size =  PQ_codes_DRAM_0_fstream.tellg();
    if (!PQ_codes_DRAM_0_size) std::cout << "PQ_codes_DRAM_0_size is 0!";
    PQ_codes_DRAM_0_fstream.seekg(0, PQ_codes_DRAM_0_fstream.beg);

    std::string PQ_codes_DRAM_1_dir_suffix("DDR_bank_1_PQ_raw");
    std::string PQ_codes_DRAM_1_dir = dir_concat(data_dir_prefix, PQ_codes_DRAM_1_dir_suffix);
    std::ifstream PQ_codes_DRAM_1_fstream(
        PQ_codes_DRAM_1_dir, 
        std::ios::in | std::ios::binary);
    PQ_codes_DRAM_1_fstream.seekg(0, PQ_codes_DRAM_1_fstream.end);
    size_t PQ_codes_DRAM_1_size =  PQ_codes_DRAM_1_fstream.tellg();
    if (!PQ_codes_DRAM_1_size) std::cout << "PQ_codes_DRAM_1_size is 0!";
    PQ_codes_DRAM_1_fstream.seekg(0, PQ_codes_DRAM_1_fstream.beg);
    
    std::string PQ_codes_DRAM_2_dir_suffix("DDR_bank_2_PQ_raw");
    std::string PQ_codes_DRAM_2_dir = dir_concat(data_dir_prefix, PQ_codes_DRAM_2_dir_suffix);
    std::ifstream PQ_codes_DRAM_2_fstream(
        PQ_codes_DRAM_2_dir, 
        std::ios::in | std::ios::binary);
    PQ_codes_DRAM_2_fstream.seekg(0, PQ_codes_DRAM_2_fstream.end);
    size_t PQ_codes_DRAM_2_size =  PQ_codes_DRAM_2_fstream.tellg();
    if (!PQ_codes_DRAM_2_size) std::cout << "PQ_codes_DRAM_2_size is 0!";
    PQ_codes_DRAM_2_fstream.seekg(0, PQ_codes_DRAM_2_fstream.beg);
    
    std::string PQ_codes_DRAM_3_dir_suffix("DDR_bank_3_PQ_raw");
    std::string PQ_codes_DRAM_3_dir = dir_concat(data_dir_prefix, PQ_codes_DRAM_3_dir_suffix);
    std::ifstream PQ_codes_DRAM_3_fstream(
        PQ_codes_DRAM_3_dir, 
        std::ios::in | std::ios::binary);
    PQ_codes_DRAM_3_fstream.seekg(0, PQ_codes_DRAM_3_fstream.end);
    size_t PQ_codes_DRAM_3_size =  PQ_codes_DRAM_3_fstream.tellg();
    if (!PQ_codes_DRAM_3_size) std::cout << "PQ_codes_DRAM_3_size is 0!";
    PQ_codes_DRAM_3_fstream.seekg(0, PQ_codes_DRAM_3_fstream.beg);

    // vec IDs
    std::string vec_ID_DRAM_0_dir_suffix("DDR_bank_0_vec_ID_raw");
    std::string vec_ID_DRAM_0_dir = dir_concat(data_dir_prefix, vec_ID_DRAM_0_dir_suffix);
    std::ifstream vec_ID_DRAM_0_fstream(
        vec_ID_DRAM_0_dir, 
        std::ios::in | std::ios::binary);
    vec_ID_DRAM_0_fstream.seekg(0, vec_ID_DRAM_0_fstream.end);
    size_t vec_ID_DRAM_0_size =  vec_ID_DRAM_0_fstream.tellg();
    if (!vec_ID_DRAM_0_size) std::cout << "vec_ID_DRAM_0_size is 0!";
    vec_ID_DRAM_0_fstream.seekg(0, vec_ID_DRAM_0_fstream.beg);

    std::string vec_ID_DRAM_1_dir_suffix("DDR_bank_1_vec_ID_raw");
    std::string vec_ID_DRAM_1_dir = dir_concat(data_dir_prefix, vec_ID_DRAM_1_dir_suffix);
    std::ifstream vec_ID_DRAM_1_fstream(
        vec_ID_DRAM_1_dir, 
        std::ios::in | std::ios::binary);
    vec_ID_DRAM_1_fstream.seekg(0, vec_ID_DRAM_1_fstream.end);
    size_t vec_ID_DRAM_1_size =  vec_ID_DRAM_1_fstream.tellg();
    if (!vec_ID_DRAM_1_size) std::cout << "vec_ID_DRAM_1_size is 0!";
    vec_ID_DRAM_1_fstream.seekg(0, vec_ID_DRAM_1_fstream.beg);

    std::string vec_ID_DRAM_2_dir_suffix("DDR_bank_2_vec_ID_raw");
    std::string vec_ID_DRAM_2_dir = dir_concat(data_dir_prefix, vec_ID_DRAM_2_dir_suffix);
    std::ifstream vec_ID_DRAM_2_fstream(
        vec_ID_DRAM_2_dir, 
        std::ios::in | std::ios::binary);
    vec_ID_DRAM_2_fstream.seekg(0, vec_ID_DRAM_2_fstream.end);
    size_t vec_ID_DRAM_2_size =  vec_ID_DRAM_2_fstream.tellg();
    if (!vec_ID_DRAM_2_size) std::cout << "vec_ID_DRAM_2_size is 0!";
    vec_ID_DRAM_2_fstream.seekg(0, vec_ID_DRAM_2_fstream.beg);

    std::string vec_ID_DRAM_3_dir_suffix("DDR_bank_3_vec_ID_raw");
    std::string vec_ID_DRAM_3_dir = dir_concat(data_dir_prefix, vec_ID_DRAM_3_dir_suffix);
    std::ifstream vec_ID_DRAM_3_fstream(
        vec_ID_DRAM_3_dir, 
        std::ios::in | std::ios::binary);
    vec_ID_DRAM_3_fstream.seekg(0, vec_ID_DRAM_3_fstream.end);
    size_t vec_ID_DRAM_3_size =  vec_ID_DRAM_3_fstream.tellg();
    if (!vec_ID_DRAM_3_size) std::cout << "vec_ID_DRAM_3_size is 0!";
    vec_ID_DRAM_3_fstream.seekg(0, vec_ID_DRAM_3_fstream.beg);

    // control signals
    std::string nlist_PQ_codes_start_addr_dir_suffix("nlist_PQ_codes_start_addr");
    std::string nlist_PQ_codes_start_addr_dir = dir_concat(data_dir_prefix, nlist_PQ_codes_start_addr_dir_suffix);
    std::ifstream nlist_PQ_codes_start_addr_fstream(
        nlist_PQ_codes_start_addr_dir, 
        std::ios::in | std::ios::binary);
    nlist_PQ_codes_start_addr_fstream.seekg(0, nlist_PQ_codes_start_addr_fstream.end);
    size_t nlist_PQ_codes_start_addr_size =  nlist_PQ_codes_start_addr_fstream.tellg();
    if (!nlist_PQ_codes_start_addr_size) std::cout << "nlist_PQ_codes_start_addr_size is 0!";
    nlist_PQ_codes_start_addr_fstream.seekg(0, nlist_PQ_codes_start_addr_fstream.beg);

    std::string nlist_vec_ID_start_addr_dir_suffix("nlist_vec_ID_start_addr");
    std::string nlist_vec_ID_start_addr_dir = dir_concat(data_dir_prefix, nlist_vec_ID_start_addr_dir_suffix);
    std::ifstream nlist_vec_ID_start_addr_fstream(
        nlist_vec_ID_start_addr_dir, 
        std::ios::in | std::ios::binary);
    nlist_vec_ID_start_addr_fstream.seekg(0, nlist_vec_ID_start_addr_fstream.end);
    size_t nlist_vec_ID_start_addr_size =  nlist_vec_ID_start_addr_fstream.tellg();
    if (!nlist_vec_ID_start_addr_size) std::cout << "nlist_vec_ID_start_addr_size is 0!";
    nlist_vec_ID_start_addr_fstream.seekg(0, nlist_vec_ID_start_addr_fstream.beg);

    std::string nlist_num_vecs_dir_suffix("nlist_num_vecs");
    std::string nlist_num_vecs_dir = dir_concat(data_dir_prefix, nlist_num_vecs_dir_suffix);
    std::ifstream nlist_num_vecs_fstream(
        nlist_num_vecs_dir, 
        std::ios::in | std::ios::binary);
    nlist_num_vecs_fstream.seekg(0, nlist_num_vecs_fstream.end);
    size_t nlist_num_vecs_size =  nlist_num_vecs_fstream.tellg();
    if (!nlist_num_vecs_size) std::cout << "nlist_num_vecs_size is 0!";
    nlist_num_vecs_fstream.seekg(0, nlist_num_vecs_fstream.beg);

    // info used to compute distance LUT

// -rw-r--r-- 1 wejiang systems-all   5120000 Aug  1 16:26 query_vectors_float32_10000_128_raw
// -rw-r--r-- 1 wejiang systems-all    131072 Aug  1 16:26 product_quantizer_float32_16_256_8_raw
// -rw-r--r-- 1 wejiang systems-all   4194304 Aug  1 16:26 vector_quantizer_float32_8192_128_raw

    std::cout << "Allocating memory...\n";

    // in init
    size_t query_num = 1000;
    size_t nlist = 8192;
    size_t nprobe = 32;
    size_t entries_per_cell = 1000;
    size_t nlist_init_bytes = 3 * nlist * sizeof(int);

    assert(nlist * 4 ==  nlist_PQ_codes_start_addr_size);
    assert(nlist * 4 ==  nlist_vec_ID_start_addr_size);
    assert(nlist * 4 ==  nlist_num_vecs_size);

    std::vector<int ,aligned_allocator<int >> nlist_init(nlist_init_bytes / sizeof(int));

    // in runtime (should from network)
    size_t cell_ID_DRAM_bytes = query_num * nprobe * sizeof(int);
    size_t LUT_DRAM_bytes = query_num * nprobe * LUT_ENTRY_NUM * M * sizeof(float);
    std::vector<int ,aligned_allocator<int >> cell_ID_DRAM(cell_ID_DRAM_bytes / sizeof(int));
    std::vector<int ,aligned_allocator<int >> LUT_DRAM(LUT_DRAM_bytes / sizeof(int));

    // in runtime (should from DRAM)
    std::vector<int ,aligned_allocator<int >> PQ_codes_DRAM_0(PQ_codes_DRAM_0_size / sizeof(int));
    std::vector<int ,aligned_allocator<int >> PQ_codes_DRAM_1(PQ_codes_DRAM_1_size / sizeof(int));
    std::vector<int ,aligned_allocator<int >> PQ_codes_DRAM_2(PQ_codes_DRAM_2_size / sizeof(int));
    std::vector<int ,aligned_allocator<int >> PQ_codes_DRAM_3(PQ_codes_DRAM_3_size / sizeof(int));

    std::vector<int ,aligned_allocator<int >> vec_ID_DRAM_0(vec_ID_DRAM_0_size / sizeof(int));
    std::vector<int ,aligned_allocator<int >> vec_ID_DRAM_1(vec_ID_DRAM_1_size / sizeof(int));
    std::vector<int ,aligned_allocator<int >> vec_ID_DRAM_2(vec_ID_DRAM_2_size / sizeof(int));
    std::vector<int ,aligned_allocator<int >> vec_ID_DRAM_3(vec_ID_DRAM_3_size / sizeof(int));
    
    // out
    size_t out_bytes = query_num * 2 * TOPK;
    std::vector<int ,aligned_allocator<int>> out(out_bytes);

    //////////     load data from disk     //////////

    // PQ codes
    char* PQ_codes_DRAM_0_char = (char*) malloc(PQ_codes_DRAM_0_size);
    PQ_codes_DRAM_0_fstream.read(PQ_codes_DRAM_0_char, PQ_codes_DRAM_0_size);
    if (!PQ_codes_DRAM_0_fstream) {
            std::cout << "error: only " << PQ_codes_DRAM_0_fstream.gcount() << " could be read";
        exit(1);
    }
    memcpy(&PQ_codes_DRAM_0[0], PQ_codes_DRAM_0_char, PQ_codes_DRAM_0_size);
    free(PQ_codes_DRAM_0_char);

    char* PQ_codes_DRAM_1_char = (char*) malloc(PQ_codes_DRAM_1_size);
    PQ_codes_DRAM_1_fstream.read(PQ_codes_DRAM_1_char, PQ_codes_DRAM_1_size);
    if (!PQ_codes_DRAM_1_fstream) {
            std::cout << "error: only " << PQ_codes_DRAM_1_fstream.gcount() << " could be read";
        exit(1);
    }
    memcpy(&PQ_codes_DRAM_1[0], PQ_codes_DRAM_1_char, PQ_codes_DRAM_1_size);
    free(PQ_codes_DRAM_1_char);

    char* PQ_codes_DRAM_2_char = (char*) malloc(PQ_codes_DRAM_2_size);
    PQ_codes_DRAM_2_fstream.read(PQ_codes_DRAM_2_char, PQ_codes_DRAM_2_size);
    if (!PQ_codes_DRAM_2_fstream) {
            std::cout << "error: only " << PQ_codes_DRAM_2_fstream.gcount() << " could be read";
        exit(1);
    }
    memcpy(&PQ_codes_DRAM_2[0], PQ_codes_DRAM_2_char, PQ_codes_DRAM_2_size);
    free(PQ_codes_DRAM_2_char);
    
    char* PQ_codes_DRAM_3_char = (char*) malloc(PQ_codes_DRAM_3_size);
    PQ_codes_DRAM_3_fstream.read(PQ_codes_DRAM_3_char, PQ_codes_DRAM_3_size);
    if (!PQ_codes_DRAM_3_fstream) {
            std::cout << "error: only " << PQ_codes_DRAM_3_fstream.gcount() << " could be read";
        exit(1);
    }
    memcpy(&PQ_codes_DRAM_3[0], PQ_codes_DRAM_3_char, PQ_codes_DRAM_3_size);
    free(PQ_codes_DRAM_3_char);

    // vec ID
    char* vec_ID_DRAM_0_char = (char*) malloc(vec_ID_DRAM_0_size);
    vec_ID_DRAM_0_fstream.read(vec_ID_DRAM_0_char, vec_ID_DRAM_0_size);
    if (!vec_ID_DRAM_0_fstream) {
            std::cout << "error: only " << vec_ID_DRAM_0_fstream.gcount() << " could be read";
        exit(1);
    }
    memcpy(&vec_ID_DRAM_0[0], vec_ID_DRAM_0_char, vec_ID_DRAM_0_size);
    free(vec_ID_DRAM_0_char);

    char* vec_ID_DRAM_1_char = (char*) malloc(vec_ID_DRAM_1_size);
    vec_ID_DRAM_1_fstream.read(vec_ID_DRAM_1_char, vec_ID_DRAM_1_size);
    if (!vec_ID_DRAM_1_fstream) {
            std::cout << "error: only " << vec_ID_DRAM_1_fstream.gcount() << " could be read";
        exit(1);
    }
    memcpy(&vec_ID_DRAM_1[0], vec_ID_DRAM_1_char, vec_ID_DRAM_1_size);
    free(vec_ID_DRAM_1_char);

    char* vec_ID_DRAM_2_char = (char*) malloc(vec_ID_DRAM_2_size);
    vec_ID_DRAM_2_fstream.read(vec_ID_DRAM_2_char, vec_ID_DRAM_2_size);
    if (!vec_ID_DRAM_2_fstream) {
            std::cout << "error: only " << vec_ID_DRAM_2_fstream.gcount() << " could be read";
        exit(1);
    }
    memcpy(&vec_ID_DRAM_2[0], vec_ID_DRAM_2_char, vec_ID_DRAM_2_size);
    free(vec_ID_DRAM_2_char);
    
    char* vec_ID_DRAM_3_char = (char*) malloc(vec_ID_DRAM_3_size);
    vec_ID_DRAM_3_fstream.read(vec_ID_DRAM_3_char, vec_ID_DRAM_3_size);
    if (!vec_ID_DRAM_3_fstream) {
            std::cout << "error: only " << vec_ID_DRAM_3_fstream.gcount() << " could be read";
        exit(1);
    }
    memcpy(&vec_ID_DRAM_3[0], vec_ID_DRAM_3_char, vec_ID_DRAM_3_size);
    free(vec_ID_DRAM_3_char);

    // control signals
    // nlist_init = nlist_PQ_codes_start_addr, nlist_vec_ID_start_addr, nlist_num_vecs,
    char* nlist_PQ_codes_start_addr_char = (char*) malloc(nlist_PQ_codes_start_addr_size);
    nlist_PQ_codes_start_addr_fstream.read(nlist_PQ_codes_start_addr_char, nlist_PQ_codes_start_addr_size);
    if (!nlist_PQ_codes_start_addr_fstream) {
            std::cout << "error: only " << nlist_PQ_codes_start_addr_fstream.gcount() << " could be read";
        exit(1);
    }
    memcpy(&nlist_init[0], nlist_PQ_codes_start_addr_char, nlist_PQ_codes_start_addr_size);
    free(nlist_PQ_codes_start_addr_char);

    char* nlist_vec_ID_start_addr_char = (char*) malloc(nlist_vec_ID_start_addr_size);
    nlist_vec_ID_start_addr_fstream.read(nlist_vec_ID_start_addr_char, nlist_vec_ID_start_addr_size);
    if (!nlist_vec_ID_start_addr_fstream) {
            std::cout << "error: only " << nlist_vec_ID_start_addr_fstream.gcount() << " could be read";
        exit(1);
    }
    memcpy(&nlist_init[nlist], nlist_vec_ID_start_addr_char, nlist_vec_ID_start_addr_size);
    free(nlist_vec_ID_start_addr_char);
    
    char* nlist_num_vecs_char = (char*) malloc(nlist_num_vecs_size);
    nlist_num_vecs_fstream.read(nlist_num_vecs_char, nlist_num_vecs_size);
    if (!nlist_num_vecs_fstream) {
            std::cout << "error: only " << nlist_num_vecs_fstream.gcount() << " could be read";
        exit(1);
    }
    memcpy(&nlist_init[2 * nlist], nlist_num_vecs_char, nlist_num_vecs_size);
    free(nlist_num_vecs_char);

// OPENCL HOST CODE AREA START

    std::vector<cl::Device> devices = get_devices();
    cl::Device device = devices[0];
    std::string device_name = device.getInfo<CL_DEVICE_NAME>();
    std::cout << "Found Device=" << device_name.c_str() << std::endl;

    //Creating Context and Command Queue for selected device
    cl::Context context(device);
    cl::CommandQueue q(context, device);

    // Import XCLBIN
    xclbin_file_name = argv[1];
    cl::Program::Binaries vadd_bins = import_binary_file();

    // Program and Kernel
    devices.resize(1);
    cl::Program program(context, devices, vadd_bins);
    cl::Kernel krnl_vector_add(program, "vadd");

	// ------------------------------------------------------------------
	// Create Buffers in Global Memory to store data
	//             o) buffer_in1 - stores source_in1
	//             o) buffer_in2 - stores source_in2
	//             o) buffer_ouput - stores Results
	// ------------------------------------------------------------------	
	

    // in init 
    OCL_CHECK(err, cl::Buffer buffer_nlist_init   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, 
            nlist_init_bytes, nlist_init.data(), &err));

	// in runtime (should from network)
    OCL_CHECK(err, cl::Buffer buffer_cell_ID_DRAM   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, 
            cell_ID_DRAM_bytes, cell_ID_DRAM.data(), &err));
    OCL_CHECK(err, cl::Buffer buffer_LUT_DRAM   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, 
            LUT_DRAM_bytes, LUT_DRAM.data(), &err));

    // in runtime (should from DRAM)
    OCL_CHECK(err, cl::Buffer buffer_PQ_codes_DRAM_0   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, 
            PQ_codes_DRAM_0_size, PQ_codes_DRAM_0.data(), &err));
    OCL_CHECK(err, cl::Buffer buffer_PQ_codes_DRAM_1   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, 
            PQ_codes_DRAM_1_size, PQ_codes_DRAM_1.data(), &err));
    OCL_CHECK(err, cl::Buffer buffer_PQ_codes_DRAM_2   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, 
            PQ_codes_DRAM_2_size, PQ_codes_DRAM_2.data(), &err));
    OCL_CHECK(err, cl::Buffer buffer_PQ_codes_DRAM_3   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, 
            PQ_codes_DRAM_3_size, PQ_codes_DRAM_3.data(), &err));
    OCL_CHECK(err, cl::Buffer buffer_in_vec_ID_DRAM_0   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, 
            vec_ID_DRAM_0_size, vec_ID_DRAM_0.data(), &err));
    OCL_CHECK(err, cl::Buffer buffer_in_vec_ID_DRAM_1   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, 
            vec_ID_DRAM_1_size, vec_ID_DRAM_1.data(), &err));
    OCL_CHECK(err, cl::Buffer buffer_in_vec_ID_DRAM_2   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, 
            vec_ID_DRAM_2_size, vec_ID_DRAM_2.data(), &err));
    OCL_CHECK(err, cl::Buffer buffer_in_vec_ID_DRAM_3   (context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, 
            vec_ID_DRAM_3_size, vec_ID_DRAM_3.data(), &err));

	// out
    OCL_CHECK(err, cl::Buffer buffer_out(context,CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, 
            out_bytes, out.data(), &err));


    // in init
    OCL_CHECK(err, err = krnl_vector_add.setArg(0, int(query_num)));
    OCL_CHECK(err, err = krnl_vector_add.setArg(1, int(nlist)));
    OCL_CHECK(err, err = krnl_vector_add.setArg(2, int(nprobe)));
    OCL_CHECK(err, err = krnl_vector_add.setArg(3, buffer_nlist_init));

    // in runtime (should from network)
    OCL_CHECK(err, err = krnl_vector_add.setArg(4, buffer_cell_ID_DRAM));
    OCL_CHECK(err, err = krnl_vector_add.setArg(5, buffer_LUT_DRAM));

    // in runtime (should from DRAM)
    OCL_CHECK(err, err = krnl_vector_add.setArg(6, buffer_PQ_codes_DRAM_0));
    OCL_CHECK(err, err = krnl_vector_add.setArg(7, buffer_PQ_codes_DRAM_1));
    OCL_CHECK(err, err = krnl_vector_add.setArg(8, buffer_PQ_codes_DRAM_2));
    OCL_CHECK(err, err = krnl_vector_add.setArg(9, buffer_PQ_codes_DRAM_3));
    OCL_CHECK(err, err = krnl_vector_add.setArg(10, buffer_in_vec_ID_DRAM_0));
    OCL_CHECK(err, err = krnl_vector_add.setArg(11, buffer_in_vec_ID_DRAM_1));
    OCL_CHECK(err, err = krnl_vector_add.setArg(12, buffer_in_vec_ID_DRAM_2));
    OCL_CHECK(err, err = krnl_vector_add.setArg(13, buffer_in_vec_ID_DRAM_3));

    // out
    OCL_CHECK(err, err = krnl_vector_add.setArg(14, buffer_out));



    // Copy input data to device global memory
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({
        // in init
        buffer_nlist_init,
        // in runtime (should from network)
        buffer_cell_ID_DRAM, 
        buffer_LUT_DRAM, 
        // in runtime (should from DRAM)
        buffer_PQ_codes_DRAM_0,
        buffer_PQ_codes_DRAM_1,
        buffer_PQ_codes_DRAM_2,
        buffer_PQ_codes_DRAM_3,
        buffer_in_vec_ID_DRAM_0, 
        buffer_in_vec_ID_DRAM_1, 
        buffer_in_vec_ID_DRAM_2, 
        buffer_in_vec_ID_DRAM_3},0/* 0 means from host*/));

    // Launch the Kernel
    OCL_CHECK(err, err = q.enqueueTask(krnl_vector_add));

    // Copy Result from Device Global Memory to Host Local Memory
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_out},CL_MIGRATE_MEM_OBJECT_HOST));
    q.finish();

// OPENCL HOST CODE AREA END

    std::cout << "TEST FINISHED (NO RESULT CHECK)" << std::endl; 

    return  0;
}

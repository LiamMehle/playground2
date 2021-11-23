#define CL_MINIMUM_OPENCL_VERSION 200
#define CL_TARGET_OPENCL_VERSION  300
//#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define NO_LOG

#define QUEUE_SIZE (1<<10)

#include "cl_boilerplate.hpp"

#include <iostream>
#include <CL/opencl.h>
#include <vector>
#include "read_to_string.hpp"
#include "check_err.hpp"
#include "macro_tools.hpp"
#include <assert.h>

#include <string.h>

#include <chrono>
#include <unistd.h>

int main() {
	cl_platform_id   platform = choose_platform();
	cl_device_id     dev = choose_device(platform);
	cl_context       ctx = make_context(platform, dev);
	cl_command_queue queue = make_command_queue(ctx, dev);
	cl_program       program = make_program(ctx, dev);
	assert(platform != NULL);
	assert(dev      != NULL);
	assert(ctx      != NULL);
	assert(queue      != NULL);
	assert(program  != NULL);

	size_t size = 1<<10;
	char* host_memory = (char*) malloc(size);
	{
		constexpr auto count = 1<<20;
		auto t0 = std::chrono::high_resolution_clock::now();
		for(int i = 0; i < count; i++)
			memset(host_memory, 'A', size);
		auto t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::micro> duration = (t1 - t0);
		std::cout << "memset total: " << duration.count() << "us\n"
		          << "per cycle:    " << (duration/count).count();
	}
	/*{
		FILE* const file = fopen("/home/wolf/git/playground2/cpp/opencl_hw/Makefile", "r");
		size = fread(host_memory, 8, size/8, file);
		fclose(file);
	}*/

	if(host_memory == NULL)
		return -1;
	cl_int err;
	cl_mem buffer[3];
	buffer[0] = clCreateBuffer(ctx,
		CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		size, host_memory, &err);
	LOG(err);

	cl_kernel kern;
	{
		auto t0 = std::chrono::high_resolution_clock::now();
		kern = make_kernel(program, "add1");
		auto t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::micro> ms_double = (t1 - t0);
		std::cout << ms_double.count() << "us\n";
	}


	err = clSetKernelArg(kern, 0, sizeof(buffer[0]), &buffer[0]);
	LOG(err);

	size_t global_dim[] = {size, 1, 1};

	//err = clEnqueueNDRangeKernel(queue, kern, 1, NULL,
	//			global_dim, NULL, 0, NULL, NULL);
	LOG(err);

	memset(host_memory, 'A', size);


	


	constexpr auto inner_iter = QUEUE_SIZE/2;
	constexpr auto iter = (1<<10*1<<8)/(inner_iter);
	printf("i -> %u\n", iter);

	// gpu profiling
// TIME CRITICAL --------------------------------------------------
	auto t0 = std::chrono::high_resolution_clock::now();
	cl_event event[2];
	int init = 0;
	for(int i = 0; i < iter; i++) {
		for(int j = 0; j < inner_iter-1; j++)
			err = clEnqueueNDRangeKernel(queue, kern, 1, NULL,
				global_dim, NULL, 0, NULL, NULL);

		clEnqueueNDRangeKernel(queue, kern, 1, NULL,
				global_dim, NULL, 0, NULL, &event[i%2]);

		//printf("i = %u\n", i);
		//LOG(err);
		//std::cout << "setup      i=" << i%2     << '\n';
		if(init == 0){
			init++;
			puts("init");
			continue;
		}
		//std::cout << "waiting on i=" << (i+1)%2 << '\n';
		//clWaitForEvents(1, &event[0]);
		//clWaitForEvents(1, &event[(i+1)%2]);
	}

	err = clFinish(queue);
	auto t1 = std::chrono::high_resolution_clock::now();
// TIME CRITICAL --------------------------------------------------

	std::chrono::duration<double, std::milli> total_time = (t1 - t0);
	std::chrono::duration<double, std::micro> cycle_time = total_time/inner_iter/iter;
	std::cout << "total_time: " << total_time.count() << "ms\n"
	          << "cycle:      " << cycle_time.count() << "us\n";
	



	LOG(err);
	err = clEnqueueReadBuffer(queue, buffer[0], CL_TRUE, 0,
				size, host_memory, 0, NULL, NULL);
	LOG(err);
	err = clFinish(queue);

	host_memory[size-1] = '\0';
	printf("%s\n", host_memory);

	return 0;
}

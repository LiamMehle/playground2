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
#include <functional>

// calculates the maximum value of sin(x) over interval [0, 6]
/*
unsigned long long profile(std::function<void()> f, const char* const name) {
	return 0;
}
*/
int main() {
	const cl_platform_id   platform = choose_platform();
	const cl_device_id     dev = choose_device(platform);
	const cl_context       ctx = make_context(platform, dev);
	const cl_command_queue queue = make_command_queue(ctx, dev);
	const cl_program       program = make_program(ctx, dev);
	assert(platform != NULL);
	assert(dev      != NULL);
	assert(ctx      != NULL);
	assert(queue    != NULL);
	assert(program  != NULL);

	constexpr size_t size = 1<<10;
	char* const host_memory = (char*) malloc(size);
	assert(host_memory != NULL);


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


	// gpu profiling
// TIME CRITICAL --------------------------------------------------
	auto t0 = std::chrono::high_resolution_clock::now();

	err = clFinish(queue);
	auto t1 = std::chrono::high_resolution_clock::now();
// TIME CRITICAL --------------------------------------------------

	std::chrono::duration<double, std::milli> total_time = (t1 - t0);
	std::chrono::duration<double, std::micro> cycle_time = total_time;
	std::cout << "total_time: " << total_time.count() << "ms\n"
	          << "cycle:      " << cycle_time.count() << "us\n";
	



	LOG(err);
	err = clEnqueueReadBuffer(queue, buffer[0], CL_TRUE, 0,
				size, host_memory, 0, NULL, NULL);
	LOG(err);
	err = clFinish(queue);
	
	free(host_memory);
	return 0;
}

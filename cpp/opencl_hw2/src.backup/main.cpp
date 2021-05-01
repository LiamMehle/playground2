#define CL_MINIMUM_OPENCL_VERSION 200
#define CL_TARGET_OPENCL_VERSION  300
//#define CL_USE_DEPRECATED_OPENCL_1_2_APIS


#include <iostream>
#include <CL/opencl.h>
#include <vector>
#include "read_to_string.hpp"
#include "check_err.hpp"
#include "macro_tools.h"
#include <assert.h>

#include <string.h>

#define QUEUE_SIZE 16

cl_platform_id choose_platform() {
	cl_platform_id ret;
	cl_uint platform_count;
	// get *a* platform
	clGetPlatformIDs(1, &ret, &platform_count);
	LOG(platform_count);
	return ret;
}

cl_device_id choose_device(cl_platform_id platform) {
	cl_device_id ret;
	cl_uint device_count;
	// CPU, GPU, ACCELERATOR, DEFAULT, ALL
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &ret, &device_count);
	LOG(device_count);
	return ret;
}

// optional error-reporting:
/*
 	void *pfn_notify (
		const char *errinfo, 
		const void *private_info, 
		size_t cb, 
		void *user_data) 
*/
cl_context make_context(cl_platform_id platform, cl_device_id device) {
	cl_context ret;
	cl_int err;
	cl_context_properties properties[] = {CL_CONTEXT_PLATFORM, (cl_context_properties)platform, 0};
	ret = clCreateContext(properties, 1, &device, NULL, NULL, &err);
	LOG(err);
	return ret;
}

cl_command_queue make_command_queue(cl_context ctx, cl_device_id dev) {
	cl_command_queue ret;
	cl_command_queue_properties properties[] = {
		CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE,
		CL_QUEUE_SIZE, QUEUE_SIZE,
		0
	};
	cl_int err;
	ret = clCreateCommandQueueWithProperties(ctx, dev, properties, &err);
	LOG(err);
	return ret;
}

cl_program make_program(cl_context ctx, cl_device_id device) {
	std::string src = read_to_string("/home/wolf/git/playground2/cpp/opencl_hw/src/kernel.ocl");
	cl_int err;
	auto c_src = src.c_str();
	const auto len   = src.size();
	const cl_program program = clCreateProgramWithSource(ctx, 1, &c_src, &len, &err);
	LOG(err);
	// second-to-last argument: void (*pfn_notify)(cl_program, void *user_data)
	err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
	LOG(err);
	return program;
}

cl_kernel make_kernel(cl_program program, const char* const name) {
	cl_int err;
	const cl_kernel ret = clCreateKernel(program, name, &err);
	LOG(err);
	return ret;
}

int main() {
	cl_platform_id platform = choose_platform();
	assert(platform != NULL);

	cl_device_id   dev = choose_device(platform);
	assert(dev != NULL);

	cl_context ctx = make_context(platform, dev);
	cl_command_queue queue = make_command_queue(ctx, dev);
	cl_program program = make_program(ctx, dev);

	const size_t size = 1<<10;
	char* host_memory = (char*) malloc(size);
	if(host_memory == NULL)
		return -1;
	cl_int err;
	cl_mem buffer[3];
	buffer[0] = clCreateBuffer(ctx, CL_MEM_READ_WRITE|CL_MEM_USE_HOST_PTR, size, host_memory, &err);
	LOG(err);

	cl_kernel kern = make_kernel(program, "to_upper");
	err = clSetKernelArg(kern, 0, sizeof(buffer[0]), &buffer[0]);
	LOG(err);

	size_t global_dim[] = {size, 1, 1};
	err = clEnqueueNDRangeKernel(queue, kern, 1, NULL, global_dim, NULL, 0, NULL, NULL);
	LOG(err);
	err = clEnqueueReadBuffer(queue, buffer[0], CL_TRUE, 0, size, host_memory, 0, NULL	, NULL);
	LOG(err);

	host_memory[size-1] = '\0';
	printf("%s\n", host_memory);

	return 0;
}
#include "cl_boilerplate.hpp"

#include <assert.h>
#include "macro_tools.hpp"
#include <string>
#include "read_to_string.hpp"

cl_platform_id choose_platform() {
	cl_platform_id ret[128];
	cl_uint platform_count;
	// get *a* platform
	clGetPlatformIDs(128, ret, &platform_count);
	return ret[0];
}

cl_device_id choose_device(cl_platform_id platform) {

	if(platform == 0) {
		platform = choose_platform();
	}

	cl_device_id ret[16];
	cl_uint device_count;
	// CPU, GPU, ACCELERATOR, DEFAULT, ALL
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 16, ret, &device_count);
	return ret[0];
}

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
		CL_QUEUE_PROPERTIES, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,
		CL_QUEUE_SIZE, QUEUE_SIZE,
		0
	};
	cl_int err;
	ret = clCreateCommandQueueWithProperties(ctx, dev, properties, &err);
//	switch(err) {
//		case CL_INVALID_CONTEXT:
//			std::cout << "[error] {createCommandQueue} CL_INVALID_CONTEXT\n";
//			break;
//		case CL_INVALID_DEVICE:
//			std::cout << "[error] {createCommandQueue} CL_INVALID_DEVICE\n";
//			break;
//		case CL_INVALID_VALUE:
//			std::cout << "[error] {createCommandQueue} CL_INVALID_VALUE\n";
//			break;
//		case CL_INVALID_QUEUE_PROPERTIES:
//			std::cout << "[error] {createCommandQueue} CL_INVALID_QUEUE_PROPERTIES\n";
//			break;
//		case CL_OUT_OF_RESOURCES:
//			std::cout << "[error] {createCommandQueue} CL_OUT_OF_RESOURCES\n";
//			break;
//		case CL_OUT_OF_HOST_MEMORY:
//			std::cout << "[error] {createCommandQueue} CL_OUT_OF_HOST_MEMORY\n";
//			break;
//	}
	LOG(err);
	assert(err == 0);
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

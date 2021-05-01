#define CL_MINIMUM_OPENCL_VERSION 200
#define CL_TARGET_OPENCL_VERSION  300

#include <CL/opencl.h>
#include "macro_tools.hpp"
#include <assert.h>

#ifndef QUEUE_SIZE
#define QUEUE_SIZE (1<<10)
#endif

enum device_vendor {
	unknown = 0,
	intel,
	nvidia,
	amd
};

cl_platform_id choose_platform();

cl_device_id choose_device(cl_platform_id platform);

// optional error-reporting:
/*
 	void *pfn_notify (
		const char *errinfo, 
		const void *private_info, 
		size_t cb, 
		void *user_data) 
*/
cl_context make_context(cl_platform_id platform, cl_device_id device);

cl_command_queue make_command_queue(cl_context ctx, cl_device_id dev);

cl_program make_program(cl_context ctx, cl_device_id device);

cl_kernel make_kernel(cl_program program, const char* const name);

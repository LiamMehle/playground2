#define CL_HPP_MINIMUM_OPENCL_VERSION 200
#define CL_HPP_TARGET_OPENCL_VERSION  300

#include <iostream>
#include <CL/opencl.hpp>
#include <vector>

#include "read_to_string.hpp"

int main() {
	// --------------------------------------< selecting a device >--------------------------------------
	std::vector<cl::Platform> platforms( 2 );
	cl::Platform::get( &platforms );
	unsigned int index = 0;
	unsigned int platform_index = 0;
	for( auto& p : platforms ) {
		const std::string name = p.getInfo<CL_PLATFORM_NAME>().data();
		const std::string vendor = p.getInfo<CL_PLATFORM_VENDOR>().data();
		std::cout << "\nname: " << name <<
		             "\nvendor: " << vendor << "\n\n";
		
		// select mesa platform if possible
		// trying to get intel
		if( vendor == "NVIDIA")
			platform_index = index;
		
		index++;
	}


	index = 0;
	unsigned int device_index = 0;
	const cl::Platform platform = platforms[index];
	std::vector<cl::Device> devices;
	platform.getDevices( CL_DEVICE_TYPE_GPU, &devices );
	for( auto& d : devices ) {
		const std::string name = d.getInfo<CL_DEVICE_NAME>().data();
		const std::string vendor = d.getInfo<CL_DEVICE_VENDOR>().data();
		std::cout << "\nname: " << name <<
		             "\nvendor: " << vendor << "\n\n";
		
		// select mesa platform if possible
		// trying to get intel
		if( vendor == "NVIDIA")
			device_index = index;
		
		index++;
	}
	const cl::Device device = devices[device_index];
	const cl::Context context( device );


	// ------------------------------------< compiling the kernel >--------------------------------------
	cl_int err;
	cl::Program program( context, read_to_string("/home/wolf/git/playground2/cpp/opencl_hw/src/kernel.ocl").data(), CL_TRUE, &err );
	std::cout << "program: " << err << '\n';
	cl::Kernel kernel( program, "test", &err );

	const unsigned size = 200;
	char src[size] = "Hello, world!";
	char dst[size];
	kernel.setArg( 0, sizeof(long), &size);
	kernel.setArg( 1, size, src );
	kernel.setArg( 2, size, dst );
	std::cout << "kernel: " << err << '\n';
}
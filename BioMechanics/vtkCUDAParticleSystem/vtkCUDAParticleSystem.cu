#include <cutil_inline.h>    // includes cuda.h and cuda_runtime_api.h
#include <cstdlib>
#include <cstdio>
#include <string.h>

#if defined(__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <cuda_gl_interop.h>

#include "thrust/device_ptr.h"
#include "thrust/for_each.h"
#include "thrust/iterator/zip_iterator.h"
#include "thrust/sort.h"

#include "vtkCUDAParticleSystemKernel.cu"

extern "C"
{

void cudaInit()
{
	int devID = cutGetMaxGflopsDeviceId();
	cudaSetDevice( devID );
}

void cudaGLInit(int argc, char **argv)
{
	cudaGLSetGLDevice( cutGetMaxGflopsDeviceId() );
}

void allocateArray(void **devPtr, size_t size)
{
	cutilSafeCall(cudaMalloc(devPtr, size));
}

void freeArray(void *devPtr)
{
	cutilSafeCall(cudaFree(devPtr));
}

void threadSync()
{
	cutilSafeCall(cutilDeviceSynchronize());
}

void copyArrayToDevice(void* device, const void* host, int offset, int size)
{
	cutilSafeCall(cudaMemcpy((char *) device + offset, host, size, cudaMemcpyHostToDevice));
}

void copyArrayFromDevice(void* host, const void* device,
		struct cudaGraphicsResource **cuda_vbo_resource, int size)
{
	//if (cuda_vbo_resource)
	//	device = mapGLBufferObject(cuda_vbo_resource);

	cutilSafeCall(cudaMemcpy(host, device, size, cudaMemcpyDeviceToHost));

	//if (cuda_vbo_resource)
	//	unmapGLBufferObject(*cuda_vbo_resource);
}

void integrateSystem(float *pos,
		float *vel,
		//float *acc,
		float deltaTime,
		uint numParticles)
{
	thrust::device_ptr<float4> d_pos4((float4 *)pos);
	thrust::device_ptr<float4> d_vel4((float4 *)vel);
	//thrust::device_ptr<float4> d_acc4((float4 *)acc);

	//thrust::for_each(
	//			thrust::make_zip_iterator(thrust::make_tuple(d_vel4, d_acc4)),
	//			thrust::make_zip_iterator(thrust::make_tuple(d_vel4+numParticles, d_acc4+numParticles)),
	//			integrate_functor(deltaTime));
	thrust::for_each(
			thrust::make_zip_iterator(thrust::make_tuple(d_pos4, d_vel4)),
			thrust::make_zip_iterator(thrust::make_tuple(d_pos4+numParticles, d_vel4+numParticles)),
			integrate_functor(deltaTime));
}

}   // extern "C"

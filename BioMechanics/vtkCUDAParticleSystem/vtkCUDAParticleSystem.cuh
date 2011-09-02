
extern "C"
{
//void cudaInit(int argc, char **argv);
void cudaInit();

void allocateArray(void **devPtr, int size);
void freeArray(void *devPtr);

void threadSync();

void copyArrayFromDevice(void* host, const void* device, struct cudaGraphicsResource **cuda_vbo_resource, int size);
void copyArrayToDevice(void* device, const void* host, int offset, int size);

void integrateSystem(float *pos, float *vel, float deltaTime, uint numParticles);

}

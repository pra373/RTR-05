#pragma once

#include<cuda.h>


// user defined function to call te kernel

void launchCUDAKernel(float*, float*, int);
#pragma once

// Declare explodeFactor in constant memory

__global__ void updatePosition_GPU(float* pInteropBuffer,float* explodeFactor, int numTriangles)
{
    

	int threadIndex = blockIdx.x * blockDim.x + threadIdx.x;

	if (threadIndex < numTriangles)
	{
		// Calculate the base index for the triangle being processed

		int baseIndex = threadIndex * 9;

        // Fetch the vertices of the triangle
        float v1x = pInteropBuffer[baseIndex];       // x-coordinate of first vertex
        float v1y = pInteropBuffer[baseIndex + 1];   // y-coordinate of first vertex
        float v1z = pInteropBuffer[baseIndex + 2];   // z-coordinate of first vertex

        float v2x = pInteropBuffer[baseIndex + 3];   // x-coordinate of second vertex
        float v2y = pInteropBuffer[baseIndex + 4];   // y-coordinate of second vertex
        float v2z = pInteropBuffer[baseIndex + 5];   // z-coordinate of second vertex

        float v3x = pInteropBuffer[baseIndex + 6];   // x-coordinate of third vertex
        float v3y = pInteropBuffer[baseIndex + 7];   // y-coordinate of third vertex
        float v3z = pInteropBuffer[baseIndex + 8];   // z-coordinate of third vertex

        // Calculate the normal for the triangle

        float abx = v2x - v1x; // Vector AB X component
        float aby = v2y - v1y; // Vector AB Y component
        float abz = v2z - v1z; // Vector AB Z component

        float acx = v3x - v1x; // Vector AC X component
        float acy = v3y - v1y; // Vector AC Y component
        float acz = v3z - v1z; // Vector AC Z component

        // Calculate the normal vector using the cross product

        float normalX = aby * acz - abz * acy; // X component
        float normalY = abz * acx - abx * acz; // Y component
        float normalZ = abx * acy - aby * acx; // Z component

        // Normalize the normal vector
        float magnitude = sqrt(normalX * normalX + normalY * normalY + normalZ * normalZ);

        const float epsilon = 1.0E-6;

        if (magnitude > epsilon)
        {
            normalX /= magnitude;
            normalY /= magnitude;
            normalZ /= magnitude;
        }

        // Update the position of the vertices based on the explode factor

        //update first vertex

        pInteropBuffer[baseIndex] = pInteropBuffer[baseIndex] + explodeFactor[0] * normalX;
        pInteropBuffer[baseIndex + 1] = pInteropBuffer[baseIndex + 1] + explodeFactor[1] * normalY;
        pInteropBuffer[baseIndex + 2] = pInteropBuffer[baseIndex + 2] + explodeFactor[2] * normalZ;

        //update second vertex

        pInteropBuffer[baseIndex + 3] = pInteropBuffer[baseIndex + 3] +  explodeFactor[0] * normalX;  
        pInteropBuffer[baseIndex + 4] = pInteropBuffer[baseIndex + 4] + explodeFactor[1] * normalY;
        pInteropBuffer[baseIndex + 5] = pInteropBuffer[baseIndex + 5] + explodeFactor[2] * normalZ;

        //update third vertex

        pInteropBuffer[baseIndex + 6] = pInteropBuffer[baseIndex + 6] + explodeFactor[0] * normalX; 
        pInteropBuffer[baseIndex + 7] = pInteropBuffer[baseIndex + 7] + explodeFactor[1] * normalY;
        pInteropBuffer[baseIndex + 8] = pInteropBuffer[baseIndex + 8] + explodeFactor[2] * normalZ;

	}

}

void launchCUDAKernel(float* pInteropBuffer, float* explodeFactor, int numTriangles)
{
    dim3 blockDim(1024, 1, 1);

    int numBlocks = ceil(numTriangles / 1024);

    //int numBlocks = (numTriangles + 1024 - 1) / 1024;

    dim3 gridDim(numBlocks, 1, 1);

    updatePosition_GPU << < gridDim, blockDim >> > (pInteropBuffer, explodeFactor, numTriangles);

}


/**
 * Copyright (c) 2012 Sander van der Burg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "interleave.h"
#include <stdlib.h>
#include <string.h>

#define MAX_NUM_OF_BITPLANES 8

void ILBM_deinterleaveToBitplaneMemory(const ILBM_Image *image, IFF_UByte **bitplanes)
{
    if(image->body != NULL)
    {
        unsigned int scanLineSize;
	unsigned int i;
	int count = 0; /* Offset in the interleaved source */
	int hOffset = 0; /* Horizontal offset in resulting bitplanes */
	
	scanLineSize = image->bitMapHeader->w / 8;
	
	for(i = 0; i < image->bitMapHeader->h; i++)
	{
	    unsigned int j;
	    
	    for(j = 0; j < image->bitMapHeader->nPlanes; j++)
	    {
		memcpy(bitplanes[j] + hOffset, image->body->chunkData + count, scanLineSize);
		count += scanLineSize;
	    }
	    
	    hOffset += scanLineSize;
	}
    }
}

IFF_UByte *ILBM_deinterleave(const ILBM_Image *image)
{
    IFF_UByte nPlanes = image->bitMapHeader->nPlanes;
    unsigned int bitplaneSize = image->bitMapHeader->w / 8 * image->bitMapHeader->h;
    unsigned int i;
    unsigned int offset = 0;
    
    IFF_UByte *result = (IFF_UByte*)malloc(bitplaneSize * nPlanes * sizeof(IFF_UByte));
    IFF_UByte *bitplanes[MAX_NUM_OF_BITPLANES];
    
    /* Set bitplane pointers */
    
    for(i = 0; i < nPlanes; i++)
    {
	bitplanes[i] = result + offset;
	offset += bitplaneSize;
    }
    
    /* Deinterleave and write results to the bitplane addresses */
    ILBM_deinterleaveToBitplaneMemory(image, bitplanes);
    
    /* Return result */
    return result;
}

IFF_UByte *ILBM_adeinterleave(const ILBM_Image *image)
{
    if(image->body == NULL)
	return NULL;
    else
    {
        unsigned int bitplaneSize, scanLineSize;
	IFF_UByte *result;
	unsigned int i;
	int count = 0; /* Offset in the interleaved source */
	int vOffset = 0; /* Vertical offset in the result */
    
	scanLineSize = image->bitMapHeader->w / 8;
	bitplaneSize = scanLineSize * image->bitMapHeader->h;
	
	result = (IFF_UByte*)malloc(bitplaneSize * image->bitMapHeader->nPlanes * sizeof(IFF_UByte));
    
	for(i = 0; i < image->bitMapHeader->h; i++)
	{
	    unsigned int j;
	    int hOffset = vOffset; /* Horizontal offset in the result */
	
	    for(j = 0; j < image->bitMapHeader->nPlanes; j++)
	    {
		memcpy(result + hOffset, image->body->chunkData + count, scanLineSize);
		
		count += scanLineSize;
	    
		hOffset += bitplaneSize;
	    }
	
	    vOffset += scanLineSize;
	}
    
	return result;
    }
}

void ILBM_interleave(ILBM_Image *image, IFF_UByte *bitplanes)
{
    unsigned int i;
    unsigned int scanLineSize = image->bitMapHeader->w / 8;
    unsigned int interleavedScanLineSize = image->bitMapHeader->nPlanes * scanLineSize;
    
    unsigned int count = 0; /* Offset in the non-interleaved bitplane data array */
    unsigned int bOffset = 0; /* Base offset in the interleaved bitplane data array */
    unsigned int chunkSize = interleavedScanLineSize * image->bitMapHeader->h;
    
    IFF_UByte *result = (IFF_UByte*)malloc(chunkSize * sizeof(IFF_UByte));
    
    for(i = 0; i < image->bitMapHeader->nPlanes; i++)
    {
	unsigned int j;
	unsigned int hOffset = bOffset;
	
	for(j = 0; j < image->bitMapHeader->h; j++)
	{
	    memcpy(result + hOffset, bitplanes + count, scanLineSize);
	    
	    count += scanLineSize;
	    hOffset += interleavedScanLineSize;
	}
	
	bOffset += scanLineSize;
    }
    
    /* Free the old body chunk data */
    free(image->body->chunkData);
    
    /* Set the new body containing the interleaved chunk data */
    IFF_setRawChunkData(image->body, result, chunkSize);
}

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

#include "bitmapheader.h"
#include <stdlib.h>
#include <libiff/io.h>
#include <libiff/util.h>
#include <libiff/error.h>
#include "ilbm.h"

#define CHUNKID "BMHD"

ILBM_BitMapHeader *ILBM_createBitMapHeader(void)
{
    ILBM_BitMapHeader *bitMapHeader = (ILBM_BitMapHeader*)IFF_allocateChunk(CHUNKID, sizeof(ILBM_BitMapHeader));
    
    if(bitMapHeader != NULL)
    {
	bitMapHeader->chunkSize = 2 * sizeof(IFF_UWord) + 2 * sizeof(IFF_Word) + 4 * sizeof(IFF_UByte) + sizeof(IFF_UWord) + 2 * sizeof(IFF_UByte) + 2 * sizeof(IFF_Word);
	bitMapHeader->pad1 = '\0';
    }
    
    return bitMapHeader;
}

IFF_Chunk *ILBM_readBitMapHeader(FILE *file, const IFF_Long chunkSize)
{
    ILBM_BitMapHeader *bitMapHeader = ILBM_createBitMapHeader();
    
    if(bitMapHeader != NULL)
    {
	IFF_UByte byte;
    
	if(!IFF_readUWord(file, &bitMapHeader->w, CHUNKID, "w"))
	{
	    ILBM_free((IFF_Chunk*)bitMapHeader);
	    return NULL;
	}
    
	if(!IFF_readUWord(file, &bitMapHeader->h, CHUNKID, "h"))
	{
	    ILBM_free((IFF_Chunk*)bitMapHeader);
	    return NULL;
	}
    
	if(!IFF_readWord(file, &bitMapHeader->x, CHUNKID, "x"))
	{
	    ILBM_free((IFF_Chunk*)bitMapHeader);
	    return NULL;
	}
    
	if(!IFF_readWord(file, &bitMapHeader->y, CHUNKID, "y"))
	{
	    ILBM_free((IFF_Chunk*)bitMapHeader);
	    return NULL;
	}
    
	if(!IFF_readUByte(file, &bitMapHeader->nPlanes, CHUNKID, "nPlanes"))
	{
	    ILBM_free((IFF_Chunk*)bitMapHeader);
	    return NULL;
	}
    
	if(!IFF_readUByte(file, &byte, CHUNKID, "masking"))
	{
	    ILBM_free((IFF_Chunk*)bitMapHeader);
	    return NULL;
	}
    
	bitMapHeader->masking = byte;
    
	if(!IFF_readUByte(file, &byte, CHUNKID, "compression"))
	{
	    ILBM_free((IFF_Chunk*)bitMapHeader);
	    return NULL;
	}
    
	bitMapHeader->compression = byte;
    
	if(!IFF_readUByte(file, &bitMapHeader->pad1, CHUNKID, "pad1"))
	{
	    ILBM_free((IFF_Chunk*)bitMapHeader);
	    return NULL;
	}
    
	if(!IFF_readUWord(file, &bitMapHeader->transparentColor, CHUNKID, "transparentColor"))
	{
	    ILBM_free((IFF_Chunk*)bitMapHeader);
	    return NULL;
	}
    
	if(!IFF_readUByte(file, &bitMapHeader->xAspect, CHUNKID, "xAspect"))
	{
	    ILBM_free((IFF_Chunk*)bitMapHeader);
	    return NULL;
	}
    
	if(!IFF_readUByte(file, &bitMapHeader->yAspect, CHUNKID, "yAspect"))
	{
	    ILBM_free((IFF_Chunk*)bitMapHeader);
	    return NULL;
	}
    
	if(!IFF_readWord(file, &bitMapHeader->pageWidth, CHUNKID, "pageWidth"))
	{
	    ILBM_free((IFF_Chunk*)bitMapHeader);
	    return NULL;
	}
    
	if(!IFF_readWord(file, &bitMapHeader->pageHeight, CHUNKID, "pageHeight"))
	{
	    ILBM_free((IFF_Chunk*)bitMapHeader);
	    return NULL;
	}
    }
    
    return (IFF_Chunk*)bitMapHeader;
}

int ILBM_writeBitMapHeader(FILE *file, const IFF_Chunk *chunk)
{
    ILBM_BitMapHeader *bitMapHeader = (ILBM_BitMapHeader*)chunk;
    
    if(!IFF_writeUWord(file, bitMapHeader->w, CHUNKID, "w"))
	return FALSE;

    if(!IFF_writeUWord(file, bitMapHeader->h, CHUNKID, "h"))
	return FALSE;
    
    if(!IFF_writeWord(file, bitMapHeader->x, CHUNKID, "x"))
	return FALSE;
	
    if(!IFF_writeWord(file, bitMapHeader->y, CHUNKID, "y"))
	return FALSE;

    if(!IFF_writeUByte(file, bitMapHeader->nPlanes, CHUNKID, "nPlanes"))
	return FALSE;

    if(!IFF_writeUByte(file, (IFF_UByte)bitMapHeader->masking, CHUNKID, "masking"))
	return FALSE;
    
    if(!IFF_writeUByte(file, (IFF_UByte)bitMapHeader->compression, CHUNKID, "compression"))
	return FALSE;
    
    if(!IFF_writeUByte(file, bitMapHeader->pad1, CHUNKID, "pad1"))
	return FALSE;
    
    if(!IFF_writeUWord(file, bitMapHeader->transparentColor, CHUNKID, "transparentColor"))
	return FALSE;
    
    if(!IFF_writeUByte(file, bitMapHeader->xAspect, CHUNKID, "xAspect"))
	return FALSE;

    if(!IFF_writeUByte(file, bitMapHeader->yAspect, CHUNKID, "yAspect"))
	return FALSE;

    if(!IFF_writeWord(file, bitMapHeader->pageWidth, CHUNKID, "pageWidth"))
	return FALSE;
	
    if(!IFF_writeWord(file, bitMapHeader->pageHeight, CHUNKID, "pageHeight"))
	return FALSE;
	
    return TRUE;
}

int ILBM_checkBitMapHeader(const IFF_Chunk *chunk)
{
    ILBM_BitMapHeader *bitMapHeader = (ILBM_BitMapHeader*)chunk;
    
    if(bitMapHeader->masking < 0 || bitMapHeader->masking > ILBM_MSK_LASSO)
    {
	IFF_error("Invalid 'BMHD'.masking value!\n");
	return FALSE;
    }
    
    if(bitMapHeader->compression < 0 || bitMapHeader->compression > ILBM_CMP_BYTE_RUN)
    {
	IFF_error("Invalid 'BMHD'.compression value!\n");
	return FALSE;
    }
    
    if(bitMapHeader->pad1 != 0)
	IFF_error("WARNING: 'BMHD'.pad1 is not 0!\n");
    
    return TRUE;
}

void ILBM_freeBitMapHeader(IFF_Chunk *chunk)
{
}

void ILBM_printBitMapHeader(const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    ILBM_BitMapHeader *bitMapHeader = (ILBM_BitMapHeader*)chunk;
    
    IFF_printIndent(stdout, indentLevel, "w = %d;\n", bitMapHeader->w);
    IFF_printIndent(stdout, indentLevel, "h = %d;\n", bitMapHeader->h);
    IFF_printIndent(stdout, indentLevel, "x = %d;\n", bitMapHeader->x);
    IFF_printIndent(stdout, indentLevel, "y = %d;\n", bitMapHeader->y);
    IFF_printIndent(stdout, indentLevel, "nPlanes = %d;\n", bitMapHeader->nPlanes);
    IFF_printIndent(stdout, indentLevel, "masking = %d;\n", bitMapHeader->masking);
    IFF_printIndent(stdout, indentLevel, "compression = %d;\n", bitMapHeader->compression);
    IFF_printIndent(stdout, indentLevel, "xAspect = %d;\n", bitMapHeader->xAspect);
    IFF_printIndent(stdout, indentLevel, "yAspect = %d;\n", bitMapHeader->yAspect);
    IFF_printIndent(stdout, indentLevel, "pageWidth = %d;\n", bitMapHeader->pageWidth);
    IFF_printIndent(stdout, indentLevel, "pageHeight = %d;\n", bitMapHeader->pageHeight);
}

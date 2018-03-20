/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

#include "../../include/main_header.h"

//#define DEBUG_OUTPUT 1

using Yaz0::Parser;

bool Parser::writeOut(u8 val)
{
    if(bufferOutPos < bufferOutSize)
    {
        bufferOut[bufferOutPos++] = val;
    }
    return (bufferOutPos < bufferOutSize);
}

u8 Parser::readIn()
{
    if(bufferInPos >= bufferInSize)
        throw new std::out_of_range("maximum buffer in pos reached");

    return bufferIn[bufferInPos++];
}

bool Parser::parseBlock()
{
    u8 header = readIn();
    
    #ifdef DEBUG_OUTPUT
        printf("\n== Header @%#04x ==\n", bufferInPos);
        printf("%c%c%c%c%c%c%c%c\n", 
            ((header >> 7) & 1) ? '1' : 'C',((header >> 6) & 1) ? '1' : 'C',
            ((header >> 5) & 1) ? '1' : 'C',((header >> 4) & 1) ? '1' : 'C',
            ((header >> 3) & 1) ? '1' : 'C',((header >> 2) & 1) ? '1' : 'C',
            ((header >> 1) & 1) ? '1' : 'C',((header >> 0) & 1) ? '1' : 'C'
        );
    #endif
    

    for(int i=7; i>=0; --i)
    {
        #ifdef DEBUG_OUTPUT
            printf("#%d | %#04x[y:%#04x]: ", i, bufferOutPos, bufferInPos);
        #endif

        if(bufferOutPos >= bufferOutSize)
            return false;

        int chunkType = (header >> i) & 1;
        if(chunkType == 1)
        {   
            u8 val = readIn();

            #ifdef DEBUG_OUTPUT
                printf(" 1:1 (%#04x)\n", val);
            #endif

            if(!writeOut(val))
                return false;
        }else{

            int length = 0;

            chunks[0] = readIn();
            chunks[1] = readIn();

            int offset = (((chunks[0] & 0xF) << 8) | chunks[1]) + 1;

            if(((chunks[0] >> 4) & 0xF) == 0) // has 3 bytes
            {
                chunks[2] = readIn();
                length = chunks[2] + 0x12;
            }else{
                length = ((chunks[0] >> 4) & 0xF) + 0x02;
            }

            #ifdef DEBUG_OUTPUT
                printf(" copy %d bytes @ %d\n", length, offset);
            #endif
            
            for(int n=0; n<length; ++n)
            {
                s32 copyOffset = (s32)bufferOutPos - (s32)offset;
                if(copyOffset < 0)
                    copyOffset = 0;

                if(!writeOut(bufferOut[copyOffset]))
                    return false;
            }
        }
    }

    return true;
}

bool Parser::decode(u8* buffer, u32 bufferSize, s32 dataSize)
{
    bufferInSize = bufferSize;
    if(bufferSize < HEADER_SIZE)
    {
        return false;
    }

    bufferIn = buffer;

    if(memcmp(bufferIn, "Yaz0", 4) != 0)
    {
        return false;
    }

    // get size and create buffer
    bufferOutSize = (bufferIn[4] << 24) | (bufferIn[5] << 16) | (bufferIn[6] << 8) | (bufferIn[7]);
    if(bufferOutSize > MAX_FILE_SIZE)
    {
        return false;
    }

    if(dataSize >= 0 && (u32)dataSize <= bufferOutSize)
        bufferOutSize = dataSize;

    bufferOut.resize(bufferOutSize);
    
    // reade flags
    bufferInPos = 8;

    for(int i=0; i<8; ++i)
        flags[i] = bufferIn[bufferInPos++];

    // read all blocks
    try{
        while(parseBlock());
    }catch(...)
    {
        // might be an invalid file, but i don't care... still return the buffer
    }

    return (bufferOutPos == bufferOutSize);
}

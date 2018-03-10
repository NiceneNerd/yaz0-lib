/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

#include "../include/main_header.h"

YAZ0::YAZ0(std::vector<u8> *bufferOut)
: YAZ0_Base(bufferOut)
{
    for(int i=0; i<8; ++i)
        flags[i] = 0;
}

bool YAZ0::writeOut(u8 val)
{
    (*bufferOut)[bufferOutPos++] = val;

    return (bufferOutPos < bufferOutSize);
}

u8 YAZ0::readIn()
{
    return bufferIn[bufferInPos++];
}

bool YAZ0::parseBlock()
{
    u8 header = readIn();
    
    /*
    printf("\n== Header @%#04x ==\n", bufferInPos);
    printf("%c%c%c%c%c%c%c%c\n", 
        ((header >> 7) & 1) ? '1' : 'C',
        ((header >> 6) & 1) ? '1' : 'C',
        ((header >> 5) & 1) ? '1' : 'C',
        ((header >> 4) & 1) ? '1' : 'C',
        ((header >> 3) & 1) ? '1' : 'C',
        ((header >> 2) & 1) ? '1' : 'C',
        ((header >> 1) & 1) ? '1' : 'C',
        ((header >> 0) & 1) ? '1' : 'C'
    );
    */

    for(int i=7; i>=0; --i)
    {
        //printf("#%d | %#04x[y:%#04x]: ", i, bufferOutPos, bufferInPos);

        if(bufferOutPos >= bufferOutSize)
            return false;

        int chunkType = (header >> i) & 1;
        if(chunkType == 1)
        {   
            //u8 val = readIn();
            //printf(" 1:1 (%#04x)\n", val);
            if(!writeOut(readIn()))
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

            //printf(" copy %d bytes @ %d\n", length, offset);

            for(int n=0; n<length; ++n)
            {
                if(!writeOut((*bufferOut)[bufferOutPos - offset]))
                    return false;
            }
        }
    }

    return true;
}

bool YAZ0::decode(u8* buffer, u32 bufferSize, s32 dataSize)
{
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

    if(dataSize >= 0 && dataSize <= bufferOutSize)
        bufferOutSize = dataSize;

    bufferOut->resize(bufferOutSize);
    
    // reade flags
    bufferInPos = 8;

    for(int i=0; i<8; ++i)
        flags[i] = bufferIn[bufferInPos++];

    // read all blocks
    while(parseBlock());

    return true;
}

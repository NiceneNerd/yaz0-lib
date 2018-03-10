/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

#include "../include/main_header.h"

void YAZ0_Creator::writeValue(u8 value)
{
    //printf(" ==> writeValue( %#04x )\n", value);   
    printf("#%d | %#02x: 1:1 (%#02x)\n", headerPos, bufferInPos, value);

    addToHeader(Type::Value);
}

void YAZ0_Creator::writeCopy(u32 offset, u32 length)
{
    u32 offsetDiff = 1;
    if(offset < bufferInPos)
        offsetDiff = bufferInPos - offset;

    printf("#%d | %#02x: copy %d bytes @ %d\n", headerPos, bufferInPos, length, offsetDiff);

    addToHeader(Type::Copy);
}

void YAZ0_Creator::addToHeader(YAZ0_Creator::Type types)
{
    --headerPos;
    if(headerPos < 0) {
        printf("\n== Header ==\n");
        headerPos = 7;
    }
}

u32 YAZ0_Creator::encodeByte(u32 valueOffset)
{
    // scan for same bytes
    for(u32 scanOffset=valueOffset; scanOffset<bufferInSize; ++scanOffset) // go through every byte
    {
        if(bufferIn[scanOffset] == bufferIn[valueOffset])
        {
            
        }else{
            u32 sameByteCount = scanOffset - valueOffset;
            if(sameByteCount > 3)
            {
                writeValue(bufferIn[valueOffset]);
                writeCopy(valueOffset+1, scanOffset-valueOffset-1);

                return sameByteCount;
            }
            break;
        }
    }

    // scan for previous bytes
    s32 searchOffsetMax = valueOffset - 1; // maximum value offset to go to
    for(u32 scanBackOffset=0; scanBackOffset<MAX_OFFSET; ++scanBackOffset) // scan through previous bytes
    {
        s32 searchOffset = valueOffset - scanBackOffset;
        if(searchOffset < 0)break;

        if(bufferIn[searchOffset] == bufferIn[valueOffset]) // same byte found
        {
            // check next 3 bytes
            for(u32 sameByteCount=0; sameByteCount<MAX_COPY_LENGTH; ++sameByteCount) // go forward again until a byte dosn't match
            {
                u32 currentSearchOffset = searchOffset + sameByteCount;
                if(currentSearchOffset > searchOffsetMax)
                    currentSearchOffset = searchOffsetMax;
                
                u32 currentValueOffset  = valueOffset + sameByteCount;

                if(bufferIn[currentSearchOffset] != bufferIn[currentValueOffset])
                {
                    if(sameByteCount >= 3)
                    {
                        //printf("   |--> @%#04x -> @%#04x-%#04x -> found (%d) => %#02x \n", valueOffset, searchOffset, searchOffset + sameByteCount-1, sameByteCount, bufferIn[searchOffset]);    
                        writeCopy(searchOffset, sameByteCount);
                        return sameByteCount;
                    }
                    break;
                }
            }
        }
    }

    writeValue(bufferIn[valueOffset]);

    return 1;
}

bool YAZ0_Creator::encode(u8* buffer, u32 bufferSize, s32 dataSize)
{
    bufferIn = buffer;
    bufferInSize = bufferSize;

    bufferInSize = 120;

    headerPos = 7;
    for(bufferInPos=0; bufferInPos<bufferInSize;) // go through every byte
    {
        bufferInPos += encodeByte(bufferInPos);
    }

    return true;
}




    /*
        printf("\n== Header ==\n");
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

    //printf("#%d | %#04x: ", i, bufferOutPos);
        //printf(" 1:1\n");
        //printf(" copy %d bytes @ %d\n", length, offset);

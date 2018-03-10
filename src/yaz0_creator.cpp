/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

#include "../include/main_header.h"

void YAZ0_Creator::writeFileHeader()
{
    for(auto c : "Yaz0")
        bufferOut->push_back(c);

    (*bufferOut)[4]   = ((bufferInSize >> 24) & 0xFF); // overwrite Yaz0's end of string zero
    bufferOut->push_back((bufferInSize >> 16) & 0xFF);
    bufferOut->push_back((bufferInSize >>  8) & 0xFF);
    bufferOut->push_back((bufferInSize      ) & 0xFF);

    for(int padding=0; padding<8; ++padding)
        bufferOut->push_back(0);
}

void YAZ0_Creator::writeValue(u8 value)
{
    printf("#%d | %#02x: 1:1 (%#02x)\n", headerPos, bufferInPos, value);

    addToHeader(Type::Value);
    bufferOut->push_back(value);
}

void YAZ0_Creator::writeCopy(u32 offset, u32 length)
{
    addToHeader(Type::Copy);
    
    u32 offsetDiff = 0;
    if(offset < bufferInPos)
        offsetDiff = bufferInPos - offset - 1;

    printf("writeCopy: @-%#04x len:%d\n", offsetDiff, length);

    bufferOut->insert(bufferOut->end(), 3, 0);
    auto buff = bufferOut->end() - 3;

    buff[0] = (offsetDiff >> 8) & 0xF;
    buff[1] = offsetDiff & 0xFF;

    if(length > 0x11) // 3 bytes
    {
        buff[2] = (length - 0x12) & 0xFF;
    }else{ // 2 bytes
        buff[0] |= ((length-3) & 0xF) << 4;
        bufferOut->pop_back();
    }

    printf("#%d | %#02x: copy %d bytes @ %d\n", headerPos, bufferInPos, length, offsetDiff);
}

void YAZ0_Creator::createHeader()
{
    if(headerOffset != 0)
        writeHeader();

    headerOffset = bufferOut->size();
    headerPos = 7;
    bufferOut->push_back(0);
}

void YAZ0_Creator::addToHeader(YAZ0_Creator::Type type)
{
    if(headerPos < 0) {
        printf("%c%c%c%c%c%c%c%c\n", ((headerValue >> 7) & 1) ? '1' : 'C',((headerValue >> 6) & 1) ? '1' : 'C',((headerValue >> 5) & 1) ? '1' : 'C',((headerValue >> 4) & 1) ? '1' : 'C',((headerValue >> 3) & 1) ? '1' : 'C',((headerValue >> 2) & 1) ? '1' : 'C',((headerValue >> 1) & 1) ? '1' : 'C',((headerValue >> 0) & 1) ? '1' : 'C');
        createHeader();
    }

    headerValue |= (u8)type << headerPos;

    --headerPos;
}

void YAZ0_Creator::writeHeader()
{
    if(headerPos != 7)
    {
        (*bufferOut)[headerOffset] = headerValue;
        headerValue = 0;
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
    bufferInSize = dataSize >= 0 ? dataSize : bufferSize;

    writeFileHeader();

    createHeader();
    for(bufferInPos=0; bufferInPos<bufferInSize;) // go through every byte
    {
        bufferInPos += encodeByte(bufferInPos);
    }
    writeHeader(); // write a maybe uncompleted header

    return true;
}

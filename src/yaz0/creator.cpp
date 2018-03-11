/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

#include "../../include/main_header.h"
#include <algorithm>

using Yaz0::Creator;

void Creator::writeFileHeader()
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

u32 Creator::writeValue(u8 value)
{
    addToHeader(Chunk_Type::Value);
    bufferOut->push_back(value);
    return moveOffset(1);
}

u32 Creator::writeCopy(u32 offset, u32 length)
{
    addToHeader(Chunk_Type::Copy);

    u32 offsetDiff = 0;
    if(offset < bufferInPos)
        offsetDiff = bufferInPos - offset - 1;

    bufferOut->insert(bufferOut->end(), 3, 0);
    auto buff = bufferOut->end() - 3;

    buff[0] = (offsetDiff >> 8) & 0xF;
    buff[1] = offsetDiff & 0xFF;

    if(length > 0x11) // 3 bytes
    {
        buff[2] = (length - 0x12) & 0xFF;
    }else{ // 2 bytes
        buff[0] |= ((length - MIN_COPY_SIZE) & 0xF) << 4;
        bufferOut->pop_back();
    }

    return moveOffset(length);
}

void Creator::createHeader()
{
    if(headerOffset != 0)
        writeHeader();

    headerOffset = bufferOut->size();
    headerPos = 7;
    bufferOut->push_back(0);
}

void Creator::addToHeader(Yaz0::Chunk_Type type)
{
    if(headerPos < 0) {
        //printf("%c%c%c%c%c%c%c%c\n", ((headerValue >> 7) & 1) ? '1' : 'C',((headerValue >> 6) & 1) ? '1' : 'C',((headerValue >> 5) & 1) ? '1' : 'C',((headerValue >> 4) & 1) ? '1' : 'C',((headerValue >> 3) & 1) ? '1' : 'C',((headerValue >> 2) & 1) ? '1' : 'C',((headerValue >> 1) & 1) ? '1' : 'C',((headerValue >> 0) & 1) ? '1' : 'C');
        createHeader();
    }

    headerValue |= (u8)type << headerPos;
    --headerPos;
}

void Creator::writeHeader()
{
    if(headerPos != 7)
    {
        (*bufferOut)[headerOffset] = headerValue;
        headerValue = 0;
    }
}

void Creator::writeChunk(Chunk chunk)
{
    switch(chunk.type)
    {
        case Chunk_Type::Repeat:
            writeValue(chunk.value);

        case Chunk_Type::Copy: 
            writeCopy(chunk.offset, chunk.length);
        break;
        case Chunk_Type::Value: 
            writeValue(chunk.value);
        break;
        default: /* ignore others */
        break;
    }
}

u32 Creator::moveOffset(u32 num)
{
    bufferInPos += num;
    return num;
}

void Creator::encodeByte()
{
    auto bestChunk = Yaz0::Chunk(bufferIn[bufferInPos]);
    u32 bestByteSize = 0;
    // search same bytes
    /*
    auto chunk = searchRepeatingBytes();
    chunk.print();
    if(chunk.type == Yaz0::Chunk_Type::Repeat)
    {
        // WIP
        //writeValue(bufferIn[bufferInPos]);
        //writeCopy(bufferInPos, sameByteCount);
        return;
    }
    */


    // scan for previous bytes
    u32 maxOffset = std::min(bufferInPos, MAX_OFFSET);
    s32 searchOffsetMax = bufferInPos - 1; // maximum value offset to go to
    for(u32 scanBackOffset=1; scanBackOffset<=maxOffset; ++scanBackOffset) // scan through previous bytes
    {
        auto chunk = searchCopyBytes(scanBackOffset, searchOffsetMax);
        if(!chunk.empty() && chunk.length > bestByteSize)
        {
            bestChunk = chunk;
            bestByteSize = chunk.length;
            //printf("best copy with: %d @%#04x\n", bestByteSize, bufferInPos);
            //break;
        }
    }

    writeChunk(bestChunk);
}

Yaz0::Chunk Creator::searchCopyBytes(u32 scanBackOffset, u32 searchOffsetMax)
{
    auto chunk = Chunk();
    s32 searchOffset = bufferInPos - scanBackOffset;

    if(searchOffset < 0)
        return chunk;

    if(bufferIn[searchOffset] == bufferIn[bufferInPos]) // same byte found
    {
        // check next 3 bytes
        for(u32 sameByteCount=0; sameByteCount<MAX_COPY_LENGTH; ++sameByteCount) // go forward again until a byte dosn't match
        {
            u32 currentSearchOffset = searchOffset + sameByteCount;
            if(currentSearchOffset > searchOffsetMax)
                currentSearchOffset = searchOffsetMax;

            u32 currentValueOffset  = bufferInPos + sameByteCount;

            if(currentValueOffset >= bufferInSize || bufferIn[currentSearchOffset] != bufferIn[currentValueOffset])
            {
                if(sameByteCount >= MIN_COPY_SIZE)
                {
                    //printf("   |--> @%#04x -> @%#04x-%#04x -> found (%d) => %#02x \n", bufferInPos, searchOffset, searchOffset + sameByteCount-1, sameByteCount, bufferIn[searchOffset]);    
                    chunk = Chunk(searchOffset, sameByteCount);                    
                }
                return chunk;
            }
        }
    }

    return chunk;
}

Yaz0::Chunk Creator::searchRepeatingBytes()
{
    for(u32 scanOffset=bufferInPos; scanOffset<bufferInSize; ++scanOffset)
    {
        if(bufferIn[scanOffset] != bufferIn[bufferInPos])
        {
            u32 sameByteCount = scanOffset - bufferInPos;
            if(sameByteCount > MIN_COPY_SIZE)
            {
                auto chunk = Chunk(
                    bufferIn[bufferInPos],
                    scanOffset, sameByteCount-1
                );
                chunk.print();
                return chunk;
            }
            break;
        }
    }

    auto chunk = Chunk();
    return chunk;
}

bool Creator::encode(u8* buffer, u32 bufferSize, s32 dataSize)
{
    bufferIn = buffer;
    bufferInSize = dataSize >= 0 ? dataSize : bufferSize;

    writeFileHeader();

    createHeader();

    for(bufferInPos=0; bufferInPos<bufferInSize;)
        encodeByte();

    writeHeader(); // write a (maybe) uncompleted header

    return true;
}

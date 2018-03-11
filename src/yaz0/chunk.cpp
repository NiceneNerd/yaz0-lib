/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

#include "../../include/main_header.h"

using Yaz0::Chunk;
using Yaz0::Chunk_Type;

Chunk::Chunk()
: type(Chunk_Type::Empty) 
{};

Chunk::Chunk(Chunk_Type type)
: type(type) 
{};

Chunk::Chunk(u8 value)
: type(Chunk_Type::Value), value(value) 
{};

Chunk::Chunk(u32 offset, u32 length)
: type(Chunk_Type::Copy), offset(offset), length(length) 
{};

Chunk::Chunk(u8 value, u32 offset, u32 length)
: type(Chunk_Type::Repeat), value(value), offset(offset), length(length) 
{};

u32 Chunk::getSize()
{
    switch(type)
    {
        case Chunk_Type::Copy:
            return length;
        
        case Chunk_Type::Repeat:
            return length + 1;

        case Chunk_Type::Value:
            return 1;

        default:
            return 0;
    }
}

bool Chunk::empty()
{
    return type == Chunk_Type::Empty;
}

void Chunk::print()
{
    printf("Chunk: ");
    switch(type)
    {
        case Chunk_Type::Copy  : printf("Copy");   break;
        case Chunk_Type::Repeat: printf("Repeat"); break;
        case Chunk_Type::Value : printf("Value");  break;
        default                : printf("Empty");  break;
    }
    printf(", val:%#02x, offset:%#02x, length:%d\n", value, offset, length);
}
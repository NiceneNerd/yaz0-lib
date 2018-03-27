/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

namespace Yaz0
{
    enum class Chunk_Type { Copy, Value, Repeat, Empty };

    struct Chunk
    {
        Chunk_Type type = Chunk_Type::Empty;

        u8 value;
        u32 offset;
        u32 length;

        Chunk();
        Chunk(Chunk_Type type);
        Chunk(u8 value);
        Chunk(u32 offset, u32 length);
        Chunk(u8 value, u32 offset, u32 length);

        s32 getSavedSize();
        bool empty();

        void print();
    };
};

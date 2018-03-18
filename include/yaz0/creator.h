/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/
namespace Yaz0
{
    class Creator : public Base
    {
        private:
            const u32 MAX_OFFSET = 4095;
            const u32 MAX_COPY_LENGTH = 273;
            const u32 MIN_COPY_SIZE = 3;

            int headerPos    = 0;
            int headerOffset = 0;
            u8 headerValue   = 0;

            void writeFileHeader();

            // helper functions
            u32 writeValue(u8 value);
            u32 writeCopy(u32 offset, u32 length);

            void createHeader();
            void addToHeader(Chunk_Type type);
            void writeHeader();

            void writeChunk(Chunk chunk);

            u32 moveOffset(u32 num);

            // encoding logic

            /**
             * encodes the next byte, it may encode more bytes if it finds a copy 
             */
            void encodeByte();

            /**
             * searches for copies of the current byte(s) in the previous bytes
             */
            Chunk searchCopyBytes(u32 scanBackOffset);

            /**
             * searches only if the current byte is repeated right after it
             */
            Chunk searchRepeatingBytes();
            
        public:
            bool encode(u8* buffer, u32 bufferSize, s32 dataSize = -1);
    };
};
/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

namespace Yaz0
{
    class Parser : public Base
    {
        private:
            const u32 HEADER_SIZE = 16;

            u8 flags[8];
            u8 chunks[3];

            bool writeOut(u8 val);
            u8 readIn();

            bool parseBlock();

        public:
            bool decode(u8* buffer, u32 bufferSize, s32 dataSize = -1);
    };
};
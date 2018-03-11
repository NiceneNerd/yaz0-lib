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
            u8 flags[8];
            u8 chunks[3];

            bool writeOut(u8 val);
            u8 readIn();

            bool parseBlock();

        public:
            Parser(std::vector<u8> *bufferOut);

            bool decode(u8* buffer, u32 bufferSize, s32 dataSize = -1);
    };
};
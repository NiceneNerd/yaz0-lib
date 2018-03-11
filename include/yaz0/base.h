/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/
namespace Yaz0
{
    class Base
    {
        private:

        protected:
            u32 MAX_FILE_SIZE = 256 * 1024 * 1024; // 256 MB

            u8* bufferIn     = nullptr;
            u32 bufferInSize = 0;
            u32 bufferInPos  = 0;

            std::vector<u8> *bufferOut = nullptr;
            u32 bufferOutSize = 0;
            u32 bufferOutPos  = 0;

        public:
            Base(std::vector<u8> *bufferOut);

            u8* getData();
            u32 getSize();

    };
};
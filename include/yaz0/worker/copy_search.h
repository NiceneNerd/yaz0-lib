/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

namespace Yaz0 { namespace Worker
{
    class Copy_Search
    {
        private:
            bool isRunning = false;
            std::thread worker;

            u8* dataPtr = nullptr;
            u32 offsetStart = 0;
            u32 offsetEnd   = 0;

            bool dataReady = false;
            
            void runtime();

        public:
            void start();
            void stop();

            void startSearching();
            bool isReady();
            int getChunk();
    };
}}
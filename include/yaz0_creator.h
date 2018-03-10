/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/
class YAZ0_Creator : public YAZ0_Base
{
    private:
        const int MAX_OFFSET = 4095;
        const int MAX_COPY_LENGTH = 273;

        enum class Type { Copy, Value };

        int headerPos = 0;

        void writeValue(u8 value);
        void writeCopy(u32 offset, u32 length);
        void addToHeader(Type types);

        u32 encodeByte(u32 valueOffset);

    public:
        YAZ0_Creator(std::vector<u8> *bufferOut) : YAZ0_Base(bufferOut) {}

        bool encode(u8* buffer, u32 bufferSize, s32 dataSize = -1);
};
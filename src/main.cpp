/**
* C++ implementation of Yaz0
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

#include "../include/main_header.h"

void buffer_delete_callback(char* data, void* vector)
{
    delete reinterpret_cast<std::vector<u8> *> (vector);
}

NAN_METHOD(decode)
{
    if(info.Length() < 1)
    {
        Nan::ThrowTypeError("Wrong number of arguments (1 or 2 needed)");
        return;
    }

    int dataSize = -1;
    auto bufferInObj = info[0]->ToObject();

    if(info.Length() >= 2)
    {
        dataSize = (int)info[1]->NumberValue();
    }

    u8* bufferIn = (u8*)node::Buffer::Data(bufferInObj);
    auto bufferInSize = node::Buffer::Length(bufferInObj);

    std::vector<u8> *bufferOut = new std::vector<u8>();

    auto yaz0 = Yaz0::Parser(bufferOut);
    yaz0.decode(bufferIn, bufferInSize, dataSize);

    info.GetReturnValue().Set(Nan::NewBuffer((char*)yaz0.getData(), yaz0.getSize(), buffer_delete_callback, bufferOut).ToLocalChecked());
}

NAN_METHOD(encode)
{
    if(info.Length() < 1)
    {
        Nan::ThrowTypeError("Wrong number of arguments (1 or 2 needed)");
        return;
    }

    int dataSize = -1;
    auto bufferInObj = info[0]->ToObject();

    if(info.Length() >= 2)
    {
        dataSize = (int)info[1]->NumberValue();
    }

    u8* bufferIn = (u8*)node::Buffer::Data(bufferInObj);
    auto bufferInSize = node::Buffer::Length(bufferInObj);

    std::vector<u8> *bufferOut = new std::vector<u8>();

    auto yaz0 = Yaz0::Creator(bufferOut);
    yaz0.encode(bufferIn, bufferInSize, dataSize);

    info.GetReturnValue().Set(Nan::NewBuffer((char*)yaz0.getData(), yaz0.getSize(), buffer_delete_callback, bufferOut).ToLocalChecked());
}

NAN_MODULE_INIT(Initialize)
{
    NAN_EXPORT(target, decode);
    NAN_EXPORT(target, encode);
}

NODE_MODULE(yaz0, Initialize);

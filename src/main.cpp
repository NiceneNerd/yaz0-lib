/**
* C++ implementation of Yaz0
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

#include "../include/main_header.h"

napi_value Yaz0_Encode(napi_env env, napi_callback_info info) 
{
    size_t argc = 2;
    napi_value argv[2];

    if(napi_get_cb_info(env, info, &argc, argv, NULL, NULL) != napi_ok)
    {
        napi_throw_error(env, NULL, "Failed to parse arguments");
        return nullptr;
    }

    int32_t bufferSizeLimit = -1;
    u8* bufferIn = nullptr;
    size_t bufferInSize = 0;

    if(napi_get_buffer_info(env, argv[0], (void**)&bufferIn, &bufferInSize) != napi_ok)
    {
        napi_throw_error(env, NULL, "Invalid Buffer was passed as argument");
        return nullptr;
    }

    if(argc > 1)
    {
        if(napi_get_value_int32(env, argv[1], &bufferSizeLimit) != napi_ok)
        {
            napi_throw_error(env, NULL, "Can't parse the buffer limit value");
            return nullptr;
        }
    }
    
    auto yaz0 = Yaz0::Creator();
    yaz0.encode(bufferIn, bufferInSize, bufferSizeLimit);

    napi_value resultBuffer;
    void* createdBuffer;

    if(napi_create_buffer_copy(env, yaz0.getSize(), (void**)yaz0.getData(), &createdBuffer, &resultBuffer) != napi_ok)
    {
        napi_throw_error(env, NULL, "Unable to create Buffer");
        return nullptr;
    }

    return resultBuffer;
}

napi_value Yaz0_Decode(napi_env env, napi_callback_info info) 
{
    size_t argc = 2;
    napi_value argv[2];

    if(napi_get_cb_info(env, info, &argc, argv, NULL, NULL) != napi_ok)
    {
        napi_throw_error(env, NULL, "Failed to parse arguments");
        return nullptr;
    }

    int32_t bufferSizeLimit = -1;
    u8* bufferIn = nullptr;
    size_t bufferInSize = 0;

    if(napi_get_buffer_info(env, argv[0], (void**)&bufferIn, &bufferInSize) != napi_ok)
    {
        napi_throw_error(env, NULL, "Invalid Buffer was passed as argument");
        return nullptr;
    }

    if(argc > 1)
    {
        if(napi_get_value_int32(env, argv[1], &bufferSizeLimit) != napi_ok)
        {
            napi_throw_error(env, NULL, "Can't parse the buffer limit value");
            return nullptr;
        }
    }

    auto yaz0 = Yaz0::Parser();
    if(!yaz0.decode(bufferIn, bufferInSize, bufferSizeLimit))
    {
        napi_throw_error(env, NULL, "Error parsing file");
        return nullptr;
    }

    napi_value resultBuffer;
    void* createdBuffer;

    if(napi_create_buffer_copy(env, yaz0.getSize(), (void**)yaz0.getData(), &createdBuffer, &resultBuffer) != napi_ok)
    {
        napi_throw_error(env, NULL, "Unable to create Buffer");
        return nullptr;
    }

    return resultBuffer;
}

napi_value Init(napi_env env, napi_value exports) 
{
    //napi_status status;

    napi_value fnEncode;
    if(napi_create_function(env, NULL, 0, Yaz0_Encode, NULL, &fnEncode) != napi_ok) 
        napi_throw_error(env, NULL, "Unable to wrap native function");

    if(napi_set_named_property(env, exports, "encode", fnEncode) != napi_ok) 
        napi_throw_error(env, NULL, "Unable to populate exports");


    napi_value fnDecode;
    if(napi_create_function(env, NULL, 0, Yaz0_Decode, NULL, &fnDecode) != napi_ok) 
        napi_throw_error(env, NULL, "Unable to wrap native function");

    if(napi_set_named_property(env, exports, "decode", fnDecode) != napi_ok) 
        napi_throw_error(env, NULL, "Unable to populate exports");

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)

/*
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
*/
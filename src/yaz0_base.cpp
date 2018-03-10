/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

#include "../include/main_header.h"

YAZ0_Base::YAZ0_Base(std::vector<u8> *bufferOut)
: bufferOut(bufferOut)
{
    
}

u8* YAZ0_Base::getData()
{
    return bufferOut->data();
}

u32 YAZ0_Base::getSize()
{
    return (u32)bufferOut->size();
}

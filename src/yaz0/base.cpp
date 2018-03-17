/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

#include "../../include/main_header.h"

u8* Yaz0::Base::getData()
{
    return bufferOut.data();
}

u32 Yaz0::Base::getSize()
{
    return (u32)bufferOut.size();
}

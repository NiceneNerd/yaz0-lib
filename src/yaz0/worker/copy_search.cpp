/**
* @copyright 2018 - Max Bebök
* @author Max Bebök
* @license GNU-GPLv3 - see the "LICENSE" file in the root directory
*/

#include "../../../include/main_header.h"
#include <chrono>

using Yaz0::Worker::Copy_Search;

void Copy_Search::runtime()
{
    while(isRunning)
    {
        printf("eee\n");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void Copy_Search::start()
{
    if(!isRunning)
    {
        worker = std::thread([=] { runtime(); });
    }
    isRunning = true;
}

void Copy_Search::stop()
{
    isRunning = false;
    worker.join();
}

void Copy_Search::startSearching()
{

}

bool Copy_Search::isReady()
{

}

int Copy_Search::getChunk()
{

}

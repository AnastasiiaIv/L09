#include <string>
#include <fstream>
#include <queue>

#include <boost/log/trivial.hpp>

#include "core/LogSetup.h"
#include "CrawlerData.h"
#include "Page.h"
#include "Utils.h"



int main(int argc, char *argv[])
{
    LogSetup::init();
    BOOST_LOG_TRIVIAL(debug) << "Log setup complete";

    if (auto returnValue = programArguments(argc, argv); returnValue != 0) {
        return returnValue;
    }

    auto&&[host, target] = divideIntoHostAndTarget(CrawlerData::url);
    BOOST_LOG_TRIVIAL(debug) << "Initial Host: " << host << " target:" << target;

    ThreadPool downloaders{CrawlerData::networkThreads};
    ThreadPool parsers{CrawlerData::parserThreads};
    BOOST_LOG_TRIVIAL(debug) << "Initialized thread pools";

    ThreadData::ImageList imageList;
    std::atomic<size_t> parserAmount{1};
    std::mutex globalMutex;
    Downloader downloader{host, "443"};

    ThreadData threadData{
        downloader,
        downloaders,
        parsers,
        imageList,
        globalMutex,
        parserAmount,
    };

    globalMutex.lock();
    downloaders.enqueue(download, Page{target, CrawlerData::depth}, std::ref(threadData));

    globalMutex.lock();     // And wait until all parsers end
    std::ofstream(CrawlerData::output);
    imageList.
    imageList.pop()
}
#pragma once

#include <string>
#include <tuple>
#include <boost/lockfree/stack.hpp>

#include "core/Parser.h"
#include "core/Downloader.h"
#include "core/ThreadPool.h"
#include "Page.h"

struct ThreadData
{
    using ImageList = boost::lockfree::stack<std::string, boost::lockfree::capacity<500>>;

    Downloader &downloader;
    ThreadPool &downloaders;
    ThreadPool &parsers;
    ImageList &imageList;
    std::mutex &globalMutex;
    std::atomic<size_t> &parserAmount;
};

int programArguments(int argc, char *argv[]);

std::tuple<std::string, std::string> divideIntoHostAndTarget(const std::string &string);

void afterParse(const Page &page, ThreadData &data);

void parse(const PageDownloaded &page, ThreadData &data);

void afterDownload(const PageDownloaded &page, ThreadData &data);

void download(const Page &page, ThreadData &data);


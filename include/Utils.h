#pragma once

#include <string>
#include <fstream>
#include <tuple>
#include <boost/lockfree/stack.hpp>

#include "core/Parser.h"
#include "core/Downloader.h"
#include "core/ThreadPool.h"
#include "CrawlerData.h"
#include "Page.h"

struct ThreadData
{
    using ImageContainer = std::queue<std::string>;

    Downloader &downloader;
    ThreadPool &downloaders;
    ThreadPool &parsers;
    ImageContainer &imageContainer;
    std::mutex &globalMutex;
    std::atomic<size_t> &parserAmount;

    std::mutex containerMutex{};
};

int programArguments(int argc, char *argv[]);

std::tuple<std::string, std::string> divideIntoHostAndTarget(const std::string &string);

void afterParse(const Page &page, ThreadData &data);

void parse(const PageDownloaded &page, ThreadData &data);

void afterDownload(const PageDownloaded &page, ThreadData &data);

void download(const Page &page, ThreadData &data);

void containerToFileWithFilter(ThreadData::ImageContainer &container);
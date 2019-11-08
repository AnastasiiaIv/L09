#include <string>
#include <boost/program_options.hpp>
#include <iostream>

#include "CrawlerData.h"

int main(int argc, char *argv[])
{
    namespace options = boost::program_options;

    options::options_description allOptions("Available options");
    allOptions.add_options()
        ("url",
         options::value<std::string>(&CrawlerData::url),
         "URL to web page")
        ("depth",
         options::value<size_t>(&CrawlerData::depth)->default_value(5),
            "Search depth")
        ("network_threads",
         options::value<size_t>(&CrawlerData::networkThreads)->default_value(2),
         "Downloader thread amount")
        ("parser_threads",
         options::value<size_t>(&CrawlerData::parserThreads)->default_value(2),
         "Parser thread amount")
        ("output",
         options::value<std::string>(&CrawlerData::output),
            "Path to output file")
        ("help", "Print help message");

    options::variables_map variablesMap;
    options::store(options::parse_command_line(argc, argv, allOptions), variablesMap);
    options::notify(variablesMap);

    if (variablesMap.count("help")) {
        std::cout << allOptions << "\n";
        return 1;
    }
}
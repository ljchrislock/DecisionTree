#ifndef CSV_UTILS_H
#define CSV_UTILS_H

#include <vector>
#include <map>
#include <string>

namespace CsvUtils
{
    std::vector<std::map<std::string, std::string>> readCSV(const std::string &filename);
}

#endif

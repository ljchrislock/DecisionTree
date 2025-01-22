#include "CsvUtils.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<std::map<std::string, std::string>> CsvUtils::readCSV(const std::string &filename)
{
    std::vector<std::map<std::string, std::string>> data;
    std::ifstream file(filename);
    std::string line, cell;

    std::vector<std::string> headers;

    if (std::getline(file, line))
    {
        // Check for BOM and remove it if present
        if (line.substr(0, 3) == "\xEF\xBB\xBF")
        {
            line = line.substr(3); // Remove BOM characters from the start
        }

        // Read the header line
        std::stringstream ss(line);
        while (std::getline(ss, cell, ','))
            headers.push_back(cell);
    }

    // Process each line as a row of data
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::map<std::string, std::string> instance;
        for (const auto &header : headers)
        {
            if (!std::getline(ss, cell, ','))
                break;
            instance[header] = cell;
        }
        data.push_back(instance);
    }
    return data;
}
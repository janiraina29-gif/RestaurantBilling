#include "FileManager.h"

#include <cstdio>
#include <fstream>
#include <stdexcept>

// A folder is usable if we can open (and, if needed, create) a file inside it.
bool FileManager::canWriteInto(const std::string& dir)
{
    std::ofstream probe((dir + "meta.txt").c_str(), std::ios::app);
    return probe.is_open();
}

FileManager::FileManager()
    : dataDir("")
{
    if (canWriteInto("Data/")) {
        dataDir = "Data/";
    } else if (canWriteInto("../Data/")) {
        dataDir = "../Data/";
    }
}

bool FileManager::isReady() const { return !dataDir.empty(); }

const std::string& FileManager::getDataDir() const { return dataDir; }

bool FileManager::readLines(const std::string& fileName, std::vector<std::string>& lines) const
{
    lines.clear();
    if (!isReady()) {
        return false;
    }
    std::ifstream in((dataDir + fileName).c_str());
    if (!in.is_open()) {
        return false;
    }
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty() && line[line.size() - 1] == '\r') {
            line.erase(line.size() - 1);   // tolerate Windows line endings
        }
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    return true;
}

void FileManager::writeLines(const std::string& fileName, const std::vector<std::string>& lines) const
{
    if (!isReady()) {
        throw std::runtime_error("Data folder not found. Create a folder named Data next to the program.");
    }
    std::string target = dataDir + fileName;
    std::string temp = target + ".tmp";

    {
        std::ofstream out(temp.c_str());
        if (!out.is_open()) {
            throw std::runtime_error("Cannot create " + temp);
        }
        for (size_t i = 0; i < lines.size(); i++) {
            out << lines[i] << '\n';
        }
        out.flush();
        if (!out.good()) {
            throw std::runtime_error("Write failed for " + temp + " (disk full?)");
        }
    }   // the file is closed here

    std::remove(target.c_str());   // ignore failure: the file may not exist yet
    if (std::rename(temp.c_str(), target.c_str()) != 0) {
        throw std::runtime_error("Cannot replace " + target);
    }
}

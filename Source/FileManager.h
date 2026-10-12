#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>

// The only class that touches the disk. It knows WHERE the data folder is and
// how to read/write text files line by line. It knows nothing about menu items,
// bills, etc. - the entity classes turn themselves into lines (toLine/fromLine).
class FileManager {
private:
    std::string dataDir;   // "Data/" or "../Data/" (empty if not found)

    static bool canWriteInto(const std::string& dir);

public:
    // Looks for the Data folder: first ./Data/ (run from the project folder),
    // then ../Data/ (run from inside the Executable folder).
    FileManager();

    bool isReady() const;
    const std::string& getDataDir() const;

    // Returns false if the file does not exist (normal on the first run).
    bool readLines(const std::string& fileName, std::vector<std::string>& lines) const;

    // Writes to a temporary file first, then replaces the real file,
    // so a failed write cannot destroy the previous good copy.
    // Throws std::runtime_error on any failure.
    void writeLines(const std::string& fileName, const std::vector<std::string>& lines) const;
};

#endif

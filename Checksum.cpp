#include "Checksum.h"

uint64_t simpleHash(const std::string& data) {
    uint64_t hash = 5381;
    for (unsigned char c : data) {
        hash = ((hash << 5) + hash) ^ c;
    }
    return hash;
}

std::string getFileContents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error while opening the file: " << filename << "\n";
        return "";
    }

    std::string content;
    std::string line;

    while (std::getline(file, line)) {
        content += line + "\n";
    }

    return content;
}

void writeChecksum(const std::string& filename, uint64_t checksum) {
    std::ofstream out(filename + ".hash");
    out << checksum << " " << filename << "\n";
    out.close();
}

bool verifyChecksum(const std::string& filename) {
    std::ifstream in(filename + ".hash");
    if (!in) {
        std::cerr << "No file with checksum" << std::endl;
        return false;
    }

    uint64_t expected;
    std::string fname_in_hash;
    in >> expected >> fname_in_hash;

    std::string data = getFileContents(filename);
    uint64_t actual = simpleHash(data);

    return actual == expected;
}

bool createChecksumFile(const std::string& filename) {
    std::string data = getFileContents(filename);
    if (data.empty()) {
        return false;
    }

    uint64_t checksum = simpleHash(data);

    std::ofstream out(filename + ".hash");
    if (!out) {
        std::cerr << "Couldn't open the checksum file" << std::endl;
        return false;
    }

    out << checksum << " " << filename << "\n";
    std::cout << "The checksum was successfully created " << checksum << std::endl;
    return true;
}
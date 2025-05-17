#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <cstdint>

uint64_t simpleHash(const std::string&);

std::string getFileContents(const std::string&);

void writeChecksum(const std::string&, uint64_t);

bool verifyChecksum(const std::string&);

bool createChecksumFile(const std::string&);
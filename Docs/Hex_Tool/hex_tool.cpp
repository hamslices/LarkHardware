/**
 * @file    hex_tool.cpp
 * @author  HamSlices 2025
 * @version 2.0
 *
 * @brief   A command-line utility to convert Intel HEX files to raw binary files.
 *
 * @details This tool extracts data from a specified Intel HEX file, retaining only
 *          the bytes that fall within a given start address and size range. It
 *          pads any unspecified memory locations within this range with 0xFF.
 *
 *          After generating the binary file, it calculates a 32-bit CRC checksum
 *          of the output data. This hash is printed to the console and is used by
 *          the post-build script to verify a successful firmware flash.
 *
 * @usage   hex_tool <input.hex> <output.bin> <start_addr_hex> <size_hex>
 * @example hex_tool app.hex app.bin 0x08000000 0xE738
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <map>
#include <iomanip>
#include <algorithm>

 // The CRC32 polynomial used in the firmware for hash calculation.
constexpr uint32_t POLYNOMIAL = 0xEDB88320u;

/**
 * @brief Calculates the CRC32 hash of a block of data.
 * @param data The vector of bytes to be hashed.
 * @return The 32-bit CRC hash.
 */
uint32_t crc32(const std::vector<uint8_t>& data) {
    uint32_t crc = 0xFFFFFFFFu;

    for (uint8_t byte : data) {
        crc ^= byte;
        for (int j = 0; j < 8; j++) {
            if (crc & 1u)
                crc = (crc >> 1) ^ POLYNOMIAL;
            else
                crc >>= 1;
        }
    }

    return crc ^ 0xFFFFFFFFu;
}

/**
 * @brief Prints the command-line usage instructions.
 */
void printUsage() {
    std::cout << "Usage: hex_tool <input.hex> <output.bin> <start_addr_hex> <size_hex>\n";
    std::cout << "Example: hex_tool app.hex bank1.bin 0x08000000 0xE4F0\n";
}

int main(int argc, char* argv[]) {
    // Enforce a strict argument count of 5 (program name + 4 parameters).
    if (argc != 5) {
        printUsage();
        return 1;
    }

    // --- 1. Parse Command-Line Arguments ---
    std::string hexFilePath = argv[1];
    std::string binFilePath = argv[2];
    uint32_t startAddress = std::stoul(argv[3], nullptr, 16);
    uint32_t bankSize = std::stoul(argv[4], nullptr, 16);
    uint32_t endAddress = startAddress + bankSize;

    // --- 2. Read and Parse the Intel HEX File ---
    std::ifstream hexFile(hexFilePath);
    if (!hexFile.is_open()) {
        std::cerr << "Error: Could not open input file: " << hexFilePath << std::endl;
        return 1;
    }

    std::map<uint32_t, uint8_t> memoryMap;
    uint32_t extendedLinearAddress = 0;
    std::string line;

    while (std::getline(hexFile, line)) {
        if (line.empty() || line[0] != ':') continue;

        try {
            uint8_t byteCount = std::stoi(line.substr(1, 2), nullptr, 16);
            uint16_t addressOffset = std::stoi(line.substr(3, 4), nullptr, 16);
            uint8_t recordType = std::stoi(line.substr(7, 2), nullptr, 16);

            if (recordType == 0x04) { // Extended Linear Address Record
                uint16_t upperAddress = std::stoi(line.substr(9, 4), nullptr, 16);
                extendedLinearAddress = upperAddress << 16;
            }
            else if (recordType == 0x00) { // Data Record
                uint32_t currentAddress = extendedLinearAddress + addressOffset;
                for (uint8_t i = 0; i < byteCount; ++i) {
                    uint32_t byteAddress = currentAddress + i;
                    // Only store bytes that fall within our target address range.
                    if (byteAddress >= startAddress && byteAddress < endAddress) {
                        uint8_t byteValue = std::stoi(line.substr(9 + i * 2, 2), nullptr, 16);
                        memoryMap[byteAddress] = byteValue;
                    }
                }
            }
            else if (recordType == 0x01) { // End of File Record
                break;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Warning: Could not parse line '" << line << "'. Reason: " << e.what() << std::endl;
        }
    }
    std::cout << "Successfully parsed HEX file." << std::endl;

    // --- 3. Construct the Binary Data Vector ---
    // Initialize a vector of the final size, filled with 0xFF (default for erased flash).
    std::vector<uint8_t> binaryData(bankSize, 0xFF);
    bool dataFoundInRange = false;

    // Populate the vector with the data from our parsed memory map.
    for (const auto& pair : memoryMap) {
        uint32_t address = pair.first;
        if (address >= startAddress && address < endAddress) {
            dataFoundInRange = true;
            binaryData[address - startAddress] = pair.second;
        }
    }

    if (!dataFoundInRange) {
        std::cerr << "Error: No data found within the specified address range." << std::endl;
        return 1;
    }

    // --- 4. Write the Binary File ---
    std::ofstream binFile(binFilePath, std::ios::binary);
    if (!binFile.is_open()) {
        std::cerr << "Error: Could not create output file: " << binFilePath << std::endl;
        return 1;
    }
    binFile.write(reinterpret_cast<const char*>(binaryData.data()), binaryData.size());
    std::cout << "Successfully created binary file: " << binFilePath << std::endl;
    std::cout << "Size: " << binaryData.size() << " bytes" << std::endl;

    // --- 5. Calculate and Print the Final Hash ---
    uint32_t crc = crc32(binaryData);
    std::cout << "Generated Hash: 0x" // 1. Print the prefix as a literal string
        << std::hex                   // 2. Set mode to hex (sticky)
        << std::setw(8)               // 3. Set width for the NEXT item (non-sticky)
        << std::setfill('0')          // 4. Set fill character (sticky)
        << std::uppercase             // 5. Set case (sticky)
        << crc                        // 6. Print the number. setw(8) applies here.
        << std::endl;

    return 0;
}
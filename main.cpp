#include <iostream>
#include <fstream>
#include <vector>

// Function to read the binary file into a buffer
std::vector<uint8_t> readBinaryFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    // Read file into a buffer
    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(file), {});
    return buffer;
}

// Function to disassemble a single RISC-V instruction
std::string disassembleInstruction(uint32_t instruction) {

    return "NOP";
}

int main() {
    // Read the binary file into a buffer
    std::vector<uint8_t> buffer = readBinaryFile("your_binary_file.bin");

    // Loop through the buffer 4 bytes at a time to read 32-bit RISC-V instructions
    for (size_t i = 0; i < buffer.size(); i += 4) {
        uint32_t instruction = 0;
        // Combine 4 bytes into a single 32-bit integer
        for (int j = 0; j < 4; ++j) {
            instruction |= buffer[i + j] << (j * 8);
        }

        // Disassemble the instruction
        std::string asmMnemonic = disassembleInstruction(instruction);
        std::cout << asmMnemonic << std::endl;
    }

    return 0;
}
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Function to read the binary file into a buffer
std::vector<uint8_t> readBinaryFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(file), {});
    std::cout << "Successfully read binary file. Size: " << buffer.size() << " bytes." << std::endl;
    return buffer;
}

// Function to disassemble a single RISC-V instruction
std::string disassembleInstruction(uint32_t instruction) {
    std::cout << "Disassembling instruction: " << std::hex << instruction << std::dec << std::endl;

    uint8_t opcode = instruction & 0x7F;
    uint8_t funct3 = (instruction >> 12) & 0x7;
    uint8_t funct7 = (instruction >> 25) & 0x7F;
    uint8_t rs1 = (instruction >> 15) & 0x1F;
    uint8_t rs2 = (instruction >> 20) & 0x1F;
    uint8_t rd = (instruction >> 7) & 0x1F;

    std::string mnemonic = "UNKNOWN";

    switch (opcode) {
    case 0x33:
        if (funct3 == 0x0 && funct7 == 0x00) {
            mnemonic = "ADD";
        }
        else if (funct3 == 0x0 && funct7 == 0x20) {
            mnemonic = "SUB";
        }
        break;
    case 0x03:
        if (funct3 == 0x0) {
            mnemonic = "LB";
        }
        else if (funct3 == 0x1) {
            mnemonic = "LH";
        }
        else if (funct3 == 0x2) {
            mnemonic = "LW";
        }
        break;
    default:
        mnemonic = "UNKNOWN";
        break;
    }

    return mnemonic + " rd: " + std::to_string(rd) + ", rs1: " + std::to_string(rs1) + ", rs2: " + std::to_string(rs2);
}

int main() {
    std::vector<uint8_t> buffer = readBinaryFile("test_instructions.bin");

    for (size_t i = 0; i < buffer.size(); i += 4) {
        uint32_t instruction = 0;
        for (int j = 0; j < 4; ++j) {
            instruction |= buffer[i + j] << (j * 8);
        }

        std::cout << "Assembled 32-bit instruction: " << std::hex << instruction << std::dec << std::endl;
        std::string asmMnemonic = disassembleInstruction(instruction);
        std::cout << "Disassembled to: " << asmMnemonic << std::endl;
    }

    return 0;
}

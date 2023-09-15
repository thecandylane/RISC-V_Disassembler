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

    uint8_t opcode = instruction & 0x7F; //extracts last 7 bits
    uint8_t funct3 = (instruction >> 12) & 0x7; //shifts right 12 bits, extracts next 3 bits [14:12]
    uint8_t funct7 = (instruction >> 25) & 0x7F; //shifts right 25 bits, extracts next 7 bits [31:25]
    uint8_t rs1 = (instruction >> 15) & 0x1F; //shifts right 15, extracts next 5 bits [19:15]
    uint8_t rs2 = (instruction >> 20) & 0x1F; //shifts right 20, extracts next 5 bits[24:20]
    uint8_t rd = (instruction >> 7) & 0x1F; //shifts right 7, extracts next 5 bits[11:7]

    std::string mnemonic = "UNKNOWN";

    switch (opcode) {
    case 0x33: //R-type
        if (funct3 == 0x0 && funct7 == 0x00) {
            mnemonic = "ADD";
        }
        else if (funct3 == 0x0 && funct7 == 0x20) {
            mnemonic = "SUB";
        }
        //additional instructs
        break;
    case 0x03: //I-type load instructions
        if (funct3 == 0x0) {
            mnemonic = "LB";
        }
        else if (funct3 == 0x1) {
            mnemonic = "LH";
        }
        else if (funct3 == 0x2) {
            mnemonic = "LW";
        }
        //additional instructs
        break;
    case 0x23: //S-Type store instructions
        if (funct3 == 0x0) {
            mnemonic = "SB";
        }
        else if (funct3 == 0x1) {
            mnemonic = "SH";
        }
        else if (funct3 == 0x2) {
            mnemonic = "SW";
        }
        break;
    case 0x63: //B-Type branch instructions
        if (funct3 == 0x0) {
            mnemonic = "BEQ";
        }
        else if (funct3 == 0x1) {
            mnemonic = "BNE";
        }
        //additional B=Type
    case 0x6F: { //J-Type
        int32_t imm = (instruction & 0xFF000000) >> 20 | (instruction & 0x7FE00000) >> 9 |
            (instruction & 0x100000) >> 11 | (instruction & 0xFF000) >> 12;
        mnemonic = "JAL";
        return mnemonic + " rd: " + std::to_string(rd) + ", imm: " + std::to_string(imm);
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
        std::cout << "Processing bytes at position " << i << " to " << i + 3 << std::endl;

        uint32_t instruction = 0;
        for (int j = 0; j < 4; ++j) {
            uint8_t byte = buffer[i + j];
            std::cout << "Byte " << j << ": " << std::hex << static_cast<int>(byte) << std::dec << std::endl;

            instruction |= byte << (j * 8);
        }

        std::cout << "Assembled 32-bit instruction: " << std::hex << instruction << std::dec << std::endl;
        std::string asmMnemonic = disassembleInstruction(instruction);
        std::cout << "Disassembled to: " << asmMnemonic << std::endl;
    }

    return 0;
}


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

    int32_t imm;

    std::string mnemonic = "UNKNOWN";

    switch (opcode) {
    case 0x33: //R-type
        if (funct3 == 0x0 && funct7 == 0x00) {
            mnemonic = "ADD";
        }
        else if (funct3 == 0x0 && funct7 == 0x20) { 
            mnemonic = "SUB";
        }
        else if (funct3 == 0x0 && funct7 == 0x01) {
            mnemonic = "MUL";
        }
        else if (funct3 == 0x1 && funct7 == 0x00) {
            mnemonic = "SLL";
        }
        else if (funct3 == 0x2 && funct7 == 0x00) {
            mnemonic = "SLT";
        }
        else if (funct3 == 0x3 && funct7 == 0x00) {
            mnemonic = "SLTU";
        }
        else if (funct3 == 0x4 && funct7 == 0x00) {
            mnemonic = "XOR";
        }
        else if (funct3 == 0x4 && funct7 == 0x01) {
            mnemonic = "DIV";
        }
        else if (funct3 == 0x5 && funct7 == 0x00) {
            mnemonic = "SRL";
        }
        else if (funct3 == 0x5 && funct7 == 0x20) {
            mnemonic = "SRA";
        }
        else if (funct3 == 0x6 && funct7 == 0x00) {
            mnemonic = "OR";
        }
        else if (funct3 == 0x7 && funct7 == 0x00) {
            mnemonic = "AND";
        }
        break;
    case 0x03: { //I-type load instructions 
        imm = (instruction >> 20) & 0XFFF;

        if (funct3 == 0x0) {
            mnemonic = "LB";
        }
        else if (funct3 == 0x1) {
            mnemonic = "LH";
        }
        else if (funct3 == 0x2) {
            mnemonic = "LW";
        }
        else if (funct3 == 0x4) {
            mnemonic = "LBU";
        }
        else if (funct3 == 0x5) {
            mnemonic = "LHU";
        }
    case 0x13: // I-type arithmetic
        imm = (instruction & 0xFFF00000) >> 20;
        uint8_t shamt = (instruction >> 20) & 0x1F;
        if (funct3 == 0x0) {
            mnemonic = "ADDI";
        }
        else if (funct3 == 0x2) {
            mnemonic = "SLTI";
        }
        else if (funct3 == 0x3) {
            mnemonic = "SLTIU";
        }
        else if (funct3 == 0x4) {
            mnemonic = "XORI";
        }
        else if (funct3 == 0x6) {
            mnemonic = "ORI";
        }
        else if (funct3 == 0x7) {
            mnemonic = "ANDI";
        }
        else if (funct3 == 0x1 && funct7 == 0x00) {
            mnemonic = "SLLI";
        }
        else if (funct3 == 0x5) 
            if (funct7 == 0x00) {
                mnemonic = "SRLI";
            }
            else if (funct7 == 0x20){
                mnemonic = "SRAI";
            }
        return mnemonic + " rd: " + std::to_string(rd) + ", rs1: " + std::to_string(rs1) + ", imm: " + std::to_string(imm);
        }
        break;
    case 0x23: //S-Type store instructions
        imm = ((instruction & 0xFE000000) >> 20) | ((instruction >> 7) & 0x1F);

        if (funct3 == 0x0) {
            mnemonic = "SB";
        }
        else if (funct3 == 0x1) {
            mnemonic = "SH";
        }
        else if (funct3 == 0x2) {
            mnemonic = "SW";
        }
        return mnemonic + " rd: " + std::to_string(rd) + ", rs1: " + std::to_string(rs1) + ", imm: " + std::to_string(imm);
        break;
    case 0x63: //B-Type branch instructions
        imm = ((instruction & 0x80000000) >> 19) | ((instruction & 0x7E000000) >> 20) |
            ((instruction >> 7) & 0x1E) | ((instruction >> 7) & 0x01);
        if (funct3 == 0x0) {
            mnemonic = "BEQ";
        }
        else if (funct3 == 0x1) {
            mnemonic = "BNE";
        }
        else if (funct3 == 0x4) {
            mnemonic = "BLT";
        }
        else if (funct3 == 0x5) {
            mnemonic = "BGE";
        }
        else if (funct3 == 0x6) {
            mnemonic = "BLTU";
        }
        else if (funct3 == 0x7) {
            mnemonic = "BGEU";
        }
        //additional B=Type
    case 0x6F: { //J-Type
        imm = ((instruction & 0xFF000000) >> 20) | ((instruction & 0x7FE00000) >> 9) |
            ((instruction & 0x100000) >> 11) | ((instruction & 0xFF000) >> 12);
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


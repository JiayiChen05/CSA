#include <cstdint>
#include <iostream>

struct instruction
{
    uint8_t opcode; // 7bits
    uint8_t rd;
    uint8_t funct3;
    uint8_t rs1;
    uint8_t rs2;
    uint8_t funct7;
    uint8_t imm;
    std::string type;
};

int32_t registers[32];
int32_t PC = 0;
bool HALT = false;

struct IF_ID{// 传递instruction和pc
    uint32_t instruct = 0;
    uint32_t pc = 0; 
    bool nop = true;
};

struct ID_EX{
    uint32_t pc;// 0-2^31-1
    uint8_t rd;//0-255
    uint8_t rs1;
    uint8_t rs2;
    int32_t rs1_val;// -2^31 ~ 2^31-1
    int32_t rs2_val;
    int32_t imm;
    uint8_t opcode;
    uint8_t funct3;
    uint8_t funct7;
    std::string type;
    bool nop = true;
};

struct EX_MEM{
    int32_t alu_result;
    int32_t rs2_val;
    uint8_t rd;
    uint32_t pc;
    uint8_t opcode;
    std::string type;
    bool nop = true;
};

struct MEM_WB{
    int32_t alu_result;
    int32_t memeory_data;
    uint8_t rd;
    uint8_t opcode;
    std::string type;
    bool nop = true;
};

IF_ID if_id_reg;


void fetch(){
    if(if_id_reg.nop){
        return;
    }else{
        if_id_reg.instruct =  imem.txt[PC];
        if_id_reg.pc = PC;
        if(if_id_reg.instruct == 0xFFFFFFFF){
            HALT = true;
            if_id_reg.nop = true;
            return;
        }
        PC+= 4;// 因为32bits = 4bytes;才能完整跳到下一个指令
    }
}

ID_EX id_ex_reg;

instruction decode(uint32_t raw_intruct);

void decode_instruction(){
    if(id_ex_reg.nop){
        return;
    }else{
        instruction instruct = decode(if_id_reg.instruct);
        id_ex_reg.rs1 = instruct.rs1;
        id_ex_reg.rs2 = instruct.rs2;
        id_ex_reg.rd = instruct.rd;
        id_ex_reg.imm = instruct.imm;
        id_ex_reg.funct3 = instruct.funct3;
        id_ex_reg.funct7 = instruct.funct7;
        id_ex_reg.opcode = instruct.opcode;
        id_ex_reg.rs1_val = registers[instruct.rs1];
        id_ex_reg.rs2_val = registers[instruct.rs2];
    }
}

EX_MEM ex_mem_reg;

void execute(){
    if(ex_mem_reg.nop){
        return;
    }else{
        ex_mem_reg.rd = id_ex_reg.rd;
        ex_mem_reg.opcode = id_ex_reg.opcode;
        ex_mem_reg.pc = id_ex_reg.pc;
        ex_mem_reg.rs2_val = id_ex_reg.rs2_val;
        ex_mem_reg.type = id_ex_reg.type;
        if(ex_mem_reg.type == "R"){
            if(id_ex_reg.funct3 == 0x0){
                if(id_ex_reg.funct7 == 0x00){
                    ex_mem_reg.alu_result = id_ex_reg.rs1_val + id_ex_reg.rs2_val;
                }else if(id_ex_reg.funct7 = 0x20){
                    ex_mem_reg.alu_result = id_ex_reg.rs1_val - id_ex_reg.rs2_val;
                }
            }else if(id_ex_reg.funct3 ==0x06){
                ex_mem_reg.alu_result = id_ex_reg.rs1_val&id_ex_reg.rs2_val;
            }else if(id_ex_reg.funct3 == 0x07){
                ex_mem_reg.alu_result = id_ex_reg.rs1_val|id_ex_reg.rs2_val;
            }else if(id_ex_reg.funct3 == 0x04){
                ex_mem_reg.alu_result = id_ex_reg.rs1_val^id_ex_reg.rs2_val;
            }
        }else if(ex_mem_reg.type == "I"){
            if(id_ex_reg.funct3 == 0x0){
                ex_mem_reg.alu_result = id_ex_reg.rs1_val + id_ex_reg.imm;
            }else if(id_ex_reg.funct3 ==0x04){
                ex_mem_reg.alu_result = id_ex_reg.rs1_val^id_ex_reg.imm;
            }else if(id_ex_reg.funct3 == 0x06){
                ex_mem_reg.alu_result = id_ex_reg.rs1_val|id_ex_reg.imm;
            }else if(id_ex_reg.funct3 == 0x07){
                ex_mem_reg.alu_result = id_ex_reg.rs1_val&id_ex_reg.imm;
            }
        }else if(ex_mem_reg.type == "S"){
            ex_mem_reg.alu_result = id_ex_reg.rs1_val + id_ex_reg.imm;
        }else if(ex_mem_reg.type == "B"){
            if(id_ex_reg.funct3 == 0x0){
               if(id_ex_reg.rs1_val == id_ex_reg.rs2_val){
                PC = id_ex_reg.pc+ id_ex_reg.imm;
               }
            }else if(id_ex_reg.funct3 == 0x01){
                if(id_ex_reg.rs1_val != id_ex_reg.rs2_val){
                    PC = id_ex_reg.pc+ id_ex_reg.imm;
                }
            }

        }else if(ex_mem_reg.type == "J"){
            ex_mem_reg.alu_result = id_ex_reg.pc + 4;
            PC = id_ex_reg.pc + id_ex_reg.imm;
        }
    }
}

MEM_WB mem_wb_reg;

void memory(){
    if(mem_wb_reg.nop){
        return;
    }else{
        mem_wb_reg.rd = ex_mem_reg.rd;
        mem_wb_reg.opcode = ex_mem_reg.opcode;
        mem_wb_reg.type = ex_mem_reg.type;
        mem_wb_reg.alu_result = ex_mem_reg.alu_result;
        if(mem_wb_reg.type == "I"){
            int address = ex_mem_reg.alu_result/4; // 1 word = 4 bytes
            if(address >= 0 && address < dmem.size()){
                mem_wb_reg.memeory_data = dmem[address];
            }else{
                mem_wb_reg.memeory_data = 0;
            }
        }else if(mem_wb_reg.type == "S"){
            int address = ex_mem_reg.alu_result/4;
            if(address >= 0 && address < dmem.size()){
                dmem[address] = ex_mem_reg.rs2_val;
            }
        
        }
    }
}


void write_back(){
    if(mem_wb_reg.nop || mem_wb_reg.rd == 0){
        return;
    }else{
        if(mem_wb_reg.type == "R"){
            registers[mem_wb_reg.rd] = mem_wb_reg.alu_result;
        }else if(mem_wb_reg.type == "I"){
            if(mem_wb_reg.opcode == 0x3){
                registers[mem_wb_reg.rd] = mem_wb_reg.memeory_data;
            }else{
                registers[mem_wb_reg.rd] = mem_wb_reg.alu_result;
            }
        }else if(mem_wb_reg.type == "J"){
            registers[mem_wb_reg.rd] = mem_wb_reg.alu_result;
        }
    }
}
#include <iostream>
#include <array>
#include <vector>
// ALU算术逻辑单元, memory, register, CU控制逻辑单元, ISA, PC
const int memeory_size = 256;
class memory
{
private:
    std::array<int, memeory_size> mem = {0};
public:
    int load(int addr){
        if(addr >= 256 || addr < 0){
            return 0;
        }else{
            return mem[addr];
        }
    };
    void store(int addr, int value){
        if(addr >= 256 || addr < 0){
            throw std::runtime_error("Address out of bound!");
        }else{
            mem[addr] = value;
        }
    }
};

const int register_size = 8;
class register_file
{
private:
    std::array<int, register_size> reg;

public:
    void write(int index, int value){
        if(index < 0 || index >= register_size){
            throw std::runtime_error("Index out of bound!");
        }else{
            reg[index] = value;
        }
    }

    int read(int index){
        if(index < 0 || index >= register_size){
            return 0;
        }else{
            return reg[index];
        }
    }
};

class program_counter
{
private:
    int pc = 0;

public:
    void set(int value){
        pc = value;
    }

    void step(){
        pc++;
    }

    int get(){
        return pc;
    }
};

class ALU
{
public:
    int add(int a, int b){
        return a+b;
    }

    int sub(int a, int b){
        return a-b;
    }

    int mul(int a, int b) {
        return a * b;
    }

    int div_safe(int a, int b) {
        if (b == 0) {
            throw std::runtime_error("Division by zero");
        }
        return a / b;
    }

    int mod(int a, int b) {
        if (b == 0) {
            throw std::runtime_error("Modulo by zero");
        }
        return a % b;
    }

    int bitwise_and(int a, int b) {
        return a & b;
    }

    int bitwise_or(int a, int b) {
        return a | b;
    }

    int bitwise_xor(int a, int b) {
        return a ^ b;
    }

    int bitwise_not(int a) {
        return ~a;
    }

    bool equal(int a, int b){
        return a==b;
    }

    bool not_equal(int a, int b) {
        return a != b;
    }

    bool greater_than(int a, int b) {
        return a > b;
    }

    bool less_than(int a, int b) {
        return a < b;
    }

    bool greater_equal(int a, int b) {
        return a >= b;
    }

    bool less_equal(int a, int b) {
        return a <= b;
    }


};

enum instruction_type{
    add,
    jump,
    sub,
    load,
    store
};

struct instruction
{
    instruction_type instruct_type;
    int r1;
    int r2;
    int r3;
};


class CUISA
{
private:
    std::array<int, register_size> registers;
    std::array<int, memeory_size> memory;
    int pc = 0;
    std::vector<instruction> prog;

public: 
    void load_program(std::vector<instruction> program){
        prog = program;
    }

    void run(){
        while (pc < prog.size())
        {
            execute(prog[pc]);
            pc++;
        }
        
    }

    void execute(instruction& instruct){
        if( instruct.instruct_type== add){
            registers[instruct.r3] = registers[instruct.r1] + registers[instruct.r2];

        }else if(instruct.instruct_type == jump){
            pc = instruct.r1-1;

        }else if(instruct.instruct_type == sub){
            registers[instruct.r3] = registers[instruct.r1] - registers[instruct.r2];

        }else if (instruct.instruct_type == load){
            registers[instruct.r1] = memory[instruct.r2];

        }else if (instruct.instruct_type == store){
            memory[instruct.r2] = registers[instruct.r1];
        }
    } 
}
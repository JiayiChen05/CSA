#include <iostream>
#include <array>
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

    bool equal(int a, int b){
        return a==b;
    }

};

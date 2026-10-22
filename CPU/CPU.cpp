#include "CPU.h"
#include "CPU_REG.h"
#include "../Cartridge/Cartridge.h"
#include "../Log/logger.h"

CPU_registers registers;
uint8_t current_opcode = 0;
uint32_t cpu_instruction_count = 0;
void* cpu_current_instruction_execution = nullptr;

void CPU_reset(){
  //boots after executing boot rom registers
  //needed to put the emu in the correct state
  registers.AF = 0x01B0;
  registers.BC = 0x0013;
  registers.DE = 0x00D8;
  registers.HL = 0x014D;
  registers.SP = 0xFFFE;
  registers.PC = 0x0100;
}

void CPU_read(){
  current_opcode = cart_data[registers.PC++];
  const GameBoyCPU& instruction = instructions[current_opcode];
  cpu_current_instruction_execution = instruction.exe;
}

void CPU_write(){
  if(!current_opcode){
    const GameBoyCPU& instruction = instructions[current_opcode];
    const uint8_t pchi = ((registers.PC -1) & 0xFF00) >> 8;
    const uint8_t pclo = ((registers.PC -1) & 0xFF);
    LOG_DEBUG("Unknown instruction");
    return false;
  }
  ((*CPU_EXE_OP)cpu_current_instruction_execution)();
}

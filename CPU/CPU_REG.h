#pragma once
#ifndef CPU_REG_H
#define CPU_REG_H

#include "CPU.h"
#include <cstdint>

struct GameBoyCPU{
  const char* dissassembler;//conatains name of the opcode
  uint8_t op_length;//size of the operand
  CPU_EXE_OP exe;//pointer to the instruction
};

struct GameBoyCPU_CB{
  const char* dissassembler;
  CPU_EXE_OP exe;
};

extern const GameBoyCPU instructions[];

#endif

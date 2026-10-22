#pragma once
#ifndef CPU_REG_H
#define CPU_REG_H

#include <cstdint>

struct GameBoyCPU{
  const char* dissassembler;//conatains name of the opcode
  uint8_t op_length;//size of the operand
  void* exe;//pointer to the instruction
};

extern const GameBoyCPU instructions[256];

#endif

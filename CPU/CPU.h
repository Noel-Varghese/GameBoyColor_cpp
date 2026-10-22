#pragma once

#ifndef CPU_H
#define CPU_H

#include <cstdint>

struct CPU_registers{
  struct{
    union{
      struct{//Accumulator and flags
	uint8_t A;//register flags
	uint8_t F;
      };
      uint16_t AF;
    };
  };
  struct{
    union{
      struct{//BC
	uint8_t B;
	uint8_t C;
      };
      uint16_t BC;
    };
  };
  struct{
    union{
      struct{
	uint8_t D;//DE
	uint8_t E;
      };
      uint16_t DE;
    };
  };
  struct{
    union{
      struct{//HL
	uint8_t H;
	uint8_t L;
      };
      uint16_t HL;
    };
  };
  struct{
    union{
      struct{//stack pointer
	uint8_t S;
	uint8_t P;
      };
      uint16_t SP;
    };
  };
  uint16_t PC;
};

typedef void(*CPU_EXE_OP)();

void CPU_read();
void CPU_write();
void CPU_reset();

#endif

#pragma once


#include <cstdint>

class Cartridge;

struct CPU_registers{
    union{
      struct{//Accumulator and flags
	uint8_t F;//register flags
	uint8_t A;
      };
      uint16_t AF;
    };
    union{
      struct{//BC
	uint8_t C;
	uint8_t B;
      };
      uint16_t BC;
    };
    union{
      struct{
	uint8_t E;//DE
	uint8_t D;
      };
      uint16_t DE;
    };
    union{
      struct{//HL
	uint8_t L;
	uint8_t H;
      };
      uint16_t HL;
    };
  uint16_t SP;
  uint16_t PC;
};

typedef void(*CPU_EXE_OP)();

enum:uint8_t{
  FLAG_Z = 0x80,
    FLAG_N = 0x40,
    FLAG_H = 0x20,
    FLAG_C = 0x10
};


enum class GBModel{AUTO, DMG, SGB, SGB2, CGB};//helps detect model from the Cartridge
void CPU_set_model(GBModel m);//calls this b4 CPU reset
void CPU_init(Cartridge* c);//tells the cpu which cart to use
void CPU_reset();
bool CPU_step();

//registers
void cpu_nop(); //0x00
void cpu_LD_BC_nn(); //0x01
void cpu_LD_BC_A(); //0x02
void cpu_INC_BC(); //0x03
void cpu_INC_B(); //0x04
void cpu_DEC_B(); //0x05
void cpu_LD_B_n(); //0x06
void cpu_RLCA(); //0x07
void cpu_LD_a16_SP(); //0x08
void cpu_ADD_HL_BC(); //0x09
void cpu_LD_A_BC(); //0x0A
void cpu_DEC_BC(); //0x0B
void cpu_INC_C(); //0x0C
void cpu_DEC_C(); //0x0D
void cpu_LD_C_n(); //0x0E
void cpu_RRCA(); //0x0F
void cpu_STOP_n(); //0x10
void cpu_LD_DE_nn(); //0x11
void cpu_LD_DE_A(); //0x12
void cpu_INC_DE(); //0x13
void cpu_INC_D(); //0x14
void cpu_DEC_D(); //0x15
void cpu_LD_D_n(); //0x16
void cpu_RLA(); //0x17
void cpu_JR_n(); //0x18
void cpu_ADD_HL_DE(); //0x19
void cpu_LD_A_DE(); //0x1A
void cpu_DEC_DE(); //0x1B
void cpu_INC_E(); //0x1C
void cpu_DEC_E(); //0x1D
void cpu_LD_E_n(); //0x1E
void cpu_RRA(); //0x1F
void cpu_JR_NZ_n(); //0x20
void cpu_LD_HL_nn(); //0x21
void cpu_LD_HL_A_INC(); //0x22
void cpu_INC_HL(); //0x23
void cpu_INC_H(); //0x24
void cpu_DEC_H(); //0x25
void cpu_LD_H_n(); //0x26
void cpu_DAA(); //0x27
void cpu_JR_Z_n(); //0x28
void cpu_ADD_HL_HL(); //0x29
void cpu_LD_A_HL_INC(); //0x2A
void cpu_DEC_HL(); //0x2B
void cpu_INC_L(); //0x2C
void cpu_DEC_L(); //0x2D
void cpu_LD_L_n(); //0x2E
void cpu_CPL(); //0x2F
void cpu_JR_NC_n(); //0x30
void cpu_LD_SP_nn(); //0x31
void cpu_LD_HL_A_DEC(); //0x32
void cpu_INC_SP(); //0x33
void cpu_INC_HL_MEM(); //0x34
void cpu_DEC_HL_MEM(); //0x35
void cpu_LD_HL_n(); //0x36
void cpu_SCF(); //0x37
void cpu_JR_C_n(); //0x38
void cpu_ADD_HL_SP(); //0x39
void cpu_LD_A_HL_DEC(); //0x3A
void cpu_DEC_SP(); //0x3B
void cpu_INC_A(); //0x3C
void cpu_DEC_A(); //0x3D
void cpu_LD_A_n(); //0x3E
void cpu_CCF(); //0x3F
void cpu_LD_B_B(); //0x40
void cpu_LD_B_C(); //0x41
void cpu_LD_B_D(); //0x42
void cpu_LD_B_E(); //0x43
void cpu_LD_B_H(); //0x44
void cpu_LD_B_L(); //0x45
void cpu_LD_B_HL(); //0x46
void cpu_LD_B_A(); //0x47
void cpu_LD_C_B(); //0x48
void cpu_LD_C_C(); //0x49
void cpu_LD_C_D(); //0x4A
void cpu_LD_C_E(); //0x4B
void cpu_LD_C_H(); //0x4C
void cpu_LD_C_L(); //0x4D
void cpu_LD_C_HL(); //0x4E
void cpu_LD_C_A(); //0x4F
void cpu_LD_D_B(); //0x50
void cpu_LD_D_C(); //0x51
void cpu_LD_D_D(); //0x52
void cpu_LD_D_E(); //0x53
void cpu_LD_D_H(); //0x54
void cpu_LD_D_L(); //0x55
void cpu_LD_D_HL(); //0x56
void cpu_LD_D_A(); //0x57
void cpu_LD_E_B(); //0x58
void cpu_LD_E_C(); //0x59
void cpu_LD_E_D(); //0x5A
void cpu_LD_E_E(); //0x5B
void cpu_LD_E_H(); //0x5C
void cpu_LD_E_L(); //0x5D
void cpu_LD_E_HL(); //0x5E
void cpu_LD_E_A(); //0x5F
void cpu_LD_H_B(); //0x60
void cpu_LD_H_C(); //0x61
void cpu_LD_H_D(); //0x62
void cpu_LD_H_E(); //0x63
void cpu_LD_H_H(); //0x64
void cpu_LD_H_L(); //0x65
void cpu_LD_H_HL(); //0x66
void cpu_LD_H_A(); //0x67
void cpu_LD_L_B(); //0x68
void cpu_LD_L_C(); //0x69
void cpu_LD_L_D(); //0x6A
void cpu_LD_L_E(); //0x6B
void cpu_LD_L_H(); //0x6C
void cpu_LD_L_L(); //0x6D
void cpu_LD_L_HL(); //0x6E
void cpu_LD_L_A(); //0x6F
void cpu_LD_HL_B(); //0x70
void cpu_LD_HL_C(); //0x71
void cpu_LD_HL_D(); //0x72
void cpu_LD_HL_E(); //0x73
void cpu_LD_HL_H(); //0x74
void cpu_LD_HL_L(); //0x75
void cpu_HALT(); //0x76
void cpu_LD_HL_A(); //0x77
void cpu_LD_A_B(); //0x78
void cpu_LD_A_C(); //0x79
void cpu_LD_A_D(); //0x7A
void cpu_LD_A_E(); //0x7B
void cpu_LD_A_H(); //0x7C
void cpu_LD_A_L(); //0x7D
void cpu_LD_A_HL(); //0x7E
void cpu_LD_A_A(); //0x7F
void cpu_ADD_A_B(); //0x80
void cpu_ADD_A_C(); //0x81
void cpu_ADD_A_D(); //0x82
void cpu_ADD_A_E(); //0x83
void cpu_ADD_A_H(); //0x84
void cpu_ADD_A_L(); //0x85
void cpu_ADD_A_HL(); //0x86
void cpu_ADD_A_A(); //0x87
void cpu_ADC_A_B(); //0x88
void cpu_ADC_A_C(); //0x89
void cpu_ADC_A_D(); //0x8A
void cpu_ADC_A_E(); //0x8B
void cpu_ADC_A_H(); //0x8C
void cpu_ADC_A_L(); //0x8D
void cpu_ADC_A_HL(); //0x8E
void cpu_ADC_A_A(); //0x8F
void cpu_SUB_A_B(); //0x90
void cpu_SUB_A_C(); //0x91
void cpu_SUB_A_D(); //0x92
void cpu_SUB_A_E(); //0x93
void cpu_SUB_A_H(); //0x94
void cpu_SUB_A_L(); //0x95
void cpu_SUB_A_HL(); //0x96
void cpu_SUB_A_A(); //0x97
void cpu_SBC_A_B(); //0x98
void cpu_SBC_A_C(); //0x99
void cpu_SBC_A_D(); //0x9A
void cpu_SBC_A_E(); //0x9B
void cpu_SBC_A_H(); //0x9C
void cpu_SBC_A_L(); //0x9D
void cpu_SBC_A_HL(); //0x9E
void cpu_SBC_A_A(); //0x9F
void cpu_AND_A_B(); //0xA0
void cpu_AND_A_C(); //0xA1
void cpu_AND_A_D(); //0xA2
void cpu_AND_A_E(); //0xA3
void cpu_AND_A_H(); //0xA4
void cpu_AND_A_L(); //0xA5
void cpu_AND_A_HL(); //0xA6
void cpu_AND_A_A(); //0xA7
void cpu_XOR_A_B(); //0xA8
void cpu_XOR_A_C(); //0xA9
void cpu_XOR_A_D(); //0xAA
void cpu_XOR_A_E(); //0xAB
void cpu_XOR_A_H(); //0xAC
void cpu_XOR_A_L(); //0xAD
void cpu_XOR_A_HL(); //0xAE
void cpu_XOR_A_A(); //0xAF
void cpu_OR_A_B(); //0xB0
void cpu_OR_A_C(); //0xB1
void cpu_OR_A_D(); //0xB2
void cpu_OR_A_E(); //0xB3
void cpu_OR_A_H(); //0xB4
void cpu_OR_A_L(); //0xB5
void cpu_OR_A_HL(); //0xB6
void cpu_OR_A_A(); //0xB7
void cpu_CP_A_B(); //0xB8
void cpu_CP_A_C(); //0xB9
void cpu_CP_A_D(); //0xBA
void cpu_CP_A_E(); //0xBB
void cpu_CP_A_H(); //0xBC
void cpu_CP_A_L(); //0xBD
void cpu_CP_A_HL(); //0xBE
void cpu_CP_A_A(); //0xBF
void cpu_RET_NZ(); //0xC0
void cpu_POP_BC(); //0xC1
void cpu_JP_NZ_nn(); //0xC2
void cpu_JP_nn(); //0xC3
void cpu_CALL_NZ_nn(); //0xC4
void cpu_PUSH_BC(); //0xC5
void cpu_ADD_A_n(); //0xC6
void cpu_RST_00(); //0xC7
void cpu_RET_Z(); //0xC8
void cpu_RET(); //0xC9
void cpu_JP_Z_nn(); //0xCA
void cpu_CALL_Z_nn(); //0xCC
void cpu_CALL_nn(); //0xCD
void cpu_ADC_A_n(); //0xCE
void cpu_RST_08(); //0xCF
void cpu_RET_NC(); //0xD0
void cpu_POP_DE(); //0xD1
void cpu_JP_NC_nn(); //0xD2
void cpu_CALL_NC_nn(); //0xD4
void cpu_PUSH_DE(); //0xD5
void cpu_SUB_A_n(); //0xD6
void cpu_RST_10(); //0xD7
void cpu_RET_C(); //0xD8
void cpu_RETI(); //0xD9
void cpu_JP_C_nn(); //0xDA
void cpu_CALL_C_nn(); //0xDC
void cpu_SBC_A_n(); //0xDE
void cpu_RST_18(); //0xDF
void cpu_LDH_n_A(); //0xE0
void cpu_POP_HL(); //0xE1
void cpu_LDH_C_A(); //0xE2
void cpu_PUSH_HL(); //0xE5
void cpu_AND_A_n(); //0xE6
void cpu_RST_20(); //0xE7
void cpu_ADD_SP_n(); //0xE8
void cpu_JP_HL(); //0xE9
void cpu_LD_nn_A(); //0xEA
void cpu_XOR_A_n(); //0xEE
void cpu_RST_28(); //0xEF
void cpu_LDH_A_n(); //0xF0
void cpu_POP_AF(); //0xF1
void cpu_LDH_A_C(); //0xF2
void cpu_DI(); //0xF3
void cpu_PUSH_AF(); //0xF5
void cpu_OR_A_n(); //0xF6
void cpu_RST_30(); //0xF7
void cpu_LD_HL_SP_N(); //0xF8
void cpu_LD_SP_HL(); //0xF9
void cpu_LD_A_nn(); //0xFA
void cpu_EI(); //0xFB
void cpu_CP_A_n(); //0xFE
void cpu_RST_38(); //0xFF

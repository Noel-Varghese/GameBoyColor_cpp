#include "CPU_REG.h"

const struct GameBoyCPU instructions[] = {
   {"NOP", 0, cpu_nop},//0x00
  {"LD BC,n16", 2, cpu_LD_BC_nn},//0x01
  {"LD [BC],A", 0, cpu_LD_BC_A},//0x02
  {"INC BC", 0, cpu_INC_BC},//0x03
  {"INC B", 0, cpu_INC_B},//0x04
  {"DEC B", 0, cpu_DEC_B},//0x05
  {"LD B,n8", 1, cpu_LD_B_n},//0x06
  {"RLCA", 0, cpu_RLCA},//0x07
  {"LD [a16],SP", 2, cpu_LD_a16_SP},//0x08
  {"ADD HL,BC", 0, cpu_ADD_HL_BC},//0x09
  {"LD A,[BC]", 0, cpu_LD_A_BC},//0x0A
  {"DEC BC", 0, cpu_DEC_BC},//0x0B
  {"INC C", 0, cpu_INC_C},//0x0C
  {"DEC C", 0, cpu_DEC_C},//0x0D
  {"LD C,n8", 1, cpu_LD_C_n},//0x0E
  {"RRCA", 0, cpu_RRCA},//0x0F
  {"STOP n8", 1, cpu_STOP_n},//0x10
  {"LD DE,n16", 2, cpu_LD_DE_nn},//0x11
  {"LD [DE],A", 0, cpu_LD_DE_A},//0x12
  {"INC DE", 0, cpu_INC_DE},//0x13
  {"INC D", 0, cpu_INC_D},//0x14
  {"DEC D", 0, cpu_DEC_D},//0x15
  {"LD D,n8", 1, cpu_LD_D_n},//0x16
  {"RLA", 0, cpu_RLA},//0x17
  {"JR e8", 1, cpu_JR_n},//0x18
  {"ADD HL,DE", 0, cpu_ADD_HL_DE},//0x19
  {"LD A,[DE]", 0, cpu_LD_A_DE},//0x1A
  {"DEC DE", 0, cpu_DEC_DE},//0x1B
  {"INC E", 0, cpu_INC_E},//0x1C
  {"DEC E", 0, cpu_DEC_E},//0x1D
  {"LD E,n8", 1, cpu_LD_E_n},//0x1E
  {"RRA", 0, cpu_RRA},//0x1F
  {"JR NZ,e8", 1, cpu_JR_NZ_n},//0x20
  {"LD HL,n16", 2, cpu_LD_HL_nn},//0x21
  {"LD [HL+],A", 0, cpu_LD_HL_A_INC},//0x22
  {"INC HL", 0, cpu_INC_HL},//0x23
  {"INC H", 0, cpu_INC_H},//0x24
  {"DEC H", 0, cpu_DEC_H},//0x25
  {"LD H,n8", 1, cpu_LD_H_n},//0x26
  {"DAA", 0, cpu_DAA},//0x27
  {"JR Z,e8", 1, cpu_JR_Z_n},//0x28
  {"ADD HL,HL", 0, cpu_ADD_HL_HL},//0x29
  {"LD A,[HL+]", 0, cpu_LD_A_HL_INC},//0x2A
  {"DEC HL", 0, cpu_DEC_HL},//0x2B
  {"INC L", 0, cpu_INC_L},//0x2C
  {"DEC L", 0, cpu_DEC_L},//0x2D
  {"LD L,n8", 1, cpu_LD_L_n},//0x2E
  {"CPL", 0, cpu_CPL},//0x2F
  {"JR NC,e8", 1, cpu_JR_NC_n},//0x30
  {"LD SP,n16", 2, cpu_LD_SP_nn},//0x31
  {"LD [HL-],A", 0, cpu_LD_HL_A_DEC},//0x32
  {"INC SP", 0, cpu_INC_SP},//0x33
  {"INC [HL]", 0, cpu_INC_HL_MEM},//0x34
  {"DEC [HL]", 0, cpu_DEC_HL_MEM},//0x35
  {"LD [HL],n8", 1, cpu_LD_HL_n},//0x36
  {"SCF", 0, cpu_SCF},//0x37
  {"JR C,e8", 1, cpu_JR_C_n},//0x38
  {"ADD HL,SP", 0, cpu_ADD_HL_SP},//0x39
  {"LD A,[HL-]", 0, cpu_LD_A_HL_DEC},//0x3A
  {"DEC SP", 0, cpu_DEC_SP},//0x3B
  {"INC A", 0, cpu_INC_A},//0x3C
  {"DEC A", 0, cpu_DEC_A},//0x3D
  {"LD A,n8", 1, cpu_LD_A_n},//0x3E
  {"CCF", 0, cpu_CCF},//0x3F
  {"LD B,B", 0, cpu_LD_B_B},//0x40
  {"LD B,C", 0, cpu_LD_B_C},//0x41
  {"LD B,D", 0, cpu_LD_B_D},//0x42
  {"LD B,E", 0, cpu_LD_B_E},//0x43
  {"LD B,H", 0, cpu_LD_B_H},//0x44
  {"LD B,L", 0, cpu_LD_B_L},//0x45
  {"LD B,[HL]", 0, cpu_LD_B_HL},//0x46
  {"LD B,A", 0, cpu_LD_B_A},//0x47
  {"LD C,B", 0, cpu_LD_C_B},//0x48
  {"LD C,C", 0, cpu_LD_C_C},//0x49
  {"LD C,D", 0, cpu_LD_C_D},//0x4A
  {"LD C,E", 0, cpu_LD_C_E},//0x4B
  {"LD C,H", 0, cpu_LD_C_H},//0x4C
  {"LD C,L", 0, cpu_LD_C_L},//0x4D
  {"LD C,[HL]", 0, cpu_LD_C_HL},//0x4E
  {"LD C,A", 0, cpu_LD_C_A},//0x4F
  {"LD D,B", 0, cpu_LD_D_B},//0x50
  {"LD D,C", 0, cpu_LD_D_C},//0x51
  {"LD D,D", 0, cpu_LD_D_D},//0x52
  {"LD D,E", 0, cpu_LD_D_E},//0x53
  {"LD D,H", 0, cpu_LD_D_H},//0x54
  {"LD D,L", 0, cpu_LD_D_L},//0x55
  {"LD D,[HL]", 0, cpu_LD_D_HL},//0x56
  {"LD D,A", 0, cpu_LD_D_A},//0x57
  {"LD E,B", 0, cpu_LD_E_B},//0x58
  {"LD E,C", 0, cpu_LD_E_C},//0x59
  {"LD E,D", 0, cpu_LD_E_D},//0x5A
  {"LD E,E", 0, cpu_LD_E_E},//0x5B
  {"LD E,H", 0, cpu_LD_E_H},//0x5C
  {"LD E,L", 0, cpu_LD_E_L},//0x5D
  {"LD E,[HL]", 0, cpu_LD_E_HL},//0x5E
  {"LD E,A", 0, cpu_LD_E_A},//0x5F
  {"LD H,B", 0, cpu_LD_H_B},//0x60
  {"LD H,C", 0, cpu_LD_H_C},//0x61
  {"LD H,D", 0, cpu_LD_H_D},//0x62
  {"LD H,E", 0, cpu_LD_H_E},//0x63
  {"LD H,H", 0, cpu_LD_H_H},//0x64
  {"LD H,L", 0, cpu_LD_H_L},//0x65
  {"LD H,[HL]", 0, cpu_LD_H_HL},//0x66
  {"LD H,A", 0, cpu_LD_H_A},//0x67
  {"LD L,B", 0, cpu_LD_L_B},//0x68
  {"LD L,C", 0, cpu_LD_L_C},//0x69
  {"LD L,D", 0, cpu_LD_L_D},//0x6A
  {"LD L,E", 0, cpu_LD_L_E},//0x6B
  {"LD L,H", 0, cpu_LD_L_H},//0x6C
  {"LD L,L", 0, cpu_LD_L_L},//0x6D
  {"LD L,[HL]", 0, cpu_LD_L_HL},//0x6E
  {"LD L,A", 0, cpu_LD_L_A},//0x6F
  {"LD [HL],B", 0, cpu_LD_HL_B},//0x70
  {"LD [HL],C", 0, cpu_LD_HL_C},//0x71
  {"LD [HL],D", 0, cpu_LD_HL_D},//0x72
  {"LD [HL],E", 0, cpu_LD_HL_E},//0x73
  {"LD [HL],H", 0, cpu_LD_HL_H},//0x74
  {"LD [HL],L", 0, cpu_LD_HL_L},//0x75
  {"HALT", 0, cpu_HALT},//0x76
  {"LD [HL],A", 0, cpu_LD_HL_A},//0x77
  {"LD A,B", 0, cpu_LD_A_B},//0x78
  {"LD A,C", 0, cpu_LD_A_C},//0x79
  {"LD A,D", 0, cpu_LD_A_D},//0x7A
  {"LD A,E", 0, cpu_LD_A_E},//0x7B
  {"LD A,H", 0, cpu_LD_A_H},//0x7C
  {"LD A,L", 0, cpu_LD_A_L},//0x7D
  {"LD A,[HL]", 0, cpu_LD_A_HL},//0x7E
  {"LD A,A", 0, cpu_LD_A_A},//0x7F
  {"ADD A,B", 0, cpu_ADD_A_B},//0x80
  {"ADD A,C", 0, cpu_ADD_A_C},//0x81
  {"ADD A,D", 0, cpu_ADD_A_D},//0x82
  {"ADD A,E", 0, cpu_ADD_A_E},//0x83
  {"ADD A,H", 0, cpu_ADD_A_H},//0x84
  {"ADD A,L", 0, cpu_ADD_A_L},//0x85
  {"ADD A,[HL]", 0, cpu_ADD_A_HL},//0x86
  {"ADD A,A", 0, cpu_ADD_A_A},//0x87
  {"ADC A,B", 0, cpu_ADC_A_B},//0x88
  {"ADC A,C", 0, cpu_ADC_A_C},//0x89
  {"ADC A,D", 0, cpu_ADC_A_D},//0x8A
  {"ADC A,E", 0, cpu_ADC_A_E},//0x8B
  {"ADC A,H", 0, cpu_ADC_A_H},//0x8C
  {"ADC A,L", 0, cpu_ADC_A_L},//0x8D
  {"ADC A,[HL]", 0, cpu_ADC_A_HL},//0x8E
  {"ADC A,A", 0, cpu_ADC_A_A},//0x8F
  {"SUB A,B", 0, cpu_SUB_A_B},//0x90
  {"SUB A,C", 0, cpu_SUB_A_C},//0x91
  {"SUB A,D", 0, cpu_SUB_A_D},//0x92
  {"SUB A,E", 0, cpu_SUB_A_E},//0x93
  {"SUB A,H", 0, cpu_SUB_A_H},//0x94
  {"SUB A,L", 0, cpu_SUB_A_L},//0x95
  {"SUB A,[HL]", 0, cpu_SUB_A_HL},//0x96
  {"SUB A,A", 0, cpu_SUB_A_A},//0x97
  {"SBC A,B", 0, cpu_SBC_A_B},//0x98
  {"SBC A,C", 0, cpu_SBC_A_C},//0x99
  {"SBC A,D", 0, cpu_SBC_A_D},//0x9A
  {"SBC A,E", 0, cpu_SBC_A_E},//0x9B
  {"SBC A,H", 0, cpu_SBC_A_H},//0x9C
  {"SBC A,L", 0, cpu_SBC_A_L},//0x9D
  {"SBC A,[HL]", 0, cpu_SBC_A_HL},//0x9E
  {"SBC A,A", 0, cpu_SBC_A_A},//0x9F
  {"AND A,B", 0, cpu_AND_A_B},//0xA0
  {"AND A,C", 0, cpu_AND_A_C},//0xA1
  {"AND A,D", 0, cpu_AND_A_D},//0xA2
  {"AND A,E", 0, cpu_AND_A_E},//0xA3
  {"AND A,H", 0, cpu_AND_A_H},//0xA4
  {"AND A,L", 0, cpu_AND_A_L},//0xA5
  {"AND A,[HL]", 0, cpu_AND_A_HL},//0xA6
  {"AND A,A", 0, cpu_AND_A_A},//0xA7
  {"XOR A,B", 0, cpu_XOR_A_B},//0xA8
  {"XOR A,C", 0, cpu_XOR_A_C},//0xA9
  {"XOR A,D", 0, cpu_XOR_A_D},//0xAA
  {"XOR A,E", 0, cpu_XOR_A_E},//0xAB
  {"XOR A,H", 0, cpu_XOR_A_H},//0xAC
  {"XOR A,L", 0, cpu_XOR_A_L},//0xAD
  {"XOR A,[HL]", 0, cpu_XOR_A_HL},//0xAE
  {"XOR A,A", 0, cpu_XOR_A_A},//0xAF
  {"OR A,B", 0, cpu_OR_A_B},//0xB0
  {"OR A,C", 0, cpu_OR_A_C},//0xB1
  {"OR A,D", 0, cpu_OR_A_D},//0xB2
  {"OR A,E", 0, cpu_OR_A_E},//0xB3
  {"OR A,H", 0, cpu_OR_A_H},//0xB4
  {"OR A,L", 0, cpu_OR_A_L},//0xB5
  {"OR A,[HL]", 0, cpu_OR_A_HL},//0xB6
  {"OR A,A", 0, cpu_OR_A_A},//0xB7
  {"CP A,B", 0, cpu_CP_A_B},//0xB8
  {"CP A,C", 0, cpu_CP_A_C},//0xB9
  {"CP A,D", 0, cpu_CP_A_D},//0xBA
  {"CP A,E", 0, cpu_CP_A_E},//0xBB
  {"CP A,H", 0, cpu_CP_A_H},//0xBC
  {"CP A,L", 0, cpu_CP_A_L},//0xBD
  {"CP A,[HL]", 0, cpu_CP_A_HL},//0xBE
  {"CP A,A", 0, cpu_CP_A_A},//0xBF
  {"RET NZ", 0, cpu_RET_NZ},//0xC0
  {"POP BC", 0, cpu_POP_BC},//0xC1
  {"JP NZ,a16", 2, cpu_JP_NZ_nn},//0xC2
  {"JP a16", 2, cpu_JP_nn},//0xC3
  {"CALL NZ,a16", 2, cpu_CALL_NZ_nn},//0xC4
  {"PUSH BC", 0, cpu_PUSH_BC},//0xC5
  {"ADD A,n8", 1, cpu_ADD_A_n},//0xC6
  {"RST $00", 0, cpu_RST_00},//0xC7
  {"RET Z", 0, cpu_RET_Z},//0xC8
  {"RET", 0, cpu_RET},//0xC9
  {"JP Z,a16", 2, cpu_JP_Z_nn},//0xCA
  {"PREFIX CB", 0, nullptr},//0xCB
  {"CALL Z,a16", 2, cpu_CALL_Z_nn},//0xCC
  {"CALL a16", 2, cpu_CALL_nn},//0xCD
  {"ADC A,n8", 1, cpu_ADC_A_n},//0xCE
  {"RST $08", 0, cpu_RST_08},//0xCF
  {"RET NC", 0, cpu_RET_NC},//0xD0
  {"POP DE", 0, cpu_POP_DE},//0xD1
  {"JP NC,a16", 2, cpu_JP_NC_nn},//0xD2
  {"ILLEGAL D3", 0, nullptr},//0xD3
  {"CALL NC,a16", 2, cpu_CALL_NC_nn},//0xD4
  {"PUSH DE", 0, cpu_PUSH_DE},//0xD5
  {"SUB A,n8", 1, cpu_SUB_A_n},//0xD6
  {"RST $10", 0, cpu_RST_10},//0xD7
  {"RET C", 0, cpu_RET_C},//0xD8
  {"RETI", 0, cpu_RETI},//0xD9
  {"JP C,a16", 2, cpu_JP_C_nn},//0xDA
  {"ILLEGAL DB", 0, nullptr},//0xDB
  {"CALL C,a16", 2, cpu_CALL_C_nn},//0xDC
  {"ILLEGAL DD", 0, nullptr},//0xDD
  {"SBC A,n8", 1, cpu_SBC_A_n},//0xDE
  {"RST $18", 0, cpu_RST_18},//0xDF
  {"LDH [a8],A", 1, cpu_LDH_n_A},//0xE0
  {"POP HL", 0, cpu_POP_HL},//0xE1
  {"LDH [C],A", 0, cpu_LDH_C_A},//0xE2
  {"ILLEGAL E3", 0, nullptr},//0xE3
  {"ILLEGAL E4", 0, nullptr},//0xE4
  {"PUSH HL", 0, cpu_PUSH_HL},//0xE5
  {"AND A,n8", 1, cpu_AND_A_n},//0xE6
  {"RST $20", 0, cpu_RST_20},//0xE7
  {"ADD SP,e8", 1, cpu_ADD_SP_n},//0xE8
  {"JP HL", 0, cpu_JP_HL},//0xE9
  {"LD [a16],A", 2, cpu_LD_nn_A},//0xEA
  {"ILLEGAL EB", 0, nullptr},//0xEB
  {"ILLEGAL EC", 0, nullptr},//0xEC
  {"ILLEGAL ED", 0, nullptr},//0xED
  {"XOR A,n8", 1, cpu_XOR_A_n},//0xEE
  {"RST $28", 0, cpu_RST_28},//0xEF
  {"LDH A,[a8]", 1, cpu_LDH_A_n},//0xF0
  {"POP AF", 0, cpu_POP_AF},//0xF1
  {"LDH A,[C]", 0, cpu_LDH_A_C},//0xF2
  {"DI", 0, cpu_DI},//0xF3
  {"ILLEGAL F4", 0, nullptr},//0xF4
  {"PUSH AF", 0, cpu_PUSH_AF},//0xF5
  {"OR A,n8", 1, cpu_OR_A_n},//0xF6
  {"RST $30", 0, cpu_RST_30},//0xF7
  {"LD HL,SP+e8", 1, cpu_LD_HL_SP_N},//0xF8
  {"LD SP,HL", 0, cpu_LD_SP_HL},//0xF9
  {"LD A,[a16]", 2, cpu_LD_A_nn},//0xFA
  {"EI", 0, cpu_EI},//0xFB
  {"ILLEGAL FC", 0, nullptr},//0xFC
  {"ILLEGAL FD", 0, nullptr},//0xFD
  {"CP A,n8", 1, cpu_CP_A_n},//0xFE
  {"RST $38", 0, cpu_RST_38},//0xFF
};

static_assert(sizeof(instructions)/sizeof(instructions[0]) == 256, "opcode table must have 256 entries");

/*
  it is a single byte opcode that signals the CPU to fetch an additional byte and execute an instruction from a seperate CB bank of instruction set.
*/

const struct GameBoyCPU_CB CB_Prefix[]{
  {"RLC B", CB_RLC_B },//0x00
  {"RLC C", CB_RLC_C}, //0x01
  {"RLC D", CB_RLC_D}, //0x02
  {"RLC E", CB_RLC_E}, //0x03
  {"RLC H", CB_RLC_H}, //0x04
  {"RLC L", CB_RLC_L}, //0x05
  {"RLC HL", CB_RLC_HL}, //0x06
  {"RLC A", CB_RLC_A}, //0x07
  {"RRC B", CB_RRC_B}, //0x08
  {"RRC C", CB_RRC_C}, //0x09
  {"RRC D", CB_RRC_D}, //0x0A
  {"RRC E", CB_RRC_E}, //0x0B
  {"RRC H", CB_RRC_H}, //0x0C
  {"RRC L", CB_RRC_L}, //0x0D
  {"RRC HL", CB_RRC_HL}, //0x0E
  {"RRC A", CB_RRC_A}, //0x0F
  {"RL B", CB_RL_B}, //0x10
  {"RL C", CB_RL_C}, //0x11
  {"RL D", CB_RL_D}, //0x12
  {"RL E", CB_RL_E}, //0x13
  {"RL H", CB_RL_H}, //0x14
  {"RL L", CB_RL_L}, //0x15
  {"RL HL", CB_RL_HL}, //0x16
  {"RL A", CB_RL_A}, //0x17
  {"RR B", CB_RR_B}, //0x18
  {"RR C", CB_RR_C}, //0x19
  {"RR D", CB_RR_D}, //0x1A
  {"RR E", CB_RR_E}, //0x1B
  {"RR H", CB_RR_H}, //0x1C
  {"RR L", CB_RR_L}, //0x1D
  {"RR HL", CB_RR_HL}, //0x1E
  {"RR A", CB_RR_A}, //0x1F
  {"SLA B", CB_SLA_B}, //0x20
  {"SLA C", CB_SLA_C}, //0x21
  {"SLA D", CB_SLA_D}, //0x22
  {"SLA E", CB_SLA_E}, //0x23
  {"SLA H", CB_SLA_H}, //0x24
  {"SLA L", CB_SLA_L}, //0x25
  {"SLA HL", CB_SLA_HL}, //0x26
  {"SLA A", CB_SLA_A}, //0x27
  {"SRA B", CB_SRA_B}, //0x28
  {"SRA C", CB_SRA_C}, //0x29
  {"SRA D", CB_SRA_D}, //0x2A
  {"SRA E", CB_SRA_E}, //0x2B
  {"SRA H", CB_SRA_H}, //0x2C
  {"SRA L", CB_SRA_L}, //0x2D
  {"SRA HL", CB_SRA_HL}, //0x2E
  {"SRA A", CB_SRA_A}, //0x2F
  {"SWAP B", CB_SWAP_B}, //0x30
  {"SWAP C", CB_SWAP_C}, //0x31
  {"SWAP D", CB_SWAP_D}, //0x32
  {"SWAP E", CB_SWAP_E}, //0x33
  {"SWAP H", CB_SWAP_H}, //0x34
  {"SWAP L", CB_SWAP_L}, //0x35
  {"SWAP HL", CB_SWAP_HL}, //0x36
  {"SWAP A", CB_SWAP_A}, //0x37
  {"SRL B", CB_SRL_B}, //0x38
  {"SRL C", CB_SRL_C}, //0x39
  {"SRL D", CB_SRL_D}, //0x3A
  {"SRL E", CB_SRL_E}, //0x3B
  {"SRL H", CB_SRL_H}, //0x3C
  {"SRL L", CB_SRL_L}, //0x3D
  {"SRL HL", CB_SRL_HL}, //0x3E
  {"SRL A", CB_SRL_A}, //0x3F
  {"BIT 0, B", CB_BIT_0B}, //0x40
  {"BIT 0, C", CB_BIT_0C}, //0x41
  {"BIT 0, D", CB_BIT_0D}, //0x42
  {"BIT 0, E", CB_BIT_0E}, //0x43
  {"BIT 0, H", CB_BIT_0H}, //0x44
  {"BIT 0, L", CB_BIT_0L}, //0x45
  {"BIT 0, HL", CB_BIT_0HL}, //0x46
  {"BIT 0, A", CB_BIT_0A}, //0x47
  {"BIT 1, B", CB_BIT_1B}, //0x48
  {"BIT 1, C", CB_BIT_1C}, //0x49
  {"BIT 1, D", CB_BIT_1D}, //0x4A
  {"BIT 1, E", CB_BIT_1E}, //0x4B
  {"BIT 1, H", CB_BIT_1H}, //0x4C
  {"BIT 1, L", CB_BIT_1L}, //0x4D
  {"BIT 1, HL", CB_BIT_1HL}, //0x4E
  {"BIT 1, A", CB_BIT_1A}, //0x4F
  {"BIT 2, B", CB_BIT_2B}, //0x50
  {"BIT 2, C", CB_BIT_2C}, //0x51
  {"BIT 2, D", CB_BIT_2D}, //0x52
  {"BIT 2, E", CB_BIT_2E}, //0x53
  {"BIT 2, H", CB_BIT_2H}, //0x54
  {"BIT 2, L", CB_BIT_2L}, //0x55
  {"BIT 2, HL", CB_BIT_2HL}, //0x56
  {"BIT 2, A", CB_BIT_2A}, //0x57
  {"BIT 3, B", CB_BIT_3B}, //0x58
  {"BIT 3, C", CB_BIT_3C}, //0x59
  {"BIT 3, D", CB_BIT_3D}, //0x5A
  {"BIT 3, E", CB_BIT_3E}, //0x5B
  {"BIT 3, H", CB_BIT_3H}, //0x5C
  {"BIT 3, L", CB_BIT_3L}, //0x5D
  {"BIT 3, HL", CB_BIT_3HL}, //0x5E
  {"BIT 3, A", CB_BIT_3A}, //0x5F
  {"BIT 4, B", CB_BIT_4B}, //0x60
  {"BIT 4, C", CB_BIT_4C}, //0x61
  {"BIT 4, D", CB_BIT_4D}, //0x62
  {"BIT 4, E", CB_BIT_4E}, //0x63
  {"BIT 4, H", CB_BIT_4H}, //0x64
  {"BIT 4, L", CB_BIT_4L}, //0x65
  {"BIT 4, HL", CB_BIT_4HL}, //0x66
  {"BIT 4, A", CB_BIT_4A}, //0x67
  {"BIT 5, B", CB_BIT_5B}, //0x68
  {"BIT 5, C", CB_BIT_5C}, //0x69
  {"BIT 5, D", CB_BIT_5D}, //0x6A
  {"BIT 5, E", CB_BIT_5E}, //0x6B
  {"BIT 5, H", CB_BIT_5H}, //0x6C
  {"BIT 5, L", CB_BIT_5L}, //0x6D
  {"BIT 5, HL", CB_BIT_5HL}, //0x6E
  {"BIT 5, A", CB_BIT_5A}, //0x6F
  {"BIT 6, B", CB_BIT_6B}, //0x70
  {"BIT 6, C", CB_BIT_6C}, //0x71
  {"BIT 6, D", CB_BIT_6D}, //0x72
  {"BIT 6, E", CB_BIT_6E}, //0x73
  {"BIT 6, H", CB_BIT_6H}, //0x74
  {"BIT 6, L", CB_BIT_6L}, //0x75
  {"BIT 6, HL", CB_BIT_6HL}, //0x76
  {"BIT 6, A", CB_BIT_6A}, //0x77
  {"BIT 7, B", CB_BIT_7B}, //0x78
  {"BIT 7, C", CB_BIT_7C}, //0x79
  {"BIT 7, D", CB_BIT_7D}, //0x7A
  {"BIT 7, E", CB_BIT_7E}, //0x7B
  {"BIT 7, H", CB_BIT_7H}, //0x7C
  {"BIT 7, L", CB_BIT_7L}, //0x7D
  {"BIT 7, HL", CB_BIT_7HL}, //0x7E
  {"BIT 7, A", CB_BIT_7A}, //0x7F
  {"RES 0, B", CB_RES_0B}, //0x80
  {"RES 0, C", CB_RES_0C}, //0x81
  {"RES 0, D", CB_RES_0D}, //0x82
  {"RES 0, E", CB_RES_0E}, //0x83
  {"RES 0, H", CB_RES_0H}, //0x84
  {"RES 0, L", CB_RES_0L}, //0x85
  {"RES 0, HL", CB_RES_0HL}, //0x86
  {"RES 0, A", CB_RES_0A}, //0x87
  {"RES 1, B", CB_RES_1B}, //0x88
  {"RES 1, C", CB_RES_1C}, //0x89
  {"RES 1, D", CB_RES_1D}, //0x8A
  {"RES 1, E", CB_RES_1E}, //0x8B
  {"RES 1, H", CB_RES_1H}, //0x8C
  {"RES 1, L", CB_RES_1L}, //0x8D
  {"RES 1, HL", CB_RES_1HL}, //0x8E
  {"RES 1, A", CB_RES_1A}, //0x8F
  {"RES 2, B", CB_RES_2B}, //0x90
  {"RES 2, C", CB_RES_2C}, //0x91
  {"RES 2, D", CB_RES_2D}, //0x92
  {"RES 2, E", CB_RES_2E}, //0x93
  {"RES 2, H", CB_RES_2H}, //0x94
  {"RES 2, L", CB_RES_2L}, //0x95
  {"RES 2, HL", CB_RES_2HL}, //0x96
  {"RES 2, A", CB_RES_2A}, //0x97
  {"RES 3, B", CB_RES_3B}, //0x98
  {"RES 3, C", CB_RES_3C}, //0x99
  {"RES 3, D", CB_RES_3D}, //0x9A
  {"RES 3, E", CB_RES_3E}, //0x9B
  {"RES 3, H", CB_RES_3H}, //0x9C
  {"RES 3, L", CB_RES_3L}, //0x9D
  {"RES 3, HL", CB_RES_3HL}, //0x9E
  {"RES 3, A", CB_RES_3A}, //0x9F
  {"RES 4, B", CB_RES_4B}, //0xA0
  {"RES 4, C", CB_RES_4C}, //0xA1
  {"RES 4, D", CB_RES_4D}, //0xA2
  {"RES 4, E", CB_RES_4E}, //0xA3
  {"RES 4, H", CB_RES_4H}, //0xA4
  {"RES 4, L", CB_RES_4L}, //0xA5
  {"RES 4, HL", CB_RES_4HL}, //0xA6
  {"RES 4, A", CB_RES_4A}, //0xA7
  {"RES 5, B", CB_RES_5B}, //0xA8
  {"RES 5, C", CB_RES_5C}, //0xA9
  {"RES 5, D", CB_RES_5D}, //0xAA
  {"RES 5, E", CB_RES_5E}, //0xAB
  {"RES 5, H", CB_RES_5H}, //0xAC
  {"RES 5, L", CB_RES_5L}, //0xAD
  {"RES 5, HL", CB_RES_5HL}, //0xAE
  {"RES 5, A", CB_RES_5A}, //0xAF
  {"RES 6, B", CB_RES_6B}, //0xB0
  {"RES 6, C", CB_RES_6C}, //0xB1
  {"RES 6, D", CB_RES_6D}, //0xB2
  {"RES 6, E", CB_RES_6E}, //0xB3
  {"RES 6, H", CB_RES_6H}, //0xB4
  {"RES 6, L", CB_RES_6L}, //0xB5
  {"RES 6, HL", CB_RES_6HL}, //0xB6
  {"RES 6, A", CB_RES_6A}, //0xB7
  {"RES 7, B", CB_RES_7B}, //0xB8
  {"RES 7, C", CB_RES_7C}, //0xB9
  {"RES 7, D", CB_RES_7D}, //0xBA
  {"RES 7, E", CB_RES_7E}, //0xBB
  {"RES 7, H", CB_RES_7H}, //0xBC
  {"RES 7, L", CB_RES_7L}, //0xBD
  {"RES 7, HL", CB_RES_7HL}, //0xBE
  {"RES 7, A", CB_RES_7A}, //0xBF
  {"SET 0, B", CB_SET_0B}, //0xC0
  {"SET 0, C", CB_SET_0C}, //0xC1
  {"SET 0, D", CB_SET_0D}, //0xC2
  {"SET 0, E", CB_SET_0E}, //0xC3
  {"SET 0, H", CB_SET_0H}, //0xC4
  {"SET 0, L", CB_SET_0L}, //0xC5
  {"SET 0, HL", CB_SET_0HL}, //0xC6
  {"SET 0, A", CB_SET_0A}, //0xC7
  {"SET 1, B", CB_SET_1B}, //0xC8
  {"SET 1, C", CB_SET_1C}, //0xC9
  {"SET 1, D", CB_SET_1D}, //0xCA
  {"SET 1, E", CB_SET_1E}, //0xCB
  {"SET 1, H", CB_SET_1H}, //0xCC
  {"SET 1, L", CB_SET_1L}, //0xCD
  {"SET 1, HL", CB_SET_1HL}, //0xCE
  {"SET 1, A", CB_SET_1A}, //0xCF
  {"SET 2, B", CB_SET_2B}, //0xD0
  {"SET 2, C", CB_SET_2C}, //0xD1
  {"SET 2, D", CB_SET_2D}, //0xD2
  {"SET 2, E", CB_SET_2E}, //0xD3
  {"SET 2, H", CB_SET_2H}, //0xD4
  {"SET 2, L", CB_SET_2L}, //0xD5
  {"SET 2, HL", CB_SET_2HL}, //0xD6
  {"SET 2, A", CB_SET_2A}, //0xD7
  {"SET 3, B", CB_SET_3B}, //0xD8
  {"SET 3, C", CB_SET_3C}, //0xD9
  {"SET 3, D", CB_SET_3D}, //0xDA
  {"SET 3, E", CB_SET_3E}, //0xDB
  {"SET 3, H", CB_SET_3H}, //0xDC
  {"SET 3, L", CB_SET_3L}, //0xDD
  {"SET 3, HL", CB_SET_3HL}, //0xDE
  {"SET 3, A", CB_SET_3A}, //0xDF
  {"SET 4, B", CB_SET_4B}, //0xE0
  {"SET 4, C", CB_SET_4C}, //0xE1
  {"SET 4, D", CB_SET_4D}, //0xE2
  {"SET 4, E", CB_SET_4E}, //0xE3
  {"SET 4, H", CB_SET_4H}, //0xE4
  {"SET 4, L", CB_SET_4L}, //0xE5
  {"SET 4, HL", CB_SET_4HL}, //0xE6
  {"SET 4, A", CB_SET_4A}, //0xE7
  {"SET 5, B", CB_SET_5B}, //0xE8
  {"SET 5, C", CB_SET_5C}, //0xE9
  {"SET 5, D", CB_SET_5D}, //0xEA
  {"SET 5, E", CB_SET_5E}, //0xEB
  {"SET 5, H", CB_SET_5H}, //0xEC
  {"SET 5, L", CB_SET_5L}, //0xED
  {"SET 5, HL", CB_SET_5HL}, //0xEE
  {"SET 5, A", CB_SET_5A}, //0xEF
  {"SET 6, B", CB_SET_6B}, //0xF0
  {"SET 6, C", CB_SET_6C}, //0xF1
  {"SET 6, D", CB_SET_6D}, //0xF2
  {"SET 6, E", CB_SET_6E}, //0xF3
  {"SET 6, H", CB_SET_6H}, //0xF4
  {"SET 6, L", CB_SET_6L}, //0xF5
  {"SET 6, HL", CB_SET_6HL}, //0xF6
  {"SET 6, A", CB_SET_6A}, //0xF7
  {"SET 7, B", CB_SET_7B}, //0xF8
  {"SET 7, C", CB_SET_7C}, //0xF9
  {"SET 7, D", CB_SET_7D}, //0xFA
  {"SET 7, E", CB_SET_7E}, //0xFB
  {"SET 7, H", CB_SET_7H}, //0xFC
  {"SET 7, L", CB_SET_7L}, //0xFD
  {"SET 7, HL", CB_SET_7HL}, //0xFE
  {"SET 7, A", CB_SET_7A}, //0xFF
};

static_assert(sizeof(CB_Prefix)/sizeof(CB_Prefix[0]))==256, "CB table must have 256 entries");

#include "CPU_REG.h"

const struct GameBoyCPU instructions[256] = {
  {"NOP", 0, cpu_nop},//0x00
  {"LD BC", 2, cpu_LD_BC_nn},//0x01
  {"LD (BC),S", 0, cpu_LD_BC_S},//0x02
  {"INC (BC)", 0, cpu_INC_BC},//0x03
  {"INC B", 0, cpu_INC_B},//0x04
  {"DEC B", 0, cpu_DEC_B},//0x06
  {"LD B, ", 1, cpu_LD_B_n},//0x07
  {"LD[a16], SP", 2, cpu_LD_SP_nn},//0x08
  {"ADD HL,BC", 0, cpu_ADD_HL_BC},//0x09
  {"LD A,[BC]", 0, cpu_LD_A_BC},//0x0A
  {"DEC BC", 0, cpu_DEC_BC},//0x0B
  {"INC C", 0, cpu_INC_C},//0x0C
  {"DEC C", 0, cpu_DEC_C},//0x0D
  {"LD C,n8", 1, cpu_LD_C_n},//0x0E
  {"RRCA", 0, cpu_RRCA},//0x0F
  {"STOP n8", 1, cpu_STOP_n},//0x10
  {"LD DE, n16", 2, cpu_LD_DE_nn},//0x11
  {"LD[DE],A", 0, cpu_LD_DE_A},//0x12
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
  {"JR NC,e8", 1, cpu_JR_NC_n},//0x20
  {"LD HL,n16", 2, cpu_LD_HL_nn},//0x21
  {"LD[HL+], A", 0, cpu_LD_HL_A},//0x22
  {"INC HL", 0, cpu_INC_HL},//0x23
  {"INC H", 0, cpu_INC_H},//0x24
  {"DEC H", 0, cpu_DEC_H},//0x25
  {"LD H,n8", 1, cpu_LD_H_n},//0x26
  {"DAA", 0, cpu_DAA},//0x27
  {"JR Z, e8", 1, cpu_JR_Z_n},//0x28
  {"ADD HL, HL", 0, cpu_HL_HL},//0x29
  {"LD A,[HL+]", 0, cpu_LD_A_HL},//0x2A
  {"DEC HL", 0, cpu_DEC_HL},//0x2B
  {"INC L", 0, cpu_INC_L},//0x2C
  {"DEC L", 0, cpu_DEC_L},//0x2D
  {"LD L,n8", 1, cpu_LD_L_n},//0x2E
  {"CPL", 0, cpu_CPL},//0x2F
  {"JR NC,e8", 1, JR_NC_n},//0x30
  {"LS SP, n16", 2, cpu_LS_SP_nn},//0x31
  {"LD[HL-],A", 0, cpu_LD_HL_A},//0x32
  {"INC SP", 0, cpu_INC_SP},//0x33
  {"INC [HL]", 0, cpu_INC_HL},//0x34
  {"DEC [HL]", 0, cpu_DEC_HL},//0x35
  {"LD[HL], n8", 1, cpu_LD_HL},//0x36
  {"SCF", 0, cpu_SCF},//0x37
  {"JR C,e8", 1, cpu_JR_C_n},//0x38
  {"ADD HL, SP", 0, cpu_ADD_HL_SP},//0x39
  {"LD A, [HL-]", 0, cpu_LD_A_HL},//0x3A
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
  {"LD B, L", 0, cpu_LD_B_L},//0x45
  {"LD B, [HL]", 0, cpu_LD_B_HL},//0x46
  {"LD B, A", 0, cpu_LD_B_A},//0x47
  {"LD C, B", 0, cpu_LD_C_B},//0x48
  {"LD C, C", 0, cpu_LD_C_C},//0x49
  {"LD C, D", 0, cpu_LD_C_D},//0x4A
  {"LD C, E", 0, cpu_LD_C_E},//0x4B
  {"LD C, H", 0, cpu_LD_C_H},//0x4C
  {"LD C, L", 0, cpu_LD_C_L},//0x4D
  {"LD C, [HL]", 0, cpu_LD_C_HL},//0x4E
  {"LD C, A", 0, cpu_LD_C_A},//0x4F
  {"LD D, B", 0, cpu_LD_D_B},//0x50
  {"LD D, C", 0, cpu_LD_D_C},//0x51
  {"LD D, D", 0, cpu_LD_D_D},//0x52
  {"LD D, E", 0, cpu_LD_D_E},//0x53
  {"LD D, H", 0, cpu_LD_D_H},//0x54
  {"LD D, L", 0, cpu_LD_D_L},//0x55
  {"LD D, [HL]", 0, cpu_LD_D_HL},//0x56
  {"LD D, A", 0, cpu_LD_D_A},//0x57
  {"LD E, B", 0, cpu_LD_E_B},//0x58
  {"LD E, C", 0, cpu_LD_E_C},//0x59
  {"LD E, D", 0, cpu_LD_E_D},//0x5A
  {"LD E, E", 0, cpu_LD_E_E},//0x5B
  {"LD E, H", 0, cpu_LD_E_H},//0x5C
  {"LD E, L", 0, cpu_LD_E_L},//0x5D
  {"LD E, [HL]", 0, cpu_LD_E_HL},//0x5E
  {"LD E, A", 0, cpu_LD_E_A},//0x5F
  {"LD H, B", 0, cpu_LD_H_B},//0x60
  {"LD H, C", 0, cpu_LD_H_C},//0x61
  {"LD H, D", 0, cpu_LD_H_D},//0x62
  {"LD H, E", 0, cpu_LD_H_E},//0x63
  {"LD H, H", 0, cpu_LD_H_H},//0x64
  {"LD H, L", 0, cpu_LD_H_L},//0x65
  {"LD H, [HL]", 0, cpu_LD_H_HL},//0x66
  {"LD H, A", 0, cpu_LD_H_A},//0x67
  {"LD L, B", 0, cpu_LD_L_B},//0x68
  {"LD L, C", 0, cpu_LD_L_C},//0x69
  {"LD L, D", 0, cpu_LD_L_D},//0x6A
  {"LD L, E", 0, cpu_LD_L_E},//0x6B
  {"LD L, H",0, cpu_LD_L_H},//0x6C
  {"LD L, L", 0, cpu_LD_L_L},//0x6D
  {"LD L, [HL]", 0, cpu_LD_L_HL},//0x6E
  {"LD L, A", 0, cpu_LD_L_A },//0x6F
  {"LD[HL], B", 0, cpu_LD_HL_B},//0x70
  {"LD [HL], C", 0, cpu_LD_HL_C},//0x71
  {"LD [HL], D", 0, cpu_LD_HL_D},//0x72
  {"LD [HL], E", 0, cpu_LD_HL_E},//0x73
  {"LD [HL], H", 0, cpu_LD_HL_H},//0x74
  {"LD [HL], L", 0, cpu_LD_HL_L},//0x75
  {"HALT", 0, cpu_HALT},//0x76
  {"LD [HL], A", 0, cpu_LD_HL_A},//0x77
  {"LD A, B", 0, cpu_LD_A_B},//0x78
  {"LD A, C", 0, cpu_LD_A_C},//0x79
  {"LD A, D", 0, cpu_LD_A_D},//0x7A
  {"LD A, E", 0, cpu_LD_A_E},//0x7B
  {"LD A, H", 0, cpu_LD_A_H},//0x7C
  {"LD A, L", 0, cpu_LD_A_L},//0x7D
  {"LD A, [HL]", 0, cpu_LD_A_HL},//0x7E
  {"LD A, A", 0, cpu_LD_A_A},//0x7F
  {"ADD A, B", 0, cpu_ADD_A_B},//0x80
  {"ADD A, C", 0, cpu_ADD_A_C},//0x81
  {"ADD A, D", 0, cpu_ADD_A_D},//0x82
  {"ADD A, E", 0, cpu_ADD_A_E},//0x83
  {"ADD A, H", 0, cpu_ADD_A_H},//0x84
  {"ADD A, L", 0, cpu_ADD_A_L},//0x85
  {"ADD A, [HL]", 0, cpu_ADD_A_HL},//0x86
  {"ADD A, A", 0, cpu_ADD_A_A},//0x87
  {"ADC A, B", 0, cpu_ADC_A_B},//0x88
  {"ADC A, C", 0, cpu_ADC_A_C},//0x89
  {"ADC A, D", 0, cpu_ADC_A_D},//0x8A
  {"ADC A, E", 0, cpu_ADC_A_E},//0x8B
  {"ADC A, H", 0, cpu_ADC_A_H},//0x8C
  {"ADC A, L", 0, cpu_ADC_A_L},//0x8D
  {"ADC A, [HL]", 0, cpu_ADC_A_HL},//0x8E
  {"ADC A, A", 0, cpu_ADC_A_A},//0x8F
  {"SUB A, B", 0, cpu_SUB_A_B},//0x90
  {"SUB A, C", 0, cpu_SUB_A_C},//0x91
  {"SUB A, D", 0, cpu_SUB_A_D},//0x92
  {"SUB A, E", 0, cpu_SUB_A_E},//0x93
  {"SUB A, H", 0, cpu_SUB_A_H},//0x94
  {"SUB A, L", 0, cpu_SUB_A_L},//0x95
  {"SUB A, [HL]", 0, cpu_SUB_A_HL},//0x96
  {"SUB A, A", 0, cpu_SUB_A_A},//0x97
  {"SBC A, B", 0, cpu_SBC_A_B},//0x98
  {"SBC A, C", 0, cpu_SBC_A_C},//0x99
  {"SBC A, D", 0, cpu_SBC_A_D},//0x9A
  {"SBC A, E", 0, cpu_SBC_A_E},//0x9B
  {"SBC A, H", 0, cpu_SBC_A_H},//0x9C
  {"SBC A, L", 0, cpu_SBC_A_L},//0x9D
  {"SBC A, [HL]", 0, cpu_SBC_A_HL},//0x9E
  {"SBC A, A", 0, cpu_SBC_A_A},//0x9F
  {"AND A, B", 0, cpu_AND_A_B},//0xA0
  {"AND A, C", 0, cpu_AND_A_C},//0xA1
  {"AND A, D", 0, cpu_AND_A_D},//0xA2
  {"AND A, E", 0, cpu_AND_A_E},//0xA3
  {"AND A, H", 0, cpu_AND_A_H},//0xA4
  {"AND A, L", 0, cpu_AND_A_L},//0xA5
  {"AND A, [HL]", 0, cpu_AND_A_HL},//0xA6
  {"AND A, A", 0, cpu_AND_A_A},//0xA7
  {"XOR A, B", 0, cpu_XOR_A_B},//0xA8
  {"XOR A, C", 0, cpu_XOR_A_C},//0xA9
  {"XOR A, D", 0, cpu_XOR_A_D},//0xAA
  {"XOR A, E", 0, cpu_XOR_A_E},//0xAB
  {"XOR A, H", 0, cpu_XOR_A_H},//0xAC
  {"XOR A, L", 0, cpu_XOR_A_L},//0xAD
  {"XOR A, [HL]", 0, cpu_XOR_A_HL},//0xAE
  {"XOR A, A", 0, cpu_XOR_A_A},//0xAF
  {"OR A, B", 0, cpu_OR_A_B},//0xB0
  {"OR A, C", 0, cpu_OR_A_C},//0xB1
  {"OR A, D", 0, cpu_OR_A_D},//0xB2
  {"OR A, E", 0, cpu_OR_A_E},//0xB3
  {"OR A, H", 0, cpu_OR_A_H},//0xB4
  {"OR A, L", 0, cpu_OR_A_L},//0xB5
  {"OR A, [HL]", 0, cpu_OR_A_HL},//0xB6
  {"OR A, A", 0, cpu_OR_A_A},//0xB7
  {"CP A, B", 0, cpu_CP_A_B},//0xB8
  {"CP A, C", 0, cpu_CP_A_C},//0xB9
  {"CP A, D", 0, cpu_CP_A_D},//0xBA
  {"CP A, E", 0, cpu_CP_A_E},//0xBB
  {"CP A, H", 0, cpu_CP_A_H},//0xBC
  {"CP A, L", 0, cpu_CP_A_L},//0xBD
  {"CP A, [HL]", 0, cpu_CP_A_HL},//0xBE
  {"CP A, A", 0, cpu_CP_A_A},//0xBF
  {"RET NZ", 0, cpu_RET_NZ},//0xC0
  {"POP BC", 0, cpu_POP_BC},//0xC1
  {"JP NZ, a16", 2, cpu_JP_NZ_nn},//0xC2
  {"JP a16", 2, cpu_JP_nn},//0xC3
  {"CALL NZ, a16", 2, cpu_CALL_NZ_nn},//0xC4
  {"PUSH BC", 0, cpu_PUSH_BC},//0xC5
  {"ADD A, n8", 1, cpu_ADD_A_n},//0xC6
  {"RST $00", 0, cpu_RST_00},//0xC7
  {"RET Z", 0, cpu_RET_Z},//0xC8
  {"RET", 0, cpu_RET},//0xC9
  {"JP Z, a16", 2, cpu_JP_Z_nn},//0xCA
  {"PREFIX", 1, nullptr},//0xCB
  {"CALL Z, a16", 2, cpu_CALL_Z_nn},//0xCC
  {"CALL a16", 2, cpu_CALL_nn},//0xCD
  {"ADC A, n8", 1, cpu_ADC_A_n},//0xCE
  {"RST $08", 0, cpu_RST_08},//0xCF
  {"RET NC", 0, cpu_RET_NC},//0xD0
  {"POP DE", 0, cpu_POP_DE},//0xD1
  {"JP NC,a16", 2, cpu_JP_NC_nn},//0xD2
  {"0xD3", 0,nullptr},//0xD3
  {"CALL NC, a16", 2, cpu_CALL_NC_nn},//0xD4
  {"PUSH DE", 0, cpu_PUSH_DE},//0xD5
  {"SUB A, n8", 1, cpu_SUB_A_n},//0xD6
  {"RST $10", 0, cpu_RST_10},//0xD7
  {"RET C", 0, cpu_RET_C},//0xD8
  {"RETI", 0, cpu_RETI},//0xD9
  {"JP C, a16", 2, cpu_JP_C_nn},//0xDA
  {"0xDB", 0, nullptr},//0xDB
  {"CALL C, a16", 2, cpu_CALL_C_nn},//0xDC
  {"0xDD", 0, nullptr},//0xDD
  {"SBC A, n8", 1, cpu_SBC_A_n},//0xDE
  {"RST $18", 0, cpu_RST_18},//0xDF
  {"LDH[a8], A", 1, cpu_LDH_n_A},//0xE0
  {"POP HL", 0, cpu_POP_HL},//0xE1
  {"LDH[C], A", 0, cpu_LDH_C_A},//0xE2
  {"0xE3", 0, nullptr},//0xE3
  {"0xE4", 0, nullptr},//0xE4
  {"PUSH HL", 0, cpu_PUSH_HL},//0xE5
  {"AND A, n8", 1, cpu_AND_A_n},//0xE6
  {"RST $20", 0, cpu_RST_20},//0xE7
  {"ADD SP, e8", 1, cpu_ADD_SP_n},//0xE8
  {"JP HL", 0, cpu_JP_HL},//0xE9
  {"LD [a16], A", 2, cpu_LD_A_nn},//0xEA
  {"0xEB", 0, nullptr},//0xEB
  {"0xEC", 0, nullptr},//0xEC
  {"0xED", 0, nullptr},//0xED
  {"XOR A,n8", 1, cpu_XOR_A_n},//0xEE
  {"RST $28", 0, cpu_RST_28},//0xEF
  {"LDH A, [a8]", 1, cpu_LDH_A_n},//0xF0
  {"POP AF", 0, cpu_POP_AF},//0xF1
  {"LDH A, [C]", 0, cpu_LDH_A_C},//0xF2
  {"Dl", 0, cpu_DL},//0xF3
  {"0xF4", 0, nullptr},//0xF4
  {"PUSH AF", 0, cpu_PUSH_AF},//0xF5
  {"OR A, n8", 1, cpu_OR_A_n},//0xF6
  {"RST $30", 0, cpu_RST_30},//0xF7
  {"LD HL, SP + e8", 1,cpu_LD_HL_SP_N},//0xF8
  {"LD SP, HL", 0, cpu_LD_SP_HL},//0xF9
  {"LD A, [a16]", 2, cpu_LD_A_nn},//0xFA
  {"EL", 0, cpu_EL},//0xFB
  {"0xFC", 0, nullptr},//0xFC
  {"0xFD", 0, nullptr},//0xFD
  {"CP A,n8", 1, cpu_CP_A_n},//0xFE
  {"RST $38", 0, cpu_RST_38},//0xFF
};

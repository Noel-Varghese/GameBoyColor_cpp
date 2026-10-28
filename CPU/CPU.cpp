#include "CPU.h"
#include "CPU_REG.h"
#include "../HEART/heart.h"
#include "../Cartridge/Cartridge.h"
#include "../Log/logger.h"
#include <cstdio>
#include <string>

static Cartridge* cart = nullptr;
static CPU_registers registers;
static uint8_t current_opcode = 0;
static CPU_EXE_OP current_exec = nullptr;

static uint8_t mem_read(uint16_t addr){
  core_advance_cpu_clock(4);
  if(!cart) return 0xFF;
  return cart->read(addr);
}

static uint8_t fetch8(){//means read one byte drom the address currently pointed to by PC, then inc PC
  return cart->read(registers.PC++);
}

[[maybe_unused]] static uint16_t fetch16(){//combines 2 bytes into one 16-bit value
  uint8_t low = fetch8();//little endian
  uint8_t high = fetch8();
  return (uint16_t)((high << 8) | low);
}

static void setFlagz(bool z, bool n, bool h, bool c){//sets the Gameboy flag
  registers.F = (z ? FLAG_Z : 0) | (n ? FLAG_N : 0) | (h ? FLAG_H : 0) | (c ? FLAG_C : 0);
}

static void ALU_ADD(uint8_t v, bool is_c = false){
  uint8_t a = registers.A;
  uint8_t c = (is_c && (registers.F & FLAG_C)) ? 1:0;
  uint16_t sum = (uint16_t)a+v+c;
  registers.A = (uint8_t)sum;
  setFlagz(registers.A == 0, false, ((a & 0x0F) + (v & 0x0F) + c) > 0x0F, sum > 0xFF);
  //Z=result is 0, N=0for aff, H= carry out of bit 3, C=carry out of bit 7
}

static void ALU_SUB(uint8_t v, bool is_c = false, bool store = true){//for subtracting
  uint8_t a = registers.A;
  uint8_t c = (is_c && (registers.F & FLAG_C))?1:0;
  int res = (int)a-v-c;
  if(store) registers.A = (uint8_t)res;
  setFlagz(
	   (uint8_t)res == 0,
	   true,
	   (a & 0x0F) < ((v & 0x0F) + c), res < 0);
}

static void ALU_add_hl(uint16_t v){
  uint32_t sum = (uint32_t)registers.HL + v;
  setFlagz((registers.F & FLAG_Z) != 0, false, ((registers.HL & 0x0FFF) + (v & 0x0FFF)) > 0x0FFF,
	   sum > 0xFFFF);
  registers.HL = (uint16_t)sum;
  core_advance_cpu_clocks(4);
}


void CPU_init(Cartridge* c){
  cart = c;
}
  
void CPU_reset(){
  //boots after executing boot rom registers
  //needed to put the emu in the correct state
  if(cart && cart->is_cgb()){
    registers.AF = 0x1180;//Game Boy Color state
    registers.BC = 0x0000;
    registers.DE = 0xFF56;
    registers.HL = 0x000D;
  }else{
    registers.AF = 0x01B0;//original game boy (DMG) state, used for games that are not GBC games
    registers.BC = 0x0013;
    registers.DE = 0x00D8;
    registers.HL = 0x014D;
  }
  registers.SP = 0xFFFE;
  registers.PC = 0x0100;
}

[[maybe_unused]]static std::string trace_line(uint16_t PC){
  char buff[128];
  snprintf(buff, sizeof(buff), "PC=%04X op=%02X %-12s AF=%04X BC=%04X DE=%04X HL=%04X SP=%04X",
           PC, current_opcode, instructions[current_opcode].dissassembler,
           registers.AF, registers.BC, registers.DE, registers.HL, registers.SP);
  return std::string(buff);
}


static void CPU_fetch(){
  current_opcode = mem_read(registers.PC++);
  current_exec = instructions[current_opcode].exe;
}

static bool CPU_execute(){
  if(!current_exec){
    char buff[64];
    snprintf(buff, sizeof(buff), "UKNOWN OPCODE %02X AT PC=%04X",
	     current_opcode, (unsigned)(uint16_t)(registers.PC - 1));
    Log_ERROR(std::string(buff));
    return false;
  }
  current_exec();
  return true;
}

bool CPU_step(){
  [[maybe_unused]] const uint16_t PC = registers.PC;
  CPU_fetch();
  LOG_DEBUG(trace_line(PC));
  return CPU_execute();
}

void cpu_nop() {} //0x00
void cpu_LD_BC_nn() {} //0x01
void cpu_LD_BC_A() {} //0x02
void cpu_INC_BC() {} //0x03
void cpu_INC_B() {} //0x04
void cpu_DEC_B() {} //0x05
void cpu_RLCA() {} //0x07
void cpu_LD_B_n() {} //0x06
void cpu_LD_a16_SP() {} //0x08
void cpu_ADD_HL_BC() {} //0x09
void cpu_LD_A_BC() {} //0x0A
void cpu_DEC_BC() {} //0x0B
void cpu_INC_C() {} //0x0C
void cpu_DEC_C() {} //0x0D
void cpu_LD_C_n() {} //0x0E
void cpu_RRCA() {} //0x0F
void cpu_STOP_n() {} //0x10
void cpu_LD_DE_nn() {} //0x11
void cpu_LD_DE_A() {} //0x12
void cpu_INC_DE() {} //0x13
void cpu_INC_D() {} //0x14
void cpu_DEC_D() {} //0x15
void cpu_LD_D_n() {} //0x16
void cpu_RLA() {} //0x17
void cpu_JR_n() {} //0x18
void cpu_ADD_HL_DE() {} //0x19
void cpu_LD_A_DE() {} //0x1A
void cpu_DEC_DE() {} //0x1B
void cpu_INC_E() {} //0x1C
void cpu_DEC_E() {} //0x1D
void cpu_LD_E_n() {} //0x1E
void cpu_RRA() {} //0x1F
void cpu_JR_NZ_n() {} //0x20
void cpu_LD_HL_nn() {} //0x21
void cpu_LD_HL_A_INC() {} //0x22
void cpu_INC_HL() {} //0x23
void cpu_INC_H() {} //0x24
void cpu_DEC_H() {} //0x25
void cpu_LD_H_n() {} //0x26
void cpu_DAA() {} //0x27
void cpu_JR_Z_n() {} //0x28
void cpu_ADD_HL_HL() {} //0x29
void cpu_LD_A_HL_INC() {} //0x2A
void cpu_DEC_HL() {} //0x2B
void cpu_INC_L() {} //0x2C
void cpu_DEC_L() {} //0x2D
void cpu_LD_L_n() {} //0x2E
void cpu_CPL() {} //0x2F
void cpu_JR_NC_n() {} //0x30
void cpu_LD_SP_nn() {} //0x31
void cpu_LD_HL_A_DEC() {} //0x32
void cpu_INC_SP() {} //0x33
void cpu_INC_HL_MEM() {} //0x34
void cpu_DEC_HL_MEM() {} //0x35
void cpu_LD_HL_n() {} //0x36
void cpu_SCF() {} //0x37
void cpu_JR_C_n() {} //0x38
void cpu_ADD_HL_SP() {} //0x39
void cpu_LD_A_HL_DEC() {} //0x3A
void cpu_DEC_SP() {} //0x3B
void cpu_INC_A() {} //0x3C
void cpu_DEC_A() {} //0x3D
void cpu_LD_A_n() {} //0x3E
void cpu_CCF() {} //0x3F
void cpu_LD_B_B() {} //0x40
void cpu_LD_B_C() {} //0x41
void cpu_LD_B_D() {} //0x42
void cpu_LD_B_E() {} //0x43
void cpu_LD_B_H() {} //0x44
void cpu_LD_B_L() {} //0x45
void cpu_LD_B_HL() {} //0x46
void cpu_LD_B_A() {} //0x47
void cpu_LD_C_B() {} //0x48
void cpu_LD_C_C() {} //0x49
void cpu_LD_C_D() {} //0x4A
void cpu_LD_C_E() {} //0x4B
void cpu_LD_C_H() {} //0x4C
void cpu_LD_C_L() {} //0x4D
void cpu_LD_C_HL() {} //0x4E
void cpu_LD_C_A() {} //0x4F
void cpu_LD_D_B() {} //0x50
void cpu_LD_D_C() {} //0x51
void cpu_LD_D_D() {} //0x52
void cpu_LD_D_E() {} //0x53
void cpu_LD_D_H() {} //0x54
void cpu_LD_D_L() {} //0x55
void cpu_LD_D_HL() {} //0x56
void cpu_LD_D_A() {} //0x57
void cpu_LD_E_B() {} //0x58
void cpu_LD_E_C() {} //0x59
void cpu_LD_E_D() {} //0x5A
void cpu_LD_E_E() {} //0x5B
void cpu_LD_E_H() {} //0x5C
void cpu_LD_E_L() {} //0x5D
void cpu_LD_E_HL() {} //0x5E
void cpu_LD_E_A() {} //0x5F
void cpu_LD_H_B() {} //0x60
void cpu_LD_H_C() {} //0x61
void cpu_LD_H_D() {} //0x62
void cpu_LD_H_E() {} //0x63
void cpu_LD_H_H() {} //0x64
void cpu_LD_H_L() {} //0x65
void cpu_LD_H_HL() {} //0x66
void cpu_LD_H_A() {} //0x67
void cpu_LD_L_B() {} //0x68
void cpu_LD_L_C() {} //0x69
void cpu_LD_L_D() {} //0x6A
void cpu_LD_L_E() {} //0x6B
void cpu_LD_L_H() {} //0x6C
void cpu_LD_L_L() {} //0x6D
void cpu_LD_L_HL() {} //0x6E
void cpu_LD_L_A() {} //0x6F
void cpu_LD_HL_B() {} //0x70
void cpu_LD_HL_C() {} //0x71
void cpu_LD_HL_D() {} //0x72
void cpu_LD_HL_E() {} //0x73
void cpu_LD_HL_H() {} //0x74
void cpu_LD_HL_L() {} //0x75
void cpu_HALT() {} //0x76
void cpu_LD_HL_A() {} //0x77
void cpu_LD_A_B() {} //0x78
void cpu_LD_A_C() {} //0x79
void cpu_LD_A_D() {} //0x7A
void cpu_LD_A_E() {} //0x7B
void cpu_LD_A_H() {} //0x7C
void cpu_LD_A_L() {} //0x7D
void cpu_LD_A_HL() {} //0x7E
void cpu_LD_A_A() {} //0x7F
void cpu_ADD_A_B() {} //0x80
void cpu_ADD_A_C() {} //0x81
void cpu_ADD_A_D() {} //0x82
void cpu_ADD_A_E() {} //0x83
void cpu_ADD_A_H() {} //0x84
void cpu_ADD_A_L() {} //0x85
void cpu_ADD_A_HL() {} //0x86
void cpu_ADD_A_A() {} //0x87
void cpu_ADC_A_B() {} //0x88
void cpu_ADC_A_C() {} //0x89
void cpu_ADC_A_D() {} //0x8A
void cpu_ADC_A_E() {} //0x8B
void cpu_ADC_A_H() {} //0x8C
void cpu_ADC_A_L() {} //0x8D
void cpu_ADC_A_HL() {} //0x8E
void cpu_ADC_A_A() {} //0x8F
void cpu_SUB_A_B() {} //0x90
void cpu_SUB_A_C() {} //0x91
void cpu_SUB_A_D() {} //0x92
void cpu_SUB_A_E() {} //0x93
void cpu_SUB_A_H() {} //0x94
void cpu_SUB_A_L() {} //0x95
void cpu_SUB_A_HL() {} //0x96
void cpu_SUB_A_A() {} //0x97
void cpu_SBC_A_B() {} //0x98
void cpu_SBC_A_C() {} //0x99
void cpu_SBC_A_D() {} //0x9A
void cpu_SBC_A_E() {} //0x9B
void cpu_SBC_A_H() {} //0x9C
void cpu_SBC_A_L() {} //0x9D
void cpu_SBC_A_HL() {} //0x9E
void cpu_SBC_A_A() {} //0x9F
void cpu_AND_A_B() {} //0xA0
void cpu_AND_A_C() {} //0xA1
void cpu_AND_A_D() {} //0xA2
void cpu_AND_A_E() {} //0xA3
void cpu_AND_A_H() {} //0xA4
void cpu_AND_A_L() {} //0xA5
void cpu_AND_A_HL() {} //0xA6
void cpu_AND_A_A() {} //0xA7
void cpu_XOR_A_B() {} //0xA8
void cpu_XOR_A_C() {} //0xA9
void cpu_XOR_A_D() {} //0xAA
void cpu_XOR_A_E() {} //0xAB
void cpu_XOR_A_H() {} //0xAC
void cpu_XOR_A_L() {} //0xAD
void cpu_XOR_A_HL() {} //0xAE
void cpu_XOR_A_A() {} //0xAF
void cpu_OR_A_B() {} //0xB0
void cpu_OR_A_C() {} //0xB1
void cpu_OR_A_D() {} //0xB2
void cpu_OR_A_E() {} //0xB3
void cpu_OR_A_H() {} //0xB4
void cpu_OR_A_L() {} //0xB5
void cpu_OR_A_HL() {} //0xB6
void cpu_OR_A_A() {} //0xB7
void cpu_CP_A_B() {} //0xB8
void cpu_CP_A_C() {} //0xB9
void cpu_CP_A_D() {} //0xBA
void cpu_CP_A_E() {} //0xBB
void cpu_CP_A_H() {} //0xBC
void cpu_CP_A_L() {} //0xBD
void cpu_CP_A_HL() {} //0xBE
void cpu_CP_A_A() {} //0xBF
void cpu_RET_NZ() {} //0xC0
void cpu_POP_BC() {} //0xC1
void cpu_JP_NZ_nn() {} //0xC2
void cpu_JP_nn() {} //0xC3
void cpu_CALL_NZ_nn() {} //0xC4
void cpu_PUSH_BC() {} //0xC5
void cpu_ADD_A_n() {} //0xC6
void cpu_RST_00() {} //0xC7
void cpu_RET_Z() {} //0xC8
void cpu_RET() {} //0xC9
void cpu_JP_Z_nn() {} //0xCA
void cpu_CALL_Z_nn() {} //0xCC
void cpu_CALL_nn() {} //0xCD
void cpu_ADC_A_n() {} //0xCE
void cpu_RST_08() {} //0xCF
void cpu_RET_NC() {} //0xD0
void cpu_POP_DE() {} //0xD1
void cpu_JP_NC_nn() {} //0xD2
void cpu_CALL_NC_nn() {} //0xD4
void cpu_PUSH_DE() {} //0xD5
void cpu_SUB_A_n() {} //0xD6
void cpu_RST_10() {} //0xD7
void cpu_RET_C() {} //0xD8
void cpu_RETI() {} //0xD9
void cpu_JP_C_nn() {} //0xDA
void cpu_CALL_C_nn() {} //0xDC
void cpu_SBC_A_n() {} //0xDE
void cpu_RST_18() {} //0xDF
void cpu_LDH_n_A() {} //0xE0
void cpu_POP_HL() {} //0xE1
void cpu_LDH_C_A() {} //0xE2
void cpu_PUSH_HL() {} //0xE5
void cpu_AND_A_n() {} //0xE6
void cpu_RST_20() {} //0xE7
void cpu_ADD_SP_n() {} //0xE8
void cpu_JP_HL() {} //0xE9
void cpu_LD_nn_A() {} //0xEA
void cpu_XOR_A_n() {} //0xEE
void cpu_RST_28() {} //0xEF
void cpu_LDH_A_n() {} //0xF0
void cpu_POP_AF() {} //0xF1
void cpu_LDH_A_C() {} //0xF2
void cpu_DI() {} //0xF3
void cpu_PUSH_AF() {} //0xF5
void cpu_OR_A_n() {} //0xF6
void cpu_RST_30() {} //0xF7
void cpu_LD_HL_SP_N() {} //0xF8
void cpu_LD_SP_HL() {} //0xF9
void cpu_LD_A_nn() {} //0xFA
void cpu_EI() {} //0xFB
void cpu_CP_A_n() {} //0xFE
void cpu_RST_38() {} //0xFF   

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
static GBModel model = GBModel::AUTO;
static bool ime = false;
static uint8_t ime_delay = 0;

void CPU_set_model(GBModel m){
  model = m;
}


static void setFlagz(bool z, bool n, bool h, bool c){//sets the Gameboy flag
  registers.F = (z ? FLAG_Z : 0) | (n ? FLAG_N : 0) | (h ? FLAG_H : 0) | (c ? FLAG_C : 0);
}

#include "CPU_INS.h"
/*
  the #include "CPU_INS.h" is put here as it means paste the file here if it was included with the other headers it would give error as some of the things are defined over here
 */

void CPU_init(Cartridge* c){
  cart = c;
}
  
void CPU_reset(){
  //boots after executing boot rom registers
  //needed to put the emu in the correct state
  GBModel m = model;
  if(m == GBModel::AUTO) m = (cart && cart->is_cgb()) ? GBModel::CGB : GBModel::DMG;
  const char* name = "DMG";
  switch(m){
    case GBModel::CGB:
    name = "CGB (GAME BOY COLOR)";
    registers.AF = 0x1180;
    registers.BC = 0x0000;
    registers.DE = 0xFF56;
    registers.HL = 0x000D;
    break;
    case GBModel::SGB:
    name = "SGB (SUPER GAME BOY)";
    registers.AF = 0x0100;
    registers.BC = 0x0014;
    registers.DE = 0x0000;
    registers.HL = 0xC060;
    break;
    case GBModel::SGB2:
    name = "SGB2 (SUPER GAME BOY 2)";
    registers.AF = 0xFF00;
    registers.BC = 0x0014;
    registers.DE = 0x0000;
    registers.HL = 0xC060;
    break;
    default:
    registers.AF = 0x01B0;
    registers.BC = 0x0013;
    registers.DE = 0x00D8;
    registers.HL = 0x014D;
    break;
  }
  registers.SP = 0xFFFE;
  registers.PC = 0x0100;
  Log_INFO(std::string("CPU reset, model: ") + name);
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
    snprintf(buff, sizeof(buff), "UNKNOWN OPCODE %02X AT PC=%04X",
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
  bool ok = CPU_execute();
  if(ime_delay && --ime_delay == 0) ime = true;
  return ok;
}

void cpu_nop() {} //0x00
void cpu_LD_BC_nn() { registers.BC = fetch16(); } //0x01
void cpu_LD_BC_A() { mem_write(registers.BC, registers.A); } //0x02
void cpu_INC_BC() {
  registers.BC++;
  core_advance_cpu_clock(4);
} //0x03
void cpu_INC_B() {
  INC8(dest_r8());
} //0x04
void cpu_DEC_B() {
  DEC8(dest_r8());
} //0x05
void cpu_RLCA() {ROTATE_RLCA();} //0x07
void cpu_LD_B_n() { set_r8(dest_r8(), fetch8()); } //0x06
void cpu_LD_a16_SP() {
  uint16_t address = fetch16();
  mem_write(address, registers.SP & 0xFF);
  mem_write(address+1, registers.SP >> 8);
} //0x08
void cpu_ADD_HL_BC() {ALU_ADD_HL(registers.BC);}//0x09
void cpu_LD_A_BC() {registers.A = mem_read(registers.BC);} //0x0A
void cpu_DEC_BC() {
  registers.BC--;
  core_advance_cpu_clock(4);
} //0x0B
void cpu_INC_C() {INC8(dest_r8());} //0x0C
void cpu_DEC_C() {DEC8(dest_r8());} //0x0D
void cpu_LD_C_n() {set_r8(dest_r8(), fetch8());} //0x0E
void cpu_RRCA() {ROTATE_RRCA();} //0x0F
void cpu_STOP_n() {
  fetch8();
  //To Be Continued
} //0x10
void cpu_LD_DE_nn() {registers.DE = fetch16();} //0x11
void cpu_LD_DE_A() {mem_write(registers.DE, registers.A);} //0x12
void cpu_INC_DE() {
  registers.DE++;
  core_advance_cpu_clock(4);
} //0x13
void cpu_INC_D() {INC8(dest_r8());} //0x14
void cpu_DEC_D() {DEC8(dest_r8());} //0x15
void cpu_LD_D_n() {set_r8(dest_r8(), fetch8());} //0x16
void cpu_RLA() {ROTATE_RLA();} //0x17
void cpu_JR_n() {JR(true);} //0x18
void cpu_ADD_HL_DE() {ALU_ADD_HL(registers.DE);} //0x19
void cpu_LD_A_DE() {registers.A = mem_read(registers.DE);} //0x1A
void cpu_DEC_DE() {
  registers.DE--;
  core_advance_cpu_clock(4);
} //0x1B
void cpu_INC_E() {INC8(dest_r8());} //0x1C
void cpu_DEC_E() {DEC8(dest_r8());} //0x1D
void cpu_LD_E_n() {set_r8(dest_r8(), fetch8());} //0x1E
void cpu_RRA() {ROTATE_RRA();} //0x1F
void cpu_JR_NZ_n() {JR(cond());} //0x20
void cpu_LD_HL_nn() {registers.HL = fetch16();} //0x21
void cpu_LD_HL_A_INC() {
  mem_write(registers.HL++,
	    registers.A);
} //0x22
void cpu_INC_HL() {
  registers.HL++;
  core_advance_cpu_clock(4);
} //0x23
void cpu_INC_H() {INC8(dest_r8());} //0x24
void cpu_DEC_H() {DEC8(dest_r8());} //0x25
void cpu_LD_H_n() {
  set_r8(dest_r8(), fetch8());} //0x26
void cpu_DAA() {ALU_DAA();} //0x27
void cpu_JR_Z_n() {JR(cond());} //0x28
void cpu_ADD_HL_HL() {ALU_ADD_HL(registers.HL);} //0x29
void cpu_LD_A_HL_INC() {registers.A = mem_read(registers.HL++);} //0x2A
void cpu_DEC_HL() {
  registers.HL--;
  core_advance_cpu_clock(4);
} //0x2B
void cpu_INC_L() {INC8(dest_r8());} //0x2C
void cpu_DEC_L() {DEC8(dest_r8());} //0x2D
void cpu_LD_L_n() {set_r8(dest_r8(), fetch8());} //0x2E
void cpu_CPL() {
  registers.A = ~registers.A;
  setFlagz(flag_z(), true, true, flag_c());
} //0x2F
void cpu_JR_NC_n() {JR(cond());} //0x30
void cpu_LD_SP_nn() {registers.SP = fetch16();} //0x31
void cpu_LD_HL_A_DEC() {
  mem_write(registers.HL--,
	    registers.A);
} //0x32
void cpu_INC_SP() {
  registers.SP++;
  core_advance_cpu_clock(4);
} //0x33
void cpu_INC_HL_MEM() {INC8(dest_r8());} //0x34
void cpu_DEC_HL_MEM() {DEC8(dest_r8());} //0x35
void cpu_LD_HL_n() {
  set_r8(dest_r8(), fetch8());
} //0x36
void cpu_SCF() {setFlagz(flag_z(), false, false, true);} //0x37
void cpu_JR_C_n() {JR(cond());} //0x38
void cpu_ADD_HL_SP() {ALU_ADD_HL(registers.SP);} //0x39
void cpu_LD_A_HL_DEC() {registers.A = mem_read(registers.HL--);} //0x3A
void cpu_DEC_SP() {
  registers.SP--;
  core_advance_cpu_clock(4);
} //0x3B
void cpu_INC_A() {INC8(dest_r8());} //0x3C
void cpu_DEC_A() {DEC8(dest_r8());} //0x3D
void cpu_LD_A_n() {set_r8(dest_r8(), fetch8());} //0x3E
void cpu_CCF() {setFlagz(flag_z(), false, false, !flag_c());} //0x3F
void cpu_LD_B_B() {set_r8(dest_r8(), src_r8());} //0x40
void cpu_LD_B_C() {set_r8(dest_r8(), src_r8());} //0x41
void cpu_LD_B_D() {set_r8(dest_r8(), src_r8());} //0x42
void cpu_LD_B_E() {set_r8(dest_r8(), src_r8());} //0x43
void cpu_LD_B_H() {set_r8(dest_r8(), src_r8());} //0x44
void cpu_LD_B_L() {set_r8(dest_r8(), src_r8());} //0x45
void cpu_LD_B_HL() {set_r8(dest_r8(), src_r8());} //0x46
void cpu_LD_B_A() {set_r8(dest_r8(), src_r8());} //0x47
void cpu_LD_C_B() {set_r8(dest_r8(), src_r8());} //0x48
void cpu_LD_C_C() {set_r8(dest_r8(), src_r8());} //0x49
void cpu_LD_C_D() {set_r8(dest_r8(), src_r8());} //0x4A
void cpu_LD_C_E() {set_r8(dest_r8(), src_r8());} //0x4B
void cpu_LD_C_H() {set_r8(dest_r8(), src_r8());} //0x4C
void cpu_LD_C_L() {set_r8(dest_r8(), src_r8());} //0x4D
void cpu_LD_C_HL() {set_r8(dest_r8(), src_r8());} //0x4E
void cpu_LD_C_A() {set_r8(dest_r8(), src_r8());} //0x4F
void cpu_LD_D_B() {set_r8(dest_r8(), src_r8());} //0x50
void cpu_LD_D_C() {set_r8(dest_r8(), src_r8());} //0x51
void cpu_LD_D_D() {set_r8(dest_r8(), src_r8());} //0x52
void cpu_LD_D_E() {set_r8(dest_r8(), src_r8());} //0x53
void cpu_LD_D_H() {set_r8(dest_r8(), src_r8());} //0x54
void cpu_LD_D_L() {set_r8(dest_r8(), src_r8());} //0x55
void cpu_LD_D_HL() {set_r8(dest_r8(), src_r8());} //0x56
void cpu_LD_D_A() {set_r8(dest_r8(), src_r8());} //0x57
void cpu_LD_E_B() {set_r8(dest_r8(), src_r8());} //0x58
void cpu_LD_E_C() {set_r8(dest_r8(), src_r8());} //0x59
void cpu_LD_E_D() {set_r8(dest_r8(), src_r8());} //0x5A
void cpu_LD_E_E() {set_r8(dest_r8(), src_r8());} //0x5B
void cpu_LD_E_H() {set_r8(dest_r8(), src_r8());} //0x5C
void cpu_LD_E_L() {set_r8(dest_r8(), src_r8());} //0x5D
void cpu_LD_E_HL() {set_r8(dest_r8(), src_r8());} //0x5E
void cpu_LD_E_A() {set_r8(dest_r8(), src_r8());} //0x5F
void cpu_LD_H_B() {set_r8(dest_r8(), src_r8());} //0x60
void cpu_LD_H_C() {set_r8(dest_r8(), src_r8());} //0x61
void cpu_LD_H_D() {set_r8(dest_r8(), src_r8());} //0x62
void cpu_LD_H_E() {set_r8(dest_r8(), src_r8());} //0x63
void cpu_LD_H_H() {set_r8(dest_r8(), src_r8());} //0x64
void cpu_LD_H_L() {set_r8(dest_r8(), src_r8());} //0x65
void cpu_LD_H_HL() {set_r8(dest_r8(), src_r8());} //0x66
void cpu_LD_H_A() {set_r8(dest_r8(), src_r8());} //0x67
void cpu_LD_L_B() {set_r8(dest_r8(), src_r8());} //0x68
void cpu_LD_L_C() {set_r8(dest_r8(), src_r8());} //0x69
void cpu_LD_L_D() {set_r8(dest_r8(), src_r8());} //0x6A
void cpu_LD_L_E() {set_r8(dest_r8(), src_r8());} //0x6B
void cpu_LD_L_H() {set_r8(dest_r8(), src_r8());} //0x6C
void cpu_LD_L_L() {set_r8(dest_r8(), src_r8());} //0x6D
void cpu_LD_L_HL() {set_r8(dest_r8(), src_r8());} //0x6E
void cpu_LD_L_A() {set_r8(dest_r8(), src_r8());} //0x6F
void cpu_LD_HL_B() {set_r8(dest_r8(), src_r8());} //0x70
void cpu_LD_HL_C() {set_r8(dest_r8(), src_r8());} //0x71
void cpu_LD_HL_D() {set_r8(dest_r8(), src_r8());} //0x72
void cpu_LD_HL_E() {set_r8(dest_r8(), src_r8());} //0x73
void cpu_LD_HL_H() {set_r8(dest_r8(), src_r8());} //0x74
void cpu_LD_HL_L() {set_r8(dest_r8(), src_r8());} //0x75
void cpu_HALT() {} //0x76
void cpu_LD_HL_A() {set_r8(dest_r8(), src_r8());} //0x77
void cpu_LD_A_B() {set_r8(dest_r8(), src_r8());} //0x78
void cpu_LD_A_C() {set_r8(dest_r8(), src_r8());} //0x79
void cpu_LD_A_D() {set_r8(dest_r8(), src_r8());} //0x7A
void cpu_LD_A_E() {set_r8(dest_r8(), src_r8());} //0x7B
void cpu_LD_A_H() {set_r8(dest_r8(), src_r8());} //0x7C
void cpu_LD_A_L() {set_r8(dest_r8(), src_r8());} //0x7D
void cpu_LD_A_HL() {set_r8(dest_r8(), src_r8());} //0x7E
void cpu_LD_A_A() {set_r8(dest_r8(), src_r8());} //0x7F
void cpu_ADD_A_B() {ALU_ADD(src_r8());} //0x80
void cpu_ADD_A_C() {ALU_ADD(src_r8());} //0x81
void cpu_ADD_A_D() {ALU_ADD(src_r8());} //0x82
void cpu_ADD_A_E() {ALU_ADD(src_r8());} //0x83
void cpu_ADD_A_H() {ALU_ADD(src_r8());} //0x84
void cpu_ADD_A_L() {ALU_ADD(src_r8());} //0x85
void cpu_ADD_A_HL() {ALU_ADD(src_r8());} //0x86
void cpu_ADD_A_A() {ALU_ADD(src_r8());} //0x87
void cpu_ADC_A_B() {ALU_ADD(src_r8(), true);} //0x88
void cpu_ADC_A_C() {ALU_ADD(src_r8(), true);} //0x89
void cpu_ADC_A_D() {ALU_ADD(src_r8(), true);} //0x8A
void cpu_ADC_A_E() {ALU_ADD(src_r8(), true);} //0x8B
void cpu_ADC_A_H() {ALU_ADD(src_r8(), true);} //0x8C
void cpu_ADC_A_L() {ALU_ADD(src_r8(), true);} //0x8D
void cpu_ADC_A_HL() {ALU_ADD(src_r8(), true);} //0x8E
void cpu_ADC_A_A() {ALU_ADD(src_r8(), true);} //0x8F
void cpu_SUB_A_B() {ALU_SUB(src_r8());} //0x90
void cpu_SUB_A_C() {ALU_SUB(src_r8());} //0x91
void cpu_SUB_A_D() {ALU_SUB(src_r8());} //0x92
void cpu_SUB_A_E() {ALU_SUB(src_r8());} //0x93
void cpu_SUB_A_H() {ALU_SUB(src_r8());} //0x94
void cpu_SUB_A_L() {ALU_SUB(src_r8());} //0x95
void cpu_SUB_A_HL() {ALU_SUB(src_r8());} //0x96
void cpu_SUB_A_A() {ALU_SUB(src_r8());} //0x97
void cpu_SBC_A_B() {ALU_SUB(src_r8(), true);} //0x98
void cpu_SBC_A_C() {ALU_SUB(src_r8(), true);} //0x99
void cpu_SBC_A_D() {ALU_SUB(src_r8(), true);} //0x9A
void cpu_SBC_A_E() {ALU_SUB(src_r8(), true);} //0x9B
void cpu_SBC_A_H() {ALU_SUB(src_r8(), true);} //0x9C
void cpu_SBC_A_L() {ALU_SUB(src_r8(), true);} //0x9D
void cpu_SBC_A_HL() {ALU_SUB(src_r8(), true);} //0x9E
void cpu_SBC_A_A() {ALU_SUB(src_r8(), true);} //0x9F
void cpu_AND_A_B() {ALU_AND(src_r8());} //0xA0
void cpu_AND_A_C() {ALU_AND(src_r8());} //0xA1
void cpu_AND_A_D() {ALU_AND(src_r8());} //0xA2
void cpu_AND_A_E() {ALU_AND(src_r8());} //0xA3
void cpu_AND_A_H() {ALU_AND(src_r8());} //0xA4
void cpu_AND_A_L() {ALU_AND(src_r8());} //0xA5
void cpu_AND_A_HL() {ALU_AND(src_r8());} //0xA6
void cpu_AND_A_A() {ALU_AND(src_r8());} //0xA7
void cpu_XOR_A_B() {ALU_XOR(src_r8());} //0xA8
void cpu_XOR_A_C() {ALU_XOR(src_r8());} //0xA9
void cpu_XOR_A_D() {ALU_XOR(src_r8());} //0xAA
void cpu_XOR_A_E() {ALU_XOR(src_r8());} //0xAB
void cpu_XOR_A_H() {ALU_XOR(src_r8());} //0xAC
void cpu_XOR_A_L() {ALU_XOR(src_r8());} //0xAD
void cpu_XOR_A_HL() {ALU_XOR(src_r8());} //0xAE
void cpu_XOR_A_A() {ALU_XOR(src_r8());} //0xAF
void cpu_OR_A_B() {ALU_OR(src_r8());} //0xB0
void cpu_OR_A_C() {ALU_OR(src_r8());} //0xB1
void cpu_OR_A_D() {ALU_OR(src_r8());} //0xB2
void cpu_OR_A_E() {ALU_OR(src_r8());} //0xB3
void cpu_OR_A_H() {ALU_OR(src_r8());} //0xB4
void cpu_OR_A_L() {ALU_OR(src_r8());} //0xB5
void cpu_OR_A_HL() {ALU_OR(src_r8());} //0xB6
void cpu_OR_A_A() {ALU_OR(src_r8());} //0xB7
void cpu_CP_A_B() {ALU_SUB(src_r8(), false, false);} //0xB8
void cpu_CP_A_C() {ALU_SUB(src_r8(), false, false);} //0xB9
void cpu_CP_A_D() {ALU_SUB(src_r8(), false, false);} //0xBA
void cpu_CP_A_E() {ALU_SUB(src_r8(), false, false);} //0xBB
void cpu_CP_A_H() {ALU_SUB(src_r8(), false, false);} //0xBC
void cpu_CP_A_L() {ALU_SUB(src_r8(), false, false);} //0xBD
void cpu_CP_A_HL() {ALU_SUB(src_r8(), false, false);} //0xBE
void cpu_CP_A_A() {ALU_SUB(src_r8(), false, false);} //0xBF
void cpu_RET_NZ() {RET_CC(cond());} //0xC0
void cpu_POP_BC() {registers.BC = pop16();} //0xC1
void cpu_JP_NZ_nn() {JP(cond());} //0xC2
void cpu_JP_nn() {JP(true);} //0xC3
void cpu_CALL_NZ_nn() {CALL(cond());} //0xC4
void cpu_PUSH_BC() {PUSH(registers.BC);} //0xC5
void cpu_ADD_A_n() {ALU_ADD(fetch8());} //0xC6
void cpu_RST_00() {RST(0x00);} //0xC7
void cpu_RET_Z() {RET_CC(cond());} //0xC8
void cpu_RET() {
  registers.PC = pop16();
  core_advance_cpu_clock(4);
} //0xC9
void cpu_JP_Z_nn() {JP(cond()); } //0xCA
void cpu_CALL_Z_nn() {CALL(cond());} //0xCC
void cpu_CALL_nn() {CALL(true);} //0xCD
void cpu_ADC_A_n() {ALU_ADD(fetch8(), true);} //0xCE
void cpu_RST_08() {RST(0x08);} //0xCF
void cpu_RET_NC() {RET_CC(cond());} //0xD0
void cpu_POP_DE() {registers.DE = pop16();} //0xD1
void cpu_JP_NC_nn() {JP(cond());} //0xD2
void cpu_CALL_NC_nn() {CALL(cond());} //0xD4
void cpu_PUSH_DE() {PUSH(registers.DE);} //0xD5
void cpu_SUB_A_n() {ALU_SUB(fetch8());} //0xD6
void cpu_RST_10() {RST(0x10);} //0xD7
void cpu_RET_C() {RET_CC(cond());} //0xD8
void cpu_RETI() {
  registers.PC = pop16();
  core_advance_cpu_clock(4);
  ime = true;
} //0xD9
void cpu_JP_C_nn() {JP(cond());} //0xDA
void cpu_CALL_C_nn() {CALL(cond());} //0xDC
void cpu_SBC_A_n() {ALU_SUB(fetch8(), true);} //0xDE
void cpu_RST_18() {RST(0x18);} //0xDF
void cpu_LDH_n_A() {
  mem_write(
	    0xFF00 + fetch8(),
	    registers.A
	    );
} //0xE0
void cpu_POP_HL() {registers.HL = pop16();} //0xE1
void cpu_LDH_C_A() {
  mem_write(
	    0xFF00 + registers.C,
	    registers.A
	    );
} //0xE2
void cpu_PUSH_HL() {PUSH(registers.HL);} //0xE5
void cpu_AND_A_n() {ALU_AND(fetch8());} //0xE6
void cpu_RST_20() {RST(0x20);} //0xE7
void cpu_ADD_SP_n() {
  registers.SP = SP_plus_e8();
  core_advance_cpu_clock(8);
} //0xE8
void cpu_JP_HL() {registers.PC = registers.HL;} //0xE9
void cpu_LD_nn_A() {
  mem_write(fetch16(), registers.A);
} //0xEA
void cpu_XOR_A_n() {ALU_XOR(fetch8());} //0xEE
void cpu_RST_28() {RST(0x28);} //0xEF
void cpu_LDH_A_n() {
  registers.A = mem_read(0xFF00 + fetch8());
} //0xF0
void cpu_POP_AF() {registers.AF = pop16() & 0xFFF0;} //0xF1
void cpu_LDH_A_C() {registers.A = mem_read(0xFF00 + registers.C);} //0xF2
void cpu_DI() {
  ime = false;
  ime_delay = 0;
} //0xF3
void cpu_PUSH_AF() {PUSH(registers.AF);} //0xF5
void cpu_OR_A_n() {ALU_OR(fetch8());} //0xF6
void cpu_RST_30() {RST(0x30);} //0xF7
void cpu_LD_HL_SP_N() {
  registers.HL = SP_plus_e8();
  core_advance_cpu_clock(4);
} //0xF8
void cpu_LD_SP_HL() {
  registers.SP = registers.HL;
  core_advance_cpu_clock(4);
} //0xF9
void cpu_LD_A_nn() {
  registers.A = mem_read(fetch16());
} //0xFA
void cpu_EI() {
  ime_delay = 2;
} //0xFB
void cpu_CP_A_n() {
  ALU_SUB(fetch8(), false, false);
} //0xFE
void cpu_RST_38() {RST(0x38);} //0xFF   

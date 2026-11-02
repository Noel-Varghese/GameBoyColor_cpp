#pragma once

//flag readers
static inline bool flag_z(){ return (registers.F & FLAG_Z) != 0; }
static inline bool flag_n(){ return (registers.F & FLAG_N) != 0; }
static inline bool flag_h(){ return (registers.F & FLAG_H) != 0; }
static inline bool flag_c(){ return (registers.F & FLAG_C) != 0; }

//inorder to read and write the memory bus

static uint8_t mem_read(uint16_t address){
  core_advance_cpu_clock(4);//explain how many T-cycles are consumed  
  return cart->read(address);//reads a byte from the memory
}

mem_write(uint16_t address, uint8_t value){
  core_advance_cpu_clock(4);
  (void)address;
  (void)value;//send the data to the memory bus; TO BE CONTINUED ONCE MEMORY_BUS.h/.cpp is done
}

static uint8_t fetch8(){
  return mem_read(registers.PC++);//helps read the next byte of instructions
}

[[maybe_unused]] static uint16_t fetch16(){//reads 16 bit value from the instruction stream
  uint8_t low = fetch8();
  uint8_t high = fetch8();
  return (uint16_t)((high << 8) | low);//reads a 16-bit immediate value as little-endian by fetching lowByte first and then the highByte
}

//the bits of an opcode say which registers to use, so this functions help serve a whole group
static uint8_t get_r8(uint8_t i){
  switch(i & 0x07){
  case 0: return registers.B;
  case 1: return registers.C;
  case 2: return registers.D;
  case 3: return registers.E;
  case 4: return registers.H;
  case 5: return registers.L;
  case 6: return mem_read(registers.HL);
  default: return registers.A;
  }
}

static uint8_t set_r8(uint8_t i, uint8_t v){
  switch(i & 0x07){
  case 0: return registers.B = v; break;
  case 1: return registers.C = v; break;
  case 2: return registers.D = v; break;
  case 3: return registers.E = v; break;
  case 4: return registers.H = v; break;
  case 5: return registers.L = v; break;
  case 6: return mem_read(registers.HL);
  default: return registers.A;
  }
}



/*
src_r8 is the traffic police for opcodes 0x80 -> 0xBF
the last 3 bits of the opcode already say which register
   to use, so we only have to read them:

        last 3 bits   register
        000 (0)       B
        001 (1)       C
        010 (2)       D
        011 (3)       E
        100 (4)       H
        101 (5)       L
        110 (6)       [HL]  = the byte in memory at address HL
        111 (7)       A

   "& 0x07" keeps only those last 3 bits.
   Reading [HL] touches memory, so it costs 4 extra cycles.

   So every instruction in the group uses the same one-liner:
        void cpu_ADD_A_B() { ALU_ADD(src_r8()); }
*/

static uint8_t src_r8(){ return get_r8(current_opcode); }
/*
┌───────────┬───────────┬───────────────┐
│  Opcode   │   DEST    │      SRC      │
│  8 bits   │  3 bits   │     3 bits    │
└───────────┴───────────┴───────────────┘
 */
static uint8_t dest_r8(){ return (current_opcode >> 3) & 0x07; }

static void ALU_ADD(uint8_t v, bool is_c = false){//used for addition
  uint8_t a = registers.A;
  uint8_t c = (is_c && (registers.F & FLAG_C))?1:0;
  uint16_t sum = (uint16_t)a+v+c;
  registers.A = (uint8_t)sum;
  setFlagz(registers.A == 0, false, ((a & 0x0F) + (v & 0x0F) + c) > 0x0F, sum > 0xFF);
  //Z=result is 0, N=0for aff, H= carry out of bit 3, C=carry out of bit 7
}

//conditions for JUMP, JR, CALL, RETURN: bits 4-3
static bool condition(){
  switch((current_opcode >> 3) & 0x03){
  case 0: return !FLAG_Z();
  case 1: return FLAG_Z();
  case 2: return !FLAG_C();
  default: return FLAG_C();
  }
}

//8-bit ALU

static void ALU_SUB(uint8_t v, bool is_c = 0, bool store = true){//used for subtraction
  uint8_t a = registers.A;
  uint8_t c = (is_c && (registers.F & FLAG_C))?1:0;
  int res = (int)a-v-c;
  if(store) registers.A = (uint8_t)res;
  setFlagz(
	   (uint8_t)res == 0,
	   true,
	   (a & 0x0F) < ((v & 0x0F) + c), res < 0);
}

static void ALU_AND(uint8_t v){
  registers.A &= v;
  setFlagz(registers.A == 0, false, true, false);
}

static void ALU_XOR(uint8_t v){
  registers.A ^= v;
  setFlagz(registers.A == 0, false, false, false);
}

static void ALU_OR(uint8_t v){
  registers.A |= v;
  setFlagz(registers.A == 0, false, false, false);
}

static void INC8(uint8_t i){
  uint8_t v = get_r8(i);
  uint8_t r = (uint8_t)(v + 1);
  set_r8(i, r);
  setFlagz(r == 0, false, (v & 0x0F) == 0x0F, FLAG_C());
}

static void DEC8(uint8_t i){
  uint8_t v = get_r8(i);
  uint8_t r = (uint8_t)(v - 1);
  set_r8(i, r);
  setFlagz(r == 0, true, (v & 0x0F) == 0x0F, FLAG_C());
}

//for the 16-bit adds

static void ALU_ADD_HL(uint16_t v){
  uint32_t sum = (uint32_t)registers.HL + v;
  setFlagz(
    (registers.F & FLAG_Z) != 0,
    false,
    ((registers.HL & 0x0FFF)+(v & 0x0FFF)) > 0x0FFF,
    sum > 0xFFFF
  );  
  registers.HL = (uint16_t)sum;
  core_advance_cpu_clock(4);
}

//Take the current SP and add it to the immediate 8-bit value to it

static uint16_t SP_plus_e8(){
  uint8_t raw = fetch8();
  uint16_t sp = registers.SP;//just stores the current SP locally
  setFlagz(false, false, ((sp & 0x0F) + (raw & 0x0F)) > 0x0F, ((sp & 0x0F) + raw) > 0x0F);
  return (uint16_t)(sp + (uint8_t)raw);
}

//Rotates of A (RLCA, RRCA, RLA, RRA)
//Z is always 0 here

static void ROTATE_RLCA(){//Rotate A Left
  uint8_t a = registers.A, c = a >> 7;//extracts the seventh bit
  registers.A = (uint8_t)((a << 1) | c);//moves everything to the left
  setFlagz(false, false, false, c);
}

static void ROTATE_RRCA(){//opposite of RLCA
  uint8_t a = registers.A, c = a & 1;
  registers.A = (uint8_t)((a >> 1) | (c << 7));//moves everything right
  setFlagz(false, false, false, c);
}

static void ROTATE_RLA(){//Rotate Left A through carry; bit 7 becomes Carry, old carry enters bit 0
  uint8_t a = registers.A, old = flag_c() ? 1:0;
  registers.A = (uint8_t)((a << 1)|old);
  setFlagz(false, false, false, (a >> 7) != 0);
}

static void ROTATE_RRA(){//Rotate Right A through Carry; bit 0 becomes carry, old carry enters bit 7
  uint8_t a = registers.A, old = flag_c()?1:0;
  registers.A = (uint8_t)((a >> 1) | (old << 7));
  setFlagz(false, false, false, (a & 1) != 0);
}

//Decimal Adjust Accumulator
static void ALU_DAA(){//corrects A into the proper decimal/BCD representation
  uint8_t a = registers.A, adj = 0;
  bool car = flag_c();
  if(!flag_n()){//if addition
    if(flag_h() || (a & 0x0F) > 0x09) adj |= 0x06;
    if(carry || a > 0x99){
      adj |= 0x60;
      car = true;
    }
    a += adj;
  }else{
    if(flag_h()) adj |= 0x06;
    if(carry) adj |= 0x60;
    a -= adj;
  }
  registers.A = a;
  setFlagz(a == 0, flag_n(), false, car);
}

#!/bin/bash

sed -i \
  -e 's/\bcpu_LD_SP_nn\b/cpu_LD_a16_SP/' \
  -e 's/\bcpu_LS_SP_nn\b/cpu_LD_SP_nn/' \
  -e 's/\bcpu_JR_NC_n\b/cpu_JR_NZ_n/' \
  -e 's/\bJR_NC_n\b/cpu_JR_NC_n/' \
  -e 's/\bcpu_HL_HL\b/cpu_ADD_HL_HL/' \
  -e 's/\bcpu_LD_HL\b/cpu_LD_HL_n/' \
  -e 's/\bcpu_LD_A_nn_A\b/cpu_LD_nn_A/' \
  -e 's/\bcpu_LD_BC_S\b/cpu_LD_BC_A/' \
  -e 's/\bcpu_DL\b/cpu_DI/' \
  -e 's/\bcpu_EL\b/cpu_EI/' \
  -e 's|^void RLCA();$|void cpu_RLCA() {} //0x07|' \
  -e 's|^void cpu_DEC_B() {} //0x06|void cpu_DEC_B() {} //0x05|' \
  -e 's|^void cpu_LD_B_n() {} //0x07|void cpu_LD_B_n() {} //0x06|' \
  -e 's|if(!current_opcode){|if(!current_exec){|' CPU/CPU.cpp

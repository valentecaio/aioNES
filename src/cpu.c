#include <stdbool.h>
#include <stdint.h>

#include "cpu.h"


/**************************** CONSTANTS ****************************/

CPUInstruction cpu_instruction_table[256] = {
    [0x61] = {"ADC", 2, 6, "indirect, x indexed"},
    [0x65] = {"ADC", 2, 3, "zero page"},
    [0x69] = {"ADC", 2, 2, "immediate"},
    [0x6D] = {"ADC", 3, 4, "absolute"},
    [0x71] = {"ADC", 2, 5, "indirect, y indexed"},
    [0x75] = {"ADC", 2, 4, "zero page, x indexed"},
    [0x79] = {"ADC", 3, 4, "absolute, y indexed"},
    [0x7D] = {"ADC", 3, 4, "absolute, x indexed"},
    [0x21] = {"AND", 2, 6, "indirect, x indexed"},
    [0x25] = {"AND", 2, 3, "zero page"},
    [0x29] = {"AND", 2, 2, "immediate"},
    [0x2D] = {"AND", 3, 4, "absolute"},
    [0x31] = {"AND", 2, 5, "indirect, y indexed"},
    [0x35] = {"AND", 2, 4, "zero page, x indexed"},
    [0x39] = {"AND", 3, 4, "absolute, y indexed"},
    [0x3D] = {"AND", 3, 4, "absolute, x indexed"},
    [0x06] = {"ASL", 2, 5, "zero page"},
    [0x0A] = {"ASL", 1, 2, "accumulator"},
    [0x0E] = {"ASL", 3, 6, "absolute"},
    [0x16] = {"ASL", 2, 6, "zero page, x indexed"},
    [0x1E] = {"ASL", 3, 7, "absolute, x indexed"},
    [0x90] = {"BCC", 2, 2, ""},
    [0xB0] = {"BCS", 2, 2, ""},
    [0xF0] = {"BEQ", 2, 2, ""},
    [0x24] = {"BIT", 2, 3, "zero page"},
    [0x2C] = {"BIT", 3, 4, "absolute"},
    [0x30] = {"BMI", 2, 2, ""},
    [0xD0] = {"BNE", 2, 2, ""},
    [0x10] = {"BPL", 2, 2, ""},
    [0x00] = {"BRK", 1, 7, ""},
    [0x50] = {"BVC", 2, 2, ""},
    [0x70] = {"BVS", 2, 2, ""},
    [0x18] = {"CLC", 1, 2, ""},
    [0xD8] = {"CLD", 1, 2, ""},
    [0x58] = {"CLI", 1, 2, ""},
    [0xB8] = {"CLV", 1, 2, ""},
    [0xC1] = {"CMP", 2, 6, "indirect, x indexed"},
    [0xC5] = {"CMP", 2, 3, "zero page"},
    [0xC9] = {"CMP", 2, 2, "immediate"},
    [0xCD] = {"CMP", 3, 4, "absolute"},
    [0xD1] = {"CMP", 2, 5, "indirect, y indexed"},
    [0xD5] = {"CMP", 2, 4, "zero page, x indexed"},
    [0xD9] = {"CMP", 3, 4, "absolute, y indexed"},
    [0xDD] = {"CMP", 3, 4, "absolute, x indexed"},
    [0xE0] = {"CPX", 2, 2, "immediate"},
    [0xE4] = {"CPX", 2, 3, "zero page"},
    [0xEC] = {"CPX", 3, 4, "absolute"},
    [0xC0] = {"CPY", 2, 2, "immediate"},
    [0xC4] = {"CPY", 2, 3, "zero page"},
    [0xCC] = {"CPY", 3, 4, "absolute"},
    [0xC6] = {"DEC", 2, 5, "zero page"},
    [0xCE] = {"DEC", 3, 6, "absolute"},
    [0xD6] = {"DEC", 2, 6, "zero page, x indexed"},
    [0xDE] = {"DEC", 3, 7, "absolute, x indexed"},
    [0xCA] = {"DEX", 1, 2, ""},
    [0x88] = {"DEY", 1, 2, ""},
    [0x41] = {"EOR", 2, 6, "indirect, x indexed"},
    [0x45] = {"EOR", 2, 3, "zero page"},
    [0x49] = {"EOR", 2, 2, "immediate"},
    [0x4D] = {"EOR", 3, 4, "absolute"},
    [0x51] = {"EOR", 2, 5, "indirect, y indexed"},
    [0x55] = {"EOR", 2, 4, "zero page, x indexed"},
    [0x59] = {"EOR", 3, 4, "absolute, y indexed"},
    [0x5D] = {"EOR", 3, 4, "absolute, x indexed"},
    [0xE6] = {"INC", 2, 5, "zero page"},
    [0xEE] = {"INC", 3, 6, "absolute"},
    [0xF6] = {"INC", 2, 6, "zero page, x indexed"},
    [0xFE] = {"INC", 3, 7, "absolute, x indexed"},
    [0xE8] = {"INX", 1, 2, ""},
    [0xC8] = {"INY", 1, 2, ""},
    [0x4C] = {"JMP", 3, 3, "absolute"},
    [0x6C] = {"JMP", 3, 5, "indirect"},
    [0x20] = {"JSR", 3, 6, ""},
    [0xA1] = {"LDA", 2, 6, "indirect, x indexed"},
    [0xA5] = {"LDA", 2, 3, "zero page"},
    [0xA9] = {"LDA", 2, 2, "immediate"},
    [0xAD] = {"LDA", 3, 4, "absolute"},
    [0xB1] = {"LDA", 2, 5, "indirect, y indexed"},
    [0xB5] = {"LDA", 2, 4, "zero page, x indexed"},
    [0xB9] = {"LDA", 3, 4, "absolute, y indexed"},
    [0xBD] = {"LDA", 3, 4, "absolute, x indexed"},
    [0xA2] = {"LDX", 2, 2, "immediate"},
    [0xA6] = {"LDX", 2, 3, "zero page"},
    [0xAE] = {"LDX", 3, 4, "absolute"},
    [0xB6] = {"LDX", 2, 4, "zero page, y indexed"},
    [0xBE] = {"LDX", 3, 4, "absolute, y indexed"},
    [0xA0] = {"LDY", 2, 2, "immediate"},
    [0xA4] = {"LDY", 2, 3, "zero page"},
    [0xAC] = {"LDY", 3, 4, "absolute"},
    [0xB4] = {"LDY", 2, 4, "zero page, x indexed"},
    [0xBC] = {"LDY", 3, 4, "absolute, x indexed"},
    [0x46] = {"LSR", 2, 5, "zero page"},
    [0x4A] = {"LSR", 1, 2, "accumulator"},
    [0x4E] = {"LSR", 3, 6, "absolute"},
    [0x56] = {"LSR", 2, 6, "zero page, x indexed"},
    [0x5E] = {"LSR", 3, 7, "absolute, x indexed"},
    [0xEA] = {"NOP", 1, 2, ""},
    [0x01] = {"ORA", 2, 6, "indirect, x indexed"},
    [0x05] = {"ORA", 2, 3, "zero page"},
    [0x09] = {"ORA", 2, 2, "immediate"},
    [0x0D] = {"ORA", 3, 4, "absolute"},
    [0x11] = {"ORA", 2, 5, "indirect, y indexed"},
    [0x15] = {"ORA", 2, 4, "zero page, x indexed"},
    [0x19] = {"ORA", 3, 4, "absolute, y indexed"},
    [0x1D] = {"ORA", 3, 4, "absolute, x indexed"},
    [0x48] = {"PHA", 1, 3, ""},
    [0x08] = {"PHP", 1, 3, ""},
    [0x68] = {"PLA", 1, 4, ""},
    [0x28] = {"PLP", 1, 4, ""},
    [0x26] = {"ROL", 2, 5, "zero page"},
    [0x2A] = {"ROL", 1, 2, "accumulator"},
    [0x2E] = {"ROL", 3, 6, "absolute"},
    [0x36] = {"ROL", 2, 6, "zero page, x indexed"},
    [0x3E] = {"ROL", 3, 7, "absolute, x indexed"},
    [0x66] = {"ROR", 2, 5, "zero page"},
    [0x6A] = {"ROR", 1, 2, "accumulator"},
    [0x6E] = {"ROR", 3, 6, "absolute"},
    [0x76] = {"ROR", 2, 6, "zero page, x indexed"},
    [0x7E] = {"ROR", 3, 7, "absolute, x indexed"},
    [0x40] = {"RTI", 1, 6, ""},
    [0x60] = {"RTS", 1, 6, ""},
    [0xE1] = {"SBC", 2, 6, "indirect, x indexed"},
    [0xE5] = {"SBC", 2, 3, "zero page"},
    [0xE9] = {"SBC", 2, 2, "immediate"},
    [0xED] = {"SBC", 3, 4, "absolute"},
    [0xF1] = {"SBC", 2, 5, "indirect, y indexed"},
    [0xF5] = {"SBC", 2, 4, "zero page, x indexed"},
    [0xF9] = {"SBC", 3, 4, "absolute, y indexed"},
    [0xFD] = {"SBC", 3, 4, "absolute, x indexed"},
    [0x38] = {"SEC", 1, 2, ""},
    [0xF8] = {"SED", 1, 2, ""},
    [0x78] = {"SEI", 1, 2, ""},
    [0x81] = {"STA", 2, 6, "indirect, x indexed"},
    [0x85] = {"STA", 2, 3, "zero page"},
    [0x8D] = {"STA", 3, 4, "absolute"},
    [0x91] = {"STA", 2, 6, "indirect, y indexed"},
    [0x95] = {"STA", 2, 4, "zero page, x indexed"},
    [0x99] = {"STA", 3, 5, "absolute, y indexed"},
    [0x9D] = {"STA", 3, 5, "absolute, x indexed"},
    [0x86] = {"STX", 2, 3, "zero page"},
    [0x8E] = {"STX", 3, 4, "absolute"},
    [0x96] = {"STX", 2, 4, "zero page, y indexed"},
    [0x84] = {"STY", 2, 3, "zero page"},
    [0x8C] = {"STY", 3, 4, "absolute"},
    [0x94] = {"STY", 2, 4, "zero page, x indexed"},
    [0xAA] = {"TAX", 1, 2, ""},
    [0xA8] = {"TAY", 1, 2, ""},
    [0xBA] = {"TSX", 1, 2, ""},
    [0x8A] = {"TXA", 1, 2, ""},
    [0x9A] = {"TXS", 1, 2, ""},
    [0x98] = {"TYA", 1, 2, ""},
};


/**************************** CPU STATE ****************************/

uint8_t mem[CPU_MEM_SIZE] = {0};
uint8_t reg_a = 0;
uint8_t reg_x = 0;
uint8_t reg_y = 0;
uint8_t flags = 0;


/*********************** AUXILIARY FUNCTIONS ***********************/

void set_flag(CPUFlags flag, bool value) {
    flags &= ~flag;
    if (value) {
        flags |= flag;
    }
}

// returns true if the given flag is set, false otherwise
bool get_flag(CPUFlags flag) {
    return (flags & flag) != 0;
}

void set_flags_n_z(uint8_t result) {
    set_flag(CPU_FLAG_ZERO, result == 0);
    set_flag(CPU_FLAG_NEGATIVE, result & BIT_7);
}

/*
The 6502 has a "zero page wrap" mechanism. If the addition of the X register to
the zero page index results in a value greater than 255 the processor performs
a wraparound or modulo operation, discarding the higher bits that exceed 8 bits
*/
uint8_t zero_page_x(uint8_t addr) {
    return (addr + reg_x) & 0xFF;
}

uint8_t zero_page_y(uint8_t addr) {
    return (addr + reg_y) & 0xFF;
}

uint16_t indirect_x(uint8_t addr) {
    uint8_t effective_addr_l = mem[(addr + reg_x) & 0xFF];
    uint8_t effective_addr_h = mem[(addr + reg_x + 1) & 0xFF];
    return effective_addr_h << 8 | effective_addr_l;
}

uint16_t indirect_y(uint8_t addr) {
    uint8_t effective_addr_l = mem[addr];
    uint8_t effective_addr_h = mem[(addr + 1) & 0xFF];
    return (effective_addr_h << 8 | effective_addr_l) + reg_y;
}


/************************** LDA **************************/
void cpu_lda_immediate(uint8_t operand) { set_flags_n_z(reg_a = operand); }
void cpu_lda_zero_page(uint8_t addr)   { cpu_lda_immediate(mem[addr]); }
void cpu_lda_zero_page_x(uint8_t addr) { cpu_lda_immediate(mem[zero_page_x(addr)]); }
void cpu_lda_absolute(uint16_t addr)   { cpu_lda_immediate(mem[addr]); }
void cpu_lda_absolute_x(uint16_t addr) { cpu_lda_immediate(mem[addr + reg_x]); }
void cpu_lda_absolute_y(uint16_t addr) { cpu_lda_immediate(mem[addr + reg_y]); }
void cpu_lda_indirect_x(uint8_t addr)  { cpu_lda_immediate(mem[indirect_x(addr)]); }
void cpu_lda_indirect_y(uint8_t addr)  { cpu_lda_immediate(mem[indirect_y(addr)]); }


/************************** LDX **************************/
void cpu_ldx_immediate(uint8_t operand) { set_flags_n_z(reg_x = operand); }
void cpu_ldx_zero_page(uint8_t addr)   { cpu_ldx_immediate(mem[addr]); }
void cpu_ldx_zero_page_y(uint8_t addr) { cpu_ldx_immediate(mem[zero_page_y(addr)]); }
void cpu_ldx_absolute(uint16_t addr)   { cpu_ldx_immediate(mem[addr]); }
void cpu_ldx_absolute_y(uint16_t addr) { cpu_ldx_immediate(mem[addr + reg_y]); }


/************************** LDY **************************/
void cpu_ldy_immediate(uint8_t operand) { set_flags_n_z(reg_y = operand); }
void cpu_ldy_zero_page(uint8_t addr)   { cpu_ldy_immediate(mem[addr]); }
void cpu_ldy_zero_page_x(uint8_t addr) { cpu_ldy_immediate(mem[zero_page_x(addr)]); }
void cpu_ldy_absolute(uint16_t addr)   { cpu_ldy_immediate(mem[addr]); }
void cpu_ldy_absolute_x(uint16_t addr) { cpu_ldy_immediate(mem[addr + reg_x]); }


/************************** STA **************************/
void cpu_sta_zero_page(uint8_t addr)   { mem[addr] = reg_a; }
void cpu_sta_zero_page_x(uint8_t addr) { mem[zero_page_x(addr)] = reg_a; }
void cpu_sta_absolute(uint16_t addr)   { mem[addr] = reg_a; }
void cpu_sta_absolute_x(uint16_t addr) { mem[addr + reg_x] = reg_a; }
void cpu_sta_absolute_y(uint16_t addr) { mem[addr + reg_y] = reg_a; }
void cpu_sta_indirect_x(uint8_t addr)  { mem[indirect_x(addr)] = reg_a; }
void cpu_sta_indirect_y(uint8_t addr)  { mem[indirect_y(addr)] = reg_a; }


/************************** STX **************************/
void cpu_stx_zero_page(uint8_t addr)   { mem[addr] = reg_x; }
void cpu_stx_zero_page_y(uint8_t addr) { mem[zero_page_y(addr)] = reg_x; }
void cpu_stx_absolute(uint16_t addr)   { mem[addr] = reg_x; }


/************************** STY **************************/
void cpu_sty_zero_page(uint8_t addr)   { mem[addr] = reg_y; }
void cpu_sty_zero_page_x(uint8_t addr) { mem[zero_page_x(addr)] = reg_y; }
void cpu_sty_absolute(uint16_t addr)   { mem[addr] = reg_y; }


/************************** ADC **************************/
void cpu_adc_immediate(uint8_t operand) {
    uint16_t sum = reg_a + operand + (flags & CPU_FLAG_CARRY);
    set_flag(CPU_FLAG_CARRY, sum > 0xFF);
    set_flag(CPU_FLAG_OVERFLOW, (~(reg_a ^ operand) & (reg_a ^ sum)) & BIT_7);
    reg_a = sum & 0xFF;
    set_flags_n_z(reg_a);
}
void cpu_adc_zero_page(uint8_t addr)   { cpu_adc_immediate(mem[addr]); }
void cpu_adc_zero_page_x(uint8_t addr) { cpu_adc_immediate(mem[zero_page_x(addr)]); }
void cpu_adc_absolute(uint16_t addr)   { cpu_adc_immediate(mem[addr]); }
void cpu_adc_absolute_x(uint16_t addr) { cpu_adc_immediate(mem[(addr + reg_x) & 0xFFFF]); }
void cpu_adc_absolute_y(uint16_t addr) { cpu_adc_immediate(mem[(addr + reg_y) & 0xFFFF]); }
void cpu_adc_indirect_x(uint8_t addr)  { cpu_adc_immediate(mem[indirect_x(addr)]); }
void cpu_adc_indirect_y(uint8_t addr)  { cpu_adc_immediate(mem[indirect_y(addr)]); }


/************************** SBC **************************/
void cpu_sbc_immediate(uint8_t operand) {
    uint16_t diff = reg_a - operand - (1 - (flags & CPU_FLAG_CARRY));
    set_flag(CPU_FLAG_CARRY, diff < 0x100);
    set_flag(CPU_FLAG_OVERFLOW, ((reg_a ^ diff) & BIT_7) && ((reg_a ^ operand) & BIT_7));
    reg_a = diff & 0xFF;
    set_flags_n_z(reg_a);
}
void cpu_sbc_zero_page(uint8_t addr)   { cpu_sbc_immediate(mem[addr]); }
void cpu_sbc_zero_page_x(uint8_t addr) { cpu_sbc_immediate(mem[zero_page_x(addr)]); }
void cpu_sbc_absolute(uint16_t addr)   { cpu_sbc_immediate(mem[addr]); }
void cpu_sbc_absolute_x(uint16_t addr) { cpu_sbc_immediate(mem[addr + reg_x]); }
void cpu_sbc_absolute_y(uint16_t addr) { cpu_sbc_immediate(mem[addr + reg_y]); }
void cpu_sbc_indirect_x(uint8_t addr)  { cpu_sbc_immediate(mem[indirect_x(addr)]); }
void cpu_sbc_indirect_y(uint8_t addr)  { cpu_sbc_immediate(mem[indirect_y(addr)]); }


/************************** INC **************************/
void cpu_inc_zero_page(uint8_t addr)   { set_flags_n_z(++mem[addr]); }
void cpu_inc_zero_page_x(uint8_t addr) { set_flags_n_z(++mem[zero_page_x(addr)]); }
void cpu_inc_absolute(uint16_t addr)   { set_flags_n_z(++mem[addr]); }
void cpu_inc_absolute_x(uint16_t addr) { set_flags_n_z(++mem[addr + reg_x]); }


/************************** DEC **************************/
void cpu_dec_zero_page(uint8_t addr)   { set_flags_n_z(--mem[addr]); }
void cpu_dec_zero_page_x(uint8_t addr) { set_flags_n_z(--mem[zero_page_x(addr)]); }
void cpu_dec_absolute(uint16_t addr)   { set_flags_n_z(--mem[addr]); }
void cpu_dec_absolute_x(uint16_t addr) { set_flags_n_z(--mem[addr + reg_x]); }


/******************* INX, INY, DEX, DEY ******************/
void cpu_inx() { set_flags_n_z(++reg_x); }
void cpu_iny() { set_flags_n_z(++reg_y); }
void cpu_dex() { set_flags_n_z(--reg_x); }
void cpu_dey() { set_flags_n_z(--reg_y); }


/******************* TAX, TAY, TXA, TYA ******************/
void cpu_tax() { set_flags_n_z(reg_x = reg_a); }
void cpu_tay() { set_flags_n_z(reg_y = reg_a); }
void cpu_txa() { set_flags_n_z(reg_a = reg_x); }
void cpu_tya() { set_flags_n_z(reg_a = reg_y); }


/************************** AND **************************/
void cpu_and_immediate(uint8_t operand) { set_flags_n_z(reg_a &= operand); }
void cpu_and_zero_page(uint8_t addr)   { cpu_and_immediate(mem[addr]); }
void cpu_and_zero_page_x(uint8_t addr) { cpu_and_immediate(mem[zero_page_x(addr)]); }
void cpu_and_absolute(uint16_t addr)   { cpu_and_immediate(mem[addr]); }
void cpu_and_absolute_x(uint16_t addr) { cpu_and_immediate(mem[addr + reg_x]); }
void cpu_and_absolute_y(uint16_t addr) { cpu_and_immediate(mem[addr + reg_y]); }
void cpu_and_indirect_x(uint8_t addr)  { cpu_and_immediate(mem[indirect_x(addr)]); }
void cpu_and_indirect_y(uint8_t addr)  { cpu_and_immediate(mem[indirect_y(addr)]); }


/************************** EOR **************************/
void cpu_eor_immediate(uint8_t operand) { set_flags_n_z(reg_a ^= operand); }
void cpu_eor_zero_page(uint8_t addr)   { cpu_eor_immediate(mem[addr]); }
void cpu_eor_zero_page_x(uint8_t addr) { cpu_eor_immediate(mem[zero_page_x(addr)]); }
void cpu_eor_absolute(uint16_t addr)   { cpu_eor_immediate(mem[addr]); }
void cpu_eor_absolute_x(uint16_t addr) { cpu_eor_immediate(mem[addr + reg_x]); }
void cpu_eor_absolute_y(uint16_t addr) { cpu_eor_immediate(mem[addr + reg_y]); }
void cpu_eor_indirect_x(uint8_t addr)  { cpu_eor_immediate(mem[indirect_x(addr)]); }
void cpu_eor_indirect_y(uint8_t addr)  { cpu_eor_immediate(mem[indirect_y(addr)]); }


/************************** ORA **************************/
void cpu_ora_immediate(uint8_t operand) { set_flags_n_z(reg_a |= operand); }
void cpu_ora_zero_page(uint8_t addr)   { cpu_ora_immediate(mem[addr]); }
void cpu_ora_zero_page_x(uint8_t addr) { cpu_ora_immediate(mem[zero_page_x(addr)]); }
void cpu_ora_absolute(uint16_t addr)   { cpu_ora_immediate(mem[addr]); }
void cpu_ora_absolute_x(uint16_t addr) { cpu_ora_immediate(mem[addr + reg_x]); }
void cpu_ora_absolute_y(uint16_t addr) { cpu_ora_immediate(mem[addr + reg_y]); }
void cpu_ora_indirect_x(uint8_t addr)  { cpu_ora_immediate(mem[indirect_x(addr)]); }
void cpu_ora_indirect_y(uint8_t addr)  { cpu_ora_immediate(mem[indirect_y(addr)]); }


/************************** CMP **************************/
void cpu_cmp_immediate(uint8_t operand) {
    set_flags_n_z(reg_a - operand);
    set_flag(CPU_FLAG_CARRY, reg_a >= operand);
}
void cpu_cmp_zero_page(uint8_t addr)   { cpu_cmp_immediate(mem[addr]); }
void cpu_cmp_zero_page_x(uint8_t addr) { cpu_cmp_immediate(mem[zero_page_x(addr)]); }
void cpu_cmp_absolute(uint16_t addr)   { cpu_cmp_immediate(mem[addr]); }
void cpu_cmp_absolute_x(uint16_t addr) { cpu_cmp_immediate(mem[addr + reg_x]); }
void cpu_cmp_absolute_y(uint16_t addr) { cpu_cmp_immediate(mem[addr + reg_y]); }
void cpu_cmp_indirect_x(uint8_t addr)  { cpu_cmp_immediate(mem[indirect_x(addr)]); }
void cpu_cmp_indirect_y(uint8_t addr)  { cpu_cmp_immediate(mem[indirect_y(addr)]); }


/******************************* CPX *******************************/
void cpu_cpx_immediate(uint8_t operand) {
    set_flags_n_z(reg_x - operand);
    set_flag(CPU_FLAG_CARRY, reg_x >= operand);
}
void cpu_cpx_zero_page(uint8_t addr) { cpu_cpx_immediate(mem[addr]); }
void cpu_cpx_absolute(uint16_t addr) { cpu_cpx_immediate(mem[addr]); }


/******************************* CPY *******************************/
void cpu_cpy_immediate(uint8_t operand) {
    set_flags_n_z(reg_y - operand);
    set_flag(CPU_FLAG_CARRY, reg_y >= operand);
}
void cpu_cpy_zero_page(uint8_t addr) { cpu_cpy_immediate(mem[addr]); }
void cpu_cpy_absolute(uint16_t addr) { cpu_cpy_immediate(mem[addr]); }


/******************************* BIT *******************************/
void cpu_bit_absolute(uint16_t addr) {
    set_flag(CPU_FLAG_ZERO, (reg_a & mem[addr]) == 0);
    set_flag(CPU_FLAG_OVERFLOW, mem[addr] & CPU_FLAG_OVERFLOW);
    set_flag(CPU_FLAG_NEGATIVE, mem[addr] & CPU_FLAG_NEGATIVE);
}
void cpu_bit_zero_page(uint8_t addr) { cpu_bit_absolute(addr); }


/******************************* ASL *******************************/
void cpu_asl_accumulator() {
    set_flag(CPU_FLAG_CARRY, reg_a & BIT_7);
    set_flags_n_z(reg_a <<= 1);
}
void cpu_asl_absolute(uint16_t addr) {
    set_flag(CPU_FLAG_CARRY, mem[addr] & BIT_7);
    set_flags_n_z(mem[addr] <<= 1);
}
void cpu_asl_absolute_x(uint16_t addr) { cpu_asl_absolute((addr + reg_x) & 0xFFFF); }
void cpu_asl_zero_page(uint8_t addr)   { cpu_asl_absolute(addr); }
void cpu_asl_zero_page_x(uint8_t addr) { cpu_asl_absolute(zero_page_x(addr)); }


/******************************* LSR *******************************/
void cpu_lsr_accumulator() {
    set_flag(CPU_FLAG_CARRY, reg_a & BIT_0);
    set_flags_n_z(reg_a >>= 1);
}
void cpu_lsr_absolute(uint16_t addr) {
    set_flag(CPU_FLAG_CARRY, mem[addr] & BIT_0);
    set_flags_n_z(mem[addr] >>= 1);
}
void cpu_lsr_absolute_x(uint16_t addr) { cpu_lsr_absolute((addr + reg_x) & 0xFFFF); }
void cpu_lsr_zero_page(uint8_t addr)   { cpu_lsr_absolute(addr); }
void cpu_lsr_zero_page_x(uint8_t addr) { cpu_lsr_absolute(zero_page_x(addr)); }


/******************************* ROL *******************************/
// C <- [76543210] <- C
void cpu_rol_accumulator() {
    // store the carry flag in a temporary variable
    bool carry = get_flag(CPU_FLAG_CARRY);

    // push bit 7 to carry flag
    set_flag(CPU_FLAG_CARRY, (reg_a >> 7) & 0x01);

    // push the old carry flag value to the bit 0, shifting the rest to the left
    reg_a = (reg_a << 1) | carry;

    set_flags_n_z(reg_a);
}
void cpu_rol_absolute(uint16_t addr) {
    bool carry = get_flag(CPU_FLAG_CARRY);
    set_flag(CPU_FLAG_CARRY, (mem[addr] >> 8) & 0x01);
    mem[addr] = (mem[addr] << 1) | carry;
    set_flags_n_z(mem[addr]);
}
void cpu_rol_absolute_x(uint16_t addr) { cpu_rol_absolute((addr + reg_x) & 0xFFFF); }
void cpu_rol_zero_page(uint8_t addr)   { cpu_rol_absolute(addr); }
void cpu_rol_zero_page_x(uint8_t addr) { cpu_rol_absolute(zero_page_x(addr)); }


/******************************* ROR *******************************/
// C -> [76543210] -> C
void cpu_ror_accumulator() {
    // store the carry flag in a temporary variable
    bool carry = get_flag(CPU_FLAG_CARRY);

    // push bit 0 to carry flag
    set_flag(CPU_FLAG_CARRY, reg_a & 0x01);

    // push the old carry flag value to the bit 7, shifting the rest to the right
    reg_a = (reg_a >> 1) | (carry << 7);

    set_flags_n_z(reg_a);
}
void cpu_ror_absolute(uint16_t addr) {
    bool carry = get_flag(CPU_FLAG_CARRY);
    set_flag(CPU_FLAG_CARRY, mem[addr] & 0x01);
    mem[addr] = (mem[addr] >> 1) | (carry << 7);
    set_flags_n_z(mem[addr]);
}
void cpu_ror_absolute_x(uint16_t addr) { cpu_ror_absolute((addr + reg_x) & 0xFFFF); }
void cpu_ror_zero_page(uint8_t addr)   { cpu_ror_absolute(addr); }
void cpu_ror_zero_page_x(uint8_t addr) { cpu_ror_absolute(zero_page_x(addr)); }


/**************** CLC, CLD, CLI, CLV, SEC, SEC, SEI ****************/
void cpu_clc() { set_flag(CPU_FLAG_CARRY,     false); }
void cpu_cld() { set_flag(CPU_FLAG_DECIMAL,   false); }
void cpu_cli() { set_flag(CPU_FLAG_INTERRUPT, false); }
void cpu_clv() { set_flag(CPU_FLAG_OVERFLOW,  false); }
void cpu_sec() { set_flag(CPU_FLAG_CARRY,     true); }
void cpu_sed() { set_flag(CPU_FLAG_DECIMAL,   true); }
void cpu_sei() { set_flag(CPU_FLAG_INTERRUPT, true); }


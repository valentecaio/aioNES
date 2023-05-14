#include <stdbool.h>
#include <stdint.h>

#include "6502.h"

void set_flags_n_z(uint8_t result) {
    flag_n = (result & 0x80) != 0;
    flag_z = (result == 0);
}


/************************** LDA **************************/
void c6502_lda_immediate(uint8_t operand) {
    reg_a = operand;
    set_flags_n_z(reg_a);
}

void c6502_lda_zero_page(uint8_t addr) {
    c6502_lda_immediate(mem[addr]);
}

void c6502_lda_zero_page_x(uint8_t addr) {
    c6502_lda_immediate(mem[(addr + reg_x) & 0xFF]);
}

void c6502_lda_absolute(uint16_t addr) {
    c6502_lda_immediate(mem[addr]);
}

void c6502_lda_absolute_x(uint16_t addr) {
    c6502_lda_immediate(mem[addr + reg_x]);
}

void c6502_lda_absolute_y(uint16_t addr) {
    c6502_lda_immediate(mem[addr + reg_y]);
}

void c6502_lda_indirect_x(uint8_t addr) {
    uint8_t effective_addr_l = mem[(addr + reg_x) & 0xFF];
    uint8_t effective_addr_h = mem[(addr + reg_x + 1) & 0xFF];
    uint16_t effective_addr = effective_addr_l + (effective_addr_h << 8);
    c6502_lda_immediate(mem[effective_addr]);
}

void c6502_lda_indirect_y(uint8_t addr) {
    uint8_t effective_addr_l = mem[addr];
    uint8_t effective_addr_h = mem[(addr + 1) & 0xFF];
    uint16_t effective_addr = (effective_addr_l + (effective_addr_h << 8)) + reg_y;
    c6502_lda_immediate(mem[effective_addr]);
}


/************************** LDX **************************/
void c6502_ldx_immediate(uint8_t operand) {
    reg_x = operand;
    set_flags_n_z(reg_x);
}

void c6502_ldx_zero_page(uint8_t addr) {
    c6502_ldx_immediate(mem[addr]);
}

void c6502_ldx_zero_page_y(uint8_t addr) {
    c6502_ldx_immediate(mem[(addr + reg_y) & 0xFF]);
}

void c6502_ldx_absolute(uint16_t addr) {
    c6502_ldx_immediate(mem[addr]);
}

void c6502_ldx_absolute_y(uint16_t addr) {
    c6502_ldx_immediate(mem[addr + reg_y]);
}


/************************** LDY **************************/
void c6502_ldy_immediate(uint8_t operand) {
    reg_y = operand;
    set_flags_n_z(reg_y);
}

void c6502_ldy_zero_page(uint8_t addr) {
    c6502_ldy_immediate(mem[addr]);
}

void c6502_ldy_zero_page_x(uint8_t addr) {
    c6502_ldy_immediate(mem[(addr + reg_x) & 0xFF]);
}

void c6502_ldy_absolute(uint16_t addr) {
    c6502_ldy_immediate(mem[addr]);
}

void c6502_ldy_absolute_x(uint16_t addr) {
    c6502_ldy_immediate(mem[addr + reg_x]);
}


/************************** STA **************************/
void c6502_sta_zero_page(uint8_t addr) {
    mem[addr] = reg_a;
}

void c6502_sta_zero_page_x(uint8_t addr) {
    mem[(addr + reg_x) & 0xFF] = reg_a;
}

void c6502_sta_absolute(uint16_t addr) {
    mem[addr] = reg_a;
}

void c6502_sta_absolute_x(uint16_t addr) {
    mem[addr + reg_x] = reg_a;
}

void c6502_sta_absolute_y(uint16_t addr) {
    mem[addr + reg_y] = reg_a;
}

void c6502_sta_indirect_x(uint8_t addr) {
    uint8_t effective_addr_l = mem[(addr + reg_x) & 0xFF];
    uint8_t effective_addr_h = mem[(addr + reg_x + 1) & 0xFF];
    uint16_t effective_addr = effective_addr_l + (effective_addr_h << 8);
    mem[effective_addr] = reg_a;
}

void c6502_sta_indirect_y(uint8_t addr) {
    uint8_t effective_addr_l = mem[addr];
    uint8_t effective_addr_h = mem[(addr + 1) & 0xFF];
    uint16_t effective_addr = (effective_addr_l + (effective_addr_h << 8)) + reg_y;
    mem[effective_addr] = reg_a;
}


/************************** STX **************************/
void c6502_stx_zero_page(uint8_t addr) {
    mem[addr] = reg_x;
}

void c6502_stx_zero_page_y(uint8_t addr) {
    mem[(addr + reg_y) & 0xFF] = reg_x;
}

void c6502_stx_absolute(uint16_t addr) {
    mem[addr] = reg_x;
}


/************************** STY **************************/
void c6502_sty_zero_page(uint8_t addr) {
    mem[addr] = reg_y;
}

void c6502_sty_zero_page_x(uint8_t addr) {
    mem[(addr + reg_x) & 0xFF] = reg_y;
}

void c6502_sty_absolute(uint16_t addr) {
    mem[addr] = reg_y;
}


/************************** ADC **************************/
void c6502_adc_immediate(uint8_t operand) {
    uint16_t result = reg_a + operand + (flag_c ? 1 : 0);
    flag_v = ((reg_a ^ result) & (operand ^ result) & 0x80) != 0;
    flag_c = result > 0xFF;
    reg_a = result & 0xFF;
    set_flags_n_z(reg_a);
}

void c6502_adc_zero_page(uint8_t addr) {
    c6502_adc_immediate(mem[addr]);
}

/*
The 6502 has a "zero page wrap" mechanism. If the addition of the X register to
the zero page index results in a value greater than 255 the processor performs
a wraparound or modulo operation, discarding the higher bits that exceed 8 bits
*/
void c6502_adc_zero_page_x(uint8_t addr) {
    c6502_adc_immediate(mem[(addr + reg_x) & 0xFF]);
}

void c6502_adc_absolute(uint16_t addr) {
    c6502_adc_immediate(mem[addr]);
}

void c6502_adc_absolute_x(uint16_t addr) {
    c6502_adc_immediate(mem[(addr + reg_x) & 0xFFFF]);
}

void c6502_adc_absolute_y(uint16_t addr) {
    c6502_adc_immediate(mem[(addr + reg_y) & 0xFFFF]);
}

void c6502_adc_indirect_x(uint8_t addr) {
    uint8_t effective_addr_l = mem[(addr + reg_x) & 0xFF];
    uint8_t effective_addr_h = mem[(addr + reg_x + 1) & 0xFF];
    uint16_t effective_addr = effective_addr_l + (effective_addr_h << 8);
    c6502_adc_immediate(mem[effective_addr]);
}

void c6502_adc_indirect_y(uint8_t addr) {
    uint8_t effective_addr_l = mem[addr];
    uint8_t effective_addr_h = mem[(addr + 1) & 0xFF];
    uint16_t effective_addr = (effective_addr_l + (effective_addr_h << 8)) + reg_y;
    c6502_adc_immediate(mem[effective_addr]);
}


/************************** SBC **************************/
void c6502_sbc_immediate(uint8_t operand) {
    uint16_t result = reg_a - operand - (flag_c ? 0 : 1);
    reg_a = (uint8_t)result;
    set_flags_n_z(reg_a);
    flag_c = (result < 0x100);
    flag_v = ((reg_a ^ result) & (operand ^ result) & 0x80) != 0;
}

void c6502_sbc_zero_page(uint8_t addr) {
    c6502_sbc_immediate(mem[addr]);
}

void c6502_sbc_zero_page_x(uint8_t addr) {
    c6502_sbc_immediate(mem[(addr + reg_x) & 0xFF]);
}

void c6502_sbc_absolute(uint16_t addr) {
    c6502_sbc_immediate(mem[addr]);
}

void c6502_sbc_absolute_x(uint16_t addr) {
    c6502_sbc_immediate(mem[addr + reg_x]);
}

void c6502_sbc_absolute_y(uint16_t addr) {
    c6502_sbc_immediate(mem[addr + reg_y]);
}

void c6502_sbc_indirect_x(uint8_t addr) {
    uint8_t effective_addr_l = mem[(addr + reg_x) & 0xFF];
    uint8_t effective_addr_h = mem[(addr + reg_x + 1) & 0xFF];
    uint16_t effective_addr = effective_addr_l + (effective_addr_h << 8);
    c6502_sbc_immediate(mem[effective_addr]);
}

void c6502_sbc_indirect_y(uint8_t addr) {
    uint8_t effective_addr_l = mem[addr];
    uint8_t effective_addr_h = mem[(addr + 1) & 0xFF];
    uint16_t effective_addr = (effective_addr_l + (effective_addr_h << 8)) + reg_y;
    c6502_sbc_immediate(mem[effective_addr]);
}


/************************** INC **************************/
void c6502_inc_zero_page(uint8_t addr)   { set_flags_n_z(++mem[addr]); }
void c6502_inc_zero_page_x(uint8_t addr) { set_flags_n_z(++mem[(addr + reg_x) & 0xFF]); }
void c6502_inc_absolute(uint16_t addr)   { set_flags_n_z(++mem[addr]); }
void c6502_inc_absolute_x(uint16_t addr) { set_flags_n_z(++mem[addr + reg_x]); }


/************************** DEC **************************/
void c6502_dec_zero_page(uint8_t addr)   { set_flags_n_z(--mem[addr]); }
void c6502_dec_zero_page_x(uint8_t addr) { set_flags_n_z(--mem[(addr + reg_x) & 0xFF]); }
void c6502_dec_absolute(uint16_t addr)   { set_flags_n_z(--mem[addr]); }
void c6502_dec_absolute_x(uint16_t addr) { set_flags_n_z(--mem[addr + reg_x]); }


/******************* INX, INY, DEX, DEY ******************/
void c6502_inx() { set_flags_n_z(++reg_x); }
void c6502_iny() { set_flags_n_z(++reg_y); }
void c6502_dex() { set_flags_n_z(--reg_x); }
void c6502_dey() { set_flags_n_z(--reg_y); }


/******************* TAX, TAY, TXA, TYA ******************/
void c6502_tax() { set_flags_n_z(reg_x = reg_a); }
void c6502_tay() { set_flags_n_z(reg_y = reg_a); }
void c6502_txa() { set_flags_n_z(reg_a = reg_x); }
void c6502_tya() { set_flags_n_z(reg_a = reg_y); }


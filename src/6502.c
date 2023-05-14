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
    reg_a = mem[addr];
    set_flags_n_z(reg_a);
}

void c6502_lda_zero_page_x(uint8_t addr) {
    uint8_t zp_addr = (addr + reg_x) & 0xFF;
    reg_a = mem[zp_addr];
    set_flags_n_z(reg_a);
}

void c6502_lda_absolute(uint16_t addr) {
    reg_a = mem[addr];
    set_flags_n_z(reg_a);
}

void c6502_lda_absolute_x(uint16_t addr) {
    reg_a = mem[addr + reg_x];
    set_flags_n_z(reg_a);
}

void c6502_lda_absolute_y(uint16_t addr) {
    reg_a = mem[addr + reg_y];
    set_flags_n_z(reg_a);
}

void c6502_lda_indirect_x(uint8_t addr) {
    uint8_t effective_addr_l = mem[(addr + reg_x) & 0xFF];
    uint8_t effective_addr_h = mem[(addr + reg_x + 1) & 0xFF];
    uint16_t effective_addr = effective_addr_l + (effective_addr_h << 8);
    reg_a = mem[effective_addr];
    set_flags_n_z(reg_a);
}

void c6502_lda_indirect_y(uint8_t addr) {
    uint8_t effective_addr_l = mem[addr];
    uint8_t effective_addr_h = mem[(addr + 1) & 0xFF];
    uint16_t effective_addr = (effective_addr_l + (effective_addr_h << 8)) + reg_y;
    reg_a = mem[effective_addr];
    set_flags_n_z(reg_a);
}


/************************** LDX **************************/
void c6502_ldx_immediate(uint8_t operand) {
    reg_x = operand;
    set_flags_n_z(reg_x);
}

void c6502_ldx_zero_page(uint8_t addr) {
    reg_x = mem[addr];
    set_flags_n_z(reg_x);
}

void c6502_ldx_zero_page_y(uint8_t addr) {
    uint8_t zp_addr = (addr + reg_y) & 0xFF;
    reg_x = mem[zp_addr];
    set_flags_n_z(reg_x);
}

void c6502_ldx_absolute(uint16_t addr) {
    reg_x = mem[addr];
    set_flags_n_z(reg_x);
}

void c6502_ldx_absolute_y(uint16_t addr) {
    reg_x = mem[addr + reg_y];
    set_flags_n_z(reg_x);
}


/************************** LDY **************************/
void c6502_ldy_immediate(uint8_t operand) {
    reg_y = operand;
    set_flags_n_z(reg_y);
}

void c6502_ldy_zero_page(uint8_t addr) {
    reg_y = mem[addr];
    set_flags_n_z(reg_y);
}

void c6502_ldy_zero_page_x(uint8_t addr) {
    uint8_t zp_addr = (addr + reg_x) & 0xFF;
    reg_y = mem[zp_addr];
    set_flags_n_z(reg_y);
}

void c6502_ldy_absolute(uint16_t addr) {
    reg_y = mem[addr];
    set_flags_n_z(reg_y);
}

void c6502_ldy_absolute_x(uint16_t addr) {
    reg_y = mem[addr + reg_x];
    set_flags_n_z(reg_y);
}


/************************** STA **************************/
void c6502_sta_immediate(uint8_t addr) {
    mem[addr] = reg_a;
}

void c6502_sta_zero_page(uint8_t addr) {
    mem[addr] = reg_a;
}

void c6502_sta_zero_page_x(uint8_t addr) {
    uint8_t zp_addr = (addr + reg_x) & 0xFF;
    mem[zp_addr] = reg_a;
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
    uint8_t zp_addr = (addr + reg_y) & 0xFF;
    mem[zp_addr] = reg_x;
}

void c6502_stx_absolute(uint16_t addr) {
    mem[addr] = reg_x;
}


/************************** STY **************************/
void c6502_sty_zero_page(uint8_t addr) {
    mem[addr] = reg_y;
}

void c6502_sty_zero_page_x(uint8_t addr) {
    uint8_t zp_addr = (addr + reg_x) & 0xFF;
    mem[zp_addr] = reg_y;
}

void c6502_sty_absolute(uint16_t addr) {
    mem[addr] = reg_y;
}


#include <stdbool.h>
#include <stdint.h>

#include "RP2A03.h"

void set_flag(RP2A03Flags flag, bool value) {
    flags &= ~flag;
    if (value) {
        flags |= flag;
    }
}

// returns true if the given flag is set, false otherwise
bool get_flag(RP2A03Flags flag) {
    return (flags & flag) != 0;
}

void set_flags_n_z(uint8_t result) {
    set_flag(RP2A03_FLAG_ZERO, result == 0);
    set_flag(RP2A03_FLAG_NEGATIVE, result & BIT_7);
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
void rp2A03_lda_immediate(uint8_t operand) { set_flags_n_z(reg_a = operand); }
void rp2A03_lda_zero_page(uint8_t addr)   { rp2A03_lda_immediate(mem[addr]); }
void rp2A03_lda_zero_page_x(uint8_t addr) { rp2A03_lda_immediate(mem[zero_page_x(addr)]); }
void rp2A03_lda_absolute(uint16_t addr)   { rp2A03_lda_immediate(mem[addr]); }
void rp2A03_lda_absolute_x(uint16_t addr) { rp2A03_lda_immediate(mem[addr + reg_x]); }
void rp2A03_lda_absolute_y(uint16_t addr) { rp2A03_lda_immediate(mem[addr + reg_y]); }
void rp2A03_lda_indirect_x(uint8_t addr)  { rp2A03_lda_immediate(mem[indirect_x(addr)]); }
void rp2A03_lda_indirect_y(uint8_t addr)  { rp2A03_lda_immediate(mem[indirect_y(addr)]); }


/************************** LDX **************************/
void rp2A03_ldx_immediate(uint8_t operand) { set_flags_n_z(reg_x = operand); }
void rp2A03_ldx_zero_page(uint8_t addr)   { rp2A03_ldx_immediate(mem[addr]); }
void rp2A03_ldx_zero_page_y(uint8_t addr) { rp2A03_ldx_immediate(mem[zero_page_y(addr)]); }
void rp2A03_ldx_absolute(uint16_t addr)   { rp2A03_ldx_immediate(mem[addr]); }
void rp2A03_ldx_absolute_y(uint16_t addr) { rp2A03_ldx_immediate(mem[addr + reg_y]); }


/************************** LDY **************************/
void rp2A03_ldy_immediate(uint8_t operand) { set_flags_n_z(reg_y = operand); }
void rp2A03_ldy_zero_page(uint8_t addr)   { rp2A03_ldy_immediate(mem[addr]); }
void rp2A03_ldy_zero_page_x(uint8_t addr) { rp2A03_ldy_immediate(mem[zero_page_x(addr)]); }
void rp2A03_ldy_absolute(uint16_t addr)   { rp2A03_ldy_immediate(mem[addr]); }
void rp2A03_ldy_absolute_x(uint16_t addr) { rp2A03_ldy_immediate(mem[addr + reg_x]); }


/************************** STA **************************/
void rp2A03_sta_zero_page(uint8_t addr)   { mem[addr] = reg_a; }
void rp2A03_sta_zero_page_x(uint8_t addr) { mem[zero_page_x(addr)] = reg_a; }
void rp2A03_sta_absolute(uint16_t addr)   { mem[addr] = reg_a; }
void rp2A03_sta_absolute_x(uint16_t addr) { mem[addr + reg_x] = reg_a; }
void rp2A03_sta_absolute_y(uint16_t addr) { mem[addr + reg_y] = reg_a; }
void rp2A03_sta_indirect_x(uint8_t addr)  { mem[indirect_x(addr)] = reg_a; }
void rp2A03_sta_indirect_y(uint8_t addr)  { mem[indirect_y(addr)] = reg_a; }


/************************** STX **************************/
void rp2A03_stx_zero_page(uint8_t addr)   { mem[addr] = reg_x; }
void rp2A03_stx_zero_page_y(uint8_t addr) { mem[zero_page_y(addr)] = reg_x; }
void rp2A03_stx_absolute(uint16_t addr)   { mem[addr] = reg_x; }


/************************** STY **************************/
void rp2A03_sty_zero_page(uint8_t addr)   { mem[addr] = reg_y; }
void rp2A03_sty_zero_page_x(uint8_t addr) { mem[zero_page_x(addr)] = reg_y; }
void rp2A03_sty_absolute(uint16_t addr)   { mem[addr] = reg_y; }


/************************** ADC **************************/
void rp2A03_adc_immediate(uint8_t operand) {
    uint16_t sum = reg_a + operand + (flags & RP2A03_FLAG_CARRY);
    set_flag(RP2A03_FLAG_CARRY, sum > 0xFF);
    set_flag(RP2A03_FLAG_OVERFLOW, (~(reg_a ^ operand) & (reg_a ^ sum)) & BIT_7);
    reg_a = sum & 0xFF;
    set_flags_n_z(reg_a);
}
void rp2A03_adc_zero_page(uint8_t addr)   { rp2A03_adc_immediate(mem[addr]); }
void rp2A03_adc_zero_page_x(uint8_t addr) { rp2A03_adc_immediate(mem[zero_page_x(addr)]); }
void rp2A03_adc_absolute(uint16_t addr)   { rp2A03_adc_immediate(mem[addr]); }
void rp2A03_adc_absolute_x(uint16_t addr) { rp2A03_adc_immediate(mem[(addr + reg_x) & 0xFFFF]); }
void rp2A03_adc_absolute_y(uint16_t addr) { rp2A03_adc_immediate(mem[(addr + reg_y) & 0xFFFF]); }
void rp2A03_adc_indirect_x(uint8_t addr)  { rp2A03_adc_immediate(mem[indirect_x(addr)]); }
void rp2A03_adc_indirect_y(uint8_t addr)  { rp2A03_adc_immediate(mem[indirect_y(addr)]); }


/************************** SBC **************************/
void rp2A03_sbc_immediate(uint8_t operand) {
    uint16_t diff = reg_a - operand - (1 - (flags & RP2A03_FLAG_CARRY));
    set_flag(RP2A03_FLAG_CARRY, diff < 0x100);
    set_flag(RP2A03_FLAG_OVERFLOW, ((reg_a ^ diff) & BIT_7) && ((reg_a ^ operand) & BIT_7));
    reg_a = diff & 0xFF;
    set_flags_n_z(reg_a);
}
void rp2A03_sbc_zero_page(uint8_t addr)   { rp2A03_sbc_immediate(mem[addr]); }
void rp2A03_sbc_zero_page_x(uint8_t addr) { rp2A03_sbc_immediate(mem[zero_page_x(addr)]); }
void rp2A03_sbc_absolute(uint16_t addr)   { rp2A03_sbc_immediate(mem[addr]); }
void rp2A03_sbc_absolute_x(uint16_t addr) { rp2A03_sbc_immediate(mem[addr + reg_x]); }
void rp2A03_sbc_absolute_y(uint16_t addr) { rp2A03_sbc_immediate(mem[addr + reg_y]); }
void rp2A03_sbc_indirect_x(uint8_t addr)  { rp2A03_sbc_immediate(mem[indirect_x(addr)]); }
void rp2A03_sbc_indirect_y(uint8_t addr)  { rp2A03_sbc_immediate(mem[indirect_y(addr)]); }


/************************** INC **************************/
void rp2A03_inc_zero_page(uint8_t addr)   { set_flags_n_z(++mem[addr]); }
void rp2A03_inc_zero_page_x(uint8_t addr) { set_flags_n_z(++mem[zero_page_x(addr)]); }
void rp2A03_inc_absolute(uint16_t addr)   { set_flags_n_z(++mem[addr]); }
void rp2A03_inc_absolute_x(uint16_t addr) { set_flags_n_z(++mem[addr + reg_x]); }


/************************** DEC **************************/
void rp2A03_dec_zero_page(uint8_t addr)   { set_flags_n_z(--mem[addr]); }
void rp2A03_dec_zero_page_x(uint8_t addr) { set_flags_n_z(--mem[zero_page_x(addr)]); }
void rp2A03_dec_absolute(uint16_t addr)   { set_flags_n_z(--mem[addr]); }
void rp2A03_dec_absolute_x(uint16_t addr) { set_flags_n_z(--mem[addr + reg_x]); }


/******************* INX, INY, DEX, DEY ******************/
void rp2A03_inx() { set_flags_n_z(++reg_x); }
void rp2A03_iny() { set_flags_n_z(++reg_y); }
void rp2A03_dex() { set_flags_n_z(--reg_x); }
void rp2A03_dey() { set_flags_n_z(--reg_y); }


/******************* TAX, TAY, TXA, TYA ******************/
void rp2A03_tax() { set_flags_n_z(reg_x = reg_a); }
void rp2A03_tay() { set_flags_n_z(reg_y = reg_a); }
void rp2A03_txa() { set_flags_n_z(reg_a = reg_x); }
void rp2A03_tya() { set_flags_n_z(reg_a = reg_y); }


/************************** AND **************************/
void rp2A03_and_immediate(uint8_t operand) { set_flags_n_z(reg_a &= operand); }
void rp2A03_and_zero_page(uint8_t addr)   { rp2A03_and_immediate(mem[addr]); }
void rp2A03_and_zero_page_x(uint8_t addr) { rp2A03_and_immediate(mem[zero_page_x(addr)]); }
void rp2A03_and_absolute(uint16_t addr)   { rp2A03_and_immediate(mem[addr]); }
void rp2A03_and_absolute_x(uint16_t addr) { rp2A03_and_immediate(mem[addr + reg_x]); }
void rp2A03_and_absolute_y(uint16_t addr) { rp2A03_and_immediate(mem[addr + reg_y]); }
void rp2A03_and_indirect_x(uint8_t addr)  { rp2A03_and_immediate(mem[indirect_x(addr)]); }
void rp2A03_and_indirect_y(uint8_t addr)  { rp2A03_and_immediate(mem[indirect_y(addr)]); }


/************************** EOR **************************/
void rp2A03_eor_immediate(uint8_t operand) { set_flags_n_z(reg_a ^= operand); }
void rp2A03_eor_zero_page(uint8_t addr)   { rp2A03_eor_immediate(mem[addr]); }
void rp2A03_eor_zero_page_x(uint8_t addr) { rp2A03_eor_immediate(mem[zero_page_x(addr)]); }
void rp2A03_eor_absolute(uint16_t addr)   { rp2A03_eor_immediate(mem[addr]); }
void rp2A03_eor_absolute_x(uint16_t addr) { rp2A03_eor_immediate(mem[addr + reg_x]); }
void rp2A03_eor_absolute_y(uint16_t addr) { rp2A03_eor_immediate(mem[addr + reg_y]); }
void rp2A03_eor_indirect_x(uint8_t addr)  { rp2A03_eor_immediate(mem[indirect_x(addr)]); }
void rp2A03_eor_indirect_y(uint8_t addr)  { rp2A03_eor_immediate(mem[indirect_y(addr)]); }


/************************** ORA **************************/
void rp2A03_ora_immediate(uint8_t operand) { set_flags_n_z(reg_a |= operand); }
void rp2A03_ora_zero_page(uint8_t addr)   { rp2A03_ora_immediate(mem[addr]); }
void rp2A03_ora_zero_page_x(uint8_t addr) { rp2A03_ora_immediate(mem[zero_page_x(addr)]); }
void rp2A03_ora_absolute(uint16_t addr)   { rp2A03_ora_immediate(mem[addr]); }
void rp2A03_ora_absolute_x(uint16_t addr) { rp2A03_ora_immediate(mem[addr + reg_x]); }
void rp2A03_ora_absolute_y(uint16_t addr) { rp2A03_ora_immediate(mem[addr + reg_y]); }
void rp2A03_ora_indirect_x(uint8_t addr)  { rp2A03_ora_immediate(mem[indirect_x(addr)]); }
void rp2A03_ora_indirect_y(uint8_t addr)  { rp2A03_ora_immediate(mem[indirect_y(addr)]); }


/************************** CMP **************************/
void rp2A03_cmp_immediate(uint8_t operand) {
    set_flags_n_z(reg_a - operand);
    set_flag(RP2A03_FLAG_CARRY, reg_a >= operand);
}
void rp2A03_cmp_zero_page(uint8_t addr)   { rp2A03_cmp_immediate(mem[addr]); }
void rp2A03_cmp_zero_page_x(uint8_t addr) { rp2A03_cmp_immediate(mem[zero_page_x(addr)]); }
void rp2A03_cmp_absolute(uint16_t addr)   { rp2A03_cmp_immediate(mem[addr]); }
void rp2A03_cmp_absolute_x(uint16_t addr) { rp2A03_cmp_immediate(mem[addr + reg_x]); }
void rp2A03_cmp_absolute_y(uint16_t addr) { rp2A03_cmp_immediate(mem[addr + reg_y]); }
void rp2A03_cmp_indirect_x(uint8_t addr)  { rp2A03_cmp_immediate(mem[indirect_x(addr)]); }
void rp2A03_cmp_indirect_y(uint8_t addr)  { rp2A03_cmp_immediate(mem[indirect_y(addr)]); }


/******************************* CPX *******************************/
void rp2A03_cpx_immediate(uint8_t operand) {
    set_flags_n_z(reg_x - operand);
    set_flag(RP2A03_FLAG_CARRY, reg_x >= operand);
}
void rp2A03_cpx_zero_page(uint8_t addr) { rp2A03_cpx_immediate(mem[addr]); }
void rp2A03_cpx_absolute(uint16_t addr) { rp2A03_cpx_immediate(mem[addr]); }


/******************************* CPY *******************************/
void rp2A03_cpy_immediate(uint8_t operand) {
    set_flags_n_z(reg_y - operand);
    set_flag(RP2A03_FLAG_CARRY, reg_y >= operand);
}
void rp2A03_cpy_zero_page(uint8_t addr) { rp2A03_cpy_immediate(mem[addr]); }
void rp2A03_cpy_absolute(uint16_t addr) { rp2A03_cpy_immediate(mem[addr]); }


/******************************* BIT *******************************/
void rp2A03_bit_absolute(uint16_t addr) {
    set_flag(RP2A03_FLAG_ZERO, (reg_a & mem[addr]) == 0);
    set_flag(RP2A03_FLAG_OVERFLOW, mem[addr] & RP2A03_FLAG_OVERFLOW);
    set_flag(RP2A03_FLAG_NEGATIVE, mem[addr] & RP2A03_FLAG_NEGATIVE);
}
void rp2A03_bit_zero_page(uint8_t addr) { rp2A03_bit_absolute(addr); }


/******************************* ASL *******************************/
void rp2A03_asl_accumulator() {
    set_flag(RP2A03_FLAG_CARRY, reg_a & BIT_7);
    set_flags_n_z(reg_a <<= 1);
}
void rp2A03_asl_absolute(uint16_t addr) {
    set_flag(RP2A03_FLAG_CARRY, mem[addr] & BIT_7);
    set_flags_n_z(mem[addr] <<= 1);
}
void rp2A03_asl_absolute_x(uint16_t addr) { rp2A03_asl_absolute((addr + reg_x) & 0xFFFF); }
void rp2A03_asl_zero_page(uint8_t addr)   { rp2A03_asl_absolute(addr); }
void rp2A03_asl_zero_page_x(uint8_t addr) { rp2A03_asl_absolute(zero_page_x(addr)); }


/******************************* LSR *******************************/
void rp2A03_lsr_accumulator() {
    set_flag(RP2A03_FLAG_CARRY, reg_a & BIT_0);
    set_flags_n_z(reg_a >>= 1);
}
void rp2A03_lsr_absolute(uint16_t addr) {
    set_flag(RP2A03_FLAG_CARRY, mem[addr] & BIT_0);
    set_flags_n_z(mem[addr] >>= 1);
}
void rp2A03_lsr_absolute_x(uint16_t addr) { rp2A03_lsr_absolute((addr + reg_x) & 0xFFFF); }
void rp2A03_lsr_zero_page(uint8_t addr)   { rp2A03_lsr_absolute(addr); }
void rp2A03_lsr_zero_page_x(uint8_t addr) { rp2A03_lsr_absolute(zero_page_x(addr)); }


/******************************* ROL *******************************/
// C <- [76543210] <- C
void rp2A03_rol_accumulator() {
    // store the carry flag in a temporary variable
    bool carry = get_flag(RP2A03_FLAG_CARRY);

    // push bit 7 to carry flag
    set_flag(RP2A03_FLAG_CARRY, (reg_a >> 7) & 0x01);

    // push the old carry flag value to the bit 0, shifting the rest to the left
    reg_a = (reg_a << 1) | carry;

    set_flags_n_z(reg_a);
}
void rp2A03_rol_absolute(uint16_t addr) {
    bool carry = get_flag(RP2A03_FLAG_CARRY);
    set_flag(RP2A03_FLAG_CARRY, (mem[addr] >> 8) & 0x01);
    mem[addr] = (mem[addr] << 1) | carry;
    set_flags_n_z(mem[addr]);
}
void rp2A03_rol_absolute_x(uint16_t addr) { rp2A03_rol_absolute((addr + reg_x) & 0xFFFF); }
void rp2A03_rol_zero_page(uint8_t addr)   { rp2A03_rol_absolute(addr); }
void rp2A03_rol_zero_page_x(uint8_t addr) { rp2A03_rol_absolute(zero_page_x(addr)); }


/******************************* ROR *******************************/
// C -> [76543210] -> C
void rp2A03_ror_accumulator() {
    // store the carry flag in a temporary variable
    bool carry = get_flag(RP2A03_FLAG_CARRY);

    // push bit 0 to carry flag
    set_flag(RP2A03_FLAG_CARRY, reg_a & 0x01);

    // push the old carry flag value to the bit 7, shifting the rest to the right
    reg_a = (reg_a >> 1) | (carry << 7);

    set_flags_n_z(reg_a);
}
void rp2A03_ror_absolute(uint16_t addr) {
    bool carry = get_flag(RP2A03_FLAG_CARRY);
    set_flag(RP2A03_FLAG_CARRY, mem[addr] & 0x01);
    mem[addr] = (mem[addr] >> 1) | (carry << 7);
    set_flags_n_z(mem[addr]);
}
void rp2A03_ror_absolute_x(uint16_t addr) { rp2A03_ror_absolute((addr + reg_x) & 0xFFFF); }
void rp2A03_ror_zero_page(uint8_t addr)   { rp2A03_ror_absolute(addr); }
void rp2A03_ror_zero_page_x(uint8_t addr) { rp2A03_ror_absolute(zero_page_x(addr)); }


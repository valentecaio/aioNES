#ifndef C6502_INSTRUCTIONS_H
#define C6502_INSTRUCTIONS_H

#include <stdbool.h>
#include <stdint.h>

#include "libretro/libretro.h"
#include "cartridge.h"

/*
$0000–$07FF   $0800   2 KB internal RAM
$0800–$0FFF   $0800   Mirrors of $0000–$07FF
$1000–$17FF   $0800   Mirrors of $0000–$07FF
$1800–$1FFF   $0800   Mirrors of $0000–$07FF
$2000–$2007   $0008   NES PPU registers
$2008–$3FFF   $1FF8   Mirrors of $2000–$2007 (repeats every 8 bytes)
$4000–$4017   $0018   NES APU and I/O registers
$4018–$401F   $0008   APU and I/O functionality that is normally disabled. See CPU Test Mode.
$4020–$FFFF   $BFE0   Cartridge space: PRG ROM, PRG RAM, and mapper registers
*/
uint8_t mem[65536] = {0};
uint8_t reg_a = 0;   // accumulator
uint8_t reg_x = 0;
uint8_t reg_y = 0;
bool flag_n = false; // negative    keeps track if the result in the ALU is negative
bool flag_z = false; // zero        keeps track if the result of a calculation is zero
bool flag_v = false; // overflow
bool flag_c = false; // carry

void set_flags(uint8_t result);


/*  Load Accumulator (flags: N,Z)
    LDA #$aa        $A9     2   c6502_lda_immediate       Immediate Addressing
    LDA $aa         $A5     2   c6502_lda_zero_page       Zero Page Addressing
    LDA $aa,X       $B5     2   c6502_lda_zero_page_x     Zero Page, X-Indexed Addressing
    LDA $aaaa       $AD     3   c6502_lda_absolute        Absolute Addressing
    LDA $aaaa,X     $BD     3   c6502_lda_absolute_x      Absolute, X-Indexed Addressing
    LDA $aaaa,Y     $B9     3   c6502_lda_absolute_y      Absolute, Y-Indexed Addressing
    LDA ($aa),X     $A1     2   c6502_lda_indirect_x      Indirect, X-Indexed Addressing
    LDA ($aa,Y)     $B1     2   c6502_lda_indirect_y      Indirect Y-Indexed Addressing
*/
void c6502_lda_immediate(uint8_t operand);
void c6502_lda_zero_page(uint8_t addr);
void c6502_lda_zero_page_x(uint8_t addr);
void c6502_lda_absolute(uint16_t addr);
void c6502_lda_absolute_x(uint16_t addr);
void c6502_lda_absolute_y(uint16_t addr);
void c6502_lda_indirect_x(uint8_t addr);
void c6502_lda_indirect_y(uint8_t addr);


/*  Load X Register (flags: N,Z)
    LDX #$aa        $A2     2   c6502_ldx_immediate
    LDX $aa         $A6     2   c6502_ldx_zero_page
    LDX $aa,Y       $B6     2   c6502_ldx_zero_page_y
    LDX $aaaa       $AE     3   c6502_ldx_absolute
    LDX $aaaa,Y     $BE     3   c6502_ldx_absolute_y
*/
void c6502_ldx_immediate(uint8_t operand);
void c6502_ldx_zero_page(uint8_t addr);
void c6502_ldx_zero_page_y(uint8_t addr);
void c6502_ldx_absolute(uint16_t addr);
void c6502_ldx_absolute_y(uint16_t addr);


/*  Load Y Register (flags: N,Z)
    LDY #$aa        $A0     2   c6502_ldy_immediate
    LDY $aa         $A4     2   c6502_ldy_zero_page
    LDY $aa,X       $B4     2   c6502_ldy_zero_page_x
    LDY $aaaa       $AC     3   c6502_ldy_absolute
    LDY $aaaa,X     $BC     3   c6502_ldy_absolute_x
*/
void c6502_ldy_immediate(uint8_t operand);
void c6502_ldy_zero_page(uint8_t addr);
void c6502_ldy_zero_page_x(uint8_t addr);
void c6502_ldy_absolute(uint16_t addr);
void c6502_ldy_absolute_x(uint16_t addr);


/*  Store Accumulator (flags: none)
    STA $aa         $85     2   c6502_sta_zero_page
    STA $aa,X       $95     2   c6502_sta_zero_page_x
    STA $aaaa       $8D     3   c6502_sta_absolute
    STA $aaaa,X     $9D     3   c6502_sta_absolute_x
    STA $aaaa,Y     $99     3   c6502_sta_absolute_y
    STA ($aa,X)     $81     2   c6502_sta_indirect_x
    STA ($aa),Y     $91     2   c6502_sta_indirect_y
*/
void c6502_sta_zero_page(uint8_t addr);
void c6502_sta_zero_page_x(uint8_t addr);
void c6502_sta_absolute(uint16_t addr);
void c6502_sta_absolute_x(uint16_t addr);
void c6502_sta_absolute_y(uint16_t addr);
void c6502_sta_indirect_x(uint8_t addr);
void c6502_sta_indirect_y(uint8_t addr);


/*  Store X Register (flags: none)
    STX $aa     $86     2   c6502_stx_zero_page
    STX $aa,Y   $96     2   c6502_stx_zero_page_y
    STX $aaaa   $8E     3   c6502_stx_absolute
*/
void c6502_stx_zero_page(uint8_t addr);
void c6502_stx_zero_page_y(uint8_t addr);
void c6502_stx_absolute(uint16_t addr);


/*  Store Y Register (flags: none)
    STY $aa     $84     2   c6502_sty_zero_page
    STY $aa,X   $94     2   c6502_sty_zero_page_x
    STY $aaaa   $8C     3   c6502_sty_absolute
*/
void c6502_sty_zero_page(uint8_t addr);
void c6502_sty_zero_page_x(uint8_t addr);
void c6502_sty_absolute(uint16_t addr);


/*  Add with Carry (flags: N,V,Z,C)
    ADC #$aa        $69     2   c6502_adc_immediate
    ADC $aa         $65     2   c6502_adc_zero_page
    ADC $aa,X       $75     2   c6502_adc_zero_page_x
    ADC $aaaa       $6D     3   c6502_adc_absolute
    ADC $aaaa,X     $7D     3   c6502_adc_absolute_x
    ADC $aaaa,Y     $79     3   c6502_adc_absolute_y
    ADC ($aa,X)     $61     2   c6502_adc_indirect_x
    ADC ($aa),Y     $71     2   c6502_adc_indirect_y
*/
void c6502_adc_immediate(uint8_t operand);
void c6502_adc_zero_page(uint8_t addr);
void c6502_adc_zero_page_x(uint8_t addr);
void c6502_adc_absolute(uint16_t addr);
void c6502_adc_absolute_x(uint16_t addr);
void c6502_adc_absolute_y(uint16_t addr);
void c6502_adc_indirect_x(uint8_t addr);
void c6502_adc_indirect_y(uint8_t addr);


/*  Subtract with Carry (flags: N,V,Z,C)
    SBC #$aa        $E9     2   c6502_sbc_immediate
    SBC $aa         $E5     2   c6502_sbc_zero_page
    SBC $aa,X       $F5     2   c6502_sbc_zero_page_x
    SBC $aaaa       $ED     3   c6502_sbc_absolute
    SBC $aaaa,X     $FD     3   c6502_sbc_absolute_x
    SBC $aaaa,Y     $F9     3   c6502_sbc_absolute_y
    SBC ($aa,X)     $E1     2   c6502_sbc_indirect_x
    SBC ($aa),Y     $F1     2   c6502_sbc_indirect_y
*/
void c6502_sbc_immediate(uint8_t operand);
void c6502_sbc_zero_page(uint8_t addr);
void c6502_sbc_zero_page_x(uint8_t addr);
void c6502_sbc_absolute(uint16_t addr);
void c6502_sbc_absolute_x(uint16_t addr);
void c6502_sbc_absolute_y(uint16_t addr);
void c6502_sbc_indirect_x(uint8_t addr);
void c6502_sbc_indirect_y(uint8_t addr);


/*  Increment memory (flags: N, Z)
    INC $aa         $E6     2   c6502_inc_zero_page
    INC $aa,X       $F6     2   c6502_inc_zero_page_x
    INC $aaaa       $EE     3   c6502_inc_absolute
    INC $aaaa,X     $FE     3   c6502_inc_absolute_x
*/
void c6502_inc_zero_page(uint8_t addr);
void c6502_inc_zero_page_x(uint8_t addr);
void c6502_inc_absolute(uint16_t addr);
void c6502_inc_absolute_x(uint16_t addr);


/*  Increment X register (flags: N, Z)
    INX             $E8     1   c6502_inx_implied
*/
void c6502_inx_implied();


/*  Increment X register (flags: N, Z)
    INY             $C8     1   c6502_iny_implied
*/
void c6502_iny_implied();


#endif /* C6502_INSTRUCTIONS_H */

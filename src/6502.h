#ifndef C6502_INSTRUCTIONS_H
#define C6502_INSTRUCTIONS_H

#include <stdbool.h>

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
unsigned char mem[65536] = {0};
unsigned char reg_a = 0;           // accumulator
unsigned char reg_x = 0;
unsigned char reg_y = 0;
bool flag_n = false; // negative    keeps track if the result in the ALU is negative
bool flag_z = false; // zero        keeps track if the result of a calculation is zero
bool flag_v = false; // overflow
bool flag_c = false; // carry

void set_flags(unsigned char result);


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
void c6502_lda_immediate(unsigned char operand);
void c6502_lda_zero_page(unsigned char addr);
void c6502_lda_zero_page_x(unsigned char addr);
void c6502_lda_absolute(unsigned short addr);
void c6502_lda_absolute_x(unsigned short addr);
void c6502_lda_absolute_y(unsigned short addr);
void c6502_lda_indirect_x(unsigned short addr);
void c6502_lda_indirect_y(unsigned short addr);


/*  Load X Register (flags: N,Z)
    LDX #$aa        $A2     2   c6502_ldx_immediate
    LDX $aa         $A6     2   c6502_ldx_zero_page
    LDX $aa,Y       $B6     2   c6502_ldx_zero_page_y
    LDX $aaaa       $AE     3   c6502_ldx_absolute
    LDX $aaaa,Y     $BE     3   c6502_ldx_absolute_y
*/
void c6502_ldx_immediate(unsigned char operand);
void c6502_ldx_zero_page(unsigned char addr);
void c6502_ldx_zero_page_y(unsigned char addr);
void c6502_ldx_absolute(unsigned short addr);
void c6502_ldx_absolute_y(unsigned short addr);


/*  Load Y Register (flags: N,Z)
    LDY #$aa        $A0     2   c6502_ldy_immediate
    LDY $aa         $A4     2   c6502_ldy_zero_page
    LDY $aa,X       $B4     2   c6502_ldy_zero_page_x
    LDY $aaaa       $AC     3   c6502_ldy_absolute
    LDY $aaaa,X     $BC     3   c6502_ldy_absolute_x
*/
void c6502_ldy_immediate(unsigned char operand);
void c6502_ldy_zero_page(unsigned char addr);
void c6502_ldy_zero_page_x(unsigned char addr);
void c6502_ldy_absolute(unsigned short addr);
void c6502_ldy_absolute_x(unsigned short addr);


/*  Store Accumulator (flags: none)
    STA $aa         $85     2   c6502_sta_zero_page
    STA $aa,X       $95     2   c6502_sta_zero_page_x
    STA $aaaa       $8D     3   c6502_sta_absolute
    STA $aaaa,X     $9D     3   c6502_sta_absolute_x
    STA $aaaa,Y     $99     3   c6502_sta_absolute_y
    STA ($aa,X)     $81     2   c6502_sta_indirect_x
    STA ($aa),Y     $91     2   c6502_sta_indirect_y
*/
void c6502_sta_zero_page(unsigned char addr);
void c6502_sta_zero_page_x(unsigned char addr);
void c6502_sta_zero_page_y(unsigned char addr);
void c6502_sta_absolute(unsigned short addr);
void c6502_sta_absolute_x(unsigned short addr);
void c6502_sta_absolute_y(unsigned short addr);
void c6502_sta_indirect_x(unsigned short addr);
void c6502_sta_indirect_y(unsigned short addr);


/*  Store X Register (flags: none)
    STX $aa     $86     2   c6502_stx_zero_page
    STX $aa,Y   $96     2   c6502_stx_zero_page_y
    STX $aaaa   $8E     3   c6502_stx_absolute
*/
void c6502_stx_zero_page(unsigned char addr);
void c6502_stx_zero_page_y(unsigned char addr);
void c6502_stx_absolute(unsigned short addr);


/*  Store Y Register (flags: none)
    STY $aa     $84     2   c6502_sty_zero_page
    STY $aa,X   $94     2   c6502_sty_zero_page_x
    STY $aaaa   $8C     3   c6502_sty_absolute
*/
void c6502_sty_zero_page(unsigned char addr);
void c6502_sty_zero_page_x(unsigned char addr);
void c6502_sty_absolute(unsigned short addr);

#endif /* C6502_INSTRUCTIONS_H */

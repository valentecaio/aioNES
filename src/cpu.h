#ifndef CPU_H
#define CPU_H

#include <stdbool.h>
#include <stdint.h>

#include "libretro/libretro.h"
#include "cartridge.h"

/*
    This file implements the Ricoh 2A03 (CPU) processor assembly language instructions.

    addr range    size    description
    -----------------------------------------------------------------
    $0000–$07FF   $0800   2 KB internal RAM
    $0800–$0FFF   $0800   Mirrors of $0000–$07FF
    $1000–$17FF   $0800   Mirrors of $0000–$07FF
    $1800–$1FFF   $0800   Mirrors of $0000–$07FF
    $2000–$2007   $0008   NES PPU registers
    $2008–$3FFF   $1FF8   Mirrors of $2000–$2007 (repeats every 8 bytes)
    $4000–$4017   $0018   NES APU and I/O registers
    $4018–$401F   $0008   APU and I/O functionality that is normally disabled
    $4020–$FFFF   $BFE0   Cartridge space: PRG ROM, PRG RAM, and mapper registers
*/

#define BIT_0 0b00000001
#define BIT_1 0b00000010
#define BIT_2 0b00000100
#define BIT_3 0b00001000
#define BIT_4 0b00010000
#define BIT_5 0b00100000
#define BIT_6 0b01000000
#define BIT_7 0b10000000

#define CPU_RAM_ADDR_START       0x0000
#define CPU_RAM_SIZE             0x0800 // 2 KB
#define CPU_PPU_ADDR_START       0x2000
#define CPU_PPU_SIZE             0x0008 // 8 bytes
#define CPU_APU_ADDR_START       0x4000
#define CPU_APU_SIZE             0x0018 // 24 bytes
#define CPU_CARTRIDGE_ADDR_START 0x4020
#define CPU_CARTRIDGE_SIZE       0xBFE0 // 49120 bytes
#define CPU_MEM_SIZE             0xFFFF // 64 KB


/**************************** CPU STATE ****************************/
extern uint8_t mem[]; // 64 KB of memory
extern uint8_t reg_a; // accumulator
extern uint8_t reg_x;
extern uint8_t reg_y;
extern uint8_t flags;

/*  7  bit  0
    ---- ----
    NVss DIZC
    |||| ||||
    |||| |||+- Carry              set if the result of a calculation is greater than 255
    |||| ||+-- Zero               set if the result of a calculation is zero
    |||| |+--- Interrupt Disable
    |||| +---- Decimal
    ||++------ No CPU effect      ignored in this implementation
    |+-------- Overflow           set if the result of a calculation is greater than 127 or less than -128
    +--------- Negative           set if the result in the ALU is negative
*/
typedef enum CPUFlags {
    CPU_FLAG_CARRY     = 0b00000001, // C flag bitmask
    CPU_FLAG_ZERO      = 0b00000010, // Z flag bitmask
    CPU_FLAG_INTERRUPT = 0b00000100, // I flag bitmask
    CPU_FLAG_DECIMAL   = 0b00001000, // D flag bitmask
    CPU_FLAG_OVERFLOW  = 0b01000000, // V flag bitmask
    CPU_FLAG_NEGATIVE  = 0b10000000  // N flag bitmask
} CPUFlags;


/************************** LOOK UP TABLE **************************/

typedef struct {
    const char* mnemonic;
    uint8_t numBytes;
    uint8_t numCycles;
    const char* addr_mode;
} CPUInstruction;

// lookup table (LUT) for the number of bytes and cycles each instruction takes
// the index is the opcode
extern CPUInstruction cpu_instruction_table[];


/******************************* LDA *******************************
    Load Accumulator (flags: N,Z)
    LDA #$aa        $A9     2   cpu_lda_immediate    Immediate Addressing
    LDA $aa         $A5     2   cpu_lda_zero_page    Zero Page Addressing
    LDA $aa,X       $B5     2   cpu_lda_zero_page_x  Zero Page, X-Indexed Addressing
    LDA $aaaa       $AD     3   cpu_lda_absolute     Absolute Addressing
    LDA $aaaa,X     $BD     3   cpu_lda_absolute_x   Absolute, X-Indexed Addressing
    LDA $aaaa,Y     $B9     3   cpu_lda_absolute_y   Absolute, Y-Indexed Addressing
    LDA ($aa),X     $A1     2   cpu_lda_indirect_x   Indirect, X-Indexed Addressing
    LDA ($aa,Y)     $B1     2   cpu_lda_indirect_y   Indirect Y-Indexed Addressing
*/
void cpu_lda_immediate(uint8_t operand);
void cpu_lda_zero_page(uint8_t addr);
void cpu_lda_zero_page_x(uint8_t addr);
void cpu_lda_absolute(uint16_t addr);
void cpu_lda_absolute_x(uint16_t addr);
void cpu_lda_absolute_y(uint16_t addr);
void cpu_lda_indirect_x(uint8_t addr);
void cpu_lda_indirect_y(uint8_t addr);


/******************************* LDX *******************************
    Load X Register (flags: N,Z)
    LDX #$aa        $A2     2   cpu_ldx_immediate
    LDX $aa         $A6     2   cpu_ldx_zero_page
    LDX $aa,Y       $B6     2   cpu_ldx_zero_page_y
    LDX $aaaa       $AE     3   cpu_ldx_absolute
    LDX $aaaa,Y     $BE     3   cpu_ldx_absolute_y
*/
void cpu_ldx_immediate(uint8_t operand);
void cpu_ldx_zero_page(uint8_t addr);
void cpu_ldx_zero_page_y(uint8_t addr);
void cpu_ldx_absolute(uint16_t addr);
void cpu_ldx_absolute_y(uint16_t addr);


/******************************* LDY *******************************
    Load Y Register (flags: N,Z)
    LDY #$aa        $A0     2   cpu_ldy_immediate
    LDY $aa         $A4     2   cpu_ldy_zero_page
    LDY $aa,X       $B4     2   cpu_ldy_zero_page_x
    LDY $aaaa       $AC     3   cpu_ldy_absolute
    LDY $aaaa,X     $BC     3   cpu_ldy_absolute_x
*/
void cpu_ldy_immediate(uint8_t operand);
void cpu_ldy_zero_page(uint8_t addr);
void cpu_ldy_zero_page_x(uint8_t addr);
void cpu_ldy_absolute(uint16_t addr);
void cpu_ldy_absolute_x(uint16_t addr);


/******************************* STA *******************************
    Store Accumulator (flags: none)
    STA $aa         $85     2   cpu_sta_zero_page
    STA $aa,X       $95     2   cpu_sta_zero_page_x
    STA $aaaa       $8D     3   cpu_sta_absolute
    STA $aaaa,X     $9D     3   cpu_sta_absolute_x
    STA $aaaa,Y     $99     3   cpu_sta_absolute_y
    STA ($aa,X)     $81     2   cpu_sta_indirect_x
    STA ($aa),Y     $91     2   cpu_sta_indirect_y
*/
void cpu_sta_zero_page(uint8_t addr);
void cpu_sta_zero_page_x(uint8_t addr);
void cpu_sta_absolute(uint16_t addr);
void cpu_sta_absolute_x(uint16_t addr);
void cpu_sta_absolute_y(uint16_t addr);
void cpu_sta_indirect_x(uint8_t addr);
void cpu_sta_indirect_y(uint8_t addr);


/******************************* STX *******************************
    Store X Register (flags: none)
    STX $aa         $86     2   cpu_stx_zero_page
    STX $aa,Y       $96     2   cpu_stx_zero_page_y
    STX $aaaa       $8E     3   cpu_stx_absolute
*/
void cpu_stx_zero_page(uint8_t addr);
void cpu_stx_zero_page_y(uint8_t addr);
void cpu_stx_absolute(uint16_t addr);


/******************************* STY *******************************
    Store Y Register (flags: none)
    STY $aa         $84     2   cpu_sty_zero_page
    STY $aa,X       $94     2   cpu_sty_zero_page_x
    STY $aaaa       $8C     3   cpu_sty_absolute
*/
void cpu_sty_zero_page(uint8_t addr);
void cpu_sty_zero_page_x(uint8_t addr);
void cpu_sty_absolute(uint16_t addr);


/******************************* ADC *******************************
    Add with Carry (flags: N,V,Z,C)
    ADC #$aa        $69     2   cpu_adc_immediate
    ADC $aa         $65     2   cpu_adc_zero_page
    ADC $aa,X       $75     2   cpu_adc_zero_page_x
    ADC $aaaa       $6D     3   cpu_adc_absolute
    ADC $aaaa,X     $7D     3   cpu_adc_absolute_x
    ADC $aaaa,Y     $79     3   cpu_adc_absolute_y
    ADC ($aa,X)     $61     2   cpu_adc_indirect_x
    ADC ($aa),Y     $71     2   cpu_adc_indirect_y
*/
void cpu_adc_immediate(uint8_t operand);
void cpu_adc_zero_page(uint8_t addr);
void cpu_adc_zero_page_x(uint8_t addr);
void cpu_adc_absolute(uint16_t addr);
void cpu_adc_absolute_x(uint16_t addr);
void cpu_adc_absolute_y(uint16_t addr);
void cpu_adc_indirect_x(uint8_t addr);
void cpu_adc_indirect_y(uint8_t addr);


/******************************* SBC *******************************
    Subtract with Carry (flags: N,V,Z,C)
    SBC #$aa        $E9     2   cpu_sbc_immediate
    SBC $aa         $E5     2   cpu_sbc_zero_page
    SBC $aa,X       $F5     2   cpu_sbc_zero_page_x
    SBC $aaaa       $ED     3   cpu_sbc_absolute
    SBC $aaaa,X     $FD     3   cpu_sbc_absolute_x
    SBC $aaaa,Y     $F9     3   cpu_sbc_absolute_y
    SBC ($aa,X)     $E1     2   cpu_sbc_indirect_x
    SBC ($aa),Y     $F1     2   cpu_sbc_indirect_y
*/
void cpu_sbc_immediate(uint8_t operand);
void cpu_sbc_zero_page(uint8_t addr);
void cpu_sbc_zero_page_x(uint8_t addr);
void cpu_sbc_absolute(uint16_t addr);
void cpu_sbc_absolute_x(uint16_t addr);
void cpu_sbc_absolute_y(uint16_t addr);
void cpu_sbc_indirect_x(uint8_t addr);
void cpu_sbc_indirect_y(uint8_t addr);


/******************************* INC *******************************
    Increment memory (flags: N, Z)
    INC $aa         $E6     2   cpu_inc_zero_page
    INC $aa,X       $F6     2   cpu_inc_zero_page_x
    INC $aaaa       $EE     3   cpu_inc_absolute
    INC $aaaa,X     $FE     3   cpu_inc_absolute_x
*/
void cpu_inc_zero_page(uint8_t addr);
void cpu_inc_zero_page_x(uint8_t addr);
void cpu_inc_absolute(uint16_t addr);
void cpu_inc_absolute_x(uint16_t addr);


/******************************* DEC *******************************
    Decrement memory (flags: N, Z)
    DEC $aa         $C6     2   cpu_dec_zero_page
    DEC $aa,X       $D6     2   cpu_dec_zero_page_x
    DEC $aaaa       $CE     3   cpu_dec_absolute
    DEC $aaaa,X     $DE     3   cpu_dec_absolute_x
*/
void cpu_dec_zero_page(uint8_t addr);
void cpu_dec_zero_page_x(uint8_t addr);
void cpu_dec_absolute(uint16_t addr);
void cpu_dec_absolute_x(uint16_t addr);


/************************ INX, INY, DEX, DEY ***********************
    Increment and Decrement (Registers) Group (flags: N, Z)
    INX     $E8     1   cpu_inx      Increment X register (reg_x)
    INY     $C8     1   cpu_iny      Increment Y register (reg_y)
    DEX     $CA     1   cpu_dex      Decrement X register (reg_x)
    DEY     $88     1   cpu_dey      Decrement Y register (reg_y)
*/
void cpu_inx();
void cpu_iny();
void cpu_dex();
void cpu_dey();


/************************ TAX, TAY, TXA, TYA ***********************
    Register Transfer Group (flags: N, Z)
    TAX     $AA     1   cpu_tax      Transfer accumulator to X
    TAY     $A8     1   cpu_tay      Transfer accumulator to Y
    TXA     $8A     1   cpu_txa      Transfer X to accumulator
    TYA     $98     1   cpu_tya      Transfer Y to accumulator
*/
void cpu_tax();
void cpu_tay();
void cpu_txa();
void cpu_tya();


/******************************* AND *******************************
    Logical AND (flags: N, Z)
    AND #$aa        $29     2   cpu_and_immediate
    AND $aa         $25     2   cpu_and_zero_page
    AND $aa,X       $35     2   cpu_and_zero_page_x
    AND $aaaa       $2D     3   cpu_and_absolute
    AND $aaaa,X     $3D     3   cpu_and_absolute_x
    AND $aaaa,Y     $39     3   cpu_and_absolute_y
    AND ($aa,X)     $21     2   cpu_and_indirect_x
    AND ($aa),Y     $31     2   cpu_and_indirect_y
*/
void cpu_and_immediate(uint8_t operand);
void cpu_and_zero_page(uint8_t addr);
void cpu_and_zero_page_x(uint8_t addr);
void cpu_and_absolute(uint16_t addr);
void cpu_and_absolute_x(uint16_t addr);
void cpu_and_absolute_y(uint16_t addr);
void cpu_and_indirect_x(uint8_t addr);
void cpu_and_indirect_y(uint8_t addr);


/******************************* XOR *******************************
    Logical Exclusive OR (flags: N, Z)
    EOR #$aa        $49     2   cpu_eor_immediate
    EOR $aa         $45     2   cpu_eor_zero_page
    EOR $aa,X       $55     2   cpu_eor_zero_page_x
    EOR $aaaa       $4D     3   cpu_eor_absolute
    EOR $aaaa,X     $5D     3   cpu_eor_absolute_x
    EOR $aaaa,Y     $59     3   cpu_eor_absolute_y
    EOR ($aa,X)     $41     2   cpu_eor_indirect_x
    EOR ($aa),Y     $51     2   cpu_eor_indirect_y
*/
void cpu_eor_immediate(uint8_t operand);
void cpu_eor_zero_page(uint8_t addr);
void cpu_eor_zero_page_x(uint8_t addr);
void cpu_eor_absolute(uint16_t addr);
void cpu_eor_absolute_x(uint16_t addr);
void cpu_eor_absolute_y(uint16_t addr);
void cpu_eor_indirect_x(uint8_t addr);
void cpu_eor_indirect_y(uint8_t addr);


/******************************* ORA *******************************
    Logical Inclusive OR (flags: N, Z)
    ORA #$aa        $09     2   cpu_ora_immediate
    ORA $aa         $05     2   cpu_ora_zero_page
    ORA $aa,X       $15     2   cpu_ora_zero_page_x
    ORA $aaaa       $0D     3   cpu_ora_absolute
    ORA $aaaa,X     $1D     3   cpu_ora_absolute_x
    ORA $aaaa,Y     $19     3   cpu_ora_absolute_y
    ORA ($aa,X)     $01     2   cpu_ora_indirect_x
    ORA ($aa),Y     $11     2   cpu_ora_indirect_y
*/
void cpu_ora_immediate(uint8_t operand);
void cpu_ora_zero_page(uint8_t addr);
void cpu_ora_zero_page_x(uint8_t addr);
void cpu_ora_absolute(uint16_t addr);
void cpu_ora_absolute_x(uint16_t addr);
void cpu_ora_absolute_y(uint16_t addr);
void cpu_ora_indirect_x(uint8_t addr);
void cpu_ora_indirect_y(uint8_t addr);


/******************************* CMP *******************************
    Compare
    CMP #$aa        $C9     2   cpu_cmp_immediate
    CMP $aa         $C5     2   cpu_cmp_zero_page
    CMP $aa,X       $D5     2   cpu_cmp_zero_page_x
    CMP $aaaa       $CD     3   cpu_cmp_absolute
    CMP $aaaa,X     $DD     3   cpu_cmp_absolute_x
    CMP $aaaa,Y     $D9     3   cpu_cmp_absolute_y
    CMP ($aa,X)     $C1     2   cpu_cmp_indirect_x
    CMP ($aa),Y     $D1     2   cpu_cmp_indirect_y

    Flags:
    Z: set if the accumulator is equal to the compared value;
    N: set to the value of bit 7 of the result of (accumulator - operand);
    C: set if the accumulator is greater than or equal to the compared value;
*/
void cpu_cmp_immediate(uint8_t operand);
void cpu_cmp_zero_page(uint8_t addr);
void cpu_cmp_zero_page_x(uint8_t addr);
void cpu_cmp_absolute(uint16_t addr);
void cpu_cmp_absolute_x(uint16_t addr);
void cpu_cmp_absolute_y(uint16_t addr);
void cpu_cmp_indirect_x(uint8_t addr);
void cpu_cmp_indirect_y(uint8_t addr);


/******************************* CPX *******************************
    Compare X Register (flags: N, Z, C)
    CPX #$aa        $E0     2   cpu_cpx_immediate
    CPX $aa         $E4     2   cpu_cpx_zero_page
    CPX $aaaa       $EC     3   cpu_cpx_absolute
*/
void cpu_cpx_immediate(uint8_t operand);
void cpu_cpx_zero_page(uint8_t addr);
void cpu_cpx_absolute(uint16_t addr);


/******************************* CPY *******************************
    Compare Y Register (flags: N, Z, C)
    CPY #$aa        $C0     2   cpu_cpy_immediate
    CPY $aa         $C4     2   cpu_cpy_zero_page
    CPY $aaaa       $CC     3   cpu_cpy_absolute
*/
void cpu_cpy_immediate(uint8_t operand);
void cpu_cpy_zero_page(uint8_t addr);
void cpu_cpy_absolute(uint16_t addr);


/******************************* BIT *******************************
    Bit Test
    BIT $aa         $24     2   cpu_bit_zero_page
    BIT $aaaa       $2C     3   cpu_bit_absolute

    Flags:
    Z: set if the result of the AND operation is zero (no matching bits);
    N: set to the value of bit 7 of the memory value;
    V: set to the value of bit 6 of the memory value;
*/
void cpu_bit_zero_page(uint8_t addr);
void cpu_bit_absolute(uint16_t addr);


/******************************* ASL *******************************
    Arithmetic Shift Left (flags: N, Z, C)
    ASL A           $0A     1   cpu_asl_accumulator
    ASL $aa         $06     2   cpu_asl_zero_page
    ASL $aa,X       $16     2   cpu_asl_zero_page_x
    ASL $aaaa       $0E     3   cpu_asl_absolute
    ASL $aaaa,X     $1E     3   cpu_asl_absolute_x

    The C flag carries the old bit 7:       C <- [76543210] <- 0
*/
void cpu_asl_accumulator();
void cpu_asl_zero_page(uint8_t addr);
void cpu_asl_zero_page_x(uint8_t addr);
void cpu_asl_absolute(uint16_t addr);
void cpu_asl_absolute_x(uint16_t addr);


/******************************* LSR *******************************
    Logical Shift Right (flags: N, Z, C)
    LSR A           $4A     1   cpu_lsr_accumulator
    LSR $aa         $46     2   cpu_lsr_zero_page
    LSR $aa,X       $56     2   cpu_lsr_zero_page_x
    LSR $aaaa       $4E     3   cpu_lsr_absolute
    LSR $aaaa,X     $5E     3   cpu_lsr_absolute_x

    The C flag carries the old bit 0:       0 -> [76543210] -> C
*/
void cpu_lsr_accumulator();
void cpu_lsr_zero_page(uint8_t addr);
void cpu_lsr_zero_page_x(uint8_t addr);
void cpu_lsr_absolute(uint16_t addr);
void cpu_lsr_absolute_x(uint16_t addr);


/******************************* ROL *******************************
    Rotate Left (flags: N, Z, C)
    ROL A           $2A     1   cpu_rol_accumulator
    ROL $aa         $26     2   cpu_rol_zero_page
    ROL $aa,X       $36     2   cpu_rol_zero_page_x
    ROL $aaaa       $2E     3   cpu_rol_absolute
    ROL $aaaa,X     $3E     3   cpu_rol_absolute_x

    The C flag carries the old bit 7:       C <- [76543210] <- C
*/
void cpu_rol_accumulator();
void cpu_rol_zero_page(uint8_t addr);
void cpu_rol_zero_page_x(uint8_t addr);
void cpu_rol_absolute(uint16_t addr);
void cpu_rol_absolute_x(uint16_t addr);


/******************************* ROR *******************************
    Rotate Right (flags: N, Z, C)
    ROR A           $6A     1   cpu_ror_accumulator
    ROR $aa         $66     2   cpu_ror_zero_page
    ROR $aa,X       $76     2   cpu_ror_zero_page_x
    ROR $aaaa       $6E     3   cpu_ror_absolute
    ROR $aaaa,X     $7E     3   cpu_ror_absolute_x

    The C flag carries the old bit 0:       C -> [76543210] -> C
*/
void cpu_ror_accumulator();
void cpu_ror_zero_page(uint8_t addr);
void cpu_ror_zero_page_x(uint8_t addr);
void cpu_ror_absolute(uint16_t addr);
void cpu_ror_absolute_x(uint16_t addr);


/**************** CLC, CLD, CLI, CLV, SEC, SEC, SEI ****************
    Status Flag Change Group
    CLC     $18     1   cpu_clc      C = 0 -> Clear carry flag
    CLD     $D8     1   cpu_cld      D = 0 -> Clear decimal mode
    CLI     $58     1   cpu_cli      I = 0 -> Clear interrupt disable bit
    CLV     $B8     1   cpu_clv      V = 0 -> Clear overflow flag
    SEC     $38     1   cpu_sec      C = 1 -> Set carry flag
    SED     $F8     1   cpu_sed      D = 1 -> Set decimal mode
    SEI     $78     1   cpu_sei      I = 1 -> Set interrupt disable status
*/
void cpu_clc();
void cpu_cld();
void cpu_cli();
void cpu_clv();
void cpu_sec();
void cpu_sed();
void cpu_sei();


#endif /* CPU_H */

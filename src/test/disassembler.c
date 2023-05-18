#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "disassembler.h"
#include "../libretro/libretro.h"
#include "../cpu.h"

extern retro_log_printf_t log_cb;

void disassemble() {
    log_cb(RETRO_LOG_INFO, "DISASSEMBLING PRG ROM:\n");
    uint16_t base_addr = CPU_CARTRIDGE_ADDR_START + 16;
    uint8_t *prg_rom = &mem[base_addr];
    for(int i=0; i < pgr_rom_size; i++) {
        uint8_t opcode = prg_rom[i];
        CPUInstruction inst = cpu_instruction_lut[opcode];
        char buf[128] = {" "};

        if (inst.numBytes == 0) {
            log_cb(RETRO_LOG_INFO, "[$%04X]: WRONG INSTRUCTION IN THIS MEMORY ADDRESS\n", base_addr+i);
            break;
        }

        sprintf(buf, "[$%04X]: $%02X - %s", base_addr+i, opcode, inst.mnemonic);
        if (inst.numBytes > 1) {
            sprintf(&buf[18], " 0x%02X", prg_rom[++i]);
        }
        if (inst.numBytes > 2) {
            sprintf(&buf[23], " 0x%02X", prg_rom[++i]);
        }

        for(int j = strlen(buf); j < 31; j++) {
            buf[j] = ' ';
        }
        sprintf(&buf[31], strcmp(inst.addr_mode, "") ? "[%s]" : "", inst.addr_mode);

        log_cb(RETRO_LOG_INFO, "%s\n", buf);
    }
}


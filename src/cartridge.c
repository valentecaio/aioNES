#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cartridge.h"
#include "libretro/libretro.h"
#include "RP2A03.h"

extern retro_log_printf_t log_cb;

uint16_t pgr_rom_size, chr_rom_size;
uint8_t expansion_device;
bool mirroring;
bool ppu_timing;

void cartridge_parse_header(const struct retro_game_info *info)
{
   uint8_t buf[1+3*16], *header;
   uint8_t lsb, msb;
   bool is_nes2dot0 = false;

   FILE *ptr = fopen(info->path,"rb");
   log_cb(RETRO_LOG_INFO, "Rom path %s\n", info->path);

   fread(&mem[RP2A03_CARTRIDGE_ADDR_START], RP2A03_CARTRIDGE_SIZE, 1, ptr);
   header = &mem[RP2A03_CARTRIDGE_ADDR_START];

   for(int i=0; i<16; i++){
      sprintf(&buf[i*3], "%02x ", header[i]);
   }
   log_cb(RETRO_LOG_INFO, "header: %s\n", buf);

   log_cb(RETRO_LOG_INFO, "header[4]: %o\n", header[4]);
   log_cb(RETRO_LOG_INFO, "header[9]: %o\n", header[9]);

   // PGR ROM size in 16 KB units
   lsb = header[4];
   msb = header[9] & 0x0F;
   pgr_rom_size = ((msb << 8) | lsb) * 16 * 1024;
   log_cb(RETRO_LOG_INFO, "pgr_rom_size LSB: 0x%02x (%d B)\n", lsb, lsb);
   log_cb(RETRO_LOG_INFO, "pgr_rom_size MSB: 0x%02x (%d B)\n", msb, msb);
   log_cb(RETRO_LOG_INFO, "pgr_rom_size: 0x%02x (%d B)\n", pgr_rom_size, pgr_rom_size);

   // CHR ROM size in 8 KB units
   lsb = header[5];
   msb = (header[9] & 0xF0) >> 4;
   chr_rom_size = ((msb << 8) | lsb) * 8 * 1024;
   log_cb(RETRO_LOG_INFO, "chr_rom_size_LSB: 0x%02x (%d B)\n", lsb, lsb);
   log_cb(RETRO_LOG_INFO, "chr_rom_size_MSB: 0x%02x (%d B)\n", msb, msb);
   log_cb(RETRO_LOG_INFO, "chr_rom_size: 0x%02x (%d B)\n", chr_rom_size, chr_rom_size);

   // 1 for vertical, 0 for horizontal
   mirroring = header[6] & 0b1;
   log_cb(RETRO_LOG_INFO, "mirroring: %s (%d)\n", mirroring ? "vertical" : "horizontal", mirroring);

   // validate file type
   is_nes2dot0 = (header[7] & 0b1100) == 2;
   log_cb(RETRO_LOG_INFO, "is_nes2dot0: %s (%d)\n", is_nes2dot0 ? "true" : "false", is_nes2dot0);

   // 0 for NTSC, 1 for PAL, 2 for multi, 3 for dendy
   ppu_timing = header[12] & 0b11;
   log_cb(RETRO_LOG_INFO, "ppu_timing: %s (%d)\n", ppu_timing ? "PAL" : "NTSC", ppu_timing);

   // 1 for Standard NES controllers
   expansion_device = header[15] & 0b111111;
   log_cb(RETRO_LOG_INFO, "expansion_device: %x\n", expansion_device);
}

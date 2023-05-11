#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libretro/libretro.h"
#include "cartridge.h"

extern retro_log_printf_t log_cb;

void cartridge_parse_header(const struct retro_game_info *info)
{
   unsigned char buf[1+3*16], header[16], mirroring, ppu_timing;
   bool is_nes2dot0 = false;
   int pgr_rom_size, chr_rom_size, expansion_device;
   FILE *ptr = fopen(info->path,"rb");
   fread(header, sizeof(header), 1, ptr);

   log_cb(RETRO_LOG_INFO, "Rom size: %lu\n", info->size);
   log_cb(RETRO_LOG_INFO, "Rom path %s\n", info->path);

   for(int i=0; i<16; i++){
      sprintf(&buf[i*3], "%02x ", header[i]);
   }
   log_cb(RETRO_LOG_INFO, "header: %s\n", buf);

   // PGR ROM size in 16 KB units
   pgr_rom_size = header[4]*16*1024;
   log_cb(RETRO_LOG_INFO, "pgr_rom_size: %d\n", pgr_rom_size);

   // CHR ROM size in 8 KB units
   chr_rom_size = header[5]*8*1024;
   log_cb(RETRO_LOG_INFO, "chr_rom_size: %d\n", chr_rom_size);

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

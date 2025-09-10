#include <zephyr/linker/sections.h>
#include <zephyr/devicetree.h>

#include <zephyr/linker/linker-defs.h>
#include <zephyr/linker/linker-tool.h>

/** enable CONFIG_SRAM2 or any other partition in soc Kconfig,
 * this is just an example to show relocation of code/data/bss script
 */

  #define _LocalCodeRAM_DATA_SECTION_NAME  .localcoderam_data
  #define _LocalCodeRAM_BSS_SECTION_NAME   .localcoderam_bss
  #define _LocalCodeRAM_TEXT_SECTION_NAME  .localcoderam_text


MEMORY
{
  LocalCodeRAM(wx) : ORIGIN = 0x2003fe00, LENGTH = 0x3fe00
}

#include <zephyr/arch/arm/cortex_m/scripts/linker.ld>
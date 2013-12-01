/*
 * GENERATED FILE - DO NOT EDIT
 * (C) Code Red Technologies Ltd, 2008-2013
 * Linker script memory definitions
 * Created from LinkMemoryTemplate
 * By LPCXpresso v5.2.4 [Build 2122] [2013-04-29]  on Thu Oct 31 19:51:55 CET 2013)
*/

MEMORY
{
  /* Define each memory region */
  MFlash128 (rx) : ORIGIN = 0x0, LENGTH = 0x20000 /* 128k */
  RamLoc16 (rwx) : ORIGIN = 0x10000000, LENGTH = 0x4000 /* 16k */
  RamAHB16 (rwx) : ORIGIN = 0x2007c000, LENGTH = 0x4000 /* 16k */

}
  /* Define a symbol for the top of each memory region */
  __top_MFlash128 = 0x0 + 0x20000;
  __top_RamLoc16 = 0x10000000 + 0x4000;
  __top_RamAHB16 = 0x2007c000 + 0x4000;


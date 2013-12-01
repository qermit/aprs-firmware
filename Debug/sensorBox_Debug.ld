/*
 * GENERATED FILE - DO NOT EDIT
 * (C) Code Red Technologies Ltd, 2008-2013
 * Generated linker script file for LPC1754
 * Created from generic_c.ld (vLPCXpresso v5.2 (4 [Build 2122] [2013-04-29] ))
 * By LPCXpresso v5.2.4 [Build 2122] [2013-04-29]  on Thu Oct 31 19:51:55 CET 2013
 */


INCLUDE "sensorBox_Debug_lib.ld"
INCLUDE "sensorBox_Debug_mem.ld"

ENTRY(ResetISR)

SECTIONS
{

    /* MAIN TEXT SECTION */    
    .text : ALIGN(4)
    {
        FILL(0xff)
        __vectors_start__ = ABSOLUTE(.) ;
        KEEP(*(.isr_vector))
        
        /* Global Section Table */
        . = ALIGN(4) ;
        __section_table_start = .;
        __data_section_table = .;
        LONG(LOADADDR(.data));
        LONG(    ADDR(.data)) ;
        LONG(  SIZEOF(.data));
        LONG(LOADADDR(.data_RAM2));
        LONG(    ADDR(.data_RAM2)) ;
        LONG(  SIZEOF(.data_RAM2));
        __data_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        LONG(    ADDR(.bss_RAM2));
        LONG(  SIZEOF(.bss_RAM2));
        __bss_section_table_end = .;
        __section_table_end = . ;
        /* End of Global Section Table */
        

        *(.after_vectors*)
        
    } >MFlash128
    
    .text : ALIGN(4)    
    {
         *(.text*)
        *(.rodata .rodata.*)
        . = ALIGN(4);
        
    } > MFlash128

    /*
     * for exception handling/unwind - some Newlib functions (in common
     * with C++ and STDC++) use this. 
     */
    .ARM.extab : ALIGN(4)
    {
    	*(.ARM.extab* .gnu.linkonce.armextab.*)
    } > MFlash128
    __exidx_start = .;
    
    .ARM.exidx : ALIGN(4)
    {
    	*(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > MFlash128
    __exidx_end = .;
    
    _etext = .;
        
    
    /* DATA section for RamAHB16 */
    .data_RAM2 : ALIGN(4)
    {
       FILL(0xff)
    	*(.data.$RAM2*)
    	*(.data.$RamAHB16*)
       . = ALIGN(4) ;
    } > RamAHB16 AT>MFlash128
    
    /* MAIN DATA SECTION */
    

    .uninit_RESERVED : ALIGN(4)
    {
        KEEP(*(.bss.$RESERVED*))
        . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > RamLoc16

	
	/* Main DATA section (RamLoc16) */
	.data : ALIGN(4)
	{
	   FILL(0xff)
	   _data = . ;
	   *(vtable)
	   *(.data*)
	   . = ALIGN(4) ;
	   _edata = . ;
	} > RamLoc16 AT>MFlash128

    /* BSS section for RamAHB16 */
    .bss_RAM2 : ALIGN(4)
    {
    	*(.bss.$RAM2*)
    	*(.bss.$RamAHB16*)
       . = ALIGN(4) ;
    } > RamAHB16

    /* MAIN BSS SECTION */
    .bss : ALIGN(4)
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        _ebss = .;
        PROVIDE(end = .);
    } > RamLoc16
        
    /* NOINIT section for RamAHB16 */
    .noinit_RAM2 (NOLOAD) : ALIGN(4)
    {
    	*(.noinit.$RAM2*)
    	*(.noinit.$RamAHB16*)
       . = ALIGN(4) ;
    } > RamAHB16 
    
    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD): ALIGN(4)
    {
        _noinit = .;
        *(.noinit*) 
         . = ALIGN(4) ;
        _end_noinit = .;
    } > RamLoc16
    
    PROVIDE(_pvHeapStart = .);
    PROVIDE(_vStackTop = __top_RamLoc16 - 0);
}

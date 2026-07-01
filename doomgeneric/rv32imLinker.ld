MEMORY
{
  FLASH (rwx) : ORIGIN = 0x00000000, LENGTH = 48M
  RAM (rwx) : ORIGIN = 0x3000000, LENGTH = 16M 
}

SECTIONS
{
  .text :
  {
    *(.entry)
    *(.timeLower) /* 0x10 */
    *(.timeUpper) /* 0x10 */
    *(.text)
    *(.text.*)
    *(.rodata)
    *(.rodata.*)
    . = ALIGN(4);
    *(.wad)
  } >FLASH

  .data :
  {
    *(.screen) /* 0x18 */
    *(.data)
    . = ALIGN(4);
  } >FLASH

  .bss :
  {
    *(.bss)
    . = ALIGN(4);
  } >RAM
}

.set BOOT3_MEM_LOCATION,    0x8200     
.set BOOT3_SEC,             0x5
.set NUM_BOOT3_SEC,         0x02                    # read 2 secs

# start address should be 0:7e00, in protected mode, the beginning address of the second bootloader
.section .text
.extern SetPageTable
.globl _start
_start:
.code32    
	# Set up the PageTable for Long Mode
	call SetPageTable

    # Entering bootloader-3
    movl $BOOT3_MEM_LOCATION, %eax
    jmp %eax
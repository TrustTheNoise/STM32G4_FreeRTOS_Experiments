.syntax unified 	@ Use the unified assembly syntax (for ARM and Thumb instructions)
.cpu cortex-m4 		@ Specify the target CPU as Cortex-M4

.text				@ Start of the code section

.global while_delay_asm				@ Make the function globally accessible for linking
.type while_delay_asm, %function	@ Define 'while_delay_asm' as a function

@ Relative jump is used, so if the function expands, branch lines needs to be updated.
while_delay_asm:
@ r0 is function parameter of counts for delay
	movs	r3, #0				@ r3-counter

	cmp	r3, r0					@ checks if counts<counts_for_delay
	bcs.n while_delay_asm+0x0a	@ [relative] if counts>counts_for_delay branch to exit

	adds r3, #1					@ if not add 1 to counts

	b.n while_delay_asm+0x02	@ [relative] loop

	bx lr						@ exit from function

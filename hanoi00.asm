loadi	r4,10000
jmp main
hanoi:
	push	 r5
	loadr	 r5, r4
	addi	 r5, 1
	addi	r4, 1

	load	r0, 0
	store	 r0, 65534
	loadi	r0, 1
	load	 r1, 65534
	cmpr	r1, r0
	jnz	elseLabel
ifLabel:
	load	r0,-3(BR)
	writed r0
	loadi	r1, ' '
	writec	r1
	load	r0,-4(BR)
	writed r0
	writec	r1
	load	r0,-5(BR)
	writed r0
	writec	r1
	load	r0,-6(BR)
	writed r0
	loadi	 r0, '\n'
	writec	r0
	jmp	ifExitLabel
elseLabel:
;	loadi	r0, 1
;	push	r0
;	loadi	r0, 3
;	push	r0
;	loadi	r0, 2
;	push	r0
;	loadi	r0, 1
;	push	r0
;	call	 hanoi
;	subi	 r4, 4
;	load	r0,-3(BR)
;	writed r0
;	loadi	r1, ' '
;	writec	r1
;	load	r0,-4(BR)
;	writed r0
;	loadi	 r0, '\n'
;	writec	r0
ifExitLabel:
	subi	r4, 1
	pop	 r5
	writed r5
	loadi	 r0, '\n'
	writec	r0
	return
main:
loadi	r0, 1
store	r0, 0
load	r0, 0
push	r0
loadi	r0, 3
push	r0
loadi	r0, 2
push	r0
loadi	r0, 1
push	r0
call	 hanoi
subi	 r4, 4
halt

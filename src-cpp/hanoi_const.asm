loadi	r4, 10000
jmp	main
hanoi:
push	r5
loadr	r5, r4
addi	r5, 1
addi	r4, 1
load	r0, -6(BR)
loadr	r3, r0
loadi	r0, 1
cmpr	r3, r0
jnz	elseLabel0
ifLabel0:
load	r0, -3(BR)
writed	r0
loadi	r1, ' '
writec	r1
load	r0, -4(BR)
writed	r0
loadi	r0, '\n'
writec	r0
jmp	ifExitLabel0
elseLabel0:
load	r0, -6(BR)
subi	r0, 1
push	r0
load	r0, -4(BR)
push	r0
load	r0, -5(BR)
push	r0
load	r0, -3(BR)
push	r0
call	hanoi
subi	r4, 4
load	r0, -3(BR)
writed	r0
loadi	r1, ' '
writec	r1
load	r0, -4(BR)
writed	r0
loadi	r0, '\n'
writec	r0
load	r0, -6(BR)
subi	r0, 1
push	r0
load	r0, -3(BR)
push	r0
load	r0, -4(BR)
push	r0
load	r0, -5(BR)
push	r0
call	hanoi
subi	r4, 4
ifExitLabel0:
subi	r4, 1
pop	r5
return
main:
loadi	r0, 5
push	r0
loadi	r0, 3
push	r0
loadi	r0, 2
push	r0
loadi	r0, 1
push	r0
call	hanoi
subi	r4, 4
halt

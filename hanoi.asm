jmp main
loadi	r4,10000
hanoi
load	r0, 0
store	 r0, 65534
loadi	r0, 1
load	 r1, 65534
cmpr	r1, r0
jnz	elseLabel
ifLabel:loadi	r0, ' '
writec	r0
loadi	 r0, '\n'
writec	r0
jmp	ifExitLabel
elseLabel:
load	r0, -1
pop	r0
load	r0, -1
pop	r0
load	r0, -1
pop	r0
load	r0, 0
subi	r0,1
store	r0, 1
load	r0, 1
store	r0, 2
load	r0, 2
pop	r0
loadi	r0, ' '
writec	r0
loadi	 r0, '\n'
writec	r0
load	r0, -1
pop	r0
load	r0, -1
pop	r0
load	r0, -1
pop	r0
load	r0, 0
subi	r0,1
store	r0, 1
load	r0, 1
store	r0, 2
load	r0, 2
pop	r0
ifExitLabel:
main:
loadi	r0, 5
store	r0, 0
loadi	r0, 1
pop	r0
loadi	r0, 2
pop	r0
loadi	r0, 3
pop	r0
load	r0, 0
pop	r0
halt

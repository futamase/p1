loadi	r4,10000
jmp main
main:
loadi	r0, 10
store	r0, 0
load	r0, 0
addi	r0, 5
muli	r0, 2
div	r0, 0
store	r0, 1
load	r0,0
writed r0
loadi	r1, ' '
writec	r1
load	r0,1
writed r0
loadi	 r0, '\n'
writec	r0
halt

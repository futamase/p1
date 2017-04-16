loadi	r0, 0
store	r0, 0
loadi	r0, 0
store	r0, 1
whileLabel:
load	r0, 0
store	 r0, 65534
loadi	r0, 10
load	 r1, 65534
cmpr	r1, r0
jgt	whileEndLabel
load	r0, 1
addi	r0,1
store	r0, 1
load	r0, 0
addi	r0,1
store	r0, 0
jmp	 whileLabel
whileEndLabel:
load	r0,1
writed r0
loadi	 r0, '\n'
writec	r0

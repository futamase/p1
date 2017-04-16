loadi	r0, 100
store	r0, 0
loadi	r0, 49
store	r0, 1
load	r0, 1
sub	r0,0
store	 r0, 65534
loadi	r0, 50
load	 r1, 65534
cmpr	r1, r0
jlt	elseLabel
ifLabel:loadi	r0, 2
mul	r0,0
store	r0, 2
jmp	ifExitLabel
elseLabel:
loadi	r0, 3
mul	r0,0
store	r0, 2
ifExitLabel:
load	r0,0
writed r0
loadi	r0, ' '
writec	r0
load	r0,1
writed r0
loadi	r0, ' '
writec	r0
load	r0,2
writed r0
loadi	 r0, '\n'
writec	r0

loadi	r4,10000
jmp main
main:
loadi	r0, 0
store	r0, 0
loadi	r0, 0
store	r0, 5
loadi	r0, 3827
store	r0, 1
loadi	r0, 8358
store	r0, 2
loadi	r0, 3797
store	r0, 3
whileLabel:
load	r0, 0
store	 r0, 65534
loadi	r0, 1000
load	 r1, 65534
cmpr	r1, r0
jge	whileEndLabel
load	r0, 1
add	r0,2
store	r0, 8
load	r0, 8
mul	r0,3
store	r0, 9
load	r0, 1
add	r0,2
store	r0, 10
load	r0, 10
mul	r0,3
store	r0, 11
load	r0, 11

loadi	r4,10000
jmp main
main:
loadi	r0, 18
store	r0, 0
loadi	r0, 30
store	r0, 1
loadi	r0, 1
add	r0,0
store	r0, 3
load	r0, 3
muli	r0,2
store	r0, 4
loadi	r0, 10
add	r0,1
store	r0, 5
loadi	r0, 3
mul	r0,5
store	r0, 6
load	r0, 4
mul	r0,6
store	r0, 7
loadi	r0, 1
add	r0,7
store	r0, 8
load	r0, 8
store	r0, 2
load	r0,0
writed r0
loadi	r1, ' '
writec	r1
load	r0,1
writed r0
writec	r1
load	r0,2
writed r0
loadi	 r0, '\n'
writec	r0
halt

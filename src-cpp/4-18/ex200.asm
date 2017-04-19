loadi	r4, 10000
jmp	main
main:
loadi	r0, 5
store	r0, 0
loadi	r0, 6
store	r0, 1
load	r0, 0
add	r0, 1
muli	r0, 2
load	r2, 0
add	r2, 1
muli	r2, 2
addr	r0, r2
store	r0, 2
writed	r0
loadi	r0, '\n'
writec	r0
halt

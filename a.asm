loadi	r4,10000
jmp main
hoge:
loadi	r0, 10
store	r0, 0
load	r0,0(BR)
writed r0
loadi	 r0, '\n'
writec	r0
hoge2:
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
main:
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

loadi	r4, 10000
jmp	main
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
whileLabel0:
load	r0, 0
loadr	r3, r0
loadi	r0, 1000
cmpr	r3, r0
jge	whileEndLabel0
load	r0, 1
add	r0, 2
mul	r0, 3
loadr	r2, r0
div	r2, label0
mul	r2, label1
subr	r0, r2
store	r0, 1
divi	r0, 10
store	r0, 1
divi	r0, 10
store	r0, 6
load	r0, 1
add	r0, 2
mul	r0, 3
loadr	r2, r0
div	r2, label2
mul	r2, label3
subr	r0, r2
store	r0, 1
divi	r0, 10
store	r0, 1
divi	r0, 10
store	r0, 7
load	r0, 6
mul	r0, 6
load	r1, 7
mul	r1, 7
addr	r0, r1
loadr	r3, r0
load	r0, label4
cmpr	r3, r0
jge	elseLabel0
ifLabel0:
load	r0, 5
addi	r0, 1
store	r0, 5
jmp	ifExitLabel0
elseLabel0:
ifExitLabel0:
load	r0, 0
addi	r0, 1
store	r0, 0
jmp	whileLabel0
whileEndLabel0:
load	r0, 5
muli	r0, 4
store	r0, 4
writed	r0
loadi	r0, '\n'
writec	r0
halt
label0: data 100000
label1: data 100000
label2: data 100000
label3: data 100000
label4: data 1000000

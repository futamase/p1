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
store	r0, 65534
loadi	r0, 1000
load	r1, 65534
cmpr	r1, r0
jge	whileEndLabel
load	r0, 1
add	    r0,2
store	r0, 8
load	r0, 8
mul	    r0,3
store	r0, 9
load	r0, 1
add	    r0,2
store	r0, 10
load	r0, 10
mul	    r0,3
store	r0, 11
load	r0, 11
div	    r0, label0
store	r0, 12
load	r0, 12
mul	    r0, label1
store	r0, 13
load	r0, 9
sub	    r0,13
store	r0, 14
load	r0, 14
store	r0, 15
load	r0, 15
store	r0, 1
load	r0, 1
divi	r0,10
store	r0, 8
load	r0, 8
store	r0, 9
load	r0, 9
store	r0, 1
load	r0, 1
divi	r0,10
store	r0, 8
load	r0, 8
store	r0, 9
load	r0, 9
store	r0, 6
load	r0, 1
add	    r0,2
store	r0, 8
load	r0, 8
mul	    r0,3
store	r0, 9
load	r0, 1
add	    r0,2
store	r0, 10
load	r0, 10
mul	    r0,3
store	r0, 11
load	r0, 11
div	    r0, label2
store	r0, 12
load	r0, 12
mul	    r0, label3
store	r0, 13
load	r0, 9
sub	    r0,13
store	r0, 14
load	r0, 14
store	r0, 15
load	r0, 15
store	r0, 1
load	r0, 1
divi	r0,10
store	r0, 8
load	r0, 8
store	r0, 9
load	r0, 9
store	r0, 1
load	r0, 1
divi	r0,10
store	r0, 8
load	r0, 8
store	r0, 9
load	r0, 9
store	r0, 7
load	r0, 6
mul	    r0,6
store	r0, 8
load	r0, 7
mul	    r0,7
store	r0, 9
load	r0, 8
add	    r0,9
store	r0, 10
load	r0, 10
store	r0, 11
load	r0, 11
store	r0, 65534
load	r0, label4
load	r1, 65534
cmpr	r1, r0
jge	elseLabel
ifLabel:load	r0, 5
addi	r0,1
store	r0, 8
load	r0, 8
store	r0, 9
load	r0, 9
store	r0, 5
jmp	ifExitLabel
elseLabel:
ifExitLabel:
load	r0, 0
addi	r0,1
store	r0, 8
load	r0, 8
store	r0, 9
load	r0, 9
store	r0, 0
jmp	 whileLabel
whileEndLabel:
load	r0, 5
muli	r0,4
store	r0, 8
load	r0, 8
store	r0, 9
load	r0, 9
store	r0, 4
load	r0,4
writed  r0
loadi	r0, '\n'
writec	r0
halt

label0: data 100000
label1: data 100000
label2: data 100000
label3: data 100000
label4: data 1000000

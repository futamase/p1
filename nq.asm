loadi	r4,1
jmp main
putqueen:
  push	 r5
  loadr	 r5, r4
  addi	 r5, 1
  addi	r4, 7
  loadi	r0, 1
  store	r0, 0(BR)

whileLabel0:
  ;cond>>
    load	r0, 0(BR)
    store	 r0, 65534
    load	r0, -5(BR)
    load	 r1, 65534
    cmpr	r1, r0
  ;cond<<
    jgt	whileEndLabel0
    load	r0, -3(BR)
    store	r0, 4(BR)
    loadi	r0, 0
    store	r0, 5(BR)
    load	r0, -4(BR)
    subi	r0, 1
    store	r0, 1(BR)
    loadi	r0, 0
    store	r0, 6(BR)

      whileLabel1:
      ;cond
        load	r0, 1(BR)
        store	 r0, 65534
        loadi	r0, 0
        load	 r1, 65534
        cmpr	r1, r0
      ;cond
        jle	whileEndLabel1
        load	r0, 4(BR)
        divi	r0, 10
        store	r0, 5(BR)
        load	r0, 5(BR)
        muli	r0, 10
        store	r0, 5(BR)
        load	r0, 4(BR)
        sub	r0, 5(BR)
        store	r0, 3(BR)

        ;checkrow = i >>>>
        load	r0, 3(BR)
        store	 r0, 65534
        load	r0, 0(BR)
        load	 r1, 65534
        cmpr	r1, r0
        jnz	elseLabel0
        ;checkrow = i <<<<
      ifLabel0:
        loadi	r0, 1
        store	r0, 6(BR)
        jmp	ifExitLabel0
      elseLabel0:
      ; checkrow = i + (column-j)>>>
        load	r0, 3(BR)
        store	 r0, 65534
        load	r0, -4(BR)
        sub	r0, 1(BR)
        add	r0, 0(BR)
        load	 r1, 65534
        cmpr	r1, r0
        jnz	elseLabel1
      ; checkrow = i + (column-j)<<<<<
      ifLabel1:
        loadi	r0, 1
        store	r0, 6(BR)
        jmp	ifExitLabel1
      elseLabel1:

      ; checkrow = i - (column-j)>>>>>
        load	r0, 3(BR)
        store	 r0, 65534
        load	r0, -4(BR)
        sub	r0, 1(BR)
        load r2, 0(BR)
        subr	r2, r0
        load	 r1, 65534
        cmpr	r1, r2
        jnz	elseLabel2
      ; checkrow = i - (column-j)>>>>>
      ifLabel2:
        loadi	r0, 1
        store	r0, 6(BR)
        jmp	ifExitLabel2
      elseLabel2:
      ifExitLabel2:
      ifExitLabel1:
      ifExitLabel0:
        load	r0, 4(BR)
        divi	r0, 10
        store	r0, 4(BR)
        load	r0, 1(BR)
        subi	r0, 1
        store	r0, 1(BR)
        jmp	 whileLabel1
      whileEndLabel1:

    ; check = 0
    load	r0, 6(BR)
    store	 r0, 65534
    loadi	r0, 0
    load	 r1, 65534
    cmpr	r1, r0
    jnz	elseLabel3

  ifLabel3:
    load	r0, -3(BR)
    muli	r0, 10
    add	r0, 0(BR)
    store	r0, 2(BR)
    load	r0, -4(BR)
    store	 r0, 65534
    load	r0, -5(BR)
    load	 r1, 65534
    cmpr	r1, r0
    jnz	elseLabel4

  ifLabel4:
    load	r0,2(BR)
    writed r0
    loadi	 r0, '\n'
    writec	r0
    load	r0, 1
    addi	r0, 1
    store	r0, 1
    jmp	ifExitLabel4

  elseLabel4:
    load	r0, -5(BR)
    push	r0
    load	r0, -4(BR)
    addi	r0, 1
    push	r0
    load	r0, 2(BR)
    push	r0
    call	 putqueen
    subi	 r4, 3
  ifExitLabel4:
    jmp	ifExitLabel3
  elseLabel3:
  ifExitLabel3:
    load	r0, 0(BR)
    addi	r0, 1
    store	r0, 0(BR)
    jmp	 whileLabel0

whileEndLabel0:
  subi	r4, 7
  pop	 r5
  return
main:
  loadi	r0, 8
  store	r0, 0
  loadi	r0, 0
  store	r0, 1
  load	r0, 0
  push	r0
  loadi	r0, 1
  push	r0
  loadi	r0, 0
  push	r0
  call	 putqueen
  subi	 r4, 3
  load	r0,1
  writed r0
  loadi	 r0, '\n'
  writec	r0
  halt

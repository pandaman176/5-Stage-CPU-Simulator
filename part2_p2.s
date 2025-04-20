.globl _start

_start:
    li a1, 1 
    li a2, 2 
    li a3, 0 
    li a4, 16 
    mul a6, a2, a1
    li x8, 0 
    li x5, 4 
    sw a6, 0x12(zero)
    li x6, 1 
    li x7, 1 
    li a6, 2048
    sw a6, 0x36(zero)
    li a6, 10
    sw a6, 0x20(zero)

loop:
    mul a4, a1, a2 
    addi x8, x6, 0
    lw a5, 0x20(zero) 
    add a3, a3, a4
    add x6, x7, x6
    addi a5, a5, 1
    mul a6, x8, a3
    add x7, zero, x6
    sw a5, 0x20(zero)
    add a6, a6, a3
    addi a1, a1, 1
    lw a5, 0x12(zero)
    addi a2, a2, 1
    addi a5, a5, 1
    sw a5, 0x12(zero)

f3:
    lw x8, 0x36(zero)
    andi x1, x8, 1
    beq x1, zero, f2

f1:
    addi x8, x8, -1
    j end_condition

f2:
    srli x8, x8, 1

end_condition:
    sw x8, 0x36(zero)
    bnez x8, loop

    li a7, 93
    ecall

li t0, 42
li t1,12
addi sp, sp, -8
sd t0, 0(sp)
ld a7, 0(sp)
sd a7, 0(sp)
addi sp, sp, 8
ld a5, -8(sp)
beq a7, a5, 12
li a0, 1
j 8
li a0, 2
addi x0, x0, 15

# EXPECTED
# t0: 42
# t1: 12
# a7: 42
# a5: 42
# a0: 2
# zero: 0
# sp: 16384 

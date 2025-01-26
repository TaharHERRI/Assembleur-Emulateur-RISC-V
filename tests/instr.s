add t0, zero, sp
sub gp, ra, t0
addi t1,t2,-324
sd t0,400(s1)
ld a0,400(a1)
beq a2,a3,8
li s0, 23
bne a4,t0,8
li a5, 21
blt a6,a7,8
li a6, -12
bge s2,s3,8
li a6, 12
jal s4,8
li a5, 2
j 8
li t0, 1
li s5, 200
mv s6,s7
mv s8,s9
mv s10,s11
mv t3,t4
mv t5,t6


# EXPECTED
# sp: 16384
# t0: 16384
# gp: -16384
# t1: -324
# a0: 16384
# a6: -12
# s5: 200
# s4: 56

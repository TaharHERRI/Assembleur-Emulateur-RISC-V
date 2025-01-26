li a1, 1000
li a0, 10

mv t1, a1
mv t0, a0
#loop_creer_tab:
beq t0, zero, 20
sd t0, 0(t1)
addi t0, t0, -1
addi t1, t1, 4
jal ra, -16


# EXPECTED
# t1: 1040
# a1: 1000
# t0: 0
# a0: 10
# zero: 0
# sp: 16384 
# ra: 36

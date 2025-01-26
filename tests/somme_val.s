#main:
addi sp, sp, -16
sd ra, 8(sp)

#       Charger la valeur 1000 dans a1
li a1, 1000
#       Charger la valeur 10 dans a0
li a0, 10

jal ra, 20

jal ra, 64

ld ra, 8(sp)
addi sp, sp, 16
jal ra, 120



#creer_tab:
addi sp, sp, -16
sd ra, 8(sp)

mv t1, a1
mv t0, a0

#loop_creer_tab:
beq t0, zero, 20
sd t0, 0(t1)
addi t0, t0, -1
addi t1, t1, 8
jal ra, -16

#end_loop_creer_tab:
ld ra, 8(sp)
addi sp, sp, 16
jal ra, -60

#somme_elmnts:
addi sp, sp, -16
sd ra, 8(sp)

mv t1, a1
mv t0, a0
#       Initialiser t5 à 0
li t5, 0

ld t4, 0(t1)
add t5, t5, t4
addi t0, t0, -1
#loop_somme_elmnts:
beq t0, x0, 24
addi t1, t1, 8
ld t4, 0(t1)
add t5, t5, t4
addi t0, t0, -1
jal ra, -20

#end_loop_somme_elmnts:
ld ra, 8(sp)
addi sp, sp, 16
jal ra, -124

# EXPECTED
# t5: 55
# t4: 1
# t1: 1072
# a0: 10
# a1: 1000
# zero: 0
# sp: 16384 
# ra: 36
# gp: 0

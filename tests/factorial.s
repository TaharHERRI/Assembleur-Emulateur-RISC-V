# a0 !
li a0, 6

#result
li a1, 1
#val count
li t0, 2 

blt a0, t0, 32
#jump to end

mv t1,a1
addi t2,t0,-1
addi t0, t0, 1

#mul loop
beq t2, x0, -16
#jump to main loop
add a1, a1, t1
addi t2, t2, -1
#end mul
jal ra, -12
#jump to loop

# EXPECTED
# a0: 6
# a1: 720
# t0: 7
# t1: 120
# sp: 16384
# ra: 44

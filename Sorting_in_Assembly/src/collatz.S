.section .text      
.align 2            
.globl collatz     

collatz:
If:
  andi  a1, a0, 1
  bne   a1, zero, Else
Then:
  srai  a0, a0, 1
  jal   zero, End
Else:
  slli  a2, a0, 1
  add   a0, a2, a0
  addi  a0, a0, 1
End:
  jalr  zero, 0(ra)
.section .text     
.align 2 
.globl bubblesort

bubblesort:
    addi a2, x0, 1 # int swapped = 1;
    loop:
        beq a2, x0, end
        addi a2, x0, 0 # swapped = 0;
        
        addi a3, x0, 0 #i=0
        addi a4, a1, -1 #a4 = n-1
        forloop:
            bge  a3, a4, endfor
            
            slli t0, a3, 2 # t0 = i * 4
            add  t1, a0, t0 # t1 = address of p[i]
            
            lw a5, 0(t1) # a5 = p[i]
            lw a6, 4(t1) # a6 = p[i+1]
            
            bge a6, a5, endif
            sw a6, 0(t1) # swap
            sw a5, 4(t1) # swap

            addi sp, sp, -36
            sw a0, 0(sp)
            sw a1, 4(sp)
            sw a2, 8(sp)
            sw a3, 12(sp)
            sw a4, 16(sp)
            sw a5, 20(sp)
            sw a6, 24(sp)
            sw a7, 28(sp)
            sw ra, 32(sp)
            jal ra, arrayViz
            lw a0, 0(sp)
            lw a1, 4(sp)
            lw a2, 8(sp)
            lw a3, 12(sp)
            lw a4, 16(sp)
            lw a5, 20(sp)
            lw a6, 24(sp)
            lw a7, 28(sp)
            lw ra, 32(sp)
            addi sp, sp, 36
  

            addi a2, x0, 1 #swapped = 1
            
            endif:
        
            addi a3, a3, 1 # i++
            blt a3, a4, forloop
            
            endfor:
            addi a1, a1, -1 #n=n-1
        
        bne a2, zero, loop
        
    end:
    jalr x0, 0(ra)



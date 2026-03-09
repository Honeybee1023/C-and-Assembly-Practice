.section .text     
.align 2 
.globl quicksort

partition:
    addi sp, sp, -32
    sw   ra, 0(sp)
    sw   a0, 4(sp)
    sw   a1, 8(sp)
    sw   a2, 12(sp)

    slli t0, a2, 2    
    add  t0, a0, t0      # t0 = adrs of p[end]
    lw   t1, 0(t0)       # t1 = pivot
    addi t2, a1, -1      # t2 = i
    mv   t3, a1          # t3 = j

    partition_loop:
        bge  t3, a2, partition_end_loop # exit loop

        slli t4, t3, 2     
        add  t4, a0, t4      # t4 = adrs of p[j]
        lw   t5, 0(t4)       # t5 = p[j]

        bgt  t5, t1, next_j # if (p[j] <= x)

        addi t2, t2, 1       # i++
        slli t6, t2, 2       
        add  t6, a0, t6      # t6 = adrs of p[i]
        lw   a3, 0(t6)       # a3 = p[i]

        # Swap p[i] and p[j]
        sw   t5, 0(t6)
        sw   a3, 0(t4)

        # ArrayViz Call
        lw   a0, 4(sp)
        sw   t0, 16(sp)
        sw   t1, 20(sp)
        sw   t2, 24(sp)
        sw   t3, 28(sp)
        call arrayViz
        lw   t0, 16(sp)
        lw   t1, 20(sp)
        lw   t2, 24(sp)
        lw   t3, 28(sp)
        lw   a0, 4(sp)
        lw   a1, 8(sp)
        lw   a2, 12(sp)

        next_j:
            addi t3, t3, 1       # j++
            j    partition_loop

partition_end_loop:
    addi t2, t2, 1       # i + 1
    slli t6, t2, 2       
    add  t6, a0, t6      # t6 = adrs of p[i+1]
    lw   a3, 0(t6)       # a3 = p[i+1]
    
    sw   t1, 0(t6)       # p[i+1] = pivot
    sw   a3, 0(t0)       # p[end] = old p[i+1]

    lw   a0, 4(sp)
    sw   t2, 24(sp)
    call arrayViz
    lw   t2, 24(sp)
    lw   a0, 4(sp)
    lw   a1, 8(sp)
    lw   a2, 12(sp)
    
    mv   a0, t2          # return i + 1
    lw   ra, 0(sp)
    addi sp, sp, 32
    ret

quicksort:
    bge     a1, a2, done   

    addi    sp, sp, -20
    sw      ra, 0(sp)
    sw      a0, 4(sp)  
    sw      a1, 8(sp)   
    sw      a2, 12(sp)  

    call    partition   
    mv      t0, a0       # t0 = q
    sw      t0, 16(sp)   # save q


    lw      a0, 4(sp)    # get p
    lw      a1, 8(sp)    # get start
    addi    a2, t0, -1   # a2 = q - 1
    call    quicksort

    lw      a0, 4(sp)    # get p
    lw      t0, 16(sp)   # get q
    addi    a1, t0, 1    # a1 = q + 1
    lw      a2, 12(sp)   # get end
    call    quicksort

    lw      ra, 0(sp)
    addi    sp, sp, 20


    done:
    ret



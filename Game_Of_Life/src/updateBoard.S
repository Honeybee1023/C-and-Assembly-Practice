.section .text     
.align 2   
.globl updateBoard

# updateBoard
# ARGUMENTS a0: screen buffer (current board),
# a1: temporary output buffer (for new board)
# RETURNS: Nothing
updateBoard:

    # TO-DO: Translate the C implementation of updateBoard to assembly.
    # Make sure to follow RISC-V calling convention!
    addi sp, sp, -32
    sw ra, 28(sp)
    sw s0, 24(sp)
    sw s1, 20(sp)
    sw s2, 16(sp)
    sw s3, 12(sp)
    
    mv s2, a0
    mv s3, a1
    
    mv a0, s3
    call eraseBuffer
    
    li s0, 0 # y=0
    loop1: 
        li s1, 0 #x=0
        loop2:
            mv a0, s2
            mv a1, s1
            mv a2, s0
            call checkNeighbors
            mv t2, a0 #t2 is neighbor tally
            
            mv a0, s2
            mv a1, s1
            mv a2, s0
            call getPixel
            mv t3, a0 #t3 is current cell
        
            li t4, 3
            beq t2, t4, life
            li t4, 1
            beq t3, t4, currentcell
            jal x0, end
            currentcell: 
            li t4, 2
            beq t2, t4, life
            jal x0, end
            
            life:
                mv a0, s3      
                mv a1, s1      
                mv a2, s0      
                li a3, 1        
                call setPixel
            
            end:
            addi s1, s1, 1
            li t4, 32
            blt s1, t4, loop2
    
        addi s0, s0, 1
        li t4, 8
        blt s0, t4, loop1
    
    lw ra, 28(sp)
    lw s0, 24(sp)
    lw s1, 20(sp)
    lw s2, 16(sp)
    lw s3, 12(sp)
    addi sp, sp, 32

    ret


# getPixel
# ARGUMENTS a0: screen_buffer, a1: x, a2: y
# RETURNS: 1 if cell occupied, 0 otherwise
getPixel:
    # your code here
    slli a2, a2, 2  # a2: 4 * y for address 
    add a2, a2, a0  # a2: screen_buffer + 4*y 
    lw a3, 0(a2)    # a3: screen_buffer[y]
    srl a3, a3, a1  # a3: screen_buffer[y] >>_l x
    andi a0, a3, 1  # a0: a3 & 1
    ret

# checkNeighbors
# ARGUMENTS a0: game_board, a1: x index, a2: y index
# RETURNS: total occupied cells in the eight surrounding cells of (x,y) (game board wraps in x and y)
checkNeighbors:
    # TODO: Determine left, right, up, and down indices for neighbors.
    addi a3, a1, 1 # l
    andi a3, a3, 31
    addi a4, a1, -1 # r
    andi a4, a4, 31
    addi a5, a2, -1 # u
    andi a5, a5, 7
    addi a6, a2, 1 # d
    andi a6, a6, 7
    
    addi sp, sp, -4
    sw ra, 0(sp)
    
    call tallyNeighbors # tallyNeighbors(game_board, x, y, left, right, up, down)

    # TODO: Return result of tallyNeighbors
    lw ra, 0(sp)
    addi sp, sp, 4
    

    ret


# tallyNeighbors
# ARGUMENTS a0: game_board, a1: current x index, a2: current y index, a3: left neighbor index, 
# a4: right neighbor index, a5: up neighbor index, a6: down neighbor index 
# RETURNS: total occupied cells in the eight surrounding cells of current (x,y)
tallyNeighbors:
    # TODO: This procedure is functionally correct, but doesn't follow
    # RISC-V calling convention.
    # Make this procedure follow calling convention. You may only add
    # instructions that:
    # 1. Increment/decrement the stack pointer
    # 2. Put elements on the stack
    # 3. Take elements off the stack
    addi sp, sp, -44
    sw s0, 28(sp)
    sw s1, 32(sp)
    sw s2, 36(sp)
    sw s3, 40(sp)

    mv s0, a0 # s0: game_board
    mv s1, a1 # a1: x
    mv s2, a2 # a2: y
    li s3, 0  # s3: tally

    mv a1, a4
    mv a2, a5
    
    
    sw a1, 0(sp)
    sw a2, 4(sp)
    sw a3, 8(sp)
    sw a4, 12(sp)
    sw a5, 16(sp)
    sw a6, 20(sp)
    sw ra, 24(sp)


    call getPixel # getPixel(game_board, x-1, y-1)
    lw a1, 0(sp)
    lw a2, 4(sp)
    lw a3, 8(sp)
    lw a4, 12(sp)
    lw a5, 16(sp)
    lw a6, 20(sp)
    lw ra, 24(sp)
    
    add s3, s3, a0

    mv a0, s0
    mv a1, a4
    mv a2, s2

    sw a1, 0(sp)
    sw a2, 4(sp)
    sw a3, 8(sp)
    sw a4, 12(sp)
    sw a5, 16(sp)
    sw a6, 20(sp)
    sw ra, 24(sp)
    call getPixel # getPixel(game_board, x-1, y)
    lw a1, 0(sp)
    lw a2, 4(sp)
    lw a3, 8(sp)
    lw a4, 12(sp)
    lw a5, 16(sp)
    lw a6, 20(sp)
    lw ra, 24(sp)

    add s3, s3, a0

    mv a0, s0
    mv a1, a4
    mv a2, a6

    sw a1, 0(sp)
    sw a2, 4(sp)
    sw a3, 8(sp)
    sw a4, 12(sp)
    sw a5, 16(sp)
    sw a6, 20(sp)
    sw ra, 24(sp)
    call getPixel # getPixel(game_board, x-1, y+1)
    lw a1, 0(sp)
    lw a2, 4(sp)
    lw a3, 8(sp)
    lw a4, 12(sp)
    lw a5, 16(sp)
    lw a6, 20(sp)
    lw ra, 24(sp)

    add s3, s3, a0

    mv a0, s0
    mv a1, s1
    mv a2, a5

    sw a1, 0(sp)
    sw a2, 4(sp)
    sw a3, 8(sp)
    sw a4, 12(sp)
    sw a5, 16(sp)
    sw a6, 20(sp)
    sw ra, 24(sp)
    call getPixel # getPixel(game_board, x, y-1)
    lw a1, 0(sp)
    lw a2, 4(sp)
    lw a3, 8(sp)
    lw a4, 12(sp)
    lw a5, 16(sp)
    lw a6, 20(sp)
    lw ra, 24(sp)

    add s3, s3, a0

    mv a0, s0
    mv a1, s1
    mv a2, a6

    sw a1, 0(sp)
    sw a2, 4(sp)
    sw a3, 8(sp)
    sw a4, 12(sp)
    sw a5, 16(sp)
    sw a6, 20(sp)
    sw ra, 24(sp)
    call getPixel # getPixel(game_board, x, y+1)
    lw a1, 0(sp)
    lw a2, 4(sp)
    lw a3, 8(sp)
    lw a4, 12(sp)
    lw a5, 16(sp)
    lw a6, 20(sp)
    lw ra, 24(sp)

    add s3, s3, a0

    mv a0, s0
    mv a1, a3
    mv a2, a5

    sw a1, 0(sp)
    sw a2, 4(sp)
    sw a3, 8(sp)
    sw a4, 12(sp)
    sw a5, 16(sp)
    sw a6, 20(sp)
    sw ra, 24(sp)
    call getPixel # getPixel(game_board, x+1, y-1)
    lw a1, 0(sp)
    lw a2, 4(sp)
    lw a3, 8(sp)
    lw a4, 12(sp)
    lw a5, 16(sp)
    lw a6, 20(sp)
    lw ra, 24(sp)

    add s3, s3, a0

    mv a0, s0
    mv a1, a3
    mv a2, s2

    sw a1, 0(sp)
    sw a2, 4(sp)
    sw a3, 8(sp)
    sw a4, 12(sp)
    sw a5, 16(sp)
    sw a6, 20(sp)
    sw ra, 24(sp)
    call getPixel # getPixel(game_board, x+1, y)
    lw a1, 0(sp)
    lw a2, 4(sp)
    lw a3, 8(sp)
    lw a4, 12(sp)
    lw a5, 16(sp)
    lw a6, 20(sp)
    lw ra, 24(sp)

    add s3, s3, a0

    mv a0, s0
    mv a1, a3
    mv a2, a6

    sw a1, 0(sp)
    sw a2, 4(sp)
    sw a3, 8(sp)
    sw a4, 12(sp)
    sw a5, 16(sp)
    sw a6, 20(sp)
    sw ra, 24(sp)
    call getPixel # getPixel(game_board, x+1, y+1)
    lw a1, 0(sp)
    lw a2, 4(sp)
    lw a3, 8(sp)
    lw a4, 12(sp)
    lw a5, 16(sp)
    lw a6, 20(sp)
    lw ra, 24(sp)
    

    add s3, s3, a0

    mv a0, s3
    
    lw s0, 28(sp)
    lw s1, 32(sp)
    lw s2, 36(sp)
    lw s3, 40(sp)
    
    addi sp, sp, 44

    ret

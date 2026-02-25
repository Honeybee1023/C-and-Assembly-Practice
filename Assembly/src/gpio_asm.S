.section .text      # indicates a code segment
.align 2            # specifies how data should be arranged in memory
.globl pinSetup   # makes pinSetup able to be used outside of this file
.globl pinWrite     # makes pinWrite able to be used outside of this file
.globl pinRead      # makes pinRead able to be used outside of this file
.globl setPixel     # makes setPixel able to be used outside of this file
.globl eraseBuffer  # makes eraseBuffer able to be used outside of this file

# C declaration: int pinRead(int pin_num)
# ARGUMENTS a0: pin_num
# RETURNS bit read from GPIO pin
pinRead:
    lui a2, 0x60004
    addi a2, a2, 0x3C # GPIO_IN_ADDR

    lw a3, 0(a2) #a3 = input_val
    
    addi a4, x0, 1 #store the const 1 in a4
    sll a5, a4, a0 #a5 = 1 << pin_num
    and a6, a3, a5 # a6 = input_val & (1 << pin_num)
    
    bne a6, x0, one #if ((*input_val & (1 << pin_num)) != 0) branch to return 1
    beq a6, x0, zero #else branch to return 0
    zero: 
    addi a0, x0, 0 #put return value 0 into a0
    jal x0, end #branch to end so we don't run the one: label too
    one:
        addi a0, x0, 1 #put return value 1 into a0
    end:
        
    jalr x0, 0(ra) #return statement


# C declaration: void pinWrite(int pin_num, int value)
# ARGUMENTS a0: pin_num, a1: value
# RETURNS: Nothing
pinWrite:
    lui a2, 0x60004
    addi a2, a2, 0x4  # GPIO_OUT_ADDR

    lw a3, 0(a2) # a3 = output_val
    
    beq a1, x0, continue # if (value != 0) value = 1;
    addi a1, x0, 1 # if (value != 0) value = 1;
    continue:
    
    beq a1, x0, valuezero # if (value == 0)
    # not if branch:
    addi a4, x0, 1 # a4 = const 1. 
    sll a5, a4, a0 # a5 = 1 << pin_num
    or a3, a3, a5 # *output_val = *output_val | (0b1 << pin_num);
    sw a3, 0(a2) # save from register back to memory!
    jal x0, end1 # jump to end
    
    valuezero:
    addi a4, x0, 1 # a4 = const 1
    sll a5, a4, a0 # a5 = 1 << pin_num
    xori a5, a5, -1 # not a5 by XOR-ing with 111..111 = -1
    and a3, a3, a5 # *output_val = *output_val & ~(0b1 << pin_num);
    sw a3, 0(a2) # save from register back to memory!
    
    end1:
    jalr x0, 0(ra)



# C declaration: void pinSetup(int pin_num, int mode)
# ARGUMENTS a0: pin_num, a1: mode
# RETURNS: Nothing
pinSetup:
    lui a2, 0x60009
    addi a2, a2, 0x4  # IO_MUX_GPIOn_ADDR

    lui a3, 0x60004
    addi a3, a3, 0x20 # GPIO_ENABLE_ADDR

    beq a1, x0, ifbranch
    
    # elsebranch:
    lw a4, 0(a3) # int *gpio_enable_addr = (int*) GPIO_ENABLE_ADDR;
    addi a5, x0, 1 # a5 = 1
    sll a5, a5, a0 # a5 = (0b1 << pin_num);
    or a4, a4, a5 # *gpio_enable_addr = *gpio_enable_addr | (0b1 << pin_num);
    sw a4, 0(a3)
    jal x0, end2
    
    
    ifbranch:
    slli a4, a0, 2 # a4 = (4*pin_num)
    add a5, a2, a4 # a5=(IO_MUX_GPIOn_ADDR + (4*pin_num));
    lw a6, 0(a5) # int *io_mux_gpio_addr = (int*) (IO_MUX_GPIOn_ADDR + (4*pin_num));
    addi a4, x0, 3 # a4 = 0b11
    slli a4, a4, 8 #a4 = (0b11 << 8)
    or a6, a6, a4 # *io_mux_gpio_addr = *io_mux_gpio_addr | (0b11 << 8);
    sw a6, 0(a5)
    
    end2:
    jalr x0, 0(ra)


# C declaration: void setPixel(uint32_t* screen_buffer_addr, uint8_t x, uint8_t y, uint8_t val);
# ARGUMENTS a0: screen_buffer base address, a1: x, a2: y, a3: val
# RETURNS: Nothing
setPixel:

    addi a7, x0, 1
    beq a3, a7, ifbranch1
    
    slli a2, a2, 2
    add a6, a2, a0
    lw a4, 0(a6) # a4 = screen_buffer[getY(location)] 
    addi a5, x0, 1 #a5=1
    sll a5, a5, a1 # a5 = 1 << getX(location)
    xori a5, a5, -1 # not a5
    and a4, a4, a5 # screen_buffer[getY(location)] &= ~(1 << getX(location));
    sw a4, 0(a6)
    jal x0, end3
    
    ifbranch1:
    slli a2, a2, 2
    add a6, a2, a0
    lw a4, 0(a6) # a4 = screen_buffer[getY(location)]
    addi a5, x0, 1 #a5=1
    sll a5, a5, a1 # a5 = 1 << getX(location)
    or a4, a4, a5 # screen_buffer[getY(location)] |= (1 << getX(location));
    sw a4, 0(a6)

    end3:
    jalr x0, 0(ra)



# C declaration: void eraseBuffer(uint32_t* screen_buffer_addr)
# ARGUMENTS a0: screen_buffer base address
# RETURNS: Nothing

eraseBuffer:
    addi a1, x0, 8      # upper bound on for loop
    addi a2, x0, 0      # "i" for for loop
looping:
    slli a3, a2, 2      # calculate 4*i
    add a4, a0, a3      # get address of array element by adding base address + 4*i
    sw zero, 0(a4)      # write 0 to memory address
    addi a2, a2, 1      # increment i
    bne a2, a1, looping # continue looping if i < 8
    jalr x0, 0(ra)      # return from eraseBuffer
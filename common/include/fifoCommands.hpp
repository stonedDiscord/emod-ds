#ifndef FIFO_COMMANDS_HPP
#define FIFO_COMMANDS_HPP

// max 32 instructions
#define PERLENGTH (5)
#define PERMASK ((1<<PERLENGTH)-1)

enum ARM9Request
{
    PER_START = 1,
    PER_STOP = 2,
    PER_RESET = 3,
    PER_INITOBBRIGIDBODY = 4,
    PER_INITSPHERERIGIDBODY = 5,
    PER_KILLRIGIDBODY = 6,
    PER_APPLYFORCE = 7,
};

#endif
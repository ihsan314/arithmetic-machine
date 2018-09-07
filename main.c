/*
 ============================================================================
 Name        : Arithmetic Machine
 Author      : UBC Launchpad (edited by Ihsan Olawale)
 Version     : 1.0
 Copyright   :
 Description : Stack-based virtual machine that performs simple arithmetic.
 ============================================================================
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define STACK_SIZE   256                                 // maximum number of values on the stack
#define PUSH(vm, v)  (vm->stack[++vm->sp] = v)           // push value onto stack
#define POP(vm)      (vm->stack[vm->sp--])               // get value from top of stack
#define NCODE(vm)    (vm->code[vm->pc++])                // get next bytecode

/* all opcodes that will be implemented. */
enum opcodes {
    HALT      = 0x00, // halt
    /* in case you can't get DCONST working, you can still try things using these: */
    DCONST_M1 = 0x0A, // push -1.0 onto stack
    DCONST_0  = 0x0B, // push 0.0 onto stack
    DCONST_1  = 0x0C, // push 1.0 onto stack
    DCONST_2  = 0x0D, // push 2.0 onto stack
    /* make sure you consider endianness */
    DCONST    = 0x0F, // push next 8 bytes onto stack as double constant
    /* arithmetic operations */
    ADD       = 0x60, // add two doubles
    SUB       = 0x61, // subtract two doubles
    MUL       = 0x62, // multiply two doubles
    DIV       = 0x64, // divide two doubles
    NEG       = 0x70, // negate an double (e.g. if -1.0 is on the stack, NEG will turn it to 1.0 on the stack)

    NOP       = 0xF0, // do nothing
    /* in a real VM, we'd use a function call for print, rather than having a special opcode */
    PRINT     = 0xF2, // pops and prints top of stack
    /* store and read from registers */
    ST1       = 0xF4, // pops top of stack and stores it in r1
    LD1       = 0xF5, // load global from r1
    ST2       = 0xF6, // pops top of stack and stores it in r2
    LD2       = 0xF7, // load global from r2
};

/* defining our virtual machine */
typedef struct {
	double r1, r2;      // registers
	char* code;         // pointer to bytecode
	double* stack;      // stack
	int pc;             // program counter
	int sp;             // stack pointer
} VM;

VM* newVM(char* code /* pointer to bytecode */ ) {
    VM* vm = malloc(sizeof(VM));
    vm->code = code; 
    vm->pc = 0;
    vm->sp = -1;
    vm->r1 = vm->r2 = 0;    // init registers to 0
    vm->stack = malloc(sizeof(double) * STACK_SIZE);
    return vm;
}

void delVM(VM* vm){
        free(vm->stack);
        free(vm);
}

int run(VM* vm){
    for (;;) {
        unsigned char opcode = NCODE(vm);        // store next bytecode in `opcode'
        double a, b, v;                          // use these to store intermediate values when implementing opcodes below
        switch (opcode) {   // decode
        case HALT: return EXIT_SUCCESS;  // exit successfully
        case NOP: break;    // pass
        case DCONST_M1:     // push -1.0 onto stack
            PUSH(vm, -1.0);
            // TODO: implement this.
            break;
        case DCONST_0:      // push 0.0 onto stack
            PUSH(vm, 0.0);
            // TODO: implement this.
            break;
        case DCONST_1:      // push 1.0 onto stack
            PUSH(vm, 1.0);
            // TODO: implement this.
            break;
        case DCONST_2:      // push 2.0 onto stack
            PUSH(vm, 2.0);
            // TODO: implement this.
            break;
        case DCONST:        // reads next 8 bytes of opcode as a double, and stores it on the stack.
            // TODO: implement this.
            // HINT: use memcpy to read next 8 bytes of code as a double. make sure you consider endianness.
            /* I am not too sure about this one; I did not get to memcpy when reading up on C. */
            break;
        case ADD:           // add two doubles from top of stack and push result back onto stack
            b = POP(vm);
            a = POP(vm);
            PUSH(vm, a + b);
            break;
        case MUL:           // multiply two doubles from top of stack and push result back onto stack
            // TODO: implement this.
            b = POP(vm);
            a = POP(vm);
            PUSH(vm, a * b);
            break;
        case SUB:           // subtract two doubles from top of stack and push result back onto stack
            // TODO: implement this.
            b = POP(vm);
            a = POP(vm);
            PUSH(vm, a - b);
            break;
        case DIV:          // divide two doubles from top of stack and push result back onto stack
            //TODO: implement this.
            // HINT: make sure to deal with the division by zero case.
            b = POP(vm);
            if (b == 0)
                printf ("Cannot divide by 0\n");
            else {
                a = POP(vm);
                PUSH(vm, a / b);
            }
            break;
        case NEG:                         // negates top of stack
            //TODO: implement this.
            PUSH(vm, POP(vm) * -1);
            break;
        case LD1:          // put value from r1 on top of stack
            // TODO: implement this.
            PUSH(vm, vm->r1);
            break;
        case ST1:                         // store top of stack in r1
            // TODO: implement this.
            r1 = POP(vm);
            break;
        case LD2:           // put value from r2 on top of stack
            // TODO: implement this.
            // HINT: should be similar to LD1.
            PUSH(vm, vm->r1);
            break;
        case ST2:                         // store top of stack in r2
            // TODO: implement this.
            // HINT: should be similar to ST1.
            r2 = POP(vm);
            break;
        case PRINT:                       // print top of stack, (and discard value afterwards.)
            // TODO: implement this.
            a = POP(vm);
            printf ("%g\n", a);
            break;
        default:
            printf("InvalidOpcodeError: %x\n", opcode);  // terminate program at unknown opcode and show error.
            return EXIT_FAILURE;
        }

    }
    return EXIT_FAILURE;
}

int main(void) {
	/* in a real VM, we'd read bytecode from a file, but for brevity's sake we'll read
	from an array.
	*/
	/* simple example: push 2 onto stack, push 1 onto stack, subtract them, print the result (should print 1.0)
       then push 2 onto stack, push 2 onto stack, multiply them, print the result (should print 4.0)
       then push 0 onto stack, and try dividing (should print "Cannot divide by zero")
       then negate top of stack, print the result (should print -2.0)
       then divide and print the result (shoudl print 1.0)
       then multiply
       then store top of stack in r1 and print the new top of stack (should print 2.0)
       then store top of stack in r2 and print the new top of stack (should print 2.0)
       then push r1 on top of stack and print the result (should print 4.0)
       then push r2 on top of stack and print the result (should print 2.0)
       then exit
    */
	char bytecode[] = { DCONST_2,
		DCONST_1,
		SUB,
		PRINT,
        DCONST_2,
        DCONST_2,
        MUL,
        PRINT,
        DCONST_0,
        DIV,
        NEG,
        PRINT,
        DIV,
        PRINT,
        MUL,
        ST1,
        PRINT,
        ST2,
        PRINT,
        LD1,
        PRINT,
        LD2,
        PRINT,
		HALT };
	VM* vm = newVM(bytecode /* program to execute */ );
	int exit_status = run(vm);
	delVM(vm);
	return exit_status;
};

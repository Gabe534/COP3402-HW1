/*
Assignment:
vm - HW1 PM/0 virtual machine

Authors: Gabriel Ross & Christoph Watson

Language: C only

To Compile:
gcc -Wall -Wextra -std=c11 -O2 vm.c -o vm

To Execute (on Eustis):
./vm <input_file>

  where:
<input_file> is the path to a text file holding one PM/0 instruction
per line, as three integers OP L M

Notes:
- Implements the PM/0 virtual machine described in the homework
instructions.
- No heap allocation and no function-like macros. The PAS array is
indexed, not walked with a pointer.
- Does not implement any VM instruction as a separate function; the
fetch-execute cycle runs directly in main.

- Defines at most three functions: main, base, and one print function.
- Exits with status 0 after a normal halt and with a non-zero status
after any error.
- Tested on Eustis.

Class: COP 3402 - Systems Software

Instructor: Jie Lin, Ph.D.

Due Date: See 9/18/26
*/

#include <stdio.h>
#include <stdlib.h>

static int pas[1000];

int base(int bp, int L)
{
  int arb = bp;
  while (L > 0)
  {
    arb = pas[arb];
    L--;
  }
  return arb;
}

//Part 0: Initial declarations and reading from the input file.
int main (int argc, char *argv[])
{
  int PC = 200;
  int BP = 999;
  int SP = 1000;
  int OP = pas[PC];
  int L = pas[PC+1];
  int M = pas[PC+2];
  int index = PC;
  int baseAddress; 

  if (argc < 2)
  {
    perror("\nError: imporper number of arguments\n");
    return 1;
  }

  FILE *inputFile = fopen(argv[1], "r");

  if(!inputFile)
  {
    perror("\nError: file not found\n");
    return 1;
  }

  int input;
  while((input = fscanf(inputFile, "%d", &pas[index])) == 1)
  {
    index++;
    if(index > 999)
    {
      perror("\nError: program too large for text segment\n");
      return 1;
    }
    endInstruction = index - 1;
  }
  fclose(inputFile);

  //print initial values of the stackspace.
  printf("\tL\tM\tPC\tBP\tSP\tstack\n");
  printf("Initial values: \t%d\t%d\t%d\n", PC, BP, SP);

  while(1)
  {
    scanf("%d %d %d", &OP, &L, &M);

    if(PC < 200 || PC > 999) {
      printf("\nError: program counter left the text segment\n");
      return 0;
    }
    //
    OP = pas[PC];
    L = pas[PC+1];
    M = pas[PC+2];

    //Switches cases by OP code:
    switch(OP)
    {
      case 1:
        //LIT
        printf("LIT\t%d\t%d\t", L, M);
        PC = PC + 3;
        SP = SP - 1;
        //after SP is lowered we check if SP ever overrides text segment.
        if(SP == endInstruction)
        {
          perror("\nError: stack overflow\n");
          return;
        }
        pas[SP] = M;
        break;
  
      case 2:
        //SUB operations:
        //Advance the pc to the next instruction
        PC = PC + 3;
        //If the value beneath the top is a and the top is b...
        //I think these should be switched. TOP is b so
        int b = pas[SP];
        int a = pas[SP+1];
        /*
        int a = SP;
        int b = SP + 1;
        */
        switch(M)
        {
          case(0):
            //return from procedure restore callers record
            printf("RTN\t%d\t%d\t", L, M);
            SP = BP + 1;
            BP = pas[SP -2];
            PC = pas[SP -3];
          break;
  
          case(1):
          //push a + b;
            printf("ADD\t%d\t%d\t", L, M);
            SP = SP - 1;
            if(SP == endInstruction)
            {
             perror("\nError: stack overflow\n");
             return;
            }
            pas[SP] = a + b;
          break;
  
          case(2):
          //push a - b
            printf("SUB\t%d\t%d\t", L, M);
            SP = SP - 1;
            if(SP == endInstruction)
            {
              perror("\nError: stack overflow\n");
              return;
            }
          pas[SP] = a - b;
          break;
  
          case(3):
          //push a x b
            printf("MUL\t%d\t%d\t", L, M);
            SP = SP - 1;
            if(SP == endInstruction)
            {
              perror("\nError: stack overflow\n");
            return;
            }
            pas[SP] = a * b;
          break;
  
          case(4):
          //push a / b
            printf("DIV\t%d\t%d\t", L, M);
            if(b == 0)
            {
              printf("\nError: division by zero\n");
              break;
            }
            SP = SP - 1;
            if(SP == endInstruction)
            {
              perror("\nError: stack overflow\n");
            return;
            }
            pas[SP] = a / b;
          break;
  
          case(5):
          //push 1 if a = b, otherwise 0
            printf("EQL\t%d\t%d\t", L, M);
            SP = SP - 1;
            if(SP == endInstruction)
            {
              perror("\nError: stack overflow\n");
            return;
            }
            if(a == b)
            {
              pas[SP] = 1;
              break;
            }
            pas[SP] = 0;
          break;
  
          case(6):
          //push 1 if a != b, otherwise 0
            printf("NEQ\t%d\t%d\t", L, M);
            SP = SP - 1;
            if(SP == endInstruction)
            {
              perror("\nError: stack overflow\n");
            return;
            }
            if(a != b)
            {
              pas[SP] = 1;
              break;
            }
            pas[SP] = 0;
          break;
  
          case(7):
          //push 1 if a < b, otherwise 0
            printf("LSS\t%d\t%d\t", L, M);
            SP = SP - 1;
            if(SP == endInstruction)
            {
              perror("\nError: stack overflow\n");
            return;
            }
            if(a < b)
            {
              pas[SP] = 1;
              break;
            }
            pas[SP] = 0;
          break;
  
          case(8):
          //push 1 if a <= b, otherwise 0
            printf("LEQ\t%d\t%d\t", L, M);
            SP = SP - 1;
            if(SP == endInstruction)
            {
              perror("\nError: stack overflow\n");
            return;
            }
            if(a <= b)
            {
              pas[SP] = 1;
              break;
            }
  
            pas[SP] = 0;
          break;
  
          case(9):
          //push 1 if a > b, otherwise 0
            printf("GTR\t%d\t%d\t", L, M);
            SP = SP - 1;
            if(SP == endInstruction)
            {
              perror("\nError: stack overflow\n");
            return;
            }
            if(a > b)
            {
              pas[SP] = 1;
              break;
            }
            pas[SP] = 0;
          break;
  
          case(10):
          //push 1 if a >= b, otherwise 0
            printf("GEQ\t%d\t%d\t", L, M);
            SP = SP - 1;
            if(SP == endInstruction)
            {
              perror("\nError: stack overflow\n");
              return;
            }
            if(a >= b)
            {
              pas[SP] = 1;
              break;
            }
            pas[SP] = 0;
          break;
  
          default:
            printf("\nError: unknown OPR sub-operation\n");
          break;
  
        }
        break;
  
      case 3:
        //LOD
        printf("LOD\t%d\t%d\t", L, M);
        PC = PC + 3;
        SP = SP - 1;
        if(SP == endInstruction)
        {
          perror("\nError: stack overflow\n");
          return;
        }
        //Ensure base(BP,L) - M produces a valid address. Not in system not in text segment. 
        baseAddress = base(BP,L) - M;
        if(baseAddress <= endInstruction)
        {
          perror("\n Error: data address out of rage\n");
          return;
        }
        pas[SP] = pas[baseAddress];
        break;
  
      case 4:
        //STO
        printf("STO\t%d\t%d\t", L, M);
        PC = PC + 3;

        baseAddress = base(BP,L) - M;
        if(baseAddress <= endInstruction)
        {
          perror("\n Error: data address out of rage\n");
          return;
        }
        pas[baseAddress] = pas[SP];
        SP = SP + 1;
        break;
  
      case 5:
        //CAL
        printf("CAL\t%d\t%d\t", L, M);
        pas[SP-1] = base(BP, L); //new position = static link?
        pas[SP-2] = BP; // new new postion = current base pointer (dynamic link?)
        pas[SP-3] = PC; // new new new position = PC (return address?)
        BP = SP - 1; // current base pointer = current stack pointer - 1;
        PC = M; //where exactly does this go? we know advance must come before excecution.
        break;
  
      case 6:
        //INC
        printf("INC\t%d\t%d\t", L, M);
        PC = PC + 3;
        SP = SP - M;// Words are allocated here, we dont know what the words are just how many there are so we allocate m spaces.
        if(SP == endInstruction)
        {
          perror("\nError: stack overflow\n");
          return;
        }
        break;
  
      case 7:
        //JMP; We know jump requires setting the PC again.
        printf("JMP\t%d\t%d\t", L, M);
        PC = M;
        break; // return address a
  
      case 8:
        // We need an if else statement here, PC changes if condition met otherwise it increments as usual.
        printf("JPC\t%d\t%d\t", L, M);
        if(pas[SP] == 0)
          {
          PC = M;
          }
        else
        {
          PC = PC + 3;
        }
        SP = SP + 1;
        break;
  
      case 9:
        //SYS
        switch(M)
        {
          case 1:
            PC = PC + 3;
            printf("Output result is: %d\n", pas[SP]);
            printf("SYS\t%d\t%d\t", L, M);
            SP = SP + 1;
            //PC is the index for our PAS array; shouldn't it be SP? to get the value at the top of the stack?
            //Also it says pop so shouldnt we change the SP after getting the value? SP = SP + 1;
            break;
  
          case 2:
            int x;
            PC = PC + 3;
            printf("Please Enter an Integer: ");
            scanf("%d", &x);
            printf("%d\n", x);
            printf("SYS\t%d\t%d\t", L, M);
            SP = SP - 1;
            pas[SP] = x;
          break;
  
          case 3:
            //PC = PC + 3; unneeded as this is the last instruction.
            printf("SYS\t%d\t%d\t", L, M);
            printf("%d\t%d\t%d\t", PC, BP, SP);
            for(int i = 999; i >= SP; i--)
            {
              printf("%d    ", pas[i]);
            }
            printf("\n");
            return 0;
          break;
  
          default:
            printf("\nError: unknown SYS operation\n");
          break;
        }
        break;
      default:
        printf("\nError: unknown opcode\n");
      break;
    }

    printf("%d\t%d\t%d\t", PC, BP, SP);
    for(int i = 999; i >= SP; i--)
    {
      printf("%d    ", pas[i]);
    }
    printf("\n");
  }
}


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
  int OP;
  int L;
  int M;
  int index = PC;
  int endInstruction;
  int baseAddress;
  int x;

  if (argc != 2)
  {
  printf("\nUsage: ./vm <input file>\n");
    return 1;
  }

  FILE *inputFile = fopen(argv[1], "r");

  if(!inputFile)
  {
    printf("\nError: cannot open %s\n", argv[1]);
    return 1;
  }

  int input;
  while((input = fscanf(inputFile, "%d", &pas[index])) == 1)
  {
    index++;
    if(index > 999)
    {
      printf("\nError: program too large for the text segment\n");
      return 1;
    }
  }
  endInstruction = index - 1;

  fclose(inputFile);

  //print initial values of the stackspace.
  printf("\tL\tM\tPC\tBP\tSP\tstack\n");
  printf("Initial values: \t%d\t%d\t%d\n", PC, BP, SP);

  while(1)
  {
    //scanf("%d %d %d", &OP, &L, &M);

    //check if PC is in the range from 200 to the last instruction
    if(PC < 200 || PC + 2 > endInstruction) {
      printf("\nError: program counter left the text segment\n");
      return 1;
    }

    OP = pas[PC];
    L = pas[PC+1];
    M = pas[PC+2];
    PC = PC + 3; // advance PC

    //Switches cases by OP code:
    switch(OP)
    {
      case 1:
        //LIT
        printf("LIT\t%d\t%d\t", L, M);
        SP = SP - 1;
        //after SP is lowered we check if SP ever overrides text segment.
        if(SP <= endInstruction)
        {
          printf("\nError: stack overflow\n");
          return 1;
        }
        pas[SP] = M;
        break;
  
      case 2:
        //SUB operations:
        //Advance the pc to the next instruction
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
          //pop a and b from the stack and push a + b.
            printf("ADD\t%d\t%d\t", L, M);
            // we already have a and b so just remove them from the stack.
            // instead of removing 2 and adding 1 to bottom just remove 1 and write
            SP = SP + 1;
            /*if(SP <= endInstruction)
            {
             printf("\nError: stack overflow\n");
             return 1;
            }*/
            pas[SP] = a + b;
          break;
  
          case(2):
          //push a - b
            printf("SUB\t%d\t%d\t", L, M);
            SP = SP + 1;
            /*if(SP <= endInstruction)
            {
              printf("\nError: stack overflow\n");
              return 1;
            }*/
          pas[SP] = a - b;
          break;
  
          case(3):
          //push a x b
            printf("MUL\t%d\t%d\t", L, M);
            SP = SP + 1;
            /*if(SP <= endInstruction)
            {
              printf("\nError: stack overflow\n");
            return 1;
            }*/
            pas[SP] = a * b;
          break;
  
          case(4):
          //push a / b
            printf("DIV\t%d\t%d\t", L, M);
            if(a == 0)
            {
              printf("\nError: division by zero\n");
              return 1;
            }
            SP = SP + 1;
            /*if(SP <= endInstruction)
            {
              printf("\nError: stack overflow\n");
            return 1;
            }*/
            pas[SP] = a / b;
          break;
  
          case(5):
          //push 1 if a = b, otherwise 0
            printf("EQL\t%d\t%d\t", L, M);
            SP = SP + 1;
            /*if(SP <= endInstruction)
            {
              printf("\nError: stack overflow\n");
             return 1;
            }*/
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
            SP = SP + 1;
            /*if(SP <= endInstruction)
            {
              printf("\nError: stack overflow\n");
            return 1;
            }*/
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
            SP = SP + 1;
            /*if(SP <= endInstruction)
            {
              printf("\nError: stack overflow\n");
            return 1;
            }*/
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
            SP = SP + 1;
            /*if(SP <= endInstruction)
            {
              printf("\nError: stack overflow\n");
            return 1;
            }*/
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
            SP = SP + 1;
            /*if(SP <= endInstruction)
            {
              printf("\nError: stack overflow\n");
            return 1;
            }*/
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
            SP = SP + 1;
            /*if(SP <= endInstruction)
            {
              printf("\nError: stack overflow\n");
              return 1;
            }*/
            if(a >= b)
            {
              pas[SP] = 1;
              break;
            }
            pas[SP] = 0;
          break;
  
          default:
            printf("\nError: unknown OPR sub-operation\n");
            return 1;
          break;
        }
        break;
  
      case 3:
        //LOD
        // printf("LOD\t%d\t%d\t", L, M); //
        SP = SP - 1;
        if(SP <= endInstruction)
        {
          printf("\nError: stack overflow\n");
          return 1;
        }
        //Ensure base(BP,L) - M produces a valid address. Not in system not in text segment. 
        baseAddress = base(BP,L) - M;
        if(baseAddress <= endInstruction)
        {
          printf("\nError: data address out of range\n");
          return 1;
        }
        // printf("LOD\t%d\t%d\t", L, M); //
        pas[SP] = pas[baseAddress];
        break;
  
      case 4:
        //STO

        baseAddress = base(BP,L) - M;
        if(baseAddress <= endInstruction)
        {
          printf("\nError: data address out of range\n");
          return 1;
        }
        // printf("STO\t%d\t%d\t", L, M);//
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
        PC = M;
        break;
  
      case 6:
        //INC
        //printf("INC\t%d\t%d\t", L, M);
        SP = SP - M;// Words are allocated here, we dont know what the words are just how many there are so we allocate m spaces.
        if(SP <= endInstruction)
        {
          printf("\nError: stack overflow\n");
          return 1;
        }
        printf("INC\t%d\t%d\t", L, M);
        break;
  
      case 7:
        //JMP; We know jump requires setting the PC again.
        printf("JMP\t%d\t%d\t", L, M);
        PC = M;
        break; // return address a
  
      case 8:
        // PC changes if condition met otherwise it increments as usual.
        printf("JPC\t%d\t%d\t", L, M);
        if(pas[SP] == 0)
          {
          PC = M;
          }
        SP = SP + 1;
        break;
  
      case 9:
        //SYS
        switch(M)
        {
          case 1:
            printf("Output result is: %d\n", pas[SP]);
            printf("SYS\t%d\t%d\t", L, M);
            SP = SP + 1;
            break;

          case 2:
            printf("Please Enter an Integer: ");
            scanf("%d", &x);
            printf("%d\n", x);
            printf("SYS\t%d\t%d\t", L, M);
            SP = SP - 1;
            if(SP <= endInstruction)
            {
              printf("\nError: stack overflow\n");
              return 1;
            }
            pas[SP] = x;
          break;
  
          case 3:
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
            return 1;
          break;
        }
        break;
      default:
        printf("\nError: unknown opcode\n");
        return 1;
      break;
    }

    printf("%d\t%d\t%d\t", PC, BP, SP);

    int temp = pas[BP];

    for(int i = 999; i >= SP; i--)
    {
      if(i == temp)
      {
        printf("| ");
        temp = pas[temp];
      }

      printf("%-5d", pas[i]);
    }
    printf("\n");
  }
}

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

//Part 0: Initial declarations and reading from the input file.
int[1000] pas;
int PC = 200;
int BP = 999;
int SP = 1000;
int OP = pas[PC];
int L = pas[PC+1];
int M = pas[PC+2];
int index = 0;

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

int main (int argc, char *argv[])
{
  if (argc < 2)
  {
    return 1;
  }
  FILE *inputFile = fopen(argv[1], "r");

  if(inputFile == NULL)
  {
    return 1;
  }

  int input;
  while(input = fgetc(inputFile) != EOF)
  {
    // This is a placeholder. We must trim spaces and \n from the array.
    if(input != "\n" && input != " ")
    {
      PAS[index] = input;
      index++;
    }
  }
  fclose(inputFile);

  //Part 1: Use 1D array to build vm. Start with the instruction or OP CODE;

  switch(OP)
  {
    case 1:
      //LIT
      PC = PC + 3;
      SP = SP - 1;
      pas[SP] = M;
      break;

    case 2:
      //SUB operations:
      //Advance the pc to the next instruction
      PC = PC + 3;
      switch(M)
      {
        case(0):
          //return from procedure restore callers record
          SP = BP + 1;
          BP = pas[SP -2];
          PC = pas[SP -3];
        break;

        case(1):
        //push a + b;
          SP = SP - 1;
          //where are a and b declared? are we given a and b? or do we find them using sp?
          pas[SP] = a + b;
        break;

        case(2):
        //push a - b
          SP = SP - 1;
          pas[SP] = a - b;
        break;

        case(3):
        //push a x b
          SP = SP - 1;
          pas[SP] = a * b;
        break;

        case(4):
        //push a / b
          if(b == 0) printf("Error: division by zero\n"), break;
          
          SP = SP - 1;
          pas[SP] = a / b;
        break;

        case(5):
        //push 1 if a = b, otherwise 0
          SP = SP - 1;
          if(a == b) pas[SP] = 1, break;

          pas[SP] = 0;
        break;

        case(6):
        //push 1 if a != b, otherwise 0
          SP = SP - 1;
          if(a != b) pas[SP] = 1, break;

          pas[SP] = 0;
        break;

        case(7):
        //push 1 if a < b, otherwise 0
          SP = SP - 1;
          if(a < b) pas[SP] = 1, break;

          pas[SP] = 0;
        break;

        case(8):
        //push 1 if a <= b, otherwise 0
          SP = SP - 1;
          if(a <= b) pas[SP] = 1, break;

          pas[SP] = 0;
        break

        case(9)
        //push 1 if a > b, otherwise 0
          SP = SP - 1;
          if(a > b) pas[SP] = 1, break;

          pas[SP] = 0;
        break;

        case(10):
        //push 1 if a >= b, otherwise 0
          SP = SP - 1;
          if(a >= b) pas[SP] = 1, break;

          pas[SP] = 0;
        break;

      }
      break;

    case 3:
      //LOD
      PC = PC + 3;
      SP = SP - 1;
      pas[SP] = pas[base(BP, L) - M];
      break;

    case 4:
      //STO
      PC = PC + 3;
      pas[base(BP, L) - M] = pas[SP];
      SP = SP + 1;
      break;

    case 5:
      //CAL
      pas[SP-1] = base(BP, L); //new position = ?
      pas[SP-2] = BP; // new new postion = current base pointer;
      pas[SP-3] = PC; // new new new position = PC;
      BP = SP - 1; // current base pointer = current stack pointer - 1;
      PC = M;
      break;

    case 6:
      //INC
      PC = PC + 3;
      SP = SP - M; // Words are allocated here, we dont know what the words are just how many there are so we allocate m spaces.
      break;

    case 7:
      //JMP
      PC = M;
      break;

    case 8:
      // We need an if else statement here, PC changes if condition met otherwise it increments as usual.
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
          printf("Output result is: %d\n", PC);
          //PC is the index for our PAS array; shouldn't it be SP? to get the value at the top of the stack?
          //Also it says pop so shouldnt we change the SP after getting the value? SP = SP + 1;
          break;

        case 2:
          int x;
          printf("Please Enter an Integer: ");
          scanf("%d", &x);
          printf("%d\n", x);
          SP = SP - 1;
          pas[SP] = x;
        break;

        case 3:
          return 0;
        break;
      }  
      break;

    default:
  }
}


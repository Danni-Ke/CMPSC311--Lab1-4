
/* Student Name: Danni Ke
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  return ~(~x|~y); //Demorgan's Law 
}
/* 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) {
   // create a 32-bits int with all odd bits to be 1's  e.g  .....101010
   int OddBit_Mask = (0xAA<<24)|(0xAA<<16)|(0xAA<<8)|(0xAA); 
   // replace the orginal number's all odd bits with 1's.  e.g ..... 1x1x1x
  int result = OddBit_Mask | x; 
   // if we flip the number, the all odd bits will become zero, and if all even bits is 1's, the result will give us 0, otherwise it'll be 1's. 
    // Finally, use ! to return the 1's for right all even bits number.
  return !(~result);
}
/* 
 * bitMask - Generate a mask consisting of all 1's 
 *   lowbit and highbit
 *   Examples: bitMask(5,3) = 0x38
 *   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int bitMask(int highbit, int lowbit) {
    /*
      Here, I got two version of this function, but the first one uses a illegal type.
   unsigned AllOne=(0xFF<<24)|(0xFF<<16)|(0xFF<<8)|(0xFF);
   AllOne=AllOne<<(31&(~highbit))>>((31&(~highbit))+lowbit)<<lowbit;
   return AllOne;
     */
     //I use two pieces of masks to  make the number between highbit and lowbit become all 0's first.
      int lowbit_mask=(~0<<lowbit);
      int highbit_mask=(~0<<highbit<<1);
     // lowbit_mask will be used again to check if highbit is greater than lowbits. The Xor gate will keep the number between highbit  and lowbit.
    return ((highbit_mask) ^ lowbit_mask) & (lowbit_mask); 


 }
/* 
 * replaceByte(x,n,c) - Replace byte n in x with c
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: replaceByte(0x12345678,1,0xab) = 0x1234ab78
 *   You can assume 0 <= n <= 3 and 0 <= c <= 255
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 3
 */
int replaceByte(int x, int n, int c) {
    // left shif the n to 3 to get the exact bits we need to move to.
  int shift = n << 3;
     // create a mask to take out the number need to be replaced.
  int ReplaceZero= 0xFF<< shift;
    // combine the int c and number that has been dealt with together.
  return (x & ~ReplaceZero)|(c << shift );
}
/*
 *
 * bitParity - returns 1 if x contains an odd number of 0's
 *   Examples: bitParity(5) = 0, bitParity(7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int bitParity(int x) {
  x=x^(x>>16);
  x=x^(x>>8);
  x=x^(x>>4);
  x=x^(x>>2);
  x=x^(x>>1);

  return x&0x01;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
    //The minimum of 2's complement is -2^(N-1).
  return 1 << 31;
}
/* 
 * isNegative - return 1 if x < 0, return 0 otherwise 
 *   Example: isNegative(-1) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 i*/
int isNegative(int x) 
{ 
   //since left shift the negative int will give us 0xFFFFFFFF, we need to and 1 to take out just the sign bit.
  return(x>>31)&0x01;

}



/* & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
    //The over flow only happen when the sign of x and y are same, but the sign of sum is different.
  //Take out the sign bit of x and y
   int s_x =(x>>31)&0x01;
  int s_y =(y>>31)&0x01;
    //Take out the sign bit of sum of x and y
  int s_x_y=((x+y)>>31)&0x01;
  //Case 1 is when x and y have different sign, it will return 1;
   int case_1=s_x^s_y;
   //Case 2 is when x and y are same and sum has different sign, it also return 1, otherwise, 0;
  int case_2=(s_x&s_y)^s_x_y; 
  return case_1|!case_2 ;
}

/*


* absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
    //-x=~x+1, x=x+0 according to the two's complement
      //x>>31 will return 0xFFFFFFFF when the number is negative and therefore it can be used to flip number.
     //if it's positive it will give 0x0 , which will not flip the number. Finally add the sign. 
  return ((x>>31)^x)+((x>>31)&0x01);
}
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
    // NaN Special case will return just uf
  if((uf&0x7fffffff)>0x7f800000)
    return uf;
   //check the sign, if the sign is 0, then by using xor with 1, it becomes 1, otherwise it becomes 0;
  return (0x01<<31)^uf;
}
/* 
 * float_half - Return bit-level equivalent of expression 0.5*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_half(unsigned uf) {
   // All the special cased included, NaN, zero, and infinity, negative zero, return uf
 if((uf&0x7fffffff)>=0x7f800000||uf==0x00||uf==0x80000000)
       return uf;
    //In order to get 0.5uf, actually we need to get half of the exp, meaning that divide the exp by 2 by right shift one bit.
     //if  exp is all zero, we need to right shift the frac and add rounding .
    // if exp is one, we need to move exp and frac right shift one bit.
  else{
         // round is get by check last two bits, if they are 11 just add 1;      
         //int round=((uf&3)==3);
          //  int exp =(uf&0x7f800000);
         //int sign=(uf&0x80000000);
          // int frac=(uf&0x007fffff);

               int round=((uf&3)==3);
         unsigned exp =(uf&0x7f800000)>>23;
         unsigned sign=(uf&0x80000000);
         unsigned frac=(uf&0x007fffff);

                     
 
         if(exp==0x01){
            return sign |((((exp<<23|frac)>>0x01)) + round); 
         }
         if(exp==0x00){
            return sign+((frac>>1)+round);
         }
                 
          return sign|((exp-1)<<23)|frac;          
        }


}


    
  

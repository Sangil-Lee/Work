/*
this header file implements the algorithm for 128-bit encryption 
*/
#include<iostream>
#include "lookup_table_encoding.h"
#include "key_expand.h"
using namespace std;
void encryption(unsigned char * temp,unsigned char * extendedkeys )
{
	int kp=0;
	for(int i=0;i<16;i++)
	{
		temp[i]^=extendedkeys[i];
	}
	kp++;
	while(kp<11)
	{
	      //substitution bits
	      for(int i=0;i<16;i++)
	      {
	          temp[i]=sbox[temp[i]];
	      }
	      //shift row
	      unsigned char * temp2 = new unsigned char[16];
	       for(int i=0;i<16;i++)
	       temp2[i]=temp[i];
	      //1st column
	       temp[0]=temp2[0];
	       temp[4]=temp2[4];
	       temp[8]=temp2[8];
	       temp[12]=temp2[12];
	       //2nd column
	       temp[1]=temp2[5];
	       temp[5]=temp2[9];
	       temp[9]=temp2[13];
	       temp[13]=temp2[1];
	       //3rd column
	       temp[2]=temp2[10];
	       temp[6]=temp2[14];
	       temp[10]=temp2[2];
	       temp[14]=temp2[6];
	       //4th column
	       temp[3]=temp2[15];
	       temp[7]=temp2[3];
	       temp[11]=temp2[7];
	       temp[15]=temp2[11];

	       //MIX column
	       if(kp<10)
	       {
	       	    for (int i = 0; i < 16; i++) {
		             temp2[i] = temp[i];
	              }
	       	//1st row
	            temp[0] = (unsigned char) lookup2[temp2[0]] ^ lookup3[temp2[1]] ^ temp2[2] ^ temp2[3];
	            temp[1] = (unsigned char) temp2[0] ^ lookup2[temp2[1]] ^ lookup3[temp2[2]] ^ temp2[3];
	            temp[2] = (unsigned char) temp2[0] ^ temp2[1] ^ lookup2[temp2[2]] ^ lookup3[temp2[3]];
	            temp[3] = (unsigned char) lookup3[temp2[0]] ^ temp2[1] ^ temp2[2] ^ lookup2[temp2[3]];
            //2nd row
	            temp[4] = (unsigned char)lookup2[temp2[4]] ^ lookup3[temp2[5]] ^ temp2[6] ^ temp2[7];
	            temp[5] = (unsigned char)temp2[4] ^ lookup2[temp2[5]] ^ lookup3[temp2[6]] ^ temp2[7];
	            temp[6] = (unsigned char)temp2[4] ^ temp2[5] ^ lookup2[temp2[6]] ^ lookup3[temp2[7]];
	            temp[7] = (unsigned char)lookup3[temp2[4]] ^ temp2[5] ^ temp2[6] ^ lookup2[temp2[7]];
            //3rd row
	            temp[8] = (unsigned char)lookup2[temp2[8]] ^ lookup3[temp2[9]] ^ temp2[10] ^ temp2[11];
	            temp[9] = (unsigned char)temp2[8] ^ lookup2[temp2[9]] ^ lookup3[temp2[10]] ^ temp2[11];
	            temp[10] = (unsigned char)temp2[8] ^ temp2[9] ^ lookup2[temp2[10]] ^ lookup3[temp2[11]];
	            temp[11] = (unsigned char)lookup3[temp2[8]] ^ temp2[9] ^ temp2[10] ^ lookup2[temp2[11]];
            //4th row
	            temp[12] = (unsigned char)lookup2[temp2[12]] ^ lookup3[temp2[13]] ^ temp2[14] ^ temp2[15];
	            temp[13] = (unsigned char)temp2[12] ^ lookup2[temp2[13]] ^ lookup3[temp2[14]] ^ temp2[15];
	            temp[14] = (unsigned char)temp2[12] ^ temp2[13] ^ lookup2[temp2[14]] ^ lookup3[temp2[15]];
	            temp[15] = (unsigned char)lookup3[temp2[12]] ^ temp2[13] ^ temp2[14] ^ lookup2[temp2[15]];
		   }

		   //Add Round Key
		    for(int i=0;i<16;i++)
	        {
	          temp[i]^=extendedkeys[kp*16+i];
	        }
	        kp++;
	}
}


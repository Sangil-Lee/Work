/*
this header file implements the algorithm for 128-bit decryption 
*/
#include<iostream>
#include "lookup_table_decoding.h"
#include "key_expand.h"
using namespace std;
void decryption(unsigned char * temp,unsigned char * extendedkeys)
{
	int kp=10;
	while(kp>0)
	{
		//subtract round key
		for(int i=0;i<16;i++)
		{
			temp[i]^=extendedkeys[(kp*16)+i];
		}

		//inverse mix column step
        if(kp<10){
	     unsigned char temp2[16];
	     	for (int i = 0; i < 16; i++)
			 {
		     temp2[i] = temp[i];
			 }

	    temp[0] = (unsigned char)lookup14[temp2[0]] ^ lookup11[temp2[1]] ^ lookup13[temp2[2]] ^ lookup9[temp2[3]];
	    temp[1] = (unsigned char)lookup9[temp2[0]] ^ lookup14[temp2[1]] ^ lookup11[temp2[2]] ^ lookup13[temp2[3]];
	    temp[2] = (unsigned char)lookup13[temp2[0]] ^ lookup9[temp2[1]] ^ lookup14[temp2[2]] ^ lookup11[temp2[3]];
	    temp[3] = (unsigned char)lookup11[temp2[0]] ^ lookup13[temp2[1]] ^ lookup9[temp2[2]] ^ lookup14[temp2[3]];

	    temp[4] = (unsigned char)lookup14[temp2[4]] ^ lookup11[temp2[5]] ^ lookup13[temp2[6]] ^ lookup9[temp2[7]];
	    temp[5] = (unsigned char)lookup9[temp2[4]] ^ lookup14[temp2[5]] ^ lookup11[temp2[6]] ^ lookup13[temp2[7]];
	    temp[6] = (unsigned char)lookup13[temp2[4]] ^ lookup9[temp2[5]] ^ lookup14[temp2[6]] ^ lookup11[temp2[7]];
	    temp[7] = (unsigned char)lookup11[temp2[4]] ^ lookup13[temp2[5]] ^ lookup9[temp2[6]] ^ lookup14[temp2[7]];

	    temp[8] = (unsigned char)lookup14[temp2[8]] ^ lookup11[temp2[9]] ^ lookup13[temp2[10]] ^ lookup9[temp2[11]];
	    temp[9] = (unsigned char)lookup9[temp2[8]] ^ lookup14[temp2[9]] ^ lookup11[temp2[10]] ^ lookup13[temp2[11]];
	    temp[10] = (unsigned char)lookup13[temp2[8]] ^ lookup9[temp2[9]] ^ lookup14[temp2[10]] ^ lookup11[temp2[11]];
	    temp[11] = (unsigned char)lookup11[temp2[8]] ^ lookup13[temp2[9]] ^ lookup9[temp2[10]] ^ lookup14[temp2[11]];

	    temp[12] = (unsigned char)lookup14[temp2[12]] ^ lookup11[temp2[13]] ^ lookup13[temp2[14]] ^ lookup9[temp2[15]];
	    temp[13] = (unsigned char)lookup9[temp2[12]] ^ lookup14[temp2[13]] ^lookup11[temp2[14]] ^ lookup13[temp2[15]];
	    temp[14] = (unsigned char)lookup13[temp2[12]] ^ lookup9[temp2[13]] ^ lookup14[temp2[14]] ^ lookup11[temp2[15]];
	    temp[15] = (unsigned char)lookup11[temp2[12]] ^ lookup13[temp2[13]] ^ lookup9[temp2[14]] ^ lookup14[temp2[15]];
        }
		// Shifts rows right
     	unsigned char temp2[16];
     	for (int i = 0; i < 16; i++)
		 {
		temp2[i] = temp[i];
	     }
	     //column one
	    temp [0] = temp2[0];
	    temp [4] = temp2[4];
	    temp [8] = temp2[8];
	    temp [12] = temp2[12];
         //column two
        temp [1] = temp2[13];
	    temp [5] = temp2[1];
	    temp [9] = temp2[5];
	    temp [13] = temp2[9];
         //column three
	    temp [2] = temp2[10];
	    temp [6] = temp2[14];
	    temp [10] = temp2[2];
	    temp [14] = temp2[6];
	    //column four
	    temp [3] = temp2[7];
	    temp [7] = temp2[11];
	    temp [11] = temp2[15];
	    temp [15] = temp2[3];

	    //substitution bits
	    for(int i=0;i<16;i++)
	     {
	        temp[i]=in_sbox[temp[i]];
	     }
	     kp--;
       }

	  	//subtract round key
		for(int i=0;i<16;i++)
		{
			temp[i]^=extendedkeys[i];
		}

}


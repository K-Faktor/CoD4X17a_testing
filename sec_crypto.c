/*
===========================================================================
    Copyright (C) 2010-2013  Ninja and TheKelm of the IceOps-Team

    This file is part of CoD4X17a-Server source code.

    CoD4X17a-Server source code is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    CoD4X17a-Server source code is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
===========================================================================
*/

#include "sec_crypto.h"
#include "sec_main.h"

struct ltc_hash_descriptor sec_hashes[SEC_HASH_SIZE__];



qboolean Sec_BinaryToHex(char *in,unsigned long inSize,char *out, unsigned long *outSize){
    if(*outSize > inSize * 4+1){
	//printf("Sec_BinaryToHex: DEBUG: %lu, %lu\n",*outSize,inSize * 4);
	SecCryptErr = CRYPT_MEM;
	return qfalse;
    }
    int i,j;
    char hex[17]="0123456789abcdef";
    for(i=0,j=0;i<inSize && j < *outSize;i++,j+=2){
	//Sec_CharToHex(in[i],out+(i*2));
	out[j+1]= hex[in[i]&0xF];
	out[j]= hex[(in[i]>>4)&0xF];
	out[j+2]= 0;
	//printf("%d,%d,%c%c |\n",i,j,out[j],out[j+1]);
    }
    //printf("\n\n");
    return qtrue;

}
qboolean Sec_HashMemory(sec_hash_e algo, void *in, size_t inSize, void *out, long unsigned int *outSize,qboolean binaryOutput){
    //__asm__("int $3");
    if(in == NULL || out == NULL || *outSize < 1 || inSize < 1 || algo < 0 || algo >= SEC_HASH_SIZE__){
	SecCryptErr = CRYPT_INVALID_ARG;
	return qfalse;
    }
    hash_state md;
    int result;
    long unsigned int size;
    unsigned char *buff = NULL,*buff2 = NULL;
    struct ltc_hash_descriptor *hs = &sec_hashes[algo];
    
    SecCryptErr = CRYPT_OK;
    size = (binaryOutput ? hs->hashsize / 4: hs->hashsize);
    //printf("Debug: %lu, %lu.\n",size,*outSize);
    if(size > *outSize){
	//printf("ERROR: CRYPT_MEM, size: %lu, outSize: %lu\n",size,*outSize);
	SecCryptErr = CRYPT_MEM;
        return qfalse;
    }
    
    if(!binaryOutput) {buff = (unsigned char *)Sec_Malloc(sizeof(unsigned char) * size); buff2 = buff; }
    else {buff2 = NULL; buff = out; }
    
    if((result = hs->init(&md)) != CRYPT_OK)		   { Sec_Free(buff2); SecCryptErr = result; return qfalse; }
    if((result = hs->process(&md, in, inSize)) != CRYPT_OK){ Sec_Free(buff2); SecCryptErr = result; return qfalse; }
    if((result = hs->done(&md,buff)) != CRYPT_OK)	   { Sec_Free(buff2); SecCryptErr = result; return qfalse; }
    //__asm__("int $3");
    //printf("Debug: binary output while hashing \"%s\":\n",in);
    //for(i=0;i<hs->hashsize;++i){
    //	printf("%x ",(int)buff[i]);
    //}
    if(!binaryOutput){
	Sec_BinaryToHex((char *)buff,hs->hashsize,out,outSize);
    }
    else *outSize = hs->hashsize;
    
    Sec_Free(buff2);
    
    return (SecCryptErr == CRYPT_OK);
}

qboolean Sec_HashFile(sec_hash_e algo, const char *fname, void *out, long unsigned *outSize,qboolean binaryOutput)
{
    if(fname == NULL || out == NULL || *outSize < 1 || algo < 0 || algo >= SEC_HASH_SIZE__){
	SecCryptErr = CRYPT_INVALID_ARG;
	return qfalse;
    }
    hash_state md;
    FILE *fp;
    unsigned char buff[2];
    unsigned char *ptr;
    int x,result,size;
    struct ltc_hash_descriptor *hs = &sec_hashes[algo];
    SecCryptErr = CRYPT_OK;
    size = binaryOutput ? hs->hashsize / 4 : hs->hashsize;
    //printf("Debug: Hashing using the \"%s\" hash.\n",hs->name);
    //printf("DBG: %lu, %d, %p, %p\n",hs->hashsize,outSize,hs->done,tiger_done);
    if(size > *outSize){
	SecCryptErr = CRYPT_MEM;
        return qfalse;
    }
    unsigned long dbg = 0;
    
    if((result = hs->init(&md))!=CRYPT_OK){ SecCryptErr=result; return qfalse; }
    fp = fopen(fname,"rb");
    do{
        x = fread(buff, 1, sizeof(buff), fp);
        
        buff[x]=0;
        //printf("DBG:%d, \"%s\"\n\n",x,buff);
        if ((result = hs->process(&md, buff, x)) != CRYPT_OK){
           SecCryptErr = result;
           fclose(fp);
           return qfalse;
        }
        dbg += x;
    }while (x == sizeof(buff));
    printf("File size: %lu\n",dbg);
    fclose(fp);
    ptr = binaryOutput ? out : buff;
    if((result = hs->done(&md,buff))!=CRYPT_OK){ SecCryptErr = result; return qfalse; }
    if(!binaryOutput){
	Sec_BinaryToHex((char *)ptr,hs->hashsize,out,outSize);
    }
    else *outSize = hs->hashsize;
    return (SecCryptErr == CRYPT_OK);
}

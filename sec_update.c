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
#include "sec_update.h"
#include "sec_crypto.h"
#include "sec_common.h"
#include "sec_main.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>


#define EXECUTABLE_NAME "cod4x17a_dedrun"
#define BUFSIZE 10240
char *Sec_StrTok(char *str,char *tokens,int id){
    static char *mem[100] = {NULL};
    char *ptr,*ptr2;
    if(id<0||id>99||tokens==NULL)
	return NULL;
    if(str==NULL){
	if(mem[id]==NULL){
	    return NULL;
	}
	
	for(ptr=mem[id];*ptr != 0;++ptr){
	    //printf("---%c\n",*ptr);
	    for(ptr2=tokens;*ptr2!=0;++ptr2){
		//printf("------%c\n",*ptr2);
		if(*ptr == *ptr2){
		    //printf("DEBUG!!!!!!!! %p:\"%s\", %p:\"%s\",%p:\"%s\".\n\n",ptr,ptr,ptr2,ptr2,mem[id],mem[id]);
		    *ptr=0;
		    ptr2=mem[id];
		    mem[id]=ptr+1;
		    //printf("DEBUG!!!!!!!! %p:\"%s\", %p:\"%s\".\n\n",ptr,ptr,ptr2,ptr2);
		    //__asm__("int $3");
		    return ptr2;
		}
	    }
	}
	if(ptr!=mem[id]){
	    ptr = mem[id];
	}
	else
	    ptr = NULL;
	mem[id]=NULL;
	return ptr;
    }
    else{
    //printf("Debugging: \"%s\"\n",str);
	//mem[id]=str;
	for(ptr=str;*ptr != 0 && mem[id]==NULL;++ptr){
	    for(ptr2=tokens;*ptr2!=0 && mem[id]==NULL;++ptr2){
		if(*ptr != *ptr2){
		    mem[id]=ptr;
		}
	    }
	}
	if(mem[id] == NULL) return NULL;
	return Sec_StrTok(NULL,tokens,id); // BECAUSE I CAN.
    }	
}
qboolean Sec_IsNumeric(char *str){
    int i,j;
    if(str==NULL)
	return qfalse;
    for(i=0,j=strlen(str);i<j;++i){
	if((str[i]>='0'&&str[i]<='9')){
	    return qtrue; // First non-whitespace is a digit
	}
	else if( str[i]==' ' || str[i]=='\r' || str[i]=='\n')
	    continue; // Whitespaces at the start
	else
	    return qfalse; // Not whitespace, not a digit
    }
    return qfalse; // Only whitespaces
}
int Sec_GetHTTPPacket(int sock, sec_httpPacket_t *out){
    char buff[8000]; // Would say 'risky', but it is only 0.8% of the stack in most cases :)
    int status,datac;
    char *tmp;
    memset(buff,0,sizeof(buff));
    memset(out,0,sizeof(sec_httpPacket_t));
    status = recv(sock,buff,sizeof(buff)-1,0);
    if(status < 1){
	printf("Sec_GetHTTPPacket: Failed to receive data!\n");
	return -1;
    }
    tmp = strstr(buff,"\r\n\r\n");
    if(tmp == NULL){
	printf("Sec_GetHTTPPacket: Packet is corrupt!\n");
	return -2;
    }
    //*(tmp + 3) = 0;
    out->headerLength = tmp-buff;
    out->header = Sec_SMalloc(out->headerLength + 1);
    out->header[out->headerLength]=0; // A valid C string :)
    memcpy(out->header,buff,out->headerLength);
    //printf("DEBUG: header contents:\n\n-------------\n\n%s\n\n-------------\n\n",out->header);
    tmp = strtok(buff," ");
    tmp = strtok(NULL," ");
    if(strncmp(out->header,"HTTP/1.",7)!=0 || tmp == NULL || !Sec_IsNumeric(tmp)){
	printf("Sec_GetHTTPPacket: Packet is corrupt!\nDebug: %s\n",tmp);
	return -3;
    }
    out->code = atoi(tmp);
    tmp = strstr(out->header,"Content-Length:");
    //printf("DEBUG: \"%s\"\n",tmp);
    
    if(tmp==NULL){
	printf("Sec_GetHTTPPacket: Packet is corrupt!\nDebug: %s\n",tmp);
	return -4;
    }
    
    if(!Sec_IsNumeric(tmp + 15)){
	printf("Sec_GetHTTPPacket: Packet is corrupt!\nDebug: %s\n",tmp);
	return -5;
    }
    out->contentLength = atoi(tmp + 15);
    out->content = Sec_SMalloc(out->contentLength + 1);
    out->content[out->contentLength] = 0; // In case we wanted to use string functions on the content
    //printf("Mega debug: %d     %d, %s\n",status,out->headerLength + 4,buff+out->headerLength+4);
    tmp = buff + out->headerLength + 4; // tmp now points on the start of the content
    
    datac = status - out->headerLength - 4;
    if(datac > out->contentLength){
	printf("Sec_GetHTTPPacket: Packet is corrupt!\nDebug: %d,%d,%d,%d\n",datac,out->contentLength,out->headerLength,status);
	return -6;
    }
    memcpy(out->content,tmp,datac);
    if(datac < out->contentLength){
	do{
	    status=recv(sock,out->content+datac,out->contentLength-datac,0);
	    datac += status;
	}while(datac<out->contentLength);
	if(datac != out->contentLength){
	    printf("Sec_GetHTTPPacket: Packet is corrupt!\n");
	    return -7;
	}
    }
    return out->code;
}
void Sec_FreeHttpPacket(sec_httpPacket_t *packet){
    Sec_Free(packet->content);
    Sec_Free(packet->header);
}
void Sec_FreeFileStruct(sec_file_t *file){
    if(file->next != NULL)
	Sec_FreeFileStruct(file->next);
    Sec_Free(file);
}
void Sec_Update(char *cmdLine[]){
    char *buff;
    char *ptr,*ptr2;
    char name1[256],name2[256];
    struct sockaddr_in addr;
    struct hostent *server;
    int sock,status;
    sec_file_t files,*currFile = &files;
    sec_httpPacket_t packet,packet2;
    FILE *fp;
    qboolean dlExec = qfalse;
    int i;
    char hash[128];
    long unsigned size;
    
    printf("\n-----------------------------\n");
    printf(" CoD4X Auto Update\n");
    printf(" Current version: %g\n",SEC_VERSION);
    printf(" Current build: %d\n",BUILD_NUMBER);
    printf(" Current type: %s\n",SEC_TYPE == 's' ? "stable      " : "experimental");
    printf("-----------------------------\n\n");    
    
    
    server = gethostbyname(SEC_UPDATE_HOST);
    if(server == NULL){
	printf("Cannot connect to the update server - no such host.\n");
	return;
    }
    memset(&addr,0,sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SEC_UPDATE_PORT);
    memcpy(&addr.sin_addr.s_addr,server->h_addr,sizeof(struct hostent));
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock<0){
	printf("Cannot create a socket. Error string: %s.\n", strerror(errno));
	return;
    }
    
    status = connect(sock,(struct sockaddr *)&addr,sizeof(struct sockaddr_in));
    if(status < 0){
	printf("Cannot connect to the update server -%d, %s.\n",status,strerror(errno));
	return;
    }
    //Sec_Free(buff);
    buff = (char *)Sec_Malloc(SEC_UPDATE_INITIALBUFFSIZE);
    sprintf(buff,SEC_UPDATE_GETVERSION);
    
    status = send(sock,buff,strlen(buff),0);
    
    fp = fopen("tmp.txt","w");
    fwrite(buff,sizeof(char),status,fp);
    fflush(fp);
    fclose(fp);
    //Sec_Free(buff);
    // TODO: Do something with the status?
    status = Sec_GetHTTPPacket(sock, &packet);
    //status = recv(sock,buff,strlen(buff),0);
    fp = fopen("tmp2.txt","w");
    fwrite(packet.header,sizeof(char),packet.headerLength,fp);
    fflush(fp);
    fclose(fp);
    fp = fopen("tmp3.txt","w");
    fwrite(packet.content,sizeof(char),packet.contentLength,fp);
    fflush(fp);
    fclose(fp);
    if(status<=0){
	printf("Error receiving data. Error code: %d.\n", status);
	close(sock);
	Sec_Free(buff);
	Sec_FreeHttpPacket(&packet);
	return;
    }
    if(status==204){
	printf("\nServer is up to date.\n\n");
	close(sock);
	Sec_Free(buff);
	Sec_FreeHttpPacket(&packet);
	return;
    }
    else if(status!=200){
	printf("Error updating: update server's malfunction.\nStatus code: %d.\n",status);
	close(sock);
	Sec_Free(buff);
	Sec_FreeHttpPacket(&packet);
	return;
    }
    /* We need to parse filenames etc */
    
    ptr = Sec_StrTok(packet.content,"\n",42); // Yes, 42.
    
    while(ptr != NULL){
	
	currFile->next = Sec_GMalloc(sec_file_t,1);
	currFile = currFile->next;
	memset(currFile,0,sizeof(sec_file_t));
	ptr2 = strchr(ptr,' ');
	if(ptr2 == NULL){
	    printf("Sec_Update: Corrupt data from update server. Update aborted.\nDebug:\"%s\"\n",ptr);
	    return;
	}
	*ptr2++ = 0;
	strncpy(currFile->path,ptr,sizeof(currFile->path));
	ptr = ptr2;
	ptr2 = strchr(ptr,' ');
	if(ptr2 == NULL){
	    printf("Sec_Update: Corrupt data from update server. Update aborted.\nDebug:\"%s\"\n",ptr);
	    return;
	}
	*ptr2++ = 0;
	if(!Sec_IsNumeric(ptr)){
	    printf("Sec_Update: Corrupt data from update server - size is not a number. Update aborted.\nDebug:\"%s\"\n",ptr);
	    return;
	}
	currFile->size = atoi(ptr);
	strncpy(currFile->hash,ptr2,sizeof(currFile->hash));
	for(i=strlen(currFile->path);i>0;--i){
	    if(currFile->path[i]=='/' || currFile->path[i]=='\\'){
		strcpy(currFile->name,currFile->path + i+1);
		break;
	    }
	}
	//printf("DEBUG: File to download: link: \"%s\", name: \"%s\", size: %d, hash: \"%s\"\n\n",file.path,file.name,file.size,file.hash);
	close(sock);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	status = connect(sock,(struct sockaddr *)&addr,sizeof(struct sockaddr_in));
	if(status < 0){
	    printf("Cannot connect to the update server - %d, %s.\n",status,strerror(errno));
	    Sec_Free(buff);
	    Sec_FreeHttpPacket(&packet);
	    return;
	}
	sprintf(buff,SEC_UPDATE_DOWNLOAD(currFile->path));
	status = send(sock,buff,strlen(buff),0);
	
	strncpy(buff,currFile->name,128);
	strncat(buff,".new",128);
	fp = fopen(buff,"w");
	if(fp == NULL){
	    printf("Error opening \"%s\" for writing! Update aborted.\n",buff);
	    Sec_Free(buff);
	    Sec_FreeHttpPacket(&packet);
	    return;
	}
	status = Sec_GetHTTPPacket(sock, &packet2);	
	if(status!=200){
	    printf("Error downloading file \"%s\"! Error code: %d. Update aborted.\n",currFile->name,status);
	    fclose(fp);
	    Sec_Free(buff);
	    Sec_FreeHttpPacket(&packet);
	    Sec_FreeHttpPacket(&packet2);
	    return;
	}
	fwrite(packet2.content,sizeof(char),packet2.contentLength,fp);
	fflush(fp);
	fclose(fp);
	ptr = Sec_StrTok(NULL,"\n",42); // Yes, 42 again.
	Sec_FreeHttpPacket(&packet2);
	size = sizeof(hash);
	
	if(!Sec_HashFile(SEC_HASH_SHA256,buff,hash,&size,qfalse)){
	    printf("Error hashing the file \"%s\". Error code: %s.\nUpdate aborted.\n",currFile->name,Sec_CryptErrStr(SecCryptErr));
	    //fclose(fp);
	    Sec_Free(buff);
	    Sec_FreeHttpPacket(&packet);
	    return;
	}
	
	if(strncmp(hash,currFile->hash,size)==0){
	    printf("successfully downloaded file \"%s\".\n",currFile->name);
	    if(strncmp(currFile->name,EXECUTABLE_NAME,15)==0){
		printf("-Executable downloaded successfully\n");
		dlExec = qtrue;
	    }
	}
	else{
	    printf("Error, file \"%s\" is corrupt!\nUpdate aborted.\n",currFile->name);
	    printf("DEBUG: hash: \"%s\", correct hash: \"%s\".\n",hash,currFile->hash);
	    Sec_Free(buff);
	    Sec_FreeHttpPacket(&packet);
	    return;
	}
	
    }
    
    Sec_FreeHttpPacket(&packet);
    Sec_Free(buff);
    if(!dlExec){
	printf("\nError updating: lacking executable! Update aborted.\n");
    }
    else{
	printf("\nAll files downloaded successfully. Applying update...\n");
    }
    currFile = files.next;
    do{
	printf("Updating file %s...\n",currFile->name);
	strncpy(name1,currFile->name,sizeof(name1));
	strncat(name1,".old",sizeof(name1));
	strncpy(name2,currFile->name,sizeof(name2));
	strncat(name2,".new",sizeof(name2));
	// Check if an old file exists with this name
	fp = fopen(currFile->name,"r");
	if(fp!=NULL){ // Old file exists, back it up
	    printf("Backing up old file %s...\n",currFile->name);
	    fclose(fp);
	    fp = fopen(name1,"r");
	    if(fp!= NULL){ // Backup of old file exists, remove it
		printf("Found old backup of file %s, removing...\n",currFile->name);
		fclose(fp);
		if(remove(name1)!=0) printf("WARNING: failed to remove file %s, error string: \"%s\".\n",name1,strerror(errno));
	    }
	    if(rename(currFile->name,name1) !=0) printf("WARNING: failed to rename file %s to %s, error string: \"%s\".\n",currFile->name,name1,strerror(errno));
	}
	if(rename(name2,currFile->name) != 0) printf("WARNING: failed to rename file %s to %s, error string: \"%s\".\n",name2,currFile->name,strerror(errno));
	printf("Update on file %s successfully applied.\n",currFile->name);
	currFile = currFile->next;
    }while(currFile!=NULL);
    Sec_FreeFileStruct(files.next);
    printf("Finalizing update...\n");
    printf("Executing chmod on file \"%s\"...\n",EXECUTABLE_NAME);
    
    if(chmod(EXECUTABLE_NAME,S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0){
	printf("CRITICAL ERROR: failed to change mode of the file \"%s\"! Aborting, manual installation might be required.\n",cmdLine[0]);
	return;
    }
    printf("Executing %s...\n",EXECUTABLE_NAME);
    execv(EXECUTABLE_NAME,cmdLine);
    printf("If you can see this, call 911! Something went terribly wrong...\n");
    printf("execv's error code: %d, error string: \"%s\".\n",errno,strerror(errno));
    exit(87);
}
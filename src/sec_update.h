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
#ifndef SEC_UPDATE_H
#define SEC_UPDATE_H
#include "sec_init.h"
#include "q_platform.h"
#include "version.h"


void Sec_Update( qboolean getbasefiles );


typedef struct{
    int code;
    char status[32];
    int headerLength;
    int contentLength;
    char *header;
    char *content;
}sec_httpPacket_t;

typedef struct sec_file_s{
    char name[MAX_OSPATH];
    char path[MAX_OSPATH];
    int size;
    char hash[65];
    struct sec_file_s *next;
}sec_file_t;


#define SEC_UPDATE_VERSION "1.0"

#define SEC_UPDATE_INITIALBUFFSIZE 10240

#define SEC_TYPE 'e'
#define SEC_VERSION 1.7



//#undef QUOTE
#define SEC_UPDATE_HOST "update.iceops.in:80"
#define SEC_UPDATE_PHP(B,T) "/?ver=1.7&build=" B "&type=" T
#define SEC_UPDATE_USER_AGENT "CoD4X AutoUpdater V. " SEC_UPDATE_VERSION
//#define SEC_UPDATE_BOUNDARY "------------------------------------874ryg7v"
#define SEC_UPDATE_PORT 80


#define SEC_UPDATE_DOWNLOAD(baseurl, qpath) "GET %s%s HTTP/1.1 \r\nHost: %s \r\nUser-Agent: %s \r\nAccept-Encoding: \r\nConnection: Close\r\n\r\n", baseurl, qpath, SEC_UPDATE_HOST,SEC_UPDATE_USER_AGENT
//#define SEC_UPDATE_TMP SEC_UPDATE_PHP(BUILD_NUMBER,"e")
    #define SEC_UPDATE_GETVERSION "GET /?ver=%g&os=%s&build=%d&type=%c HTTP/1.1 \r\nHost: %s \r\nUser-Agent: %s \r\nAccept-Encoding: \r\nConnection: Close\r\n\r\n", SEC_VERSION, OS_STRING, BUILD_NUMBER, SEC_TYPE, SEC_UPDATE_HOST,SEC_UPDATE_USER_AGENT
#define SEC_UPDATE_GETGROUNDVERSION "GET /?ver=%g&os=%s&build=%d&type=%c HTTP/1.1 \r\nHost: %s \r\nUser-Agent: %s \r\nAccept-Encoding: \r\nConnection: Close\r\n\r\n", SEC_VERSION, OS_STRING, 753, 'b', SEC_UPDATE_HOST,SEC_UPDATE_USER_AGENT
//#define SEC_UPDATE_GETVERSION SEC_UPDATE_DOWNLOAD(SEC_UPDATE_TMP)

#if defined(OFFICIAL) || defined(OFFICIALTESTING) || defined(OFFICIALBETA) || defined(OFFICIALDEBUG)
    #define CAN_UPDATE
#endif

#endif

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



#include "q_shared.h"
#include "sys_net.h"
#include "msg.h"

#ifndef __SV_AUTH_H__
#define __SV_AUTH_H__


#define MAX_AUTH_ADMINS 512


typedef struct{
	char username[32];
	char salt[129];
	char sha256[65];
	//int power; Stored inside the adminPower_t object
	int uid;
}authData_admin_t; // size: 32 + 129 + 65 = 289 B

typedef struct{
	authData_admin_t admins[MAX_AUTH_ADMINS];
	int maxUID;
}authData_t; // size: MAX_AUTH_ADMINS * 289 = 512 * 289 = 147.968 ~= 148kB


void Auth_SetAdmin_f( void );
void Auth_UnsetAdmin_f( void );
void Auth_ListAdmins_f( void );
void Auth_ChangeAdminPassword( int uid,const char* oldPassword, const char *password );

qboolean Auth_AddAdminToList(const char* username, const char* password, const char* salt, int power, int uid);
void Auth_ClearAdminList( void );

void Auth_Init();
qboolean Auth_InfoAddAdmin(const char* line);
void Auth_WriteAdminConfig(char* buffer, int size);

#endif
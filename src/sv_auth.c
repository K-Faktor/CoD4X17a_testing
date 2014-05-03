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



#include "sv_auth.h"
#include "q_shared.h"
#include "qcommon.h"
#include "qcommon_io.h"
#include "cmd.h"
#include "nvconfig.h"
#include "msg.h"
#include "sys_net.h"
#include "server.h"
#include "net_game_conf.h"
#include "sha256.h"
#include "punkbuster.h"
#include "net_game.h"
#include "g_sv_shared.h"
#include "sec_main.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
/*
========================================================================

Temporary solution for admin authorization due to 3xP clan stealing CD-Keys

========================================================================
*/


authData_t auth_admins;

#define AUTH_DEFAULT_POWER 1


int Auth_Authorize(const char *login, const char *password){
    int i;
    char hstring[256];
    const char *sha256;
    authData_admin_t *user;
    int id = -1;
    
    for(i = 0, user = auth_admins.admins; i < MAX_AUTH_ADMINS; i++, user++){
	if(*user->username && !Q_stricmp(user->username,login))
	    id = i;
    }
    if(id < 0){
	return id;
    }
    user = &auth_admins.admins[id];
    
    Com_sprintf(hstring, sizeof(hstring), "%s.%s", password, user->salt);

    sha256 = Com_SHA256(hstring);

    if(Q_strncmp(user->sha256, sha256, 128))
	return -1;

    return id;
}


int Auth_GetUID(char *name){
    int i;
    authData_admin_t *user;
    int uid = -1;
    
    for(i = 0, user = auth_admins.admins; i < MAX_AUTH_ADMINS; i++, user++){
	if(*user->username && !Q_stricmp(user->username,name))
	    uid = user->uid;
    }
    return uid;
}


void Auth_SetAdmin_f( void ){

	const char* username;
	const char* password;
	const char* sha256;
	byte buff[129];
	char salt[65];
	unsigned long size = sizeof(salt);
	int power, i,uid;
	authData_admin_t* user;
	authData_admin_t* free = NULL;
	mvabuf;


	if(Cmd_Argc() != 4){
		Com_Printf("Usage: %s <username> <password> <power>\n", Cmd_Argv(0));
		Com_Printf( "Where username is loginname for this user\n" );
		Com_Printf( "Where password is the initial 6 characters long or longer password for this user which should get changed by the user on first login\n" );		
		Com_Printf( "Where power is one of the following: Any number between 1 and 100\n" );
		return;
	}

	username = Cmd_Argv(1);
	password = Cmd_Argv(2);
	power = atoi(Cmd_Argv(3));
	
	if(!username || !*username || !password || strlen(password) < 6 || power < 1 || power > 100){
		Com_Printf("Usage: %s <username> <password> <power>\n", Cmd_Argv(0));
		Com_Printf( "Where username is loginname for this user\n" );
		Com_Printf( "Where password is the initial 6 characters long or longer password for this user which should get changed by the user on first login\n" );		
		Com_Printf( "Where power is one of the following: Any number between 1 and 100\n" );
		return;
	}

	NV_ProcessBegin();
	
	uid = ++auth_admins.maxUID;
	    
		
	for(i = 0, user = auth_admins.admins; i < MAX_AUTH_ADMINS; i++, user++){

		if(!Q_stricmp(user->username, username)){
			Com_Printf("An admin with this username is already registered\n");
			return;
		}

		if(!free && !*user->username )
			free = user;
	}
	if(!free){
		Com_Printf("Too many registered admins. Limit is: %d\n", MAX_AUTH_ADMINS);
		return;
	}

	Com_RandomBytes(buff, sizeof(buff));
	//Sec_BinaryToHex((char *)buff,sizeof(buff),salt,&size);
	Sec_HashMemory(SEC_HASH_SHA256,buff,sizeof(buff),salt,&size,qfalse);
	/*for(i = 0; i < sizeof(salt) -1; i++){
		if(salt[i] > 126){
		    salt[i] -= 125;
		}
		if(salt[i] < ' '){
		    salt[i] += ' ';
		}
		if(salt[i] == ';')
			salt[i]++;

		if(salt[i] == '\\')
			salt[i]++;

		if(salt[i] == '%')
			salt[i]++;

		if(salt[i] == '"')
			salt[i]++;
		
	}

	salt[sizeof(salt) -1] = 0;*/

	sha256 = Com_SHA256(va("%s.%s", password, salt));

	Q_strncpyz(free->username, username, sizeof(free->username));
	
	Com_Printf("Debug: 1:%s 2:%s\n", username, free->username);
	
	Q_strncpyz(free->sha256, sha256, sizeof(free->sha256));
	Q_strncpyz(free->salt, (char*)salt, sizeof(free->salt));
	//free->power = power; Instead:
	SV_RemoteCmdSetAdmin(uid, NULL, power);
	
	free->uid = uid;
	Com_Printf("Registered user with Name: %s Power: %d UID: %d\n", free->username, power, uid);
	NV_ProcessEnd();
}


void Auth_UnsetAdmin_f( void ){

	const char* username;
	int i;
	authData_admin_t* user;

	if(Cmd_Argc() != 2){
		Com_Printf("Usage: %s  < username >\n");
		return;
	}

	username = Cmd_Argv(1);

	NV_ProcessBegin();

	for(i = 0, user = auth_admins.admins; i < MAX_AUTH_ADMINS; i++, user++){

		if(!Q_stricmp(user->username, username)){
			Com_Printf("Removed %s from the list of admins\n", user->username);
			Com_Memset(user, 0, sizeof(authData_admin_t));
			NV_ProcessEnd();
			return;
		}
	}
	Com_Printf("No such admin: %s\n", username);
}


void Auth_ListAdmins_f( void ){

	int i;
	authData_admin_t* user;

	Com_Printf("------- Admins: -------\n");
	for(i = 0, user = auth_admins.admins; i < MAX_AUTH_ADMINS; i++, user++){
		if(*user->username)
			Com_Printf("  %2d:   Name: %s, Power: %d, UID: @%d\n", i+1, user->username, SV_RemoteCmdGetClPowerByUID( user->uid ), user->uid);
	}
	Com_Printf("---------------------------------\n");
}


void Auth_ChangeAdminPassword( int uid,const char* oldPassword,const char* password ){

	const char* sha256;
	byte buff[129];
	char salt[65];
	unsigned long size = sizeof(salt);
	authData_admin_t *user, *user2;
	int i;
	//int uid = -1;
	mvabuf;

	
	if(!password || strlen(password) < 6){
		Com_Printf("Error: the new password must have at least 6 characters\n");
		return;
	}

	NV_ProcessBegin();
	
	for(i = 0, user2 = auth_admins.admins; i < MAX_AUTH_ADMINS; i++, user2++){
		if(*user2->username && user2->uid == uid){
		    user = user2;
		}
	}
	if(user == NULL){
	    Com_Printf("Error: unknown admin @%d!\n",uid);
	    return;
	}


	Com_RandomBytes(buff, sizeof(buff));

	Sec_HashMemory(SEC_HASH_SHA256,buff,sizeof(buff),salt,&size,qfalse);
	/*salt[sizeof(salt) -1] = 0; // Not needed

	for(i = 0; i < sizeof(salt) -1; i++){
		if(salt[i] > 126){
		    salt[i] -= 125;
		}
		if(salt[i] < ' '){
		    salt[i] += ' ';
		}
		if(salt[i] == ';')
			salt[i]++;

		if(salt[i] == '\\')
			salt[i]++;

		if(salt[i] == '%')
			salt[i]++;

		if(salt[i] == '"')
			salt[i]++;
	}*/

	sha256 = Com_SHA256(va("%s.%s", password, salt));

	Q_strncpyz(user->sha256, sha256, sizeof(user->sha256));
	Q_strncpyz(user->salt, (char *)salt, sizeof(user->salt));

	NV_ProcessEnd();

	Com_Printf("Password changed to: %s\n", password);
}

void Auth_ChangePassword_f()
{
    int uid;
    
    if(Cmd_Argc()!=4){
	Com_Printf("Usage: %s <username> <oldPassword> <newPassword>\n",Cmd_Argv(0));
	return;
    }
    
    uid = Auth_GetUID(Cmd_Argv(1));
    if(uid < 0){
	Com_Printf("Admin %s not found.\n",Cmd_Argv(1));
	return;
    }
    Auth_ChangeAdminPassword(uid,Cmd_Argv(2),Cmd_Argv(3));
}

void Auth_ClearAdminList( )
{
    Com_Memset(auth_admins.admins, 0, sizeof(auth_admins.admins));
    auth_admins.maxUID = 300000000;
}

qboolean Auth_AddAdminToList(const char* username, const char* password, const char* salt, int power, int uid){

	authData_admin_t* user;
	authData_admin_t* free = NULL;
	int i;

	if(!username || !*username || !password || strlen(password) < 6 /* || power < 1 || power > 100 Nope! */ || !salt || strlen(salt) != 64)
		return qfalse;

	for(i = 0, user = auth_admins.admins; i < MAX_AUTH_ADMINS; i++, user++){

		if(!Q_stricmp(user->username, username)){
			return qfalse;
		}

		if(!free && !*user->username ){
			free = user;
			break; // We don't need to go to the end if we have already found an empty spot
		}
	}
	if(!free)
		return qfalse;

	Q_strncpyz(free->username, username, sizeof(free->username));
	Q_strncpyz(free->sha256, password, sizeof(free->sha256));
	Q_strncpyz(free->salt, salt, sizeof(free->salt));
	free->uid = uid;
	if(uid > auth_admins.maxUID)
	    auth_admins.maxUID = uid;
	
	if(power < 1 || power > 100)
		return qtrue;
	/* power was found! add him (backward compatibility) */
	SV_RemoteCmdSetAdmin(uid, NULL, power);
	return qtrue;
}

void Auth_Login_f(){
    
    client_t *invoker;
    int id,clientNum;
    
    
    if(Cmd_Argc() != 3){
	Com_Printf("Usage: %s <login> <password>\n",Cmd_Argv(0));
	return;
    }

    clientNum = SV_RemoteCmdGetInvokerClnum();
    if(clientNum < 0 || clientNum > 63){
	Com_Error(ERR_FATAL,"Auth_Login_f: index out of bounds.\n");
	return;
    }
    
    invoker = &svs.clients[clientNum];
    
    id = Auth_Authorize(Cmd_Argv(1),Cmd_Argv(2));
    if(id < 0 || id > MAX_AUTH_ADMINS){
	//Com_PrintLogFile("Failed login attempt from slot %d with login %s. Client dropped.",clientNum,Cmd_Argv(1));
	SV_DropClient(invoker,"Incorrect login credentials.\n");
	return;
    }

    invoker->uid = auth_admins.admins[id].uid;
    invoker->power = SV_RemoteCmdGetClPower(invoker);
    Com_Printf("^2Successfully authorized. UID: %d, name: %s, power: %d\n",
			   auth_admins.admins[id].uid, auth_admins.admins[id].username, invoker->power);
}



void Auth_Init(){

	static qboolean	initialized;

	if ( initialized ) {
		return;
	}
	initialized = qtrue;
	
	Auth_ClearAdminList();

	Cmd_AddCommand ("authUnsetAdmin", Auth_UnsetAdmin_f);
	Cmd_AddCommand ("authSetAdmin", Auth_SetAdmin_f);
	Cmd_AddCommand ("authListAdmins", Auth_ListAdmins_f);
	Cmd_AddCommand ("authLogin", Auth_Login_f);
	Cmd_AddCommand ("authChangePassword", Auth_ChangePassword_f);
}


void Auth_WriteAdminConfig(char* buffer, int size)
{
    char infostring[MAX_INFO_STRING];
    int i;
    authData_admin_t *admin;
	mvabuf;

	
    Q_strcat(buffer, size, "\n//Admins authorization data\n");

    for ( admin = auth_admins.admins, i = 0; i < MAX_AUTH_ADMINS ; admin++, i++ ){

        *infostring = 0;

	if(!*admin->username)
		continue;

        Info_SetValueForKey(infostring, "type", "authAdmin");
//        Info_SetValueForKey(infostring, "power", va("%i", admin->power)); We don't write it anymore. It goes now to "admin"
        Info_SetValueForKey(infostring, "uid", va("%i", admin->uid));
        Info_SetValueForKey(infostring, "password", admin->sha256);
        Info_SetValueForKey(infostring, "salt", admin->salt);
        Info_SetValueForKey(infostring, "username", admin->username);
        Q_strcat(buffer, size, infostring);
        Q_strcat(buffer, size, "\\\n");
    }
}

qboolean Auth_InfoAddAdmin(const char* line)
{
        char password[65];
        char salt[65];
        char username[32];
        int power;
        int uid;

        power = atoi(Info_ValueForKey(line, "power"));
        uid = atoi(Info_ValueForKey(line, "uid"));
        Q_strncpyz(password, Info_ValueForKey(line, "password") , sizeof(password));
        Q_strncpyz(salt, Info_ValueForKey(line, "salt") , sizeof(salt));
        Q_strncpyz(username, Info_ValueForKey(line, "username") , sizeof(username));

        if(!Auth_AddAdminToList(username, password, salt, power,uid)){
            Com_Printf("Error: duplicated username or bad power or too many admins\n");
            return qfalse;
        }
        return qtrue;
}


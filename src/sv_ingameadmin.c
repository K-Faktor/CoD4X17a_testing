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
#include "qcommon.h"
#include "qcommon_io.h"
#include "qcommon_mem.h"
#include "qcommon_logprint.h"
#include "cvar.h"
#include "cmd.h"
#include "server.h"
#include "punkbuster.h"
#include "nvconfig.h"

#include <string.h>
#include <stdarg.h>

#define SV_OUTPUTBUF_LENGTH 1024

#ifndef MAXPRINTMSG
#define MAXPRINTMSG 1024
#endif

static cvar_t* sv_rconsys;

typedef struct adminPower_s {
    struct	adminPower_s *next;
    char	name[16];
    int	uid;
    char guid[9];
    int	power;
}adminPower_t;


typedef struct{
	int		currentCmdPower;			//used to set an execution permissionlevel - Default is 100 but if users execute commands it will be the users level
	int		currentCmdInvoker;			//used to set an Invoker UID - Default is 0 but if users execute commands it will be his own UID
	char		currentCmdInvokerGuid[9];		//Same as above but if guid is used
	int		clientnum;				//Clientnum will be -1 if rcon is used
	qboolean	authserver;
}cmdInvoker_t;


static cmdInvoker_t cmdInvoker;
static adminPower_t *adminpower;
static client_t *redirectClient;
static qboolean cmdSystemInitialized;

int SV_RemoteCmdGetInvokerUid()
{
    return cmdInvoker.currentCmdInvoker;
}

const char* SV_RemoteCmdGetInvokerGuid()
{
	Com_Error("SV_RemoteCmdGetInvokerGuid is deprecated and should no longer get called\n");
    return "";
}

int SV_RemoteCmdGetInvokerClnum()
{
    return cmdInvoker.clientnum;
}


int SV_RemoteCmdGetInvokerPower()
{
    return cmdInvoker.currentCmdPower;
}





void SV_RemoteCmdInit(){

    sv_rconsys = Cvar_RegisterBool("sv_rconsys", qtrue, CVAR_ARCHIVE, "Disable/enable the internal remote-command-system");

    if(!sv_rconsys->boolean) return;

    //Init the permission table with default values
    Cmd_ResetPower();

    Cmd_SetPower("cmdlist", 1);
    Cmd_SetPower("serverinfo", 1);
    Cmd_SetPower("systeminfo", 1);
    Cmd_SetPower("ministatus", 1);
    Cmd_SetPower("status", 30);
    Cmd_SetPower("dumpuser", 40);
    Cmd_SetPower("kick", 35);
    Cmd_SetPower("tempban", 50);
    Cmd_SetPower("unban", 50);
    Cmd_SetPower("permban", 80);
    Cmd_SetPower("btempban", 80);
    Cmd_SetPower("bpermban", 70);
    Cmd_SetPower("map", 60);
    Cmd_SetPower("map_restart", 50);
    Cmd_SetPower("adminlist", 90);
    Cmd_SetPower("cmdpowerlist", 90);
    Cmd_SetPower("tell", 60);
    Cmd_SetPower("say", 60);
    Cmd_SetPower("screentell", 70);
    Cmd_SetPower("screensay", 70);
    Cmd_SetPower("dumpbanlist", 30);
    Cmd_SetPower("setcmdminpower", 95);
    Cmd_SetPower("setadmin", 95);
    Cmd_SetPower("unsetadmin", 95);
    Cbuf_AddText("addCommand gametype \"set g_gametype $arg; map_restart\"");
    Cmd_SetPower("gametype", 60);
    Cmd_SetPower("authLogin", 1);
    Cmd_SetPower("authChangePassword", 10);
    Cmd_SetPower("authSetAdmin",95);
    Cmd_SetPower("authUnsetAdmin",95);

    cmdInvoker.currentCmdPower = 100; //Set the default to 100 to prevent any blocking on local system. If a command gets remotely executed it will be set temporarely to the expected power
    //Now read the rest from file - Changed this will happen by executing nvconfig.cfg (nonvolatile config)
    cmdSystemInitialized = qtrue;
}

void SV_RemoteCmdClearAdminList()
{

    adminPower_t *admin, **this;

    for ( this = &adminpower, admin = *this; admin ; admin = *this ){
        *this = admin->next;
        Z_Free(admin);
    }
}

/*
============
SV_RemoteCmdGetClPower
============
*/

int SV_RemoteCmdGetClPower(client_t* cl){

    adminPower_t *admin;
    int uid;
    char* guid;

    guid = &cl->pbguid[24];
    uid = cl->uid;
    // Permanent solution due to 3xp clan stealing cd-keys in vast quantities. Thanks DuffMan.
    if(uid < 1) return 1;
    if(cl->power > 1) return cl->power;
	
    for(admin = adminpower; admin ; admin = admin->next)
	{
		if(admin->uid == uid){
    	    return admin->power;
        }
    }
    return 1;
}


/*
============
SV_RemoteCmdGetClPowerByUID
============
*/

int SV_RemoteCmdGetClPowerByUID(int uid){

    adminPower_t *admin;
    if(uid < 1) return 0;

    for(admin = adminpower; admin ; admin = admin->next){

        if(admin->uid == uid){
            return admin->power;
        }
    }
    return 1;
}




/*
=================
SV_ReliableSendRedirect

Sends redirected text of console to client as reliable commands
This is used for RemoteCommand responses while client is connected to server
=================
*/

void SV_ReliableSendRedirect(char *sendbuf, qboolean lastcommand){

    int lastlinebreak;
    int remaining = strlen(sendbuf);
    int	maxlength;
    int i;
    char outputbuf[244];

    for(; remaining > 0;)
	{	//We have to split the string into smaller packages of max 240 bytes
		//This function tries to ensure that every package ends on the last possible linebreak for better formating
		maxlength = remaining;

		if(maxlength > 240) maxlength = 240;

		for(lastlinebreak=0 ,i=0; i < maxlength; i++){
			outputbuf[i] = sendbuf[i];
			if(outputbuf[i] == '"') outputbuf[i] = 0x27;	//replace all " with ' because no " are accepted
			if(outputbuf[i] == 0x0a) lastlinebreak = i;		//search for the last linebreak
		}
		if(lastlinebreak > 0){ 
			i = lastlinebreak;	//found a linebreak and send everything until that position
			remaining -= i-1;
			sendbuf += i+1;
			outputbuf[i+1] = 0x00;
		} else {		//Not a linebreak found send full 240 chars
			remaining -= i;
			sendbuf += i;
			outputbuf[i] = 0x00;
		}
		SV_SendServerCommand(redirectClient, "e \"%s\"", outputbuf);
    }
}


qboolean SV_ExecuteRemoteCmd(int clientnum, const char *msg){
	char sv_outputbuf[SV_OUTPUTBUF_LENGTH];
	char cmd[30];
	char buffer[256];
	char *printPtr;
	char oldinvokerguid[9];
	int i = 0;
	int j = 0;
	int powercmd;
	int power;
	client_t *cl;

        if(!cmdSystemInitialized){
            SV_SendServerCommand(redirectClient, "e \"Error: Remote control system is not initialized\n\"");
            Com_Printf("Error: Remote control system is not initialized\n");
            return qfalse;
        }


	if(clientnum < 0 || clientnum > 63) return qfalse;
	cl = &svs.clients[clientnum];
	redirectClient = cl;

	while ( msg[i] != ' ' && msg[i] != '\0' && msg[i] != '\n' && i < 32 ){
		i++;
	}
	
	if(i > 29 || i < 3) return qfalse;
	Q_strncpyz(cmd,msg,i+1);


	//Prevent buffer overflow as well as prevent the execution of priveleged commands by using seperator characters
	Q_strncpyz(buffer,msg,256);
	Q_strchrrepl(buffer,';','\0');
	Q_strchrrepl(buffer,'\n','\0');
	Q_strchrrepl(buffer,'\r','\0');
	// start redirecting all print outputs to the packet

    power = SV_RemoteCmdGetClPower(cl);
    powercmd = Cmd_GetPower(cmd);
	
    if(!Q_stricmpn(cmd,"auth",4)){
       printPtr = cmd;
        
    }else{
	    printPtr = buffer;
    }

	if(powercmd == -1){
            SV_SendServerCommand(redirectClient, "e \"^5Command^2: %s\n^3Command execution failed - Invalid command invoked - Type ^2$cmdlist ^3to get a list of all available commands\"", printPtr);
            return qfalse;
	}
	if(powercmd > power){
            SV_SendServerCommand(redirectClient, "e \"^5Command^2: %s\n^3Command execution failed - Insufficient power to execute this command.\n^3You need at least ^6%i ^3powerpoints to invoke this command.\n^3Type ^2$cmdlist ^3to get a list of all available commands\"",
            printPtr, powercmd);
	    return qtrue;
	}
	Com_Printf( "Command execution: %s   Invoked by: %s   InvokerUID: %i Power: %i\n", printPtr, cl->name, cl->uid, power);

	Com_BeginRedirect(sv_outputbuf, SV_OUTPUTBUF_LENGTH, SV_ReliableSendRedirect);

	i = cmdInvoker.currentCmdPower;
	cmdInvoker.currentCmdPower = power;
	cmdInvoker.authserver = qfalse;

	j = cmdInvoker.currentCmdInvoker;
	cmdInvoker.currentCmdInvoker = cl->uid;

	cmdInvoker.clientnum = clientnum;

	Cmd_ExecuteSingleCommand( 0, 0, buffer );
#ifdef PUNKBUSTER
	if(!Q_stricmpn(buffer, "pb_sv_", 6)) PbServerForceProcess();
#endif
	SV_SendServerCommand(redirectClient, "e \"^5Command^2: %s\"", buffer);

	cmdInvoker.currentCmdPower = i;
	cmdInvoker.currentCmdInvoker = j;
	Q_strncpyz(cmdInvoker.currentCmdInvokerGuid, "N/A", sizeof(cmdInvoker.currentCmdInvokerGuid));

	cmdInvoker.clientnum = -1;

	Com_EndRedirect();
	return qtrue;
}

/* Not in use anymore. Was only working when connected to authserver. However maybe we will use it again later.

void SV_ExecuteBroadcastedCmd(int uid, const char *msg){

	int i = 0;
	int j = 0;
	int powercmd;
	int power;

	while ( msg[i] != ' ' && msg[i] != '\0' && msg[i] != '\n' && i < 32 ){
		i++;
	}

	char cmd[30];
	char buffer[256];

	if(i > 29 || i < 3) return;
	Q_strncpyz(cmd,msg,i+1);
	//Prevent buffer overflow as well as prevent the execution of priveleged commands by using seperator characters
	Q_strncpyz(buffer,msg,256);
	Q_strchrrepl(buffer,';','\0');
	Q_strchrrepl(buffer,'\n','\0');
	Q_strchrrepl(buffer,'\r','\0');

	if(!uid){
		power = 100;
	}else{
		power = SV_RemoteCmdGetClPowerByUID(uid);
	}
        powercmd = Cmd_GetPower(cmd);


	if(powercmd == -1){
            return;
	}
	if(powercmd > power){
		Com_Printf( "Broadcasted command execution: %s   InvokerUID: %i Power: %i   Insufficient Power\n", buffer, uid, power);
		return;
	}

	Com_Printf( "Broadcasted command execution: %s   Invoked by: BroadcastMessage   InvokerUID: %i Power: %i\n", buffer, uid, power);

	i = cmdInvoker.currentCmdPower;
	j = cmdInvoker.currentCmdInvoker;
	cmdInvoker.currentCmdPower = power;
	cmdInvoker.currentCmdInvoker = uid;
	cmdInvoker.clientnum = -1;
	cmdInvoker.authserver = qtrue;

	Cmd_ExecuteSingleCommand( 0, 0, buffer );

	cmdInvoker.authserver = qfalse;
	cmdInvoker.currentCmdPower = i;
	cmdInvoker.currentCmdInvoker = j;
	cmdInvoker.clientnum = -1;
}
*/

qboolean SV_RemoteCmdAddAdmin(int uid, char* guid, int power)
{
        adminPower_t *admin;

        if(uid < 1){
            Com_Printf("Error: Invalid uid\n");
            return qfalse;
        }

        if(power < 1 || power > 100){
            Com_Printf("Error: Invalid powerlevel(%i). Powerlevel can not be less than 1 or greater than 100.\n");
            return qfalse;
        }

        admin = Z_Malloc(sizeof(adminPower_t));

        if(admin)
        {
            admin->uid = uid;
            admin->power = power;
            admin->next = adminpower;
            adminpower = admin;
            admin->guid[0] = '\0';
            return qtrue;

        }else{
            return qfalse;
        }
}

qboolean SV_RemoteCmdInfoAddAdmin(const char* infostring)
{
        char guid[16];
        int uid;
        int power;

        uid = atoi(Info_ValueForKey(infostring, "uid"));
        Q_strncpyz(guid, Info_ValueForKey(infostring, "guid"), sizeof(guid));
        power = atoi(Info_ValueForKey(infostring, "power"));

		if(strlen(guid >= 8) && uid < 1)
		{
			Com_Printf("^1WARNING: ^7GUID based admin authorization has been disabled. Go to https://guidError.iceops.in/ for details.\n");
		}
		
		if(uid < 1)
		{
			Com_Printf("^1WARNING: Read invalid uid from admin config\n");
			return qfalse;
		}
        return SV_RemoteCmdAddAdmin(uid, guid, power);


}

void SV_RemoteCmdWriteAdminConfig(char* buffer, int size)
{
    char infostring[1024];
    adminPower_t *admin;
	mvabuf;

	
    Q_strcat(buffer, size, "\n//Admins | Users power settings\n");

    for ( admin = adminpower ; admin ; admin = admin->next )
    {
	    if(admin->uid <= 0)
			continue;
	
        *infostring = 0;
        Info_SetValueForKey(infostring, "type", "admin");
        Info_SetValueForKey(infostring, "uid", va("%i",admin->uid));
        Info_SetValueForKey(infostring, "power", va("%i",admin->power));
        Q_strcat(buffer, size, infostring);
        Q_strcat(buffer, size, "\\\n");
    }

}


void QDECL SV_PrintAdministrativeLog( const char *fmt, ... ) {

	va_list		argptr;
	char		msg[MAXPRINTMSG];
	char		inputmsg[MAXPRINTMSG];
	struct tm 	*newtime;
	char*		ltime;
	time_t		realtime;

	va_start (argptr,fmt);
	Q_vsnprintf (inputmsg, sizeof(inputmsg), fmt, argptr);
	va_end (argptr);

	Com_UpdateRealtime();
	realtime = Com_GetRealtime();
	newtime = localtime( &realtime );
	ltime = asctime( newtime );
	ltime[strlen(ltime)-1] = 0;

	Com_sprintf(msg, sizeof(msg), "%s - Admin %i with %i power %s\n", ltime, cmdInvoker.currentCmdInvoker, cmdInvoker.currentCmdPower, inputmsg);

	Com_PrintAdministrativeLog( msg );

}

/*
============
Cmd_RemoteSetAdmin_f
============
*/

void SV_RemoteCmdSetAdmin(int uid, char* guid, int power)
{

    adminPower_t *admin;
    adminPower_t *this;

    if(uid < 1){
        Com_Printf("No such player or player without uid\n");
        return;
    }

    NV_ProcessBegin();

    for(admin = adminpower ; admin ; admin = admin->next){
            if(admin->uid == uid){
                if(admin->power != power){
                    admin->power = power;

                    Com_Printf( "Admin power changed for: uid: %i to level: %i\n", uid, power);
                    SV_PrintAdministrativeLog( "changed power of admin with uid: %i to new power: %i", uid, power);
                }
                NV_ProcessEnd();
                return;
            }
    }

    this = Z_Malloc(sizeof(adminPower_t));
    if(this){
            this->uid = uid;
            this->power = power;
            this->next = adminpower;
            adminpower = this;
            Com_Printf( "Admin added: uid: %i level: %i\n", uid, power);
            SV_PrintAdministrativeLog( "added a new admin with uid: %i and power: %i", uid, power);
    }

    NV_ProcessEnd();
}




/*
============
Cmd_RemoteUnsetAdmin_f
============
*/
void SV_RemoteCmdUnsetAdmin(int uid, char* guid)
{

    adminPower_t *admin, **this;

    if(uid < 1){
            Com_Printf("No such player\n");
            return;
    }

    NV_ProcessBegin();

    for(this = &adminpower, admin = *this; admin ; admin = *this)
    {

        if(admin->uid == uid){
                *this = admin->next;
                Z_Free(admin);
                NV_ProcessEnd();
                Com_Printf( "User removed: uid: %i\n", uid);
                SV_PrintAdministrativeLog( "removed admin with uid: %i", uid);
                return;
        }
        this = &admin->next;
    }

    Com_Printf( "Error: No such user in database\n");
    NV_ProcessEnd();
}

/*
============
Cmd_RemoteSetPermission
Changes minimum-PowerLevel of a command
============
*/
void SV_RemoteCmdSetPermission(char* command, int power)
{


    NV_ProcessBegin();
    if(Cmd_SetPower(command, power))
    {
        SV_PrintAdministrativeLog("changed required power of cmd: %s to new power: %i", command, power);
        Com_Printf("changed required power of cmd: %s to new power: %i\n", command, power);
    }else{
        Com_Printf("Failed to change power of cmd: %s Maybe this is not a valid command.\n", command);
    }
    NV_ProcessEnd();
}

/*
============
Cmd_RemoteListAdmin_f
============
*/
void SV_RemoteCmdListAdmins()
{

    adminPower_t *admin;
    int i;

    for(i = 0, admin = adminpower ; admin ; i++, admin = admin->next){
                Com_Printf( "Admin : uid: %i level: %i\n", admin->uid, admin->power);
    }
    Com_Printf( "%i registered Admins\n", i);
}

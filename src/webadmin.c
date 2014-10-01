/*
 *  webadmin.c
 *  CoD4X17a_testing
 *
 *  Created by Florian on 5/23/14.
 *  Copyright 2014 Dorg. All rights reserved.
 *
 */

#include "q_shared.h"
#include "httpftp.h"
#include "qcommon_mem.h"
#include "qcommon_io.h"
#include "msg.h"
#include "filesystem.h"
#include "sv_auth.h"
#include "webadmin.h"
#include "server.h"
#include "cmd.h"

#include <string.h>

#define TOSTRING(X) #X
#define XA(x) XML_AppendToBuffer(xmlobj, x)
#define XO(x) XML_OpenTag(xmlobj,x,0)
#define XO1(x, x1, x2) XML_OpenTag(xmlobj,x,1, x1, x2)
#define XO2(x, x1, x2, y1, y2) XML_OpenTag(xmlobj,x,2, x1, x2, y1, y2)
#define XO3(x, x1, x2, y1, y2, z1 ,z2) XML_OpenTag(xmlobj,x,3, x1, x2, y1, y2, z1, z2)
#define XC XML_CloseTag(xmlobj) 
#define XOLINK(x) XO1( TOSTRING(a), TOSTRING(href) , x)


const char* Webadmin_GetPostVal(httpPostVals_t* values, const char* search)
{
	int i;
	
	/* Get the form password and username */
	for (i = 0 ; i < MAX_POST_VALS; i++)
	{
		if(values[i].name[0] == '\0')
		{
			break;
		}
		
		if(!Q_stricmp(values[i].name, search))
		{
			return values[i].value;
		}
	}
	return NULL;
}

const char* Webadmin_GetUrlVal(const char* url, const char* search, char* outputbuf, int outputbuflen)
{
	const char *needle, *value;
	char *outbufterm;
	int len;
	
	len = strlen(search);
	
	needle = url;
	
	while( (needle = strchr(needle +1, '?') ) != NULL )
	{
		if(strncmp(needle +1, search, len) == 0)
		{	
			break;
		}
	}
	
	if(needle == NULL)
	{
		/* In case nothing was found */
		return NULL;
	}
	
	/* search has been found */
	value = strchr(needle, '=');
	if(value == NULL)
	{
		/* In case url miss the value */
		return NULL;
	}
	Q_strncpyz(outputbuf, value +1, outputbuflen);
	
	outbufterm = strchr(outputbuf, '?');
	if(outbufterm)
	{
		*outbufterm = '\0';
	}
	return value;
}

void Webadmin_BuildServerStatus(xml_t* xmlobj)
{
	int i;
	client_t* cl;
	char strbuf[MAX_STRING_CHARS];
	
	XO("table");
	XA("<th>CID</th><th>Name</th><th>Score</th><th>Guid</th><th>Ping</th>");
		
		for (cl = svs.clients, i = 0; i < sv_maxclients->integer; i++, cl++)
		{
			if (cl->state < CS_CONNECTED) {
				continue;
			}
			
			XO("tr");

				Com_sprintf(strbuf, sizeof(strbuf), "<td>%d</td>", i);
				XA(strbuf);
			
				XO("td");
					XA(cl->name);
				XC;
			
				Com_sprintf(strbuf, sizeof(strbuf), "<td>%d</td>", -1);
				XA(strbuf);

				XO("td");
					XA(cl->pbguid);
				XC;
			
				Com_sprintf(strbuf, sizeof(strbuf), "<td>%d</td>", cl->ping);
				XA(strbuf);
			XC;
		}
	XC;
}


static xml_t* xmlobjFlush;

void Webadmin_FlushRedirect(char *outputbuf, qboolean lastcommand )
{
	xml_t* xmlobj = xmlobjFlush;
	
	if(xmlobj == NULL)
	{
		Com_Error(ERR_FATAL, "Webadmin_FlushRedirect has not been initialized");
	}
		
	XA("<pre>");
	XA(outputbuf);	
	XA("</pre>");

}

#define SV_OUTPUTBUF_LENGTH 4096

void Webadmin_ConsoleCommand(xml_t* xmlobj, const char* command, int uid)
{
	char sv_outputbuf[SV_OUTPUTBUF_LENGTH];
	char buffer[960];
	char cmd[48];

	int power, powercmd, oldpower, oldinvokeruid, oldinvokerclnum, i;

	if(sv_rconsys->boolean != qfalse && (power = SV_RemoteCmdGetClPowerByUID(uid)) < 100)
	{
		/* Get the current user's power 1st */
		while ( command[i] != ' ' && command[i] != '\0' && command[i] != '\n' && i < 32 ){
			i++;
		}
		if(i > 29 || i < 3) return;

		Q_strncpyz(cmd,command,i+1);

		//Prevent buffer overflow as well as prevent the execution of priveleged commands by using seperator characters
		Q_strncpyz(buffer, command, sizeof(buffer));
		Q_strchrrepl(buffer,';','\0');
		Q_strchrrepl(buffer,'\n','\0');
		Q_strchrrepl(buffer,'\r','\0');
		// start redirecting all print outputs to the packet

		powercmd = Cmd_GetPower(cmd);
		if(powercmd > power)
		{
			XA(" Insufficient permissions! ");
			return;
		}

		xmlobjFlush = xmlobj;

		oldpower = SV_RemoteCmdGetInvokerPower();
		oldinvokeruid = SV_RemoteCmdGetInvokerUid();
		oldinvokerclnum = SV_RemoteCmdGetInvokerClnum();

		SV_RemoteCmdSetCurrentInvokerInfo(uid, power, -1);

		Com_BeginRedirect (sv_outputbuf, SV_OUTPUTBUF_LENGTH, Webadmin_FlushRedirect);
		Cmd_ExecuteSingleCommand(0,0, buffer);

		SV_RemoteCmdSetCurrentInvokerInfo(oldinvokeruid, oldpower, oldinvokerclnum);

	}else{
		xmlobjFlush = xmlobj;
		Com_BeginRedirect (sv_outputbuf, SV_OUTPUTBUF_LENGTH, Webadmin_FlushRedirect);
		Cmd_ExecuteSingleCommand(0,0, command);
#ifdef PUNKBUSTER
		if(!Q_stricmpn(command, "pb_sv_", 6)) PbServerForceProcess();
#endif
	}
	Com_EndRedirect();
	xmlobjFlush = NULL;
}


void Webadmin_BuildLoginForm(xml_t* xmlobj, qboolean badtry)
{
	
	if(badtry)
	{
		XO("p");
			XA("Invalid Login");
		XC;
	}
	XO("p");
		XO3("form", "name", "input", "action", "webadmin", "method", "post");
			XA("Username: <input type=\"text\" name=\"username\">");
			XA("Password: <input type=\"password\" name=\"password\">");
			XA("<input type=\"submit\" value=\"Submit\">");
		XC;
	XC;
	
}

void Webadmin_BuildMessage(msg_t* msg, const char* username, qboolean invalidloginattempt, const char* url, httpPostVals_t* values)
{

	xml_t xmlbase;
	xml_t* xmlobj = &xmlbase;
	char actionval[64];
	const char *postval;
	int uid;
	
	XML_Init(xmlobj, (char*)msg->data, msg->maxsize, "ISO-8859-1");
	
	XO("body");
	
	if(!Q_strncmp(url, "/webadmin", 9))
	{
		if(username == NULL || username[0] == '\0')
		{
			Webadmin_BuildLoginForm(xmlobj, invalidloginattempt);

		}else {

			uid = Auth_GetUID(username);

			XO("p");
			XO("b");
				XA("Welcome back "); XA(username);
			XC;
			XC;
			XO("p");
			Webadmin_BuildServerStatus(xmlobj);
			XC;
			
			if(Webadmin_GetUrlVal( url, "action", actionval, sizeof(actionval)))
			{
				if(strcmp(actionval, "sendcmd") == 0)
				{
					postval = Webadmin_GetPostVal(values, "consolecommand");
					if(postval)
					{
						Webadmin_ConsoleCommand(xmlobj, postval, uid);
					}
				}else if (strcmp(actionval, "logout") == 0) {
					//Auth_WipeSessionId(username);
				}
				
			}
			
			XO("p");
				XO3("form", "name", "input", "action", "webadmin?action=sendcmd", "method", "post");
					XA("Command: <input type=\"text\" name=\"consolecommand\">");
					XA("<input type=\"submit\" value=\"Submit\">");
				XC;
			XC;

		}
	}else if(!Q_strncmp(url, "/status", 7)){	
		XO("p");
			Webadmin_BuildServerStatus(xmlobj);
		XC;
	}else {
		XA("Where do you want to go ?");
		XO("p");
			XOLINK("/webadmin");
			XA("Admin panel");
			XC;
		XC;
		
		XO("p");
			XOLINK("/status");
			XA("ServerStatus");
			XC;
		XC;
	}

	XC;
	
	msg->cursize = xmlobj->bufposition;

}


qboolean HTTPCreateWebadminMessage(ftRequest_t* request, msg_t* msg, char* sessionkey, httpPostVals_t* values)
{
	byte *buf;
	char qpath[MAX_QPATH];
	int len;
	const char *session;
	
	buf = NULL;
	
	MSG_Init(msg, buf, 0);
	Com_Printf("URL: %s\n", request->url);
	if(!Q_strncmp(request->url, "/files", 6))
	{
		if(request->url[6] != '/' || request->url[7] == '\0')
		{
			return qfalse;
		}
		Com_sprintf(qpath, sizeof(qpath), "/webadmindata/%s", &request->url[7]);
		
		if(strstr(qpath, "..") != NULL || strstr(qpath, "::") != NULL)
		{
			return qfalse;
		}
		len = FS_ReadFile(qpath, (void**)&buf);
		
		if(len < 0)
		{
			return qfalse;
		}
		msg->data = buf;
		msg->cursize = len;
		msg->maxsize = len;
		FS_FreeFileKeepBuf( );
		return qtrue;
	}
	
	len = 0x20000;
	
	buf = Z_Malloc(len);
	if(buf == NULL)
	{
		return qfalse;
	}
	
	msg->data = buf;
	msg->cursize = 0;
	msg->maxsize = len;
	
	if (Q_stricmpn(request->url, "/webadmin", 9))
	{
		Webadmin_BuildMessage(msg, NULL, qfalse, request->url, values);
		return qtrue;
	}
	
	qboolean invalidlogin = qfalse;
	const char* username = NULL;
	const char* password = NULL;

	
	username = Auth_FindSessionID(sessionkey);
	
	if(username == NULL)
	{

		username = Webadmin_GetPostVal(values, "username");
		password = Webadmin_GetPostVal(values, "password");

		if(username && password)
		{
			session = Auth_GetSessionId(username, password);
			if(session == NULL)
			{
				Com_Printf("^1Invalid login\n");
				invalidlogin = qtrue;
				username = NULL;
			}else {
				Com_Printf("^2Successful login with username: %s\n", username);
			}

			
		}else {
			Com_Printf("No login!\n");
			session = NULL;
			username = NULL;
		}

		/* not longer than 127 or overflow */
		if(session != NULL)
		{
			strcpy(sessionkey, session);
		}
	}else{
		Com_Printf("Already logged in as: %s\n", username);
	}

	Webadmin_BuildMessage(msg, username, invalidlogin, request->url, values);

	return qtrue;
}

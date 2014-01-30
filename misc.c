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




#include "misc.h"
#include "q_shared.h"
#include "qcommon.h"
#include "qcommon_io.h"
#include "g_sv_shared.h"
#include "punkbuster.h"
#include "hl2rcon.h"
#include "cvar.h"

#include <string.h>


char* SL_ConvertToString(unsigned int index)
{

    char** ptr = (char**)STRBUFFBASEPTR_ADDR;
    char* base = *ptr;
    return &base[ index*12 + 4];
}


void AddRedirectLocations()
{
/*
    To Add:
    PbCaptureConsoleOutput(msg, MAXPRINTMSG);*/
/*    HL2Rcon_SourceRconSendConsole( msg, msglen);

    Com_PrintUDP( msg, msglen );
*/


}

qboolean __cdecl Com_LoadDvarsFromBuffer(const char **inputbuf, unsigned int length, const char *data_p, const char *filename)
{
    const char *cvar_name;
    int i, count;
    char buf[16384];
    const char* line;

    Com_Memset(buf, 0, sizeof(buf));

    for(i = 0; i < length; i++)
    {
         Cvar_Reset(inputbuf[i]);
    }
	Com_BeginParseSession(filename);
	count = 0;

	while ( (cvar_name = Com_Parse(&data_p)) && cvar_name[0])
	{
		for(i = 0; i < length; i++)
		{
			if(!Q_stricmp(cvar_name, inputbuf[i]))
				break;
		}
		if(i == length)
		{
			if(com_developer && com_developer->integer)
			{
				Com_PrintWarning("WARNING: unknown cvar '%s' in file '%s'\n", cvar_name, filename);
			}
			Com_SkipRestOfLine(&data_p);
		}else{
			line = Com_ParseOnLine(&data_p);
			if(com_developer && com_developer->integer)
			{
				Cvar_Set(inputbuf[i], line);
			}else{
				if((!Q_strncmp(inputbuf[i],"bg_shock_viewKickPeriod", 23) || !Q_strncmp(inputbuf[i],"bg_shock_viewKickFadeTime", 25)) && (line[0] == '0' && line[1] == '\0'))
				{
					/* Quite this spam */
				}else{
					Cvar_Set(inputbuf[i], line);
				}
			}
			if ( !buf[i] )
			{
				buf[i] = 1;
				++count;
			}
		}
	}
	Com_EndParseSession();

	if ( length == count )
	{
		/* No Errors */
		return 1;
	}

	Com_PrintError("ERROR: the following cvars were not specified in file '%s'\n", filename);
	for(i = 0; i < length; i++)
	{
		while ( buf[i] && i < length )
		{
			++i;
		}
		if(i < length)
			Com_PrintError("  %s\n", inputbuf[i]);
	}
	return 0;
}












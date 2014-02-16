;===========================================================================
;    Copyright (C) 2010-2013  Ninja and TheKelm of the IceOps-Team

;    This file is part of CoD4X17a-Server source code.

;    CoD4X17a-Server source code is free software: you can redistribute it and/or modify
;    it under the terms of the GNU Affero General Public License as
;    published by the Free Software Foundation, either version 3 of the
;    License, or (at your option) any later version.

;    CoD4X17a-Server source code is distributed in the hope that it will be useful,
;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;    GNU Affero General Public License for more details.

;    You should have received a copy of the GNU Affero General Public License
;    along with this program.  If not, see <http://www.gnu.org/licenses/>
;===========================================================================




SECTION .text

global SV_StringUsage_f
SV_StringUsage_f:
	jmp dword [oSV_StringUsage_f]

global SV_ScriptUsage_f
SV_ScriptUsage_f:
	jmp dword [oSV_ScriptUsage_f]

global SV_BeginClientSnapshot
SV_BeginClientSnapshot:
	jmp dword [oSV_BeginClientSnapshot]

global SV_EndClientSnapshot
SV_EndClientSnapshot:
	jmp dword [oSV_EndClientSnapshot]

global SV_ClientThink
SV_ClientThink:
	jmp dword [oSV_ClientThink]

global SV_SpawnServer
SV_SpawnServer:
	jmp dword [oSV_SpawnServer]

global SV_SetGametype
SV_SetGametype:
	jmp dword [oSV_SetGametype]

global SV_InitCvars
SV_InitCvars:
	jmp dword [oSV_InitCvars]

global SV_RestartGameProgs
SV_RestartGameProgs:
	jmp dword [oSV_RestartGameProgs]

global SV_ResetSekeletonCache
SV_ResetSekeletonCache:
	jmp dword [oSV_ResetSekeletonCache]

global SV_PreFrame
SV_PreFrame:
	jmp dword [oSV_PreFrame]

global SV_SendClientMessages
SV_SendClientMessages:
	jmp dword [oSV_SendClientMessages]

global SV_SetServerStaticHeader
SV_SetServerStaticHeader:
	jmp dword [oSV_SetServerStaticHeader]

global SV_ShutdownGameProgs
SV_ShutdownGameProgs:
	jmp dword [oSV_ShutdownGameProgs]

global SV_FreeClients
SV_FreeClients:
	jmp dword [oSV_FreeClients]

global SV_GameSendServerCommand
SV_GameSendServerCommand:
	jmp dword [oSV_GameSendServerCommand]

global SV_SetConfigstring
SV_SetConfigstring:
	jmp dword [oSV_SetConfigstring]

global SV_FreeClient
SV_FreeClient:
	jmp dword [oSV_FreeClient]

global SV_FreeClientScriptId
SV_FreeClientScriptId:
	jmp dword [oSV_FreeClientScriptId]

global SV_LinkEntity
SV_LinkEntity:
	jmp dword [oSV_LinkEntity]

global SV_UnlinkEntity
SV_UnlinkEntity:
	jmp dword [oSV_UnlinkEntity]

global SV_AddServerCommand_old
SV_AddServerCommand_old:
	jmp dword [oSV_AddServerCommand_old]

global SV_GameCommand
SV_GameCommand:
	jmp dword [oSV_GameCommand]

global FS_GetMapBaseName
FS_GetMapBaseName:
	jmp dword [oFS_GetMapBaseName]

SECTION .rodata

oSV_StringUsage_f dw 0x814cc7e
oSV_ScriptUsage_f dw 0x81557c4
oSV_BeginClientSnapshot dw 0x817a32e
oSV_EndClientSnapshot dw 0x817b488
oSV_ClientThink dw 0x816df7a
oSV_SpawnServer dw 0x8174a68
oSV_SetGametype dw 0x817c72c
oSV_InitCvars dw 0x819e66c
oSV_RestartGameProgs dw 0x817c68a
oSV_ResetSekeletonCache dw 0x817c602
oSV_PreFrame dw 0x8177534
oSV_SendClientMessages dw 0x817b71e
oSV_SetServerStaticHeader dw 0x8178fd4
oSV_ShutdownGameProgs dw 0x817c818
oSV_FreeClients dw 0x81720c6
oSV_GameSendServerCommand dw 0x817ce42
oSV_SetConfigstring dw 0x8173fda
oSV_FreeClient dw 0x81708bc
oSV_FreeClientScriptId dw 0x8175c5e
oSV_LinkEntity dw 0x817eb20
oSV_UnlinkEntity dw 0x817d5e0
oSV_AddServerCommand_old dw 0x817664c
oSV_GameCommand dw 0x817c674
oFS_GetMapBaseName dw  0x8127e0c

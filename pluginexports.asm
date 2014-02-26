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

%macro alias 2
	
	EXPORT %2
	global %2
	extern %1
	%2: jmp %1
	
%endmacro

%macro ralias 2
	
	EXPORT %1
	global %1
	extern %2
	%1: jmp %2
	
%endmacro


SECTION .text


ralias Plugin_Printf, Com_Printf
ralias Plugin_PrintWarning, Com_PrintWarning
ralias Plugin_PrintError, Com_PrintError
ralias Plugin_DPrintf, Com_DPrintf

ralias Plugin_ParseGetToken, Com_ParseGetToken
ralias Plugin_ParseTokenLength, Com_ParseTokenLength

ralias Plugin_Argv, Cmd_Argv
ralias Plugin_Argc, Cmd_Argc

ralias Plugin_CvarVariableStringBuffer, Cvar_VariableStringBuffer
ralias Plugin_CvarVariableValue, Cvar_VariableValue
ralias Plugin_CvarVariableIntegerValue, Cvar_VariableIntegerValue
ralias Plugin_CvarVariableString, Cvar_VariableString
ralias Plugin_CvarRegisterString, Cvar_RegisterString
ralias Plugin_CvarRegisterBool, Cvar_RegisterBool
ralias Plugin_CvarRegisterInt, Cvar_RegisterInt
ralias Plugin_CvarRegisterEnum, Cvar_RegisterEnum
ralias Plugin_CvarRegisterFloat, Cvar_RegisterFloat
ralias Plugin_CvarSetInt, Cvar_SetInt
ralias Plugin_CvarSetBool, Cvar_SetBool
ralias Plugin_CvarSetString, Cvar_SetString
ralias Plugin_CvarSetFloat, Cvar_SetFloat

ralias Plugin_FOpenFileRead, FS_SV_FOpenFileRead
ralias Plugin_FOpenFileWrite, FS_SV_FOpenFileWrite
ralias Plugin_FRead, FS_Read
ralias Plugin_FReadLine, FS_ReadLine
ralias Plugin_FWrite, FS_Write
ralias Plugin_FCloseFile, FS_FCloseFile

ralias Plugin_StringToAdr, NET_StringToAdr
ralias Plugin_Milliseconds, Sys_Milliseconds
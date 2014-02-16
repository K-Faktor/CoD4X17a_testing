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

global Scr_InitVariables
Scr_InitVariables:
	jmp dword [oScr_InitVariables]

global Scr_Init
Scr_Init:
	jmp dword [oScr_Init]

global Scr_Settings
Scr_Settings:
	jmp dword [oScr_Settings]

global Scr_AddEntity
Scr_AddEntity:
	jmp dword [oScr_AddEntity]

global Scr_Cleanup
Scr_Cleanup:
	jmp dword [oScr_Cleanup]

global GScr_Shutdown
GScr_Shutdown:
	jmp dword [oGScr_Shutdown]

global Scr_AllocArray
Scr_AllocArray:
	jmp dword [oScr_AllocArray]

global Scr_GetNumParam
Scr_GetNumParam:
	jmp dword [oScr_GetNumParam]

global Scr_GetInt
Scr_GetInt:
	jmp dword [oScr_GetInt]

global Scr_GetFloat
Scr_GetFloat:
	jmp dword [oScr_GetFloat]

global Scr_GetString
Scr_GetString:
	jmp dword [oScr_GetString]

global Scr_GetEntity
Scr_GetEntity:
	jmp dword [oScr_GetEntity]

global Scr_GetConstString
Scr_GetConstString:
	jmp dword [oScr_GetConstString]

global Scr_GetType
Scr_GetType:
	jmp dword [oScr_GetType]

global Scr_GetVector
Scr_GetVector:
	jmp dword [oScr_GetVector]

global Scr_Error
Scr_Error:
	jmp dword [oScr_Error]

global Scr_SetLoading
Scr_SetLoading:
	jmp dword [oScr_SetLoading]

global Scr_ParamError
Scr_ParamError:
	jmp dword [oScr_ParamError]

global Scr_ObjectError
Scr_ObjectError:
	jmp dword [oScr_ObjectError]

global Scr_AddInt
Scr_AddInt:
	jmp dword [oScr_AddInt]

global Scr_AddFloat
Scr_AddFloat:
	jmp dword [oScr_AddFloat]

global Scr_AddBool
Scr_AddBool:
	jmp dword [oScr_AddBool]

global Scr_AddString
Scr_AddString:
	jmp dword [oScr_AddString]

global Scr_AddUndefined
Scr_AddUndefined:
	jmp dword [oScr_AddUndefined]

global Scr_AddVector
Scr_AddVector:
	jmp dword [oScr_AddVector]

global Scr_AddArray
Scr_AddArray:
	jmp dword [oScr_AddArray]

global Scr_MakeArray
Scr_MakeArray:
	jmp dword [oScr_MakeArray]

global Scr_Notify
Scr_Notify:
	jmp dword [oScr_Notify]

global Scr_NotifyNum
Scr_NotifyNum:
	jmp dword [oScr_NotifyNum]

;Not working :(
global Scr_PrintPrevCodePos
Scr_PrintPrevCodePos:
	jmp dword [oScr_PrintPrevCodePos]

global Scr_GetFunctionHandle
Scr_GetFunctionHandle:
	jmp dword [oScr_GetFunctionHandle]

global Scr_ExecEntThread
Scr_ExecEntThread:
	jmp dword [oScr_ExecEntThread]

global Scr_ExecThread
Scr_ExecThread:
	jmp dword [oScr_ExecThread]

global Scr_FreeThread
Scr_FreeThread:
	jmp dword [oScr_FreeThread]

global Scr_CreateCanonicalFilename
Scr_CreateCanonicalFilename:
	jmp dword [oScr_CreateCanonicalFilename]

global FindVariable
FindVariable:
	jmp dword [oFindVariable]

global FindObject
FindObject:
	jmp dword [oFindObject]

global GetNewVariable
GetNewVariable:
	jmp dword [oGetNewVariable]

global ScriptParse
ScriptParse:
	jmp dword [oScriptParse]

global GetObjectA
GetObjectA:
	jmp dword [oGetObjectA]

global GetVariable
GetVariable:
	jmp dword [oGetVariable]

global ScriptCompile
ScriptCompile:
	jmp dword [oScriptCompile]

global Scr_AddSourceBuffer
Scr_AddSourceBuffer:
	jmp dword [oScr_AddSourceBuffer]

global Scr_InitAllocNode
Scr_InitAllocNode:
	jmp dword [oScr_InitAllocNode]

global Scr_BeginLoadScripts
Scr_BeginLoadScripts:
	jmp dword [oScr_BeginLoadScripts]

global Scr_SetClassMap
Scr_SetClassMap:
	jmp dword [oScr_SetClassMap]

global Scr_AddFields
Scr_AddFields:
	jmp dword [oScr_AddFields]

global Scr_SetGenericField
Scr_SetGenericField:
	jmp dword [oScr_SetGenericField]

global Scr_GetGenericField
Scr_GetGenericField:
	jmp dword [oScr_GetGenericField]

global GScr_AddFieldsForHudElems
GScr_AddFieldsForHudElems:
	jmp dword [oGScr_AddFieldsForHudElems]

global GScr_AddFieldsForRadiant
GScr_AddFieldsForRadiant:
	jmp dword [oGScr_AddFieldsForRadiant]

global Scr_AddHudElem
Scr_AddHudElem:
	jmp dword [oScr_AddHudElem]

global Scr_FreeHudElem
Scr_FreeHudElem:
	jmp dword [oScr_FreeHudElem]

global Scr_EndLoadScripts
Scr_EndLoadScripts:
	jmp dword [oScr_EndLoadScripts]

global Scr_ConstructMessageString
Scr_ConstructMessageString:
	jmp dword [oScr_ConstructMessageString]

SECTION .rodata

oScr_InitVariables dw 0x815288a
oScr_Init dw 0x815d8e2
oScr_Settings dw 0x815cf90
oScr_AddEntity dw 0x80c7770
oScr_Cleanup dw 0x815cf84
oGScr_Shutdown dw 0x80bf610
oScr_AllocArray dw 0x8153cca
oScr_GetNumParam dw 0x815d01e
oScr_GetInt dw 0x8160fee
oScr_GetFloat dw 0x816094c
oScr_GetString dw 0x8160932
oScr_GetEntity dw 0x80c76ec
oScr_GetConstString dw 0x816074c
oScr_GetType dw 0x815f7c8
oScr_GetVector dw 0x815ffe6
oScr_Error dw 0x815e9f4
oScr_SetLoading dw 0x815cfba
oScr_ParamError dw 0x815f20e
oScr_ObjectError dw 0x815f134
oScr_AddInt dw 0x815f01a
oScr_AddFloat dw 0x815ef9a
oScr_AddBool dw 0x815eac6
oScr_AddString dw 0x815ec68
oScr_AddUndefined dw 0x815eea2
oScr_AddVector dw 0x815ee12
oScr_AddArray dw 0x815d5c0
oScr_MakeArray dw 0x815ed8a
oScr_Notify dw 0x80c7604
oScr_NotifyNum dw 0x815e762
oScr_PrintPrevCodePos dw 0x814ef6e
oScr_GetFunctionHandle dw 0x814c1b4
oScr_ExecEntThread dw 0x80c765c
oScr_ExecThread dw 0x8165032
oScr_FreeThread dw 0x815d062
oScr_CreateCanonicalFilename dw 0x81516ee
oFindVariable dw 0x81542d4
oFindObject dw 0x8152294
oGetNewVariable dw 0x81545ce
oScriptParse dw 0x816b5da
oGetObjectA dw 0x8154046
oGetVariable dw 0x815540a
oScriptCompile dw 0x81491d8
oScr_AddSourceBuffer dw 0x814fbac
oScr_InitAllocNode dw 0x814fea6
oScr_BeginLoadScripts dw 0x814c266
oScr_SetClassMap dw 0x8153a3a
oScr_AddFields dw 0x81535ba
oScr_SetGenericField dw 0x80c7eae
oScr_GetGenericField dw 0x80c7d36
oGScr_AddFieldsForHudElems dw 0x808db80
oGScr_AddFieldsForRadiant dw 0x80c77ec
oScr_AddHudElem dw 0x80c7746
oScr_FreeHudElem dw 0x80c778e
oScr_EndLoadScripts dw 0x814bcee
oScr_ConstructMessageString dw 0x80c02aa

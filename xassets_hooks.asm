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

global DB_SetInitializing
DB_SetInitializing:
	jmp dword [oDB_SetInitializing]

global DB_FileExists
DB_FileExists:
	jmp dword [oDB_FileExists]

global DB_ModFileExists
DB_ModFileExists:
	jmp dword [oDB_ModFileExists]

global DB_LoadXAssets
DB_LoadXAssets:
	jmp dword [oDB_LoadXAssets]

global DB_GetXAssetTypeSize
DB_GetXAssetTypeSize:
	jmp dword [oDB_GetXAssetTypeSize]

global XAnimInit
XAnimInit:
	jmp dword [oXAnimInit]

SECTION .rodata

oDB_SetInitializing dd 0x820337c
oDB_FileExists dd 0x8204424
oDB_ModFileExists dd 0x8204470
oDB_LoadXAssets dd 0x8205e86
oDB_GetXAssetTypeSize dd 0x81da6ce
oXAnimInit dd 0x81b649c

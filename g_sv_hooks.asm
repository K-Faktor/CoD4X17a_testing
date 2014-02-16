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

global G_RunFrame
G_RunFrame:
	jmp dword [oG_RunFrame]

global G_LocalizedStringIndex
G_LocalizedStringIndex:
	jmp dword [oG_LocalizedStringIndex]

global ClientCommand
ClientCommand:
	jmp dword [oClientCommand]

global ClientConnect
ClientConnect:
	jmp dword [oClientConnect]

global ClientBegin
ClientBegin:
	jmp dword [oClientBegin]

global StopFollowing
StopFollowing:
	jmp dword [oStopFollowing]

global G_SayTo
G_SayTo:
	jmp dword [oG_SayTo]

SECTION .rodata

oG_RunFrame dw 0x80b61fc
oG_LocalizedStringIndex dw 0x80cb218
oClientCommand dw 0x80b070c
oClientConnect dw 0x80a83d4
oClientBegin dw 0x80a7700
oStopFollowing dw 0x80af24c
oG_SayTo dw 0x80ae6f4

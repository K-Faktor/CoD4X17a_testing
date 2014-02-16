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

global FS_ShutdownIwdPureCheckReferences
FS_ShutdownIwdPureCheckReferences:
    jmp dword [oFS_ShutdownIwdPureCheckReferences]

global FS_ShutdownServerIwdNames
FS_ShutdownServerIwdNames:
    jmp dword [oFS_ShutdownServerIwdNames]

global FS_ShutdownServerReferencedIwds
FS_ShutdownServerReferencedIwds:
    jmp dword [oFS_ShutdownServerReferencedIwds]

global FS_ShutdownServerReferencedFFs
FS_ShutdownServerReferencedFFs:
    jmp dword [oFS_ShutdownServerReferencedFFs]

global FS_LoadedIwdPureChecksums
FS_LoadedIwdPureChecksums:
    jmp dword [oFS_LoadedIwdPureChecksums]
	
SECTION .rodata	
	
oFS_ShutdownIwdPureCheckReferences dw 0x81866b6
oFS_ShutdownServerIwdNames dw 0x8186cfe
oFS_ShutdownServerReferencedIwds dw 0x818789c
oFS_ShutdownServerReferencedFFs dw 0x8187850
oFS_LoadedIwdPureChecksums dw 0x81283f2
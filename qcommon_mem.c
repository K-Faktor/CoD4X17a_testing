/*
===========================================================================
    Copyright (C) 2010-2013  Ninja and TheKelm of the IceOps-Team
    Copyright (C) 1999-2005 Id Software, Inc.

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

#include <malloc.h>
#include <string.h>


#define MEM_SIZE 140 //Megabyte

void Mem_Init()
{

    void *memory;
    int sizeofmemory = 1024*1024*MEM_SIZE;


    memory = memalign(0x1000, sizeofmemory);
    memset(memory, 0, sizeofmemory);
    memset((void*)0x1407e7a0, 0, 0x21C);
    *(int**)(0x1407e7a0) = memory;
    *(int*)(0x1407e8b8) = sizeofmemory;

}
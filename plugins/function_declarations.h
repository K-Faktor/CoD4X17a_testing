#ifndef PLUGIN_INCLUDES

    #error Please include pinc.h instead!

#endif /*PLUGIN_INCLUDES*/

    // ----------------------------------------------------------------------------//
    // Functions available for use in plugin API, sorted alphabetically by module. //
    // ----------------------------------------------------------------------------//

    //      == Commands ==
    
    __cdecl char* Cmd_Argv(int arg);                // Get a command argument with index arg.
    __cdecl int Cmd_Argc();                         // Get number of command arguments


    //      == Common ==
    __cdecl void G_LogPrintf( const char *fmt, ... );
    __cdecl void Com_Printf( const char *fmt, ...);                // Print to a correct place (rcon, player console, logs)
    __cdecl void Com_PrintWarning( const char *fmt, ...);          // Print to a correct place (rcon, player console, logs)
    __cdecl void Com_PrintError( const char *fmt, ...);            // Print to a correct place (rcon, player console, logs)
    __cdecl void Com_DPrintf( const char *fmt, ...);               // Same as Com_Printf, only shows up when developer is set to 1
    __cdecl char* Com_ParseGetToken(char* line);                 // Tokenize a string - get next token
    __cdecl int Com_ParseTokenLength(char* token);               // Tokenize a string - get the token's length
    
    
    //      == Cvars ==
    
    // All of the Cvars module functions are self explanatory
    __cdecl CONVAR_T* Cvar_RegisterString(const char *var_name, const char *var_value, int flags, const char *var_description);
    __cdecl CONVAR_T* Cvar_RegisterBool(const char *var_name, qboolean var_value, int flags, const char *var_description);
    __cdecl CONVAR_T* Cvar_RegisterInt(const char *var_name, int var_value, int min_value, int max_value, int flags, const char *var_description);
    __cdecl CONVAR_T* Cvar_RegisterEnum(const char *var_name, char** valnames, int defaultval, int flags, const char *var_description);
    __cdecl CONVAR_T* Cvar_RegisterFloat(const char *var_name, float var_value, float min_value, float max_value, int flags, const char *var_description);
    __cdecl void Cvar_SetInt(CONVAR_T const* var, int val);
    __cdecl void Cvar_SetBool(CONVAR_T const* var, qboolean val);
    __cdecl void Cvar_SetString(CONVAR_T const* var, char const* string);
    __cdecl void Cvar_SetFloat(CONVAR_T const* var, float val);
    __cdecl void Cvar_VariableStringBuffer(const char* cvarname, char* buff, size_t size);
    __cdecl float Cvar_VariableValue( const char *var_name );
    __cdecl int Cvar_VariableIntegerValue( const char *var_name );
    __cdecl const char* Cvar_VariableString( const char *var_name );


    //      == File handling functions == - Do we really need those?

    __cdecl int FS_SV_FOpenFileRead(const char *filename, fileHandle_t *fp); // Open a file for reading
    __cdecl fileHandle_t FS_SV_FOpenFileWrite(const char *filename);         // Open a file for writing
    __cdecl int FS_Read(void *buffer, int len, fileHandle_t f);              // Read data from file
    __cdecl int FS_ReadLine(void *buffer, int len, fileHandle_t f);          // Read a line from file
    __cdecl int FS_Write(const void *buffer, int len, fileHandle_t h);       // Write to file
    __cdecl qboolean FS_FCloseFile(fileHandle_t f);                          // Cloase an open file


    //      == Networking ==

    __cdecl int NET_StringToAdr(const char* string, netadr_t* , netadrtype_t);


    //      == Plugin Handler's functions ==

    __cdecl clientScoreboard_t Plugin_GetClientScoreboard(int clientNum);    // Get the scoreboard of a player
    __cdecl int Plugin_Cmd_GetInvokerUid();                                  // Get UID of command invoker
    __cdecl int Plugin_GetPlayerUid(int slot);                               // Get UID of a plyer
    __cdecl int Plugin_GetSlotCount();                                       // Get number of server slots
    __cdecl qboolean Plugin_IsSvRunning();                                   // Is server running?
    __cdecl void Plugin_ChatPrintf(int slot, char *fmt, ...);                  // Print to player's chat (-1 for all)
    __cdecl void Plugin_BoldPrintf(int slot, char *fmt, ...);                  // Print to the player's screen (-1 for all)
    __cdecl char *Plugin_GetPlayerName(int slot);                            // Get a name of a player
    __cdecl void Plugin_AddCommand(char *name, xcommand_t command, int defaultpower); // Add a server command
    __cdecl void *Plugin_Malloc(size_t size);                                // Same as stdlib.h function malloc
    __cdecl void Plugin_Free(void *ptr);                                     // Same as stdlib.h function free
    __cdecl void Plugin_Error(int code, const char *fmt, ...);                       // Notify the server of an error, action depends on code parameter
    __cdecl int Plugin_GetLevelTime();                                       // Self explanatory
    __cdecl int Plugin_GetServerTime();                                      // Self explanatory


    //  -- TCP Connection functions --

    __cdecl qboolean Plugin_TcpConnect(int connection, const char* remote);      // Open a new TCP connection
    __cdecl int Plugin_TcpGetData(int connection, void *buf, int size);          // Receive TCP data
    __cdecl qboolean Plugin_TcpSendData(int connection, void *data, int len);    // Send TCP data
    __cdecl void Plugin_TcpCloseConnection(int connection);                      // Close an open TCP connection
    __cdecl qboolean Plugin_UdpSendData(netadr_t* to, void* data, int len);      // Send UDP data
    __cdecl void Plugin_ServerPacketEvent(netadr_t* to, void* data, int len);    // Receive UDP data


    //  -- UIDS / GUIDs --

    __cdecl void Plugin_SetPlayerUID(unsigned int clientslot, unsigned int uid);     // Set player's UID
    __cdecl unsigned int Plugin_GetPlayerUID(unsigned int clientslot);               // Get player's UID
    __cdecl const char* Plugin_GetPlayerGUID(unsigned int clientslot);               // Get player's GUID
    __cdecl void Plugin_SetPlayerGUID(unsigned int clientslot, const char* guid);    // Set player's GUID
    __cdecl void Plugin_SetPlayerNoPB(unsigned int clientslot);                      // Turn off PunkBuster for a player
    __cdecl int Plugin_DoesServerUseUids(void);                                      // Self explanatory
    __cdecl void Plugin_SetServerToUseUids(int useuids);                             // Self explanatory


    //      == System functions ==

    __cdecl int Sys_Milliseconds();  // Milliseconds since server start

    //      == Scriptfunctions ==
    __cdecl void Plugin_ScrAddFunction(char *name, xfunction_t function);
    __cdecl void Plugin_ScrAddMethod(char *name, xfunction_t function);
    __cdecl void Plugin_ScrReplaceFunction(char *name, xfunction_t function);
    __cdecl void Plugin_ScrReplaceMethod(char *name, xfunction_t function);

    __cdecl void Scr_AddEntity(gentity_t* ent);
    __cdecl int Scr_GetNumParam( void );
    __cdecl int Scr_GetInt( unsigned int );
    __cdecl float Scr_GetFloat( unsigned int );
    __cdecl char* Scr_GetString( unsigned int );
    __cdecl gentity_t* Scr_GetEntity( unsigned int );
    __cdecl short Scr_GetConstString( unsigned int );
    __cdecl unsigned int Scr_GetType( unsigned int );
    __cdecl void Scr_GetVector( unsigned int, vec3_t* );
    __cdecl void Scr_Error( const char *string);
    __cdecl void Scr_ParamError( int, const char *string);
    __cdecl void Scr_ObjectError( const char *string);
    __cdecl void Scr_AddInt(int value);
    __cdecl void Scr_AddFloat(float);
    __cdecl void Scr_AddBool(qboolean);
    __cdecl void Scr_AddString(const char *string);
    __cdecl void Scr_AddUndefined(void);
    __cdecl void Scr_AddVector( vec3_t vec );
    __cdecl void Scr_AddArray( void );
    __cdecl void Scr_MakeArray( void );
    __cdecl void Scr_Notify( gentity_t*, unsigned short, unsigned int);
    __cdecl void Scr_NotifyNum( int, unsigned int, unsigned int, unsigned int);
    __cdecl short Scr_ExecEntThread( gentity_t* ent, int callbackHook, unsigned int numArgs);
    __cdecl short Scr_ExecThread( int callbackHook, unsigned int numArgs);
    __cdecl void  Scr_FreeThread( short threadId);

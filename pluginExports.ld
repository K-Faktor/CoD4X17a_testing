{
  extern "C"
  {
    Plugin_*;
    
    
    Com_Printf;
    Com_PrtinWarning;
    Com_PrintError;
    Com_DPrintf;
    Com_ParseGetToken;
    Com_ParseTokenLength;
    
    Cmd_Argv;
    Cmd_Argc;
    
    Cvar_VariableStringBuffer;
    Cvar_VariableValue;
    Cvar_VariableIntegerValue;
    Cvar_VariableString;
    Cvar_RegisterString;
    Cvar_RegisterBool;
    Cvar_RegisterInt;
    Cvar_RegisterEnum;
    Cvar_RegisterFloat;
    Cvar_SetInt;
    Cvar_SetBool;
    Cvar_SetString;
    Cvar_SetFloat;
    
    FS_SV_FOpenFileRead;
    FS_SV_FOpenFileWrite;
    FS_Read;
    FS_ReadLine;
    FS_Write;
    FS_FCloseFile;
    
    NET_StringToAdr;
    
    Sys_Milliseconds;


    Scr_AddEntity;
    Scr_AllocArray;
    Scr_GetNumParam;
    Scr_GetInt;
    Scr_GetFloat;
    Scr_GetString;
    Scr_GetEntity;
    Scr_GetConstString;
    Scr_GetType;
    Scr_GetVector;
    Scr_Error;
    Scr_ParamError;
    Scr_ObjectError;
    Scr_AddInt;
    Scr_AddFloat;
    Scr_AddBool;
    Scr_AddString;
    Scr_AddUndefined;
    Scr_AddVector;
    Scr_AddArray;
    Scr_MakeArray;
    Scr_Notify;
    Scr_NotifyNum;
    Scr_ExecEntThread;
    Scr_ExecThread;
    Scr_FreeThread;


  };
};


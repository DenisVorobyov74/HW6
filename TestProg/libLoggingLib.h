#ifndef logging_h
#define logging_h

enum lgg_MsgLvl {debug, info, warning, error};

#define lgg_LogInfo(MSG, ...) lgg_LogMsg(MSG, info,    __FILE__, __func__, __LINE__);
#define lgg_LogDbg(MSG, ...)  lgg_LogMsg(MSG, debug,   __FILE__, __func__, __LINE__);
#define lgg_LogWrng(MSG, ...) lgg_LogMsg(MSG, warning, __FILE__, __func__, __LINE__);
#define lgg_LogErr(MSG, ...)  lgg_LogMsg(MSG, error,   __FILE__, __func__, __LINE__);

extern void lgg_LogSys_init(char LogFileName[]);
extern void lgg_LogSys_deinit();
extern void lgg_LogMsg(char* MSG, enum lgg_MsgLvl Level, const char* file, const char* func, long line);

#endif // logging_h

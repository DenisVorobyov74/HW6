#ifndef logging_h
#define logging_h

extern FILE *lgg_LogFilePointer;
enum lgg_MsgLvl {debug, info, warning, error};

#endif // logging_h

#define lgg_log_info(MSG, Level) lgg_LogMsg(MSG, Level, __FILE__, __func__, __LINE__);

extern void lgg_LogSys_init(char LogFileName[]);
extern void lgg_LogSys_deinit();
extern void lgg_LogMsg(char* MSG, enum lgg_MsgLvl Level, const char* file, const char* func, long line);


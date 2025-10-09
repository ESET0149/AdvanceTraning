#ifndef APP_LOG_H
#define APP_LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#define LOG_FILE_PATH "C:\\Logs\\app_log.txt" 

bool AppLog_Init(const char *filename);

void AppLog(const char *fmt, ...);

void AppLog_Close(void);

#endif

#include "app_log.h"
#include <time.h>
#include <string.h>

static FILE *g_logFile = NULL;

bool AppLog_Init(const char *filename)
{
    g_logFile = fopen(LOG_FILE_PATH, "a"); // append mode
    if (g_logFile == NULL)
    {
        printf("Error: Could not open log file: %s\n", filename);
        return false;
    }

    time_t now = time(NULL);
    fprintf(g_logFile, "\n========== Log Started: %s==========\n", ctime(&now));
    fflush(g_logFile);

    return true;
}

void AppLog(const char *fmt, ...)
{
    if (g_logFile == NULL)
    {
        // Fallback: open default file if not already open
        AppLog_Init("app_log.txt");
    }

    // Timestamp
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S]", t);

    // Print to log file
    fprintf(g_logFile, "%s ", timestamp);

    va_list args;
    va_start(args, fmt);
    vfprintf(g_logFile, fmt, args);
    va_end(args);

    fprintf(g_logFile, "\n");
    fflush(g_logFile); // Ensure it’s written immediately
}

void AppLog_Close(void)
{
    if (g_logFile != NULL)
    {
        time_t now = time(NULL);
        fprintf(g_logFile, "========== Log Closed: %s==========\n", ctime(&now));
        fclose(g_logFile);
        g_logFile = NULL;
    }
}

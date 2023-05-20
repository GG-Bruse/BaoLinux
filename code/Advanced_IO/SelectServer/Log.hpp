#pragma once
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdarg>
#include <ctime>

//日志级别
#define DEBUG   0
#define NORMAL  1
#define WARNING 2
#define ERROR   3
#define FATAL   4
const char *gLevelMap[] = {
    "DEBUG",
    "NORMAL",
    "WARNING",
    "ERROR",
    "FATAL"
};

// 完整的日志功能，至少: 日志等级 时间 支持用户自定义(日志内容, 文件行，文件名)
void LogMessage(int level, const char *format, ...)
{
    //标准部分
    char stdBuffer[1024];
    const time_t timestamp = time(nullptr);
    struct tm* local_time = localtime(&timestamp);
    snprintf(stdBuffer, sizeof stdBuffer, "[%s] [%d-%d-%d-%d-%d-%d] ", gLevelMap[level], 
        local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

    //自定义部分
    char logBuffer[1024]; 
    va_list args;
    va_start(args, format);
    vsnprintf(logBuffer, sizeof logBuffer, format, args);
    va_end(args);
    printf("%s%s\n", stdBuffer, logBuffer);
}
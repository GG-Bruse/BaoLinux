#pragma once
#include <iostream>
#include <string>
#include <functional>
#include "Log.hpp"

typedef std::function<int(int, int)> fun_t;
class Task
{
public:
    Task(){}
    Task(int x, int y, fun_t func):_x(x), _y(y), _func(func) {}
    void operator ()(const std::string &name) {
        LogMessage(NORMAL, "%s处理完成: %d+%d=%d", name.c_str(), _x, _y, _func(_x, _y));
    }
public:
    int _x;
    int _y;
    fun_t _func;
};
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <jsoncpp/json/json.h>
using std::string;
using std::vector;
// 100+19X100+19X100+19

#define SEP "X"
#define SEP_LENGTH strlen(SEP)
#define SPACE " "
#define SPACE_LEN strlen(SPACE)

void SpliteMessage(string &buffer, vector<string> *out)
{
    while (true)
    {
        size_t pos = buffer.find(SEP);
        if (pos == string::npos)
            break;
        string message = buffer.substr(0, pos);
        buffer.erase(0, pos + SEP_LENGTH);
        out->push_back(message);
    }
}
string Encode(string &s) {
    return s + SEP;
}

class Request
{
public:
    string Serialize()
    {
        std::string str;
        str = std::to_string(x_);
        str += SPACE;
        str += op_; // TODO
        str += SPACE;
        str += std::to_string(y_);
        return str;
    }
    bool Deserialized(const std::string &str)
    {
        std::size_t left = str.find(SPACE);
        if (left == std::string::npos)
            return false;
        std::size_t right = str.rfind(SPACE);
        if (right == std::string::npos)
            return false;
        x_ = atoi(str.substr(0, left).c_str());
        y_ = atoi(str.substr(right + SPACE_LEN).c_str());
        if (left + SPACE_LEN > str.size())
            return false;
        else
            op_ = str[left + SPACE_LEN];
        return true;
    }

public:
    Request() {}
    Request(int x, int y, char op) : x_(x), y_(y), op_(op) {}
    ~Request() {}

public:
    int x_;
    int y_;
    char op_; // '+' '-' '*' '/' '%'
};

class Response
{
public:
    string Serialize()
    {
        string s;
        s = std::to_string(code_);
        s += SPACE;
        s += std::to_string(result_);
        return s;
    }
    bool Deserialized(const string &s)
    {
        size_t pos = s.find(SPACE);
        if (pos == string::npos)
            return false;
        code_ = atoi(s.substr(0, pos).c_str());
        result_ = atoi(s.substr(pos + SPACE_LEN).c_str());
        return true;
    }

public:
    Response() {}
    Response(int result, int code) : result_(result), code_(code) {}
    ~Response() {}

public:
    int result_; // 计算结果
    int code_;   // 计算结果的状态码
};
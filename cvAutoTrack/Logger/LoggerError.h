//
// Created by GengG on 2022/1/29.
//

#ifndef CVAUTOTRACKDLL_LOGGERERROR_H
#define CVAUTOTRACKDLL_LOGGERERROR_H
#include <iostream>
#include <string>
#include <queue>
#include <map>
// 一个记录错误码和错误详细信息的类

// set(123,"这个是测试错误，比如异常闪退，没有找到窗口");
//
// if(_error_code == 123)
// {
//     err=123;
//     return;
// }





// 记录错误的递归链
// 以后再写
class LoggerErrorType
{
    public:
    static std::map<int ,std::string> _errType;

    LoggerErrorType()=default;
    ~LoggerErrorType()=default;

    bool set(int errCode,std::string errMsg)
    {
        bool isExist = false;
        if(_errType[errCode]=="")
        {
            isExist=false;
        }
        else
        {
            isExist=true;
        }
        _errType[errCode]=errMsg;
        return !isExist; // 错误码重复，已覆盖
    }

    std::string operator [] (int code)
    {
        try
        {
            return _errType.at(code);
        }
        catch(std::out_of_range& e)
        {
            return "未定义该错误码，检查错误码 "+ std::to_string(code)+" 使用位置";
        }
    }
};

class LoggerError
{
    std::vector<int> _errorCodeQueue;
    LoggerErrorType _errorType;
public:

    LoggerError& operator=(const int& code)
    {
        _errorCodeQueue.push_back(code);
        return *this;
    }
    bool set(int errCode,std::string errMsg)
    {
        return _errorType.set(errCode,errMsg);
    }

    operator int()
    {
        return _errorCodeQueue.front();
    }

    friend std::ostream & operator<<(std::ostream & os, LoggerError & err)
    {
        for (int i = err._errorCodeQueue.size()-1; i >= 0; i--)
        {
            for (int j = 1; j < err._errorCodeQueue.size() - i; j++)
            {
                os << "    ";
            }
            os << "\u2514 \u2500 \u252C \u2500 \u2192";
            os << err._errorType[err._errorCodeQueue[i]] << "\n";

        }
        return os;
    }
};

#endif //CVAUTOTRACKDLL_LOGGERERROR_H

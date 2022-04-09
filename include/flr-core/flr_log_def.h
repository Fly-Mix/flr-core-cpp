//
// Created by York on 2022/4/5.
//

#ifndef FLR_CORE_ENGINE_FLR_LOG_H
#define FLR_CORE_ENGINE_FLR_LOG_H

/*
 * 日志级别
 * */
#define FLR_LOG_DEBUG  0
#define FLR_LOG_INFO   1
#define FLR_LOG_WARN   2
#define FLR_LOG_ERROR  3
#define FLR_LOG_FATAL  4

namespace flr {
    /*
     * flr 日志信息中继器（负责接收 flr 的内部日志，然后转发给应用层打印出来）
     * */
    class LogMessageRepeater;

    /*
     * 日志回调函数原型
     * @file 日志所在文件
     * @line 日志所在代码行
     * @func 日志所在函数
     * @severity 日志级别
     * @context  日志内容
     * */
    typedef void(*log_callback_t)(const char *file, int line, const char *func, int severity, const char *content);

    static log_callback_t log_callback = nullptr;
};

/*
 * flr 日志信息中继器（负责接收 flr 的内部日志，然后转发给应用层打印出来）
 * */
class flr::LogMessageRepeater
{
public:
    LogMessageRepeater(const char* file, int line, const char* func, int severity, log_callback_t callback)
            : _file(file)
            , _line(line)
            , _func(func)
            , _severity(severity)
            , _callback(callback)
    {
    }

    ~LogMessageRepeater()
    {
        if (_callback)
        {
            std::string content = _stream.str();
            _callback(_file.c_str(), _line, _func.c_str(), _severity, content.c_str());
        }
    }

    std::ostringstream &stream() { return _stream; }

private:
    std::string _file;
    int _line;
    std::string _func;
    int _severity;

    log_callback_t _callback;
    std::ostringstream _stream;
};

/*
 * flr 日志打印API（实际开发中使用）
 * */
#define LOG_DEBUG flr::LogMessageRepeater(__FILE__, __LINE__, __FUNCTION__, FLR_LOG_DEBUG, flr::log_callback).stream()
#define LOG_INFO  flr::LogMessageRepeater(__FILE__, __LINE__, __FUNCTION__, FLR_LOG_INFO,  flr::log_callback).stream()
#define LOG_WARN  flr::LogMessageRepeater(__FILE__, __LINE__, __FUNCTION__, FLR_LOG_WARN,  flr::log_callback).stream()
#define LOG_ERROR flr::LogMessageRepeater(__FILE__, __LINE__, __FUNCTION__, FLR_LOG_ERROR, flr::log_callback).stream()
#define LOG_FATAL flr::LogMessageRepeater(__FILE__, __LINE__, __FUNCTION__, FLR_LOG_FATAL, flr::log_callback).stream()


#endif //FLR_CORE_ENGINE_FLR_LOG_H

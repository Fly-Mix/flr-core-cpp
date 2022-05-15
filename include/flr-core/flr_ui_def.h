//
// Created by York on 2022/4/10.
//

#ifndef FLR_CORE_ENGINE_FLR_UI_H
#define FLR_CORE_ENGINE_FLR_UI_H

#include <string>
#include <sstream>

/*
 * UI Message 打印级别
 * */
#define FLR_UI_INFO  0
#define FLR_UI_TIPS   1
#define FLR_UI_WARN   2
#define FLR_UI_ERROR  3

namespace flr {
    /*
     * flr UI Message 中继器（负责接收 flr 内部的希望在UI层进行展示的信息，然后转发给应用层在UI上展示出来）
     * */
    class UiMessageRepeater;

    /*
     * UI Message 回调函数原型
     * @severity UI Message 级别
     * @context  UI Message 内容
     * */
    typedef void(*ui_message_callback_t)(int severity, const char *content);

    static ui_message_callback_t ui_message_callback = nullptr;

    /*
    * flr UI Message 打印器接口类（应用层继承实现该类，实现指定方法以完成 UI Message 在应用层的打印）
    * */
    class UiMessageAbstractPrinter {
    public:
        virtual ~UiMessageAbstractPrinter(){}
        virtual void print_ui_message(int severity, const char *content) {
            std::cout << "[flr]" << " " << content << std::endl;
        }
    };

    static UiMessageAbstractPrinter *uiMessagePrinter = nullptr;
};

/*
 * flr UI Message 中继器（负责接收 flr 内部的希望在UI层进行展示的信息，然后转发给应用层在UI上展示出来）
 * */
class flr::UiMessageRepeater {
public:
    UiMessageRepeater(int severity, ui_message_callback_t callback)
            : _severity(severity)
            , _callback(callback) {
    }

    ~UiMessageRepeater() {
        std::string content = _stream.str();
        if (_callback) {
            _callback(_severity, content.c_str());
        }
    }

    std::ostringstream &stream() { return _stream; }

private:
    int _severity;

    ui_message_callback_t _callback;
    std::ostringstream _stream;
};

/*
 * flr UI Message 打印API（实际开发中使用）
 * */
#define UI_INFO  flr::UiMessageRepeater(FLR_UI_INFO, flr::ui_message_callback).stream()
#define UI_TIPS flr::UiMessageRepeater(FLR_UI_TIPS, flr::ui_message_callback).stream()
#define UI_WARN  flr::UiMessageRepeater(FLR_UI_WARN, flr::ui_message_callback).stream()
#define UI_ERROR flr::UiMessageRepeater(FLR_UI_ERROR, flr::ui_message_callback).stream()


#endif //FLR_CORE_ENGINE_FLR_UI_H

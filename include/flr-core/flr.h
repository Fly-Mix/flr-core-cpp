//
// Created by York on 2021/4/18.
//

#ifndef FLR_CORE_ENGINE_FLR_H
#define FLR_CORE_ENGINE_FLR_H

#include <iostream>

namespace flr {
    void say_hi();

    /**
     * generate_one 操作生成的结果
     */
    struct ResultOfGenerateOne {
        std::string pubspec_file_new_content;
        std::string r_dart_file_content;
    };

    /**
     * 获取flr的处理日志信息的回调函数；用于供上层获取 init_one/generate_one 的处理日志信息，信息包括：进度信息、警告信息等
     * */
    typedef std::function<void(std::string log_msg)> opt_log_callback_t;

    /**
     * 对指定 flutter 工程进行初始化
     * @param flutter_project_root_dir flutter 工程根目录路径
     * @param flutter_sdk_version flutter sdk 版本，比如："1.12.13+hotfix.5"，"2.0.3"
     * @param dart_sdk_version dart sdk 版本，比如："2.12.2"
     * @param opt_log_callback 获取flr的处理日志信息的回调函数
     * @return pubspec.yaml 的新内容
     */
    std::string init_one(std::string flutter_project_root_dir, std::string flutter_sdk_version, std::string dart_sdk_version, opt_log_callback_t opt_log_callback);

    std::string init_one_without_cb(std::string flutter_project_root_dir, std::string flutter_sdk_version, std::string dart_sdk_version);

    /**
     * 为指定 flutter 工程扫描资源目录，自动为资源添加声明到 pubspec.yaml 和生成 r.g.dart
     * @param flutter_project_root_dir flutter 工程根目录路径
     * @param flutter_sdk_version flutter sdk 版本，比如：" 1.12.13+hotfix.5"，"2.0.3"
     * @param dart_sdk_version dart sdk 版本，比如："2.12.2"
     * @param opt_log_callback 获取flr的处理日志信息的回调函数
     * @return
     */
    ResultOfGenerateOne generate_one(std::string flutter_project_root_dir, std::string flutter_sdk_version, std::string dart_sdk_version, opt_log_callback_t opt_log_callback);

}


#endif //FLR_CORE_ENGINE_FLR_H

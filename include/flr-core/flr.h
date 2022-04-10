//
// Created by York on 2021/4/18.
//

#ifndef FLR_CORE_ENGINE_FLR_H
#define FLR_CORE_ENGINE_FLR_H

#include <iostream>
#include "flr_log_def.h"
#include "flr_ui_def.h"

namespace flr {

#pragma Message API
    void say_hi();

    /*
     * 注册 Flr Log Message 回调（供应用层打印收集或者打印 flr 回调的 Log Message）
     * */
    void register_log_message_callback(log_message_callback_t callback);

    /*
    * 注册 Flr UI Message 回调（供应用层在UI层展示 flr 回调的 UI Message）
    * */
    void register_ui_message_callback(ui_message_callback_t callback);

#pragma 业务API

    /**
     * generate_one 操作生成的结果
     */
    struct ResultOfGenerateOne {
        std::string pubspec_file_new_content;
        std::string r_dart_file_content;
    };

    /**
     * 对指定 flutter 工程进行初始化
     * @param flutter_project_root_dir flutter 工程根目录路径
     * @param flutter_sdk_version flutter sdk 版本，比如："1.12.13+hotfix.5"，"2.0.3"
     * @param dart_sdk_version dart sdk 版本，比如："2.12.2"
     * @return pubspec.yaml 的新内容
     */
    std::string init_one(std::string flutter_project_root_dir, std::string flutter_sdk_version, std::string dart_sdk_version);

    /**
     * 为指定 flutter 工程扫描资源目录，自动为资源添加声明到 pubspec.yaml 和生成 r.g.dart
     * @param flutter_project_root_dir flutter 工程根目录路径
     * @param flutter_sdk_version flutter sdk 版本，比如：" 1.12.13+hotfix.5"，"2.0.3"
     * @param dart_sdk_version dart sdk 版本，比如："2.12.2"
     * @return
     */
    ResultOfGenerateOne generate_one(std::string flutter_project_root_dir, std::string flutter_sdk_version, std::string dart_sdk_version);

}


#endif //FLR_CORE_ENGINE_FLR_H

//
// Created by York on 2021/4/18.
//
// <ryml_std.hpp> 的include顺序需要在 <ryml.hpp> 之前，否则会报编译错误，具体issue看：https://github.com/biojppm/rapidyaml/issues/133
// 三方库
#include <ryml_std.hpp>
#include <ryml.hpp>
#include <fmt/format.h>

// 系统库
#include <fstream>
#include <sstream>
#include <filesystem>
#include <set>
#include <tuple>

#include "flr-core/flr.h"
#include "constant.h"
#include "util/file_util.h"
#include "util/asset_util.h"
#include "util/code_util.h"
#include "util/array_util.h"

namespace fs = std::__fs::filesystem;

namespace flr {
    void say_hi() {
        std::cout << "hello from flr" << std::endl;

        LOG_DEBUG << "hello from flr-log-debug-message";
        LOG_INFO << "hello from flr-log-info-message";
        LOG_WARN << "hello from flr-log-warn-message";
        LOG_ERROR << "hello from flr-log-error-message";
        LOG_FATAL << "hello from flr-log-fatal-message";

        UI_INFO << "hello from flr-ui-info-message";
        UI_TIPS << "hello from flr-ui-tips-message";
        UI_WARN << "hello from flr-ui-warn-message";
        UI_ERROR << "hello from flr-ui-error-message";

        std::string fmt_hi_str = fmt::format("hello from {}", "fmt");
        UI_INFO << fmt_hi_str;

        char yml_buf[] = "{hello: 1, worlds: [2, 3], map: {A: a}}";
        ryml::Tree tree = ryml::parse(ryml::substr(yml_buf));
        LOG_INFO << "hello from ryml:\n" << tree;
    }

    void register_log_message_callback(log_message_callback_t callback) {
        flr::log_message_callback = callback;
    }

    void register_ui_message_callback(ui_message_callback_t callback) {
        flr::ui_message_callback = callback;
    }

    void default_log_message_printer(const char *file, int line, const char *func, int severity, const char *content) {
        std::cout << "[flr]" << " [" << file << ":" << line << ":" << func << "] " << content << std::endl;
    }

    void default_ui_message_printer(int severity, const char *content) {
        std::cout << content << std::endl;
    }

    /**
     * @verbatim
     * check_flr_config_is_existed(pubspec_config)  ->  true/false
     *
     * 检测pubspec.yaml中是否存在flr的配置信息`flr_config`：
     *
     * ``` yaml
     * flr:
     *    core_version: 1.0.0
     *    dartfmt_line_length: 80
     *    assets:
     *    fonts:
     *```
     *
     * @endverbatim
     * @param pubspec_config
     * @return true/false
     */
    bool check_flr_config_is_existed(const ryml::NodeRef &pubspec_config) {
        if (pubspec_config.has_child("flr") == false) {
            return false;
        }

        ryml::NodeRef flr_config = pubspec_config["flr"];
        if (flr_config.is_map() == false) {
            return false;
        }

        return true;
    }

    /**
     * @verbatim
     * get_resource_dir_config(flutter_project_dir, flr_config)  ->  resource_dir_result_tuple
     *
     * 从 flr 配置信息中获取需要 flr 进行扫描的资源目录，返回结果是资源目录三元组 resource_dir_result_triplet
     *
     * resource_dir_result_tuple = [assets_legal_resource_dir_array, illegal_resource_dir_array, fonts_legal_resource_dir_array]
     *
     * === Examples
     * flr_config:
     *
     * ``` yaml
     * flr:
     *    core_version: 1.0.0
     *    dartfmt_line_length: 80
     *    assets:
     *      - lib/assets/images
     *      - lib/assets/texts
     *      - bad_path_to/non-existed_folder
     *    fonts:
     *      - lib/assets/fonts
     *```
     *
     * flutter_project_dir = "~/path/to/flutter_r_demo"
     * assets_legal_resource_dir_array = ["~/path/to/flutter_r_demo/lib/assets/images", "~/path/to/flutter_r_demo/lib/assets/texts"]
     * fonts_legal_resource_dir_array = ["~/path/to/flutter_r_demo/lib/assets/fonts"]
     * illegal_resource_dir_array = ["~/path/to/flutter_r_demo/bad_path_to/non-existed_folder"]
     *
     * @endverbatim
     *
     * @param flutter_project_root_dir
     * @param flr_config
     * @return resource_dir_result_tuple
     */
    std::tuple<std::vector<std::string>, std::vector<std::string>, std::vector<std::string>> get_resource_dir_config(std::string flutter_project_root_dir, const ryml::NodeRef &flr_config) {
        std::vector<std::string> assets_resource_dir_array;
        std::vector<std::string> fonts_resource_dir_array;

        if (flr_config.has_child("assets")) {
            ryml::NodeRef assets_seq = flr_config["assets"];
            if (assets_seq.is_seq() && assets_seq.num_children() > 0) {
                for (ryml::NodeRef asset_node: assets_seq.children()) {
                    if (asset_node.has_val() == false) {
                        continue;
                    }

                    std::string assets_resource_dir;
                    asset_node >> assets_resource_dir;

                    if (assets_resource_dir.length() <= 0 || assets_resource_dir == "") {
                        continue;
                    }

                    assets_resource_dir_array.push_back(assets_resource_dir);
                }
            }

        }

        if (flr_config.has_child("fonts")) {
            ryml::NodeRef assets_seq = flr_config["fonts"];
            if (assets_seq.is_seq() && assets_seq.num_children() > 0) {
                for (ryml::NodeRef asset_node: assets_seq.children()) {
                    if (asset_node.has_val() == false) {
                        continue;
                    }

                    std::string fonts_resource_dir;
                    asset_node >> fonts_resource_dir;

                    if (fonts_resource_dir.length() <= 0 || fonts_resource_dir == "") {
                        continue;
                    }

                    fonts_resource_dir_array.push_back(fonts_resource_dir);
                }
            }
        }

        // 过滤重复的 resource_dir
        flr::array_util::unique_string_array(assets_resource_dir_array);
        flr::array_util::unique_string_array(fonts_resource_dir_array);

        // 筛选合法的和非法的resource_dir
        std::vector<std::string> assets_legal_resource_dir_array;
        std::vector<std::string> fonts_legal_resource_dir_array;
        std::vector<std::string> illegal_resource_dir_array;

        for (std::string relative_resource_dir: assets_resource_dir_array) {
            std::string resource_dir = fmt::format("{0}/{1}", flutter_project_root_dir, relative_resource_dir);
            fs::path resource_dir_path(resource_dir);
            if (fs::exists(resource_dir)) {
                assets_legal_resource_dir_array.push_back(resource_dir);
            } else {
                illegal_resource_dir_array.push_back(resource_dir);
            }
        }

        for (std::string relative_resource_dir: fonts_resource_dir_array) {
            std::string resource_dir = fmt::format("{0}/{1}", flutter_project_root_dir, relative_resource_dir);
            fs::path resource_dir_path(resource_dir);
            if (fs::exists(resource_dir)) {
                fonts_legal_resource_dir_array.push_back(resource_dir);
            } else {
                illegal_resource_dir_array.push_back(resource_dir);
            }
        }

        std::tuple<std::vector<std::string>, std::vector<std::string>, std::vector<std::string>> resource_dir_result_tuple(assets_legal_resource_dir_array, fonts_legal_resource_dir_array, illegal_resource_dir_array);
        return resource_dir_result_tuple;
    }

    /**
     * @verbatim
     * compare_version(v1, v2) -> ret
     *
     * 比较版本字符串大小
     *
     * if v1 > v2, return 1
     * if v1 < v2, return -1
     * if v1 == v2, return 0
     *
     * 注意：版本字符串需要满足格式：MAJOR.MINOR.PATCH ，比如：2.0.3
     * @endverbatim
     *
     * @param v1
     * @param v2
     * @return ret
     */
    int compare_version(std::string v1, std::string v2) {
        int oct_v1[3], oct_v2[3];
        sscanf(v1.c_str(), "%d.%d.%d", &oct_v1[0], &oct_v1[1], &oct_v1[2]);
        sscanf(v2.c_str(), "%d.%d.%d", &oct_v2[0], &oct_v2[1], &oct_v2[2]);

        for (int i = 0; i < 3; i++) {
            if (oct_v1[i] > oct_v2[i]) {
                return 1;
            } else if (oct_v1[i] < oct_v2[i]) {
                return -1;
            }
        }

        return 0;
    }

    /**
     * @verbatim
     * is_should_support_nullsafety(flutter_sdk_version, dart_sdk_version) -> true/false
     *
     * 判断当前 flutter_sdk_version 和 dart_sdk_version 是否需要支持空安全
     * @endverbatim
     *
     * @param flutter_sdk_version flutter sdk 版本，比如："1.12.13+hotfix.5"，"2.0.3"
     * @param dart_sdk_version dart sdk 版本，比如："2.12.2"
     * @return true/false
     */
    bool is_should_support_nullsafety(std::string flutter_sdk_version, std::string dart_sdk_version) {
        if (flutter_sdk_version.empty() || dart_sdk_version.empty()) {
            return false;
        }

        /**
         * flutter sdk MAJOR.MINOR.PATCH version
         *
         * === Examples
         *
         * === Example-1
         * flutter_sdk_version = "1.12.13+hotfix.5"
         * flutter_sdk_mmp_version = "1.12.13"
         *
         * === Example-2
         * flutter_sdk_version = "1.12.13"
         * flutter_sdk_mmp_version = "1.12.13"
         */
        std::string flutter_sdk_mmp_version = flutter_sdk_version;
        {
            std::size_t hotfix_found = flutter_sdk_mmp_version.find("+");
            if (hotfix_found != std::string::npos) {
                flutter_sdk_mmp_version.erase(hotfix_found);
            }
        }

        std::string dart_sdk_mmp_version = dart_sdk_version;
        {
            std::size_t hotfix_found = dart_sdk_mmp_version.find("+");
            if (hotfix_found != std::string::npos) {
                dart_sdk_mmp_version.erase(hotfix_found);
            }
        }

        bool should_support = false;
        // v1 >= v2
        if (flr::compare_version(flutter_sdk_mmp_version, "1.10.15") >= 0) {
            // v1 >= v2
            if (flr::compare_version(dart_sdk_mmp_version, "2.12.0") >= 0) {
                should_support = true;
            }
        }

        return should_support;
    }

    /**
     * @verbatim
     * get_r_dart_library_version(flutter_sdk_version, dart_sdk_version) -> r_dart_library_version
     *
     * 返回适合 flutter_sdk_version 和 dart_sdk_version 的 r_dart_library package 的版本
     * 更多细节可看：https://github.com/YK-Unit/r_dart_library#dependency-relationship-table
     * @endverbatim
     *
     * @param flutter_sdk_version flutter sdk 版本，比如："1.12.13+hotfix.5"，"2.0.3"
     * @param dart_sdk_version dart sdk 版本，比如："2.12.2"
     * @return r_dart_library_version
     */
    std::string get_r_dart_library_version(std::string flutter_sdk_version, std::string dart_sdk_version) {
        std::string r_dart_library_version = "0.1.1";

        if (flutter_sdk_version.empty()) {
            return r_dart_library_version;
        }

        /**
         * flutter sdk MAJOR.MINOR.PATCH version
         *
         * === Examples
         *
         * === Example-1
         * flutter_sdk_version = "1.12.13+hotfix.5"
         * flutter_sdk_mmp_version = "1.12.13"
         *
         * === Example-2
         * flutter_sdk_version = "1.12.13"
         * flutter_sdk_mmp_version = "1.12.13"
         */
        std::string flutter_sdk_mmp_version = flutter_sdk_version;
        {
            std::size_t hotfix_found = flutter_sdk_mmp_version.find("+");
            if (hotfix_found != std::string::npos) {
                flutter_sdk_mmp_version.erase(hotfix_found);
            }
        }

        std::string dart_sdk_mmp_version = dart_sdk_version;
        {
            std::size_t hotfix_found = dart_sdk_mmp_version.find("+");
            if (hotfix_found != std::string::npos) {
                dart_sdk_mmp_version.erase(hotfix_found);
            }
        }

        // v1 >= v2
        if (flr::compare_version(flutter_sdk_mmp_version, "1.10.15") >= 0) {
            r_dart_library_version = "0.2.1";
            // v1 >= v2
            if (flr::compare_version(dart_sdk_mmp_version, "2.12.0") >= 0) {
                r_dart_library_version = "0.4.0-nullsafety.0";
            }
        }

        return r_dart_library_version;
    }

    std::string init_one(std::string flutter_project_root_dir,
                              std::string flutter_sdk_version,
                              std::string dart_sdk_version) {

        //  ----- Step-1 Begin -----
        // 进行环境检测；若发现不合法的环境，则提前结束：
        // - 检测pubspec.yaml是否存在
        // - 检测pubspec.yaml是否合法
        //

        std::string pubspec_file_path = flr::file_util::get_pubspec_file_path(flutter_project_root_dir);
        std::string pubspec_file_content = flr::file_util::load_pubspec_file(pubspec_file_path);
        if (pubspec_file_content.empty()) {
            std::string log_msg = fmt::format("{} not found", pubspec_file_path);
            UI_ERROR << log_msg;
            return "";
        }

        c4::substr pubspec_file_content_c4str = c4::to_substr(pubspec_file_content);
        ryml::Tree pubspec_tree = ryml::parse(pubspec_file_content_c4str);
        if (pubspec_tree.empty()) {
            std::string log_msg = fmt::format("{} is bad yaml", pubspec_file_path);
            UI_ERROR << log_msg;
            return "";
        }

        LOG_INFO << "old pubspec_tree:\n" << pubspec_tree << std::endl;

        // ----- Step-1 End -----

        // ----- Step-2 Begin -----
        // 添加 flr_config 和 r_dart_library 的依赖声明到 pubspec.yaml
        //

        //  ----- Step-2.1 -----
        //  添加flr_config到pubspec.yaml：检测当前是否存在flr_config；若不存在，则添加flr_config；若存在，则按照以下步骤处理：
        //  - 读取dartfmt_line_length选项、assets选项和fonts选项的值（这些选项值若存在，则应用于新建的flr_config；需要注意，使用前需要判断选项值是否合法：dartfmt_line_length选项值 >=80；assets选项和fonts选项的值为数组）
        //  - 新建flr_config，然后使用旧值或者默认值设置各个选项
        //
        // flr_config: Flr的配置信息
        // ```yaml
        // flr:
        //  core_version: 1.0.0
        //  dartfmt_line_length: 80
        //  assets: []
        //  fonts: []
        // ```
        //

        ryml::NodeRef pubspec_config = pubspec_tree.rootref();

        int dartfmt_line_length = flr::constant::DARTFMT_LINE_LENGTH;
        std::vector<std::string> asset_resource_dir_array;
        std::vector<std::string> font_resource_dir_array;

        if (pubspec_config.has_child("flr")) {
            ryml::NodeRef old_flr_config = pubspec_config["flr"];
            if (old_flr_config.is_map()) {
                if (old_flr_config.has_child("dartfmt_line_length")) {
                    old_flr_config["dartfmt_line_length"] >> dartfmt_line_length;
                    if (dartfmt_line_length < flr::constant::DARTFMT_LINE_LENGTH) {
                        dartfmt_line_length = flr::constant::DARTFMT_LINE_LENGTH;
                    }
                }

                if (old_flr_config.has_child("assets")) {
                    ryml::NodeRef assets_config = old_flr_config["assets"];
                    if (assets_config.is_val()) {
                        std::string asset_resource_dir;
                        assets_config >> asset_resource_dir;
                        if (asset_resource_dir.empty() == false) {
                            asset_resource_dir_array.push_back(asset_resource_dir);
                        }
                    } else if (assets_config.is_seq()) {
                        for (ryml::NodeRef asset_child: assets_config.children()) {
                            if (asset_child.is_val() == false) {
                                continue;
                            }
                            std::string asset_resource_dir;
                            asset_child >> asset_resource_dir;
                            if (asset_resource_dir.empty() == false) {
                                asset_resource_dir_array.push_back(asset_resource_dir);
                            }
                        }
                    }
                }

                if (old_flr_config.has_child("fonts")) {
                    ryml::NodeRef fonts_config = old_flr_config["fonts"];
                    if (fonts_config.is_val()) {
                        std::string font_resource_dir;
                        fonts_config >> font_resource_dir;
                        if (font_resource_dir.empty() == false) {
                            font_resource_dir_array.push_back(font_resource_dir);
                        }
                    } else if (fonts_config.is_seq()) {
                        for (ryml::NodeRef font_child: fonts_config.children()) {
                            if (font_child.is_val() == false) {
                                continue;
                            }
                            std::string font_resource_dir;
                            font_child >> font_resource_dir;
                            if (font_resource_dir.empty() == false) {
                                font_resource_dir_array.push_back(font_resource_dir);
                            }
                        }
                    }
                }
            }
        }

        ryml::NodeRef flr_config = pubspec_config["flr"];
        flr_config |= ryml::MAP;
        flr_config["core_version"] << flr::constant::CORE_VERSION;
        flr_config["dartfmt_line_length"] << dartfmt_line_length;
        flr_config["assets"] << asset_resource_dir_array;
        flr_config["fonts"] << font_resource_dir_array;

        //  ----- Step-2.2 -----
        // 添加 r_dart_library（https://github.com/YK-Unit/r_dart_library）的依赖声明
        //  - 获取正确的库版本
        //  - 添加依赖声明
        // r_dart_library的依赖声明：
        // ```yaml
        // r_dart_library: 0.1.1
        // ```
        //

        ryml::NodeRef dependencies_config = pubspec_config["dependencies"];
        dependencies_config |= ryml::MAP;
        std::string r_dart_library_version = flr::get_r_dart_library_version(flutter_sdk_version, dart_sdk_version);
        dependencies_config["r_dart_library"] << r_dart_library_version;

        // ----- Step-2 End -----

        // ----- Step-3 Begin -----
        // 对Flutter配置进行修正，以避免执行获取依赖操作时会失败：
        // - 检测Flutter配置中的assets选项是否是一个非空数组；若不是，则删除assets选项；
        // - 检测Flutter配置中的fonts选项是否是一个非空数组；若不是，则删除fonts选项。
        //

        if (pubspec_config.has_child("flutter")) {
            ryml::NodeRef flutter_config = pubspec_config["flutter"];
            flutter_config |= ryml::MAP;

            if (flutter_config.has_child("assets")) {
                ryml::NodeRef flutter_assets_config = flutter_config["assets"];
                if (flutter_assets_config.is_seq() == false || flutter_assets_config.empty()) {
                    flutter_config.remove_child(flutter_assets_config);
                }
            }

            if (flutter_config.has_child("fonts")) {
                ryml::NodeRef flutter_fonts_config = flutter_config["fonts"];
                if (flutter_fonts_config.is_seq() == false || flutter_fonts_config.empty()) {
                    flutter_config.remove_child(flutter_fonts_config);
                }
            }
        }

        // ----- Step-3 End -----

        std::string pubspec_file_new_content;
        ryml::emitrs<std::string>(pubspec_tree, pubspec_tree.root_id(), &pubspec_file_new_content);
        LOG_INFO << "new pubspec file content:\n" << pubspec_file_new_content << std::endl;
        return pubspec_file_new_content;
    }

    ResultOfGenerateOne generate_one(std::string flutter_project_root_dir,
                                  std::string flutter_sdk_version,
                                  std::string dart_sdk_version) {

        ResultOfGenerateOne result{"", ""};
        bool should_support_nullsafety = is_should_support_nullsafety(flutter_sdk_version, dart_sdk_version);

        // ----- Step-1 Begin -----
        // 进行环境检测；若发现不合法的环境，则提前结束：
        // - 检测当前flutter工程根目录是否存在pubspec.yaml
        // - 检测当前pubspec.yaml中是否存在Flr的配置
        // - 检测当前flr_config中的resource_dir配置是否合法：
        //   判断合法的标准是：assets配置或者fonts配置了至少1个legal_resource_dir

        std::string pubspec_file_path = flr::file_util::get_pubspec_file_path(flutter_project_root_dir);
        std::string pubspec_file_content = flr::file_util::load_pubspec_file(pubspec_file_path);
        if (pubspec_file_content.empty()) {
            std::string log_msg = fmt::format("{} not found", pubspec_file_path);
            UI_ERROR << log_msg;
            return result;
        }

        c4::substr pubspec_file_content_c4str = c4::to_substr(pubspec_file_content);
        ryml::Tree pubspec_tree = ryml::parse(pubspec_file_content_c4str);
        if (pubspec_tree.empty()) {
            std::string log_msg = fmt::format("{} is bad yaml", pubspec_file_path);
            UI_ERROR << log_msg;
            return result;
        }

        LOG_INFO << "old pubspec_tree:\n" << pubspec_tree << std::endl;

        ryml::NodeRef pubspec_config = pubspec_tree.rootref();
        if (flr::check_flr_config_is_existed(pubspec_config) == false) {
            std::string log_msg = fmt::format("{} is bad yaml", pubspec_file_path);
            UI_ERROR << log_msg;
            return result;
        }

        ryml::NodeRef flr_config = pubspec_config["flr"];
        flr_config |= ryml::MAP;

        std::tuple<std::vector<std::string>, std::vector<std::string>, std::vector<std::string>> resource_dir_result_tuple = get_resource_dir_config(flutter_project_root_dir, flr_config);
        std::vector<std::string> assets_legal_resource_dir_array = std::get<0>(resource_dir_result_tuple);
        std::vector<std::string> fonts_legal_resource_dir_array = std::get<1>(resource_dir_result_tuple);
        std::vector<std::string> illegal_resource_dir_array = std::get<2>(resource_dir_result_tuple);

        if (assets_legal_resource_dir_array.empty() && fonts_legal_resource_dir_array.empty()) {
            std::string log_msg = fmt::format(R"MSG(
[x]: have no valid resource directories configuration in pubspec.yaml
[*]: please manually configure the resource directories to fix it, for example:

    flr:
      core_version: {0}
      dartfmt_line_length: {1}
      # config the image and text resource directories that need to be scanned
      assets:
        - lib/assets/images
        - lib/assets/texts
      fonts:
        - lib/assets/fonts
)MSG", flr::constant::CORE_VERSION, flr::constant::DARTFMT_LINE_LENGTH);
            UI_ERROR << log_msg;
            return result;
        }

        bool is_package_project_type = flr::file_util::is_package_project_type(flutter_project_root_dir);
        std::string package_name;
        if (pubspec_config.has_child("name")) {
            pubspec_config["name"] >> package_name;
        }
        if (package_name.empty()) {
            std::string log_msg = fmt::format("[x]: {} is bad yaml：package name is required", pubspec_file_path);
            UI_ERROR << log_msg;
            return result;
        }
        // ----- Step-1 End -----

        // ----- Step-2 Begin -----
        // 进行核心逻辑版本检测：
        // 检测flr_config中的core_version和当前工具的core_version是否一致；若不一致，则按照以下规则处理：
        //  - 更新flr_config中的core_version的值为当前工具的core_version；
        //  - 生成“核心逻辑版本不一致”的警告日志，存放到警告日志数组。
        //

        // 警告日志流
        std::stringstream warning_messages;

        std::string flr_core_version = "unknown";
        if (flr_config.has_child("core_version")) {
            flr_config["core_version"] >> flr_core_version;
        }

        if (flr_core_version != flr::constant::CORE_VERSION) {
            flr_config["core_version"] << flr::constant::CORE_VERSION;

            std::string warning_msg = fmt::format(R"MSG(
[!]: warning, some team members may be using Flr tool with core_version {0}, while you are using Flr tool with core_version {1}
[*]: to fix it, you and your team members should use the Flr tool with same core_version
[*]: \"core_version\" is the core logic version of Flr tool, you can run \"flr version\" to get it
)MSG", flr_core_version, flr::constant::CORE_VERSION);
            warning_messages << warning_msg << std::endl;
        }

        // ----- Step-2 End -----

        // ----- Step-3 Begin -----
        // 获取assets_legal_resource_dir数组、fonts_legal_resource_dir数组和illegal_resource_dir数组：
        // - 从flr_config中的assets配置获取assets_legal_resource_dir数组和assets_illegal_resource_dir数组；
        // - 从flr_config中的fonts配置获取fonts_legal_resource_dir数组和fonts_illegal_resource_dir数组；
        // - 合并assets_illegal_resource_dir数组和fonts_illegal_resource_dir数组为illegal_resource_dir数组‘；若illegal_resource_dir数组长度大于0，则生成“存在非法的资源目录”的警告日志，存放到警告日志数组。
        //

        if (illegal_resource_dir_array.empty() == false) {
            std::string warning_msg = "[!]: warning, found the following resource directory which is not existed: ";
            warning_messages << warning_msg << std::endl;
            for (const std::string &illegal_resource_dir : illegal_resource_dir_array) {
                warning_msg = fmt::format("  -{0}", illegal_resource_dir);
                warning_messages << warning_msg << std::endl;
            }
        }

        // ----- Step-3 End -----

        // 扫描资源
        UI_INFO << "scan assets now ..";

        // ----- Step-4 Begin -----
        // 扫描assets_legal_resource_dir数组中的legal_resource_dir，输出有序的image_asset数组、non_svg_image_asset数组、svg_image_asset数组、illegal_image_file数组：
        // - 创建image_asset数组、illegal_image_file数组；
        // - 遍历assets_legal_resource_dir数组，按照如下处理每个资源目录：
        //  - 扫描当前资源目录和其所有层级的子目录，查找所有image_file；
        //  - 根据legal_resource_file的标准，筛选查找结果生成legal_image_file子数组和illegal_image_file子数组；illegal_image_file子数组合并到illegal_image_file数组；
        //  - 根据image_asset的定义，遍历legal_image_file子数组，生成image_asset子数；组；image_asset子数组合并到image_asset数组。
        // - 对image_asset数组做去重处理；
        // - 按照字典顺序对image_asset数组做升序排列（一般使用开发语言提供的默认的sort算法即可）；
        // - 按照SVG分类，从image_asset数组筛选得到有序的non_svg_image_asset数组和svg_image_asset数组：
        //  - 按照SVG分类，从image_asset数组筛选得到non_svg_image_asset数组和svg_image_asset数组；
        //  - 按照字典顺序对non_svg_image_asset数组和svg_image_asset数组做升序排列（一般使用开发语言提供的默认的sort算法即可）；
        // - 输出有序的image_asset数组、non_svg_image_asset数组、svg_image_asset数组、illegal_image_file数组。
        //
        std::vector<std::string> image_asset_array;
        std::vector<std::string> illegal_image_file_array;

        for (const std::string &resource_dir : assets_legal_resource_dir_array) {
            std::tuple<std::vector<std::string>, std::vector<std::string>> image_file_result_tuple = flr::file_util::find_image_files(resource_dir);
            std::vector<std::string> legal_image_file_subarray = std::get<0>(image_file_result_tuple);
            std::vector<std::string> illegal_image_file_subarray = std::get<1>(image_file_result_tuple);

            illegal_image_file_array.insert(illegal_image_file_array.end(), illegal_image_file_subarray.begin(), illegal_image_file_subarray.end());
            std::vector<std::string> image_asset_subarray = flr::asset_util::generate_image_assets(flutter_project_root_dir,package_name, legal_image_file_subarray);
            image_asset_array.insert(image_asset_array.end(), image_asset_subarray.begin(), image_asset_subarray.end());
        }

        flr::array_util::unique_string_array(image_asset_array);
        flr::array_util::sort_string_array(image_asset_array);

        std::vector<std::string> non_svg_image_asset_array;
        std::vector<std::string> svg_image_asset_array;
        for (const std::string image_asset : image_asset_array) {
            if (flr::file_util::is_svg_image_resource_file(image_asset)) {
                svg_image_asset_array.push_back(image_asset);
            } else {
                non_svg_image_asset_array.push_back(image_asset);
            }
        }

        flr::array_util::sort_string_array(non_svg_image_asset_array);
        flr::array_util::sort_string_array(svg_image_asset_array);

        // ----- Step-4 End -----

        // ----- Step-5 Begin -----
        // 扫描assets_legal_resource_dir数组中的legal_resource_dir，输出text_asset数组和illegal_text_file数组：
        // - 创建text_asset数组、illegal_text_file数组；
        // - 遍历assets_legal_resource_dir数组，按照如下处理每个资源目录：
        //  - 扫描当前资源目录和其所有层级的子目录，查找所有text_file；
        //  - 根据legal_resource_file的标准，筛选查找结果生成legal_text_file子数组和illegal_text_file子数组；illegal_text_file子数组合并到illegal_text_file数组；
        //  - 根据text_asset的定义，遍历legal_text_file子数组，生成text_asset子数组；text_asset子数组合并到text_asset数组。
        // - 对text_asset数组做去重处理；
        // - 按照字典顺序对text_asset数组做升序排列（一般使用开发语言提供的默认的sort算法即可）；
        // - 输出text_asset数组和illegal_image_file数组。
        //
        std::vector<std::string> text_asset_array;
        std::vector<std::string> illegal_text_file_array;

        for (const std::string &resource_dir : assets_legal_resource_dir_array) {
            std::tuple<std::vector<std::string>, std::vector<std::string>> text_file_result_tuple = flr::file_util::find_text_files(resource_dir);
            std::vector<std::string> legal_text_file_subarray = std::get<0>(text_file_result_tuple);
            std::vector<std::string> illegal_text_file_subarray = std::get<1>(text_file_result_tuple);

            illegal_text_file_array.insert(illegal_text_file_array.end(), illegal_text_file_subarray.begin(), illegal_text_file_subarray.end());
            std::vector<std::string> text_asset_subarray = flr::asset_util::generate_text_assets(flutter_project_root_dir,package_name, legal_text_file_subarray);
            text_asset_array.insert(text_asset_array.end(), text_asset_subarray.begin(), text_asset_subarray.end());
        }

        flr::array_util::unique_string_array(text_asset_array);
        flr::array_util::sort_string_array(text_asset_array);

        // ----- Step-5 End -----

        // ----- Step-6 Begin -----
        // 扫描fonts_legal_resource_dir数组中的legal_resource_dir，输出font_family_config数组、illegal_font_file数组：
        // - 创建font_family_config数组、illegal_font_file数组；
        // - 遍历fonts_legal_resource_dir数组，按照如下处理每个资源目录：
        //  - 扫描当前资源目录，获得其第1级子目录数组，并按照字典顺序对数组做升序排列（一般使用开发语言提供的默认的sort算法即可）；
        //  - 遍历第1级子目录数组，按照如下处理每个子目录：
        //    - 获取当前子目录的名称，生成font_family_name；
        //    - 扫描当前子目录和其所有子目录，查找所有font_file；
        //    - 根据legal_resource_file的标准，筛选查找结果生成legal_font_file数组和illegal_font_file子数组；illegal_font_file子数组合并到illegal_font_file数组；
        //    - 据font_asset的定义，遍历legal_font_file数组，生成font_asset_config数组；
        //    - 按照字典顺序对生成font_asset_config数组做升序排列（比较asset的值）；
        //    - 根据font_family_config的定义，为当前子目录组织font_family_name和font_asset_config数组生成font_family_config对象，添加到font_family_config子数组；font_family_config子数组合并到font_family_config数组。
        // - 输出font_family_config数组、illegal_font_file数组；
        // - 按照字典顺序对font_family_config数组做升序排列（比较family的值）。
        //
        std::vector<flr::code_util::FontFamilyConfig> font_family_config_array;
        std::vector<std::string> illegal_font_file_array;

        for (const std::string &resource_dir : assets_legal_resource_dir_array) {
            std::vector<std::string> font_family_dir_array = flr::file_util::find_top_child_dirs(resource_dir);

            for (const std::string &font_family_dir : font_family_dir_array) {
                std::string font_family_name = fs::path(font_family_dir).stem().string();

                std::tuple<std::vector<std::string>, std::vector<std::string>> font_file_result_tuple = flr::file_util::find_font_files_in_font_family_dir(font_family_dir);
                std::vector<std::string> legal_font_file_array = std::get<0>(font_file_result_tuple);
                std::vector<std::string> illegal_font_file_subarray = std::get<1>(font_file_result_tuple);

                illegal_font_file_array.insert(illegal_font_file_array.end(), illegal_font_file_subarray.begin(), illegal_font_file_subarray.end());

                if (legal_font_file_array.empty()) {
                    continue;
                }

                std::vector<std::string> font_asset_array = flr::asset_util::generate_font_assets(
                        flutter_project_root_dir, package_name, legal_font_file_array);
                flr::array_util::sort_string_array(font_asset_array);
                flr::code_util::FontFamilyConfig font_family_config(font_family_name, font_asset_array);
                font_family_config_array.push_back(font_family_config);
            }
        }

        std::sort(font_family_config_array.begin(), font_family_config_array.end(), [](const flr::code_util::FontFamilyConfig &a_config, const flr::code_util::FontFamilyConfig &b_config)
        {
            std::string a_family = std::get<0>(a_config);
            std::string b_family = std::get<0>(b_config);
            return a_family.compare(b_family);
        } );

        // ----- Step-6 End -----

        UI_INFO << "scan assets done !!!";

        // ----- Step-7 Begin -----
        // 检测是否存在illegal_resource_file：
        // - 合并illegal_image_file数组、illegal_text_file数组和illegal_font_file数组为illegal_resource_file数组；
        // - 若illegal_resource_file数组长度大于0，则生成“存在非法的资源文件”的警告日志，存放到警告日志数组。
        //
        std::vector<std::string> illegal_resource_file_array;
        illegal_resource_file_array.insert(illegal_resource_file_array.end(), illegal_image_file_array.begin(), illegal_image_file_array.end());
        illegal_resource_file_array.insert(illegal_resource_file_array.end(), illegal_text_file_array.begin(), illegal_text_file_array.end());
        illegal_resource_file_array.insert(illegal_resource_file_array.end(), illegal_font_file_array.begin(), illegal_font_file_array.end());
        if (illegal_resource_file_array.empty() == false) {
            warning_messages << "[!]: warning, found the following illegal resource file who's file basename contains illegal characters:" << std::endl;
            for (const std::string &resource_file : illegal_resource_file_array) {
                std::string warning_msg = fmt::format("  -{0}", resource_file);
                warning_messages << warning_msg << std::endl;
            }
            warning_messages << "[*]: to fix it, you should only use letters (a-z, A-Z), numbers (0-9), and the other legal characters ('_', '+', '-', '.', '·', '!', '@', '&', '$', '￥') to name the file" << std::endl;
        }

        // ----- Step-7 End -----

        UI_INFO << "specify scanned assets in pubspec.yaml now ...";

        // ----- Step-8 Begin -----
        // 为扫描得到的legal_resource_file添加资源声明到pubspec.yaml：
        // - 合并image_asset数组和text_asset数组为new_asset_array（image_asset数组元素在前）；
        // - 读取pubspec.yaml中flutter-assets配置，获得old_asset_array，然后和new_asset_array合并为asset数组；
        // - 修改pubspec.yaml中flutter-assets配置的值为asset数组；
        // - 修改pubspec.yaml中flutter-fonts配置的值为font_family_config数组。
        ryml::NodeRef flutter_config = pubspec_config["flutter"];
        // add flutter node if not exited
        flutter_config |= ryml::MAP;

        std::vector<std::string> new_asset_array;
        new_asset_array.insert(new_asset_array.end(), image_asset_array.begin(), image_asset_array.end());
        new_asset_array.insert(new_asset_array.end(), text_asset_array.begin(), text_asset_array.end());

        std::vector<std::string> old_asset_array;
        if (flutter_config.has_child("assets")) {
            ryml::NodeRef assets_seq = flutter_config["assets"];
            if (assets_seq.is_seq() && assets_seq.num_children() > 0) {
                for (ryml::NodeRef asset_node: assets_seq.children()) {
                    if (asset_node.has_val() == false) {
                        continue;
                    }

                    std::string asset_str;
                    asset_node >> asset_str;

                    if (asset_str.length() <= 0 || asset_str == "") {
                        continue;
                    }

                    old_asset_array.push_back(asset_str);
                }
            }
        }

        std::vector<std::string> asset_array = flr::asset_util::merge_flutter_assets(flutter_project_root_dir, package_name, new_asset_array, old_asset_array);
        if (asset_array.empty()) {
            flutter_config.remove_child("assets");
        } else {
            flutter_config["assets"] << asset_array;
        }

        std::string pubspec_file_new_content = "";
        ryml::emitrs<std::string>(pubspec_tree, pubspec_tree.root_id(), &pubspec_file_new_content);
        LOG_INFO << "new pubspec file content:\n" << pubspec_file_new_content << std::endl;
        result.pubspec_file_new_content = pubspec_file_new_content;

        //  ----- Step-8 End -----

        UI_INFO << "specify scanned assets in pubspec.yaml done !!!";

        // ----- Step-9 Begin -----
        // 分别遍历non_svg_image_asset数组、svg_image_asset数组、text_asset数组，
        // 根据asset_id生成算法，分别输出non_svg_image_asset_id字典、svg_image_asset_id 字典、text_asset_id字典。
        // 字典的key为asset，value为asset_id。
        //
        std::map<std::string, std::string> non_svg_image_asset_id_dict, svg_image_asset_id_dict, text_asset_id_dict;
        std::vector<std::string> used_non_svg_image_asset_id_array, used_svg_image_asset_id_array, used_text_asset_id_array;

        for (std::string asset: non_svg_image_asset_array) {
            std::string asset_id = flr::code_util::generate_asset_id(asset, used_non_svg_image_asset_id_array, flr::constant::PRIOR_NON_SVG_IMAGE_FILE_TYPE);
            used_non_svg_image_asset_id_array.push_back(asset_id);
            non_svg_image_asset_id_dict[asset] = asset_id;
        }

        for (std::string asset: svg_image_asset_array) {
            std::string asset_id = flr::code_util::generate_asset_id(asset, used_svg_image_asset_id_array, flr::constant::PRIOR_SVG_IMAGE_FILE_TYPE);
            used_svg_image_asset_id_array.push_back(asset_id);
            svg_image_asset_id_dict[asset] = asset_id;
        }

        for (std::string asset: text_asset_array) {
            std::string asset_id = flr::code_util::generate_asset_id(asset, used_text_asset_id_array, flr::constant::PRIOR_TEXT_FILE_TYPE);
            used_text_asset_id_array.push_back(asset_id);
            text_asset_id_dict[asset] = asset_id;
        }

        // ----- Step-9 End -----

        UI_INFO << "generate \"r.g.dart\" now ...";

        // ----- Step-10 Begin -----
        // 创建r.g.dart文件接收流
        //

        std::stringstream r_dart_file_content_stream;

        //  ----- Step-10 End -----

        // ----- Step-11 Begin -----
        // 生成 R 类的代码，追加写入r.g.dart
        //
        std::string g_R_class_code = flr::code_util::generate_R_class(package_name);
        r_dart_file_content_stream << g_R_class_code;

        //  ----- Step-11 End -----

        // ----- Step-12 Begin -----
        // 生成 AssetResource 类的代码，追加写入r.g.dart
        //
        r_dart_file_content_stream << "\n";
        std::string g_AssetResource_class_code = flr::code_util::generate_AssetResource_class(package_name, should_support_nullsafety);
        r_dart_file_content_stream << g_AssetResource_class_code;

        // ----- Step-12 End -----

        // ----- Step-13 Begin -----
        // 遍历 non_svg_image_asset 数组，生成 _R_Image_AssetResource 类，追加写入 r.g.dart
        //
        r_dart_file_content_stream << "\n";
        std::string g__R_Image_AssetResource_class_code = flr::code_util::generate__R_Image_AssetResource_class(non_svg_image_asset_array, non_svg_image_asset_id_dict, package_name, is_package_project_type);
        r_dart_file_content_stream << g__R_Image_AssetResource_class_code;

        // ----- Step-13 End -----

        // ----- Step-14 Begin -----
        // 遍历 svg_image_asset 数组，生成 _R_Svg_AssetResource 类，追加写入 r.g.dart。
        //
        r_dart_file_content_stream << "\n";
        std::string g__R_Svg_AssetResource_class_code = flr::code_util::generate__R_Svg_AssetResource_class(svg_image_asset_array, svg_image_asset_id_dict,  package_name, is_package_project_type);
        r_dart_file_content_stream << g__R_Svg_AssetResource_class_code;

        // ----- Step-14 End -----

        // ----- Step-15 Begin -----
        // 遍历 text_asset 数组，生成 _R_Image_AssetResource 类，追加写入 r.g.dart
        //
        r_dart_file_content_stream << "\n";
        std::string g__R_Text_AssetResource_class_code = flr::code_util::generate__R_Text_AssetResource_class(text_asset_array, text_asset_id_dict, package_name, is_package_project_type);
        r_dart_file_content_stream << g__R_Text_AssetResource_class_code;

        // ----- Step-15 End -----

        // ----- Step-16 Begin -----
        // 遍历non_svg_image_asset数组，生成 _R_Image 类，追加写入 r.g.dart
        //
        r_dart_file_content_stream << "\n";
        std::string g__R_Image_class_code = flr::code_util::generate__R_Image_class(non_svg_image_asset_array, non_svg_image_asset_id_dict, package_name);
        r_dart_file_content_stream << g__R_Image_class_code;

        // ----- Step-16 End -----

        // ----- Step-17 Begin -----
        // 遍历 svg_image_asset 数组，生成 _R_Svg 类，追加写入 r.g.dart。
        //
        r_dart_file_content_stream << "\n";
        std::string g__R_Svg_class_code = flr::code_util::generate__R_Svg_class(svg_image_asset_array, svg_image_asset_id_dict, package_name, should_support_nullsafety);
        r_dart_file_content_stream << g__R_Svg_class_code;

        // ----- Step-17 End -----

        // ----- Step-18 Begin -----
        // 遍历 text_asset 数组，生成 _R_Image 类，追加写入 r.g.dart。
        //
        r_dart_file_content_stream << "\n";
        std::string g__R_Text_class_code = flr::code_util::generate__R_Text_class(text_asset_array, text_asset_id_dict, package_name);
        r_dart_file_content_stream << g__R_Text_class_code;

        // ----- Step-18 End -----

        // ----- Step-19 Begin -----
        // 遍历font_family_config数组，根据下面的模板生成_R_Font_Family类，追加写入r.g.dart。
        //
        r_dart_file_content_stream << "\n";
        std::string g__R_Font_Family_class_code = flr::code_util::generate__R_FontFamily_class(font_family_config_array, package_name);
        r_dart_file_content_stream << g__R_Font_Family_class_code;

        // ----- Step-19 End -----

        // ----- Step-20 Begin -----
        // 结束操作，保存 r.g.dart
        //
        result.r_dart_file_content = r_dart_file_content_stream.str();

        // ----- Step-20 End -----

        // ----- Step-21 Begin -----
        // 判断警告日志数组是否为空，若不为空，输出所有警告日志
        //
        std::string warning_messages_str = warning_messages.str();
        if (warning_messages_str.length() > 0 ) {
            UI_WARN << warning_messages_str;
        }

        // ----- Step-21 End -----

        UI_INFO << "generate \"r.g.dart\" done !!!";

        return result;
    }
}


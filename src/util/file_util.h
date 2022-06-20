//
// Created by York on 2021/7/17.
//

#ifndef FLR_CORE_ENGINE_EXAMPLE_FILE_UTIL_H
#define FLR_CORE_ENGINE_EXAMPLE_FILE_UTIL_H

#include <iostream>
#include <vector>
#include <tuple>

namespace flr {
    namespace file_util {
        /**
         * @verbatim
         * get_flutter_sub_project_root_dirs -> [sub_project_root_dir]
         *
         * 获取flutter主工程的所有子工程的根目录
         *
         * @endverbatim
         *
         * @param flutter_main_project_root_dir
         * @return [sub_project_root_dir]
         */
        std::vector<std::string> get_flutter_sub_project_root_dirs(std::string flutter_main_project_root_dir);

        /**
         * @verbatim
         * get_pubspec_file_path(flutter_project_dir) -> pubspec_file_path
         *
         * 获取当前flutter工程的pubspec.yaml文件的路径
         *
         * === Examples
         * flutter_project_dir = "~/path/to/flutter_r_demo"
         * pubspec_file_path = "~/path/to/flutter_r_demo/pubspec.yaml"
         *
         * @endverbatim
         * @param flutter_project_dir
         * @return pubspec_file_path
         */
        std::string get_pubspec_file_path(std::string flutter_project_dir);

        /**
         * @verbatim
         * load_pubspec_file(pubspec_file_path) -> pubspec_file_content
         *
         * 读取 pubspec.yaml 文件内容
         * @endverbatim
         *
         * @param pubspec_file_path
         * @return pubspec_file_content
         */
        std::string load_pubspec_file(std::string pubspec_file_path);

        /**
         *  @verbatim
         *  is_package_project_type?(flutter_project_dir) -> true/false
         *
         * 判断当前flutter工程的工程类型是不是Package工程类型
         *
         * flutter工程共有4种工程类型：
         * - app：Flutter App工程，用于开发纯Flutter的App
         * - module：Flutter Component工程，用于开发Flutter组件以嵌入iOS和Android原生工程
         * - package：General Dart Package工程，用于开发一个供应用层开发者使用的包
         * - plugin：Plugin Package工程（属于特殊的Dart Package工程），用于开发一个调用特定平台API的包
         *
         * flutter工程的工程类型可从flutter工程目录的 .metadata 文件中读取获得
         * 如果不存在 .metadata 文件，则判断 pubspec.yaml 是否存在 author 配置，若存在，说明是一个 Package工程
         *
         * @endverbatim
         *
         * @param flutter_project_dir
         * @return true/false
         */
        bool is_package_project_type(std::string flutter_project_dir);

        /**
         * 判断当前文件是不是非SVG类图片资源文件
         * @param file
         * @return true/false
         */
        bool is_non_svg_image_resource_file(std::string file);

        /**
         * 判断当前文件是不是SVG类图片资源文件
         * @param file
         * @return true/false
         */
        bool is_svg_image_resource_file(std::string file);

        /**
         * 判断当前文件是不是图片资源文件
         * @param file
         * @return true/false
         */
        bool is_image_resource_file(std::string file);

        /**
         * 判断当前文件是不是文本资源文件
         * @param file
         * @return true/false
         */
        bool is_text_resource_file(std::string file);

        /**
         * 判断当前文件是不是字体资源文件
         * @param file
         * @return true/false
         */
        bool is_font_resource_file(std::string file);

        /**
         * @verbatim
         * is_legal_resource_file(file) -> true/false
         *
         * 判断当前资源文件是否合法
         *
         * 判断资源文件合法的标准是：
         * 其file_basename_no_extension 由字母（a-z、A-Z）、数字（0-9）、其他合法字符（'_', '+', '-', '.', '·', '!', '@', '&', '$', '￥'）组成
         *
         * === Examples
         * good_file = "~/path/to/flutter_project/lib/assets/images/test.png"
         * bad_file = "~/path/to/flutter_project/lib/assets/images/~.png"
         *
         * @endverbatim
         * @param file
         * @return true/false
         */
        bool is_legal_resource_file(std::string file);

        /**
         * @verbatim
         * find_image_files(resource_dir)  ->  image_file_result_tuple
         *
         * 扫描指定的资源目录和其所有层级的子目录，查找所有图片文件
         * 返回图片文件结果二元组 image_file_result_tuple
         * image_file_result_tuple = [legal_image_file_array, illegal_image_file_array]
         *
         * 判断文件合法的标准参考 is_legal_resource_file 方法
         *
         * === Examples
         * resource_dir = "~/path/to/flutter_project/lib/assets/images"
         * legal_image_file_array = ["~/path/to/flutter_project/lib/assets/images/test.png", "~/path/to/flutter_project/lib/assets/images/2.0x/test.png"]
         * illegal_image_file_array = ["~/path/to/flutter_project/lib/assets/images/~.png"]
         * @endverbatim
         *
         * @param resource_dir
         * @return image_file_result_tuple
         */
        std::tuple<std::vector<std::string>, std::vector<std::string>> find_image_files(std::string resource_dir);

        /**
         * @verbatim
         * find_text_files(resource_dir)  ->  text_file_result_tuple
         *
         * 扫描指定的资源目录和其所有层级的子目录，查找所有文本文件
         * 返回文本文件结果二元组 text_file_result_tuple
         * text_file_result_tuple = [legal_text_file_array, illegal_text_file_array]
         *
         * 判断文件合法的标准参考 is_legal_resource_file 方法
         *
         * === Examples
         * resource_dir = "~/path/to/flutter_project/lib/assets/jsons"
         * legal_text_file_array = ["~/path/to/flutter_project/lib/assets/jsons/city.json", "~/path/to/flutter_project/lib/assets/jsons/mock/city.json"]
         * illegal_text_file_array = ["~/path/to/flutter_project/lib/assets/jsons/~.json"]
         * @endverbatim
         *
         * @param resource_dir
         * @return text_file_result_tuple
         */
        std::tuple<std::vector<std::string>, std::vector<std::string>> find_text_files(std::string resource_dir);

        /**
         * @verbatim
         * find_top_child_dirs(resource_dir) -> top_child_dir_array
         *
         * 扫描指定的资源目录，返回其所有第一级子目录
         *
         * === Examples
         * resource_dir = "~/path/to/flutter_project/lib/assets/fonts"
         * top_child_dir_array = ["~/path/to/flutter_project/lib/assets/fonts/Amiri", "~/path/to/flutter_project/lib/assets/fonts/Open_Sans"]
         * @endverbatim
         *
         * @param resource_dir
         * @return top_child_dir_array
         */
        std::vector<std::string> find_top_child_dirs(std::string resource_dir);

        /**
        * @verbatim
        * find_font_files_in_font_family_dir(font_family_dir)  ->  font_file_result_tuple
        *
        * 扫描指定的字体家族目录和其所有层级的子目录，查找所有字体文件
        * 返回字体文件结果二元组 font_file_result_tuple
        * font_file_result_tuple = [legal_font_file_array, illegal_font_file_array]
        *
        * 判断文件合法的标准参考 is_legal_resource_file 方法
        *
        * === Examples
        * font_family_dir = "~/path/to/flutter_project/lib/assets/fonts/Amiri"
        * legal_font_file_array = ["~/path/to/flutter_project/lib/assets/fonts/Amiri/Amiri-Regular.ttf", "~/path/to/flutter_project/lib/assets/fonts/Amiri/Amiri-Bold.ttf"]
        * illegal_font_file_array = ["~/path/to/flutter_project/lib/assets/fonts/Amiri/~.ttf"]
        * @endverbatim
        *
        * @param resource_dir
        * @return font_file_result_tuple
        */
        std::tuple<std::vector<std::string>, std::vector<std::string>> find_font_files_in_font_family_dir(std::string font_family_dir);
    };
}

#endif //FLR_CORE_ENGINE_EXAMPLE_FILE_UTIL_H

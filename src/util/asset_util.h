//
// Created by York on 2021/7/25.
//

#ifndef FLR_CORE_ENGINE_EXAMPLE_ASSET_UTIL_H
#define FLR_CORE_ENGINE_EXAMPLE_ASSET_UTIL_H

#include <iostream>
#include <vector>
#include <map>

namespace flr {
    namespace asset_util {

        /**
         * @verbatim
         * is_asset_variant(legal_resource_file) -> true/false
         *
         * 判断当前的资源文件是不是资产变体（asset_variant）类型
         *
         * 判断的核心算法是：
         * - 获取资源文件的父目录；
         * - 判断父目录是否符合资产变体目录的特征
         *    资产变体映射的的资源文件要求存放在“与 main_asset 在同一个目录下的”、“符合指定特征的”子目录中；
         *    截止目前，Flutter只支持一种变体类型：倍率变体；
         *    倍率变体只适用于非SVG类图片资源；
         *    倍率变体目录特征可使用此正则来判断：“^((0\.[0-9]+)|([1-9]+[0-9]*(\.[0-9]+)?))[x]$”；
         *    倍率变体目录名称示例：“0.5x”、“1.5x”、“2.0x”、“3.0x”，“2x”、“3x”；
         *
         * @endverbatim
         *
         * @param legal_resource_file
         * @return true/false
         */
        bool is_asset_variant(std::string legal_resource_file);

        /**
         * @verbatim
         * is_image_asset(asset) -> true/false
         *
         * 判断当前资产是不是图片类资产
         *
         * === Examples
         *
         * === Example-1
         * asset = "packages/flutter_r_demo/assets/images/test.png"
         * @return true
         *
         * === Example-2
         * asset = "assets/images/test.png"
         * @return true
         *
         * @endverbatim
         *
         * @param asset
         * @return true/false
         */
        bool is_image_asset(std::string asset);

        /**
         * @verbatim
         * is_package_asset(std::string asset) -> true/false
         *
         * 判断当前资产是不是package类资产
         *
         * === Examples
         *
         * === Example-1
         * asset = "packages/flutter_r_demo/assets/images/test.png"
         * @return true
         *
         * === Example-2
         * asset = "assets/images/test.png"
         * @return false
         *
         * @endverbatim
         *
         * @param asset
         * @return true/false
         */
        bool is_package_asset(std::string asset);

        /**
         * @verbatim
         * is_specified_package_asset(package_name, asset) -> true/false
         *
         * 判断当前资产是不是指定的package的资产
         *
         * === Examples
         * package_name = "flutter_r_demo"
         *
         * === Example-1
         * asset = "packages/flutter_r_demo/assets/images/test.png"
         * @return true
         *
         * === Example-2
         * asset = "packages/hello_demo/assets/images/test.png"
         * @return false
         *
         * @endverbatim
         *
         * @param package_name
         * @param asset
         * @return true/false
         */
        bool is_specified_package_asset(std::string package_name, std::string asset);

        /**
         * @verbatim
         * get_main_resource_file(flutter_project_dir, package_name, asset) -> main_resource_file
         *
         * 获取指定flutter工程的asset对应的主资源文件
         * 注意：主资源文件不一定存在，比如图片资产可能只存在变体资源文件
         *
         * === Examples
         * flutter_project_dir = "~/path/to/flutter_r_demo"
         * package_name = "flutter_r_demo"
         *
         * === Example-1
         * asset = "packages/flutter_r_demo/assets/images/test.png"
         * main_resource_file = "~/path/to/flutter_r_demo/lib/assets/images/test.png"
         *
         * === Example-2
         * asset = "assets/images/test.png"
         * main_resource_file = "~/path/to/flutter_r_demo/assets/images/test.png"
         *
         * @endverbatim
         *
         * @param flutter_project_dir
         * @param package_name
         * @param asset
         * @return main_resource_file
         */
        std::string get_main_resource_file(std::string flutter_project_dir, std::string package_name, std::string asset);

        /**
         * @verbatim
         * is_asset_existed(flutter_project_dir, package_name, asset) -> true/false
         *
         * 判断指定flutter工程的asset是不是存在；存在的判断标准是：asset需要存在对应的资源文件
         *
         * === Examples
         * flutter_project_dir = "~/path/to/flutter_r_demo"
         * package_name = "flutter_r_demo"
         *
         * === Example-1
         * asset = "packages/flutter_r_demo/assets/images/test.png"
         * @return true
         *
         * === Example-2
         * asset = "packages/flutter_r_demo/404/not-existed.png"
         * @return false
         *
         * @endverbatim
         *
         * @param flutter_project_dir
         * @param package_name
         * @param asset
         * @return true/false
         */
        bool is_asset_existed(std::string flutter_project_dir, std::string package_name, std::string asset);

        /**
         * @verbatim
         * generate_main_asset(flutter_project_dir, package_name, legal_resource_file) -> main_asset
         *
         * 为当前资源文件生成 main_asset
         *
         * === Examples
         * flutter_project_dir = "~/path/to/flutter_r_demo"
         * package_name = "flutter_r_demo"
         *
         * === Example-1
         * legal_resource_file = "~/path/to/flutter_r_demo/lib/assets/images/test.png"
         * main_asset = "packages/flutter_r_demo/assets/images/test.png"
         *
         * === Example-2
         * legal_resource_file = "~/path/to/flutter_r_demo/lib/assets/images/3.0x/test.png"
         * main_asset = "packages/flutter_r_demo/assets/images/test.png"
         *
         * === Example-3
         * legal_resource_file = "~/path/to/flutter_r_demo/lib/assets/texts/3.0x/test.json"
         * main_asset = "packages/flutter_r_demo/assets/texts/3.0x/test.json"
         *
         * === Example-4
         * legal_resource_file = "~/path/to/flutter_r_demo/lib/assets/fonts/Amiri/Amiri-Regular.ttf"
         * main_asset = "packages/flutter_r_demo/fonts/Amiri/Amiri-Regular.ttf"
         *
         * === Example-5
         * legal_resource_file = "~/path/to/flutter_r_demo/assets/images/test.png"
         * main_asset = "assets/images/test.png"
         *
         * === Example-6
         * legal_resource_file = "~/path/to/flutter_r_demo/assets/images/3.0x/test.png"
         * main_asset = "assets/images/test.png"
         *
         * @endverbatim
         *
         * @param flutter_project_dir
         * @param package_name
         * @param legal_resource_file
         * @return main_asset
         */
        std::string generate_main_asset(std::string flutter_project_dir, std::string package_name, std::string legal_resource_file);

        /**
         * @verbatim
         * generate_image_assets(flutter_project_dir, package_name, legal_image_file_array) -> image_asset_array
         *
         * 遍历指定资源目录下扫描找到的legal_image_file数组生成image_asset数组
         *
         * === Examples
         * flutter_project_dir = "~/path/to/flutter_r_demo"
         * package_name = "flutter_r_demo"
         * legal_image_file_array = ["~/path/to/flutter_r_demo/lib/assets/images/test.png", "~/path/to/flutter_r_demo/lib/assets/images/3.0x/test.png"]
         * image_asset_array = ["packages/flutter_r_demo/assets/images/test.png"]
         *
         * @endverbatim
         *
         * @param flutter_project_dir
         * @param package_name
         * @param legal_image_file_array
         * @return image_asset_array
         */
        std::vector<std::string> generate_image_assets(std::string flutter_project_dir, std::string package_name, std::vector<std::string> legal_image_file_array);

        /**
         * @verbatim
         * generate_text_assets(flutter_project_dir, package_name, legal_text_file_array) -> text_asset_array
         *
         * 遍历指定资源目录下扫描找到的legal_text_file数组生成text_asset数组
         *
         * === Examples
         * flutter_project_dir = "~/path/to/flutter_r_demo"
         * package_name = "flutter_r_demo"
         * legal_text_file_array = ["~path/to/flutter_r_demo/lib/assets/jsons/test.json"]
         * text_asset_array = ["packages/flutter_r_demo/assets/jsons/test.json"]
         *
         * @endverbatim
         *
         * @param flutter_project_dir
         * @param package_name
         * @param legal_text_file_array
         * @return
         */
        std::vector<std::string> generate_text_assets(std::string flutter_project_dir, std::string package_name, std::vector<std::string> legal_text_file_array);

        /**
         * @verbatim
         * generate_font_assets(flutter_project_dir, package_name, legal_font_file_array) -> font_asset_array
         *
         * 遍历指定资源目录下扫描找到的legal_font_file数组生成font_asset_config数组
         *
         * === Examples
         * flutter_project_dir = "~/path/to/flutter_r_demo"
         * package_name = "flutter_r_demo"
         * legal_font_file_array = ["~path/to/flutter_r_demo/lib/assets/fonts/Amiri/Amiri-Regular.ttf"]
         * font_asset_array -> ["packages/flutter_r_demo/assets/fonts/Amiri/Amiri-Regular.ttf"]
         * @endverbatim
         *
         * @param flutter_project_dir
         * @param package_name
         * @param legal_font_file_array
         * @return
         */
        std::vector<std::string> generate_font_assets(std::string flutter_project_dir, std::string package_name, std::vector<std::string> legal_font_file_array);

        /**
         * @verbatim
         * merge_flutter_assets(new_asset_array, old_asset_array) -> merged_asset_array
         *
         * 合并新旧2个asset数组：
         * - 获取old_asset_array与new_asset_array的差异集合：diff_asset_array = old_asset_array - (old_asset_array ∩ new_asset_array)
         * - 遍历diff_asset_array，筛选合法的asset得到legal_old_asset_array；合法的asset标准是：非图片资源 + 存在对应的资源文件
         * - 按照字典序对legal_old_asset_array进行排序，并追加到new_asset_array
         * - 返回合并结果merged_asset_array
         *
         * === Examples
         * flutter_project_dir = "~/path/to/flutter_r_demo"
         * package_name = "flutter_r_demo"
         * new_asset_array = ["packages/flutter_r_demo/assets/images/test.png", "packages/flutter_r_demo/assets/jsons/test.json"]
         * old_asset_array = ["packages/flutter_r_demo/assets/htmls/test.html"]
         * merged_asset_array = ["packages/flutter_r_demo/assets/images/test.png", "packages/flutter_r_demo/assets/jsons/test.json", "packages/flutter_r_demo/assets/htmls/test.html"]
         *
         * @endverbatim
         *
         * @param flutter_project_dir
         * @param package_name
         * @param new_asset_array
         * @param old_asset_array
         * @return merged_asset_array
         */
        std::vector<std::string> merge_flutter_assets(std::string flutter_project_dir, std::string package_name, std::vector<std::string> new_asset_array, std::vector<std::string> old_asset_array);
    }
}

#endif //FLR_CORE_ENGINE_EXAMPLE_ASSET_UTIL_H

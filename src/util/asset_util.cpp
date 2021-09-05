//
// Created by York on 2021/7/25.
//

#include <fmt/format.h>

#include <filesystem>
#include <regex>
#include <algorithm>

#include "array_util.h"
#include "asset_util.h"
#include "file_util.h"
#include "array_util.h"

namespace fs = std::__fs::filesystem;

namespace flr {
    namespace asset_util {
        bool is_asset_variant(std::string legal_resource_file) {
            if (flr::file_util::is_non_svg_image_resource_file(legal_resource_file)) {
                fs::path file_path = fs::path(legal_resource_file);
                std::string parent_dir_name = file_path.parent_path().filename().string();

                std::regex ratio_regex("^((0\\.[0-9]+)|([1-9]+[0-9]*(\\.[0-9]+)?))[x]$");
                if (std::regex_match(parent_dir_name, ratio_regex)) {
                    return true;
                }
            }
            return false;
        }

        bool is_image_asset(std::string asset) {
            if (flr::file_util::is_image_resource_file(asset)) {
                return true;
            }
            return false;
        }

        bool is_package_asset(std::string asset) {
            std::string package_prefix = "packages/";
            if (asset.find(package_prefix) == 0) {
                return true;
            }
            return false;
        }

        bool is_specified_package_asset(std::string package_name, std::string asset) {
            std::string specified_package_prefix =  fmt::format("packages/{0}/", package_name);
            if (asset.find(specified_package_prefix) == 0) {
                return true;
            }
            return false;
        }

        std::string get_main_resource_file(std::string flutter_project_dir, std::string package_name, std::string asset) {
            if (is_specified_package_asset(package_name, asset)) {
                std::string specified_package_prefix =  fmt::format("packages/{0}/", package_name);

                // asset: packages/flutter_r_demo/assets/images/test.png
                // to get implied_relative_resource_file: lib/assets/images/test.png
                std::string implied_relative_resource_file = asset;
                implied_relative_resource_file.replace(0, specified_package_prefix.length(), "");
                implied_relative_resource_file = "lib/" + implied_relative_resource_file;

                // main_resource_file:  ~/path/to/flutter_r_demo/lib/assets/images/test.png
                std::string main_resource_file = fmt::format("{0}/{1}", flutter_project_dir, implied_relative_resource_file);
                return main_resource_file;
            } else {
                // asset: assets/images/test.png
                // main_resource_file:  ~/path/to/flutter_r_demo/assets/images/test.png
                std::string main_resource_file = fmt::format("{0}/{1}", flutter_project_dir, asset);
                return main_resource_file;
            }
        }

        bool is_asset_existed(std::string flutter_project_dir, std::string package_name, std::string asset) {
            /**
             * 处理指定flutter工程的asset
             * 1. 获取asset对应的main_resource_file
             * 2. 若main_resource_file是非SVG类图片资源文件，判断asset是否存在的标准是：主资源文件或者至少一个变体资源文件存在
             * 3. 若main_resource_file是SVG类图片资源文件或者其他资源文件，判断asset是否存在的标准是：主资源文件存在
             */

            std::string main_resource_file = get_main_resource_file(flutter_project_dir, package_name, asset);
            if (flr::file_util::is_non_svg_image_resource_file(main_resource_file)) {
                if(fs::exists(main_resource_file)) {
                    return true;
                }

                std::string file_name = fs::path(main_resource_file).filename();
                std::string file_dir = fs::path(main_resource_file).parent_path().string();
                bool did_find_variant_resource_file = false;

                for (const auto & entry : fs::recursive_directory_iterator(file_dir)) {
                    fs::path entry_path = entry.path();
                    if (fs::is_directory(entry_path)) {
                        continue;
                    }

                    std::string entry_filename = entry_path.filename().string();
                    if (file_name != entry_filename) {
                        continue;
                    }

                    if (is_asset_variant(entry_path.string())) {
                        did_find_variant_resource_file = true;
                        break;
                    }
                }

                if (did_find_variant_resource_file) {
                    return true;
                }
            } else {
                if(fs::exists(main_resource_file)) {
                    return true;
                }
            }

            return false;
        }

        std::string generate_main_asset(std::string flutter_project_dir, std::string package_name, std::string legal_resource_file) {
            // legal_resource_file: ~/path/to/flutter_r_demo/lib/assets/images/3.0x/test.png
            // to get main_resource_file: ~/path/to/flutter_r_demo/lib/assets/images/test.png

            std::string main_resource_file = legal_resource_file;

            if (is_asset_variant(legal_resource_file)) {
                fs::path file_path = fs::path(legal_resource_file);
                // test.png
                std::string file_basename = file_path.filename().string();
                // ~/path/to/flutter_r_demo/lib/assets/images/3.0x
                std::string file_dir = file_path.parent_path().string();
                // ~/path/to/flutter_r_demo/lib/assets/images
                std::string main_resource_file_dir = fs::path(file_dir).parent_path().string();
                // ~/path/to/flutter_r_demo/lib/assets/images/test.png
                main_resource_file = main_resource_file_dir + "/" + file_basename;
            }

            // main_resource_file:  ~/path/to/flutter_r_demo/lib/assets/images/test.png
            // to get main_relative_resource_file: lib/assets/images/test.png
            std::string flutter_project_dir_prefix = flutter_project_dir + "/";
            std::string main_relative_resource_file = main_resource_file;
            // check if startsWith flutter_project_dir_prefix
            if (main_relative_resource_file.find(flutter_project_dir_prefix) == 0) {
                main_relative_resource_file.replace(0, flutter_project_dir_prefix.length(), "");
            }

            // 判断 main_relative_resource_file 是不是 implied_resource_file 类型
            // implied_resource_file 的定义是：放置在 "lib/" 目录内 resource_file
            // 具体实现是：main_relative_resource_file 的前缀若是 "lib/" ，则其是 implied_resource_file 类型；
            //
            // implied_relative_resource_file 生成 main_asset 的算法是： main_asset = "packages/#{package_name}/#{asset_name}"
            // non-implied_relative_resource_file 生成 main_asset 的算法是： main_asset = "#{asset_name}"
            //
            std::string lib_prefix = "lib/";
            // check if startsWith flutter_project_dir_prefix
            if (main_relative_resource_file.find(lib_prefix) == 0) {
                // main_relative_resource_file: lib/assets/images/test.png
                // to get asset_name: assets/images/test.png
                std::string asset_name = main_relative_resource_file;
                asset_name.replace(0, lib_prefix.length(), "");

                std::string main_asset = fmt::format("packages/{0}/{1}", package_name, asset_name);
                return main_asset;
            } else {
                // main_relative_resource_file: assets/images/test.png
                // to get asset_name: assets/images/test.png
                std::string asset_name = main_relative_resource_file;

                std::string main_asset = asset_name;
                return main_asset;
            }
        }

        std::vector<std::string> generate_image_assets(std::string flutter_project_dir, std::string package_name, std::vector<std::string> legal_image_file_array) {
            std::vector<std::string> image_asset_array;

            for (std::string legal_image_file: legal_image_file_array) {
               std::string image_asset = generate_main_asset(flutter_project_dir, package_name, legal_image_file);
               image_asset_array.push_back(image_asset);
            }

            flr::array_util::unique_string_array(image_asset_array);
            return image_asset_array;
        }

        std::vector<std::string> generate_text_assets(std::string flutter_project_dir, std::string package_name, std::vector<std::string> legal_text_file_array) {
            std::vector<std::string> text_asset_array;

            for (std::string legal_text_file: legal_text_file_array) {
                std::string text_asset = generate_main_asset(flutter_project_dir, package_name, legal_text_file);
                text_asset_array.push_back(text_asset);
            }

            flr::array_util::unique_string_array(text_asset_array);
            return text_asset_array;
        }

        std::vector<std::string> generate_font_assets(std::string flutter_project_dir, std::string package_name, std::vector<std::string> legal_font_file_array) {
            std::vector<std::string> font_asset_array;

            for (std::string legal_font_file: legal_font_file_array) {
                std::string font_asset = generate_main_asset(flutter_project_dir, package_name, legal_font_file);
                font_asset_array.push_back(font_asset);
            }
            flr::array_util::unique_string_array(font_asset_array);
            return font_asset_array;
        }

        std::vector<std::string> merge_flutter_assets(std::string flutter_project_dir, std::string package_name, std::vector<std::string> new_asset_array, std::vector<std::string> old_asset_array) {
            std::vector<std::string> legal_old_asset_array;

            // diff_asset_array = old_asset_array - (old_asset_array ∩ new_asset_array)
            // diff_asset_array 是 old_asset_array 移除掉 old_asset_array 和 new_asset_array 的交集元素后的集合
            std::vector<std::string> diff_asset_array = old_asset_array;
            auto first_dup_it = std::remove_if(diff_asset_array.begin(), diff_asset_array.end(), [new_asset_array](std::string old_asset) {
                bool did_find = std::find(new_asset_array.begin(), new_asset_array.end(), old_asset) != new_asset_array.end();
                return did_find;
            });
            diff_asset_array.erase(first_dup_it, diff_asset_array.end());

            for (std::string asset: diff_asset_array) {
                // 若是第三方package的资源，则合并到new_asset_array
                // 引用第三方package的资源的推荐做法是：通过引用第三方package的R类来访问
                if (is_package_asset(asset)) {
                    if (is_specified_package_asset(package_name, asset) == false) {
                        legal_old_asset_array.push_back(asset);
                        continue;
                    }
                }

                // 处理指定flutter工程的asset
                // 1. 判断asset是否存在
                // 2. 若asset存在，则合并到new_asset_array
                if (is_asset_existed(flutter_project_dir, package_name, asset)) {
                    legal_old_asset_array.push_back(asset);
                }
            }

            std::sort(legal_old_asset_array.begin(), legal_old_asset_array.end());

            // merged_asset_array = new_asset_array + legal_old_asset_array
            std::vector<std::string> merged_asset_array = new_asset_array;
            merged_asset_array.insert(merged_asset_array.end(), legal_old_asset_array.begin(), legal_old_asset_array.end());
            return merged_asset_array;
        }

    }
}
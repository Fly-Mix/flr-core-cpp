//
// Created by York on 2021/7/17.
//

#include <glog/logging.h>
#include <fmt/format.h>

#include <fstream>
#include <filesystem>
#include <regex>

#include "file_util.h"
#include "../constant.h"

namespace fs = std::__fs::filesystem;

namespace flr {
    namespace file_util {
        std::vector<std::string> get_flutter_sub_project_root_dirs(std::string flutter_main_project_root_dir) {
            std::vector<std::string> sub_project_root_dirs;
            for (const auto & entry : fs::recursive_directory_iterator(flutter_main_project_root_dir)) {
                if (fs::is_directory(entry.path())) {
                    continue;
                }
                if (entry.path().has_filename() && entry.path().filename() == flr::constant::PUBSPEC_FILE_NAME) {
                    std::string parent_path = entry.path().parent_path().string();
                    if (parent_path != flutter_main_project_root_dir) {
                        sub_project_root_dirs.push_back(parent_path);
                    }
                }
            }
            return sub_project_root_dirs;
        }

        std::string get_pubspec_file_path(std::string flutter_project_dir) {
            std::string pubspec_file_path = fmt::format("{}/{}", flutter_project_dir, flr::constant::PUBSPEC_FILE_NAME);
            return pubspec_file_path;
        }

        std::string load_pubspec_file(std::string pubspec_file_path) {
            std::ifstream pubspec_file_stream(pubspec_file_path);
            if (pubspec_file_stream.bad()) {
                return "";
            }

            std::stringstream pubspec_string_stream;
            pubspec_string_stream << pubspec_file_stream.rdbuf();
            std::string pubspec_file_content = pubspec_string_stream.str();
            return pubspec_file_content;
        }

        bool is_package_project_type(std::string flutter_project_dir) {
            std::string metadata_file_path = fmt::format("{}/.metadata", flutter_project_dir);
            std::ifstream metadata_file(metadata_file_path);
            if (metadata_file.good()) {
                std::stringstream metadata_file_stream;
                metadata_file_stream << metadata_file.rdbuf();
                std::string whole_file_content = metadata_file_stream.str();
                c4::substr metadata_file_origin_content_str = c4::to_substr(whole_file_content);
                ryml::Tree metadata_config = ryml::parse(metadata_file_origin_content_str);
                std::string project_type = "unknown";
                if (metadata_config.rootref().has_child("project_type")) {
                     metadata_config["project_type"] >> project_type;
                }

                if (project_type == "package" || project_type == "plugin") {
                    return true;
                }
            } else {
                std::string pubspec_file_path = get_pubspec_file_path(flutter_project_dir);
                std::string pubspec_file_content = load_pubspec_file(pubspec_file_path);
                c4::substr pubspec_file_content_c4str = c4::to_substr(pubspec_file_content);
                ryml::Tree pubspec_tree = ryml::parse(pubspec_file_content_c4str);
                if (pubspec_tree.rootref().has_child("author")) {
                    return true;
                }
            }

            return false;
        }

        bool is_non_svg_image_resource_file(std::string file) {
            fs::path file_path = fs::path(file);
            std::set<std::string> file_types = flr::constant::NON_SVG_IMAGE_FILE_TYPES;
            if (file_path.has_extension() && file_types.find(file_path.extension()) != file_types.end()) {
                return true;
            }
            return false;
        }

        bool is_svg_image_resource_file(std::string file) {
            fs::path file_path = fs::path(file);
            std::set<std::string> file_types = flr::constant::SVG_IMAGE_FILE_TYPES;
            if (file_path.has_extension() && file_types.find(file_path.extension()) != file_types.end()) {
                return true;
            }
            return false;
        }

        bool is_image_resource_file(std::string file) {
            fs::path file_path = fs::path(file);
            std::set<std::string> file_types = flr::constant::IMAGE_FILE_TYPES;
            if (file_path.has_extension() && file_types.find(file_path.extension()) != file_types.end()) {
                return true;
            }
            return false;
        }

        bool is_text_resource_file(std::string file) {
            fs::path file_path = fs::path(file);
            std::set<std::string> file_types = flr::constant::TEXT_FILE_TYPES;
            if (file_path.has_extension() && file_types.find(file_path.extension()) != file_types.end()) {
                return true;
            }
            return false;
        }

        bool is_font_resource_file(std::string file) {
            fs::path file_path = fs::path(file);
            std::set<std::string> file_types = flr::constant::FONT_FILE_TYPES;
            if (file_path.has_extension() && file_types.find(file_path.extension()) != file_types.end()) {
                return true;
            }
            return false;
        }

        bool is_legal_resource_file(std::string file) {
            fs::path file_path = fs::path(file);
            std::string file_basename_no_extension = file_path.stem();
            std::string pattern = "^[a-zA-Z0-9_\\+\\-\\.·!@&$￥]+$";
            std::regex reg_valid_name(pattern);
            bool is_valid = std::regex_match(file_basename_no_extension, reg_valid_name);
            return is_valid;
        }

        std::tuple<std::vector<std::string>, std::vector<std::string>> find_image_files(std::string resource_dir) {
            std::vector<std::string> legal_image_file_array;
            std::vector<std::string> illegal_image_file_array;
            for (const auto & entry : fs::recursive_directory_iterator(resource_dir)) {
                fs::path entry_path = entry.path();
                if (fs::is_directory(entry_path)) {
                    continue;
                }

                std::string entry_path_str = entry_path.string();
                if (!is_image_resource_file(entry_path_str)) {
                    continue;
                }

                if (is_legal_resource_file(entry_path_str)) {
                    legal_image_file_array.push_back(entry_path_str);
                } else {
                    illegal_image_file_array.push_back(entry_path_str);
                }
            }

            std::tuple<std::vector<std::string>, std::vector<std::string>> image_file_result_tuple(legal_image_file_array, illegal_image_file_array);
            return image_file_result_tuple;
        }

        std::tuple<std::vector<std::string>, std::vector<std::string>> find_text_files(std::string resource_dir) {
            std::vector<std::string> legal_text_file_array;
            std::vector<std::string> illegal_text_file_array;
            for (const auto & entry : fs::recursive_directory_iterator(resource_dir)) {
                fs::path entry_path = entry.path();
                if (fs::is_directory(entry_path)) {
                    continue;
                }

                std::string entry_path_str = entry_path.string();
                if (!is_text_resource_file(entry_path_str)) {
                    continue;
                }

                if (is_legal_resource_file(entry_path_str)) {
                    legal_text_file_array.push_back(entry_path_str);
                } else {
                    illegal_text_file_array.push_back(entry_path_str);
                }
            }

            std::tuple<std::vector<std::string>, std::vector<std::string>> text_file_result_tuple(legal_text_file_array, illegal_text_file_array);
            return text_file_result_tuple;
        }

        std::vector<std::string> find_top_child_dirs(std::string resource_dir) {
            std::vector<std::string> top_child_dir_array;
            for (const auto & entry : fs::directory_iterator(resource_dir)) {
                fs::path entry_path = entry.path();
                if (fs::is_directory(entry_path)) {
                    top_child_dir_array.push_back(entry_path.string());
                }
            }
            return top_child_dir_array;
        }

        std::tuple<std::vector<std::string>, std::vector<std::string>> find_font_files_in_font_family_dir(std::string font_family_dir) {
            std::vector<std::string> legal_font_file_array;
            std::vector<std::string> illegal_font_file_array;
            for (const auto & entry : fs::recursive_directory_iterator(font_family_dir)) {
                fs::path entry_path = entry.path();
                if (fs::is_directory(entry_path)) {
                    continue;
                }

                std::string entry_path_str = entry_path.string();
                if (!is_font_resource_file(entry_path_str)) {
                    continue;
                }

                if (is_legal_resource_file(entry_path_str)) {
                    legal_font_file_array.push_back(entry_path_str);
                } else {
                    illegal_font_file_array.push_back(entry_path_str);
                }
            }

            std::tuple<std::vector<std::string>, std::vector<std::string>> font_file_result_tuple(legal_font_file_array, illegal_font_file_array);
            return font_file_result_tuple;
        }
    };
}
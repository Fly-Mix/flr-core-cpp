//
// Created by York on 2021/7/24.
//

#include <fmt/format.h>

#include <string>
#include <regex>
#include <filesystem>
#include <ctype.h>

#include "code_util.h"
#include "../constant.h"

namespace fs = std::__fs::filesystem;

namespace flr {
    namespace code_util {
        std::string generate_R_class(std::string package_name) {
            std::string code = R"CODE(
// IT IS GENERATED BY FLR - DO NOT MODIFY BY HAND
// YOU CAN GET MORE DETAILS ABOUT FLR FROM:
// - https://github.com/Fly-Mix/flr-cli
// - https://github.com/Fly-Mix/flr-vscode-extension
// - https://github.com/Fly-Mix/flr-as-plugin
//
// ignore: unused_import
import 'package:flutter/widgets.dart';
// ignore: unused_import
import 'package:flutter/services.dart' show rootBundle;
// ignore: unused_import
import 'package:path/path.dart' as path;
// ignore: unused_import
import 'package:flutter_svg/flutter_svg.dart';
// ignore: unused_import
import 'package:r_dart_library/asset_svg.dart';
/// This `R` class is generated and contains references to static asset resources.
class R {
  /// package name: #{package_name}
  static const package = "#{package_name}";
  /// This `R.image` struct is generated, and contains static references to static non-svg type image asset resources.
  static const image = _R_Image();
  /// This `R.svg` struct is generated, and contains static references to static svg type image asset resources.
  static const svg = _R_Svg();
  /// This `R.text` struct is generated, and contains static references to static text asset resources.
  static const text = _R_Text();
  /// This `R.fontFamily` struct is generated, and contains static references to static font asset resources.
  static const fontFamily = _R_FontFamily();
}
)CODE";
            // 移除 "\n" 首字符
            code.erase(0, 1);
            return code;
        }

        std::string generate_AssetResource_class(std::string package_name, bool should_support_nullsafety) {
            std::string code = R"CODE(
/// Asset resource’s metadata class.
/// For example, here is the metadata of `packages/flutter_demo/assets/images/example.png` asset:
/// - packageName：flutter_demo
/// - assetName：assets/images/example.png
/// - fileDirname：assets/images
/// - fileBasename：example.png
/// - fileBasenameNoExtension：example
/// - fileExtname：.png
)CODE";
            // 移除 "\n" 首字符
            code.erase(0, 1);

            if (should_support_nullsafety) {
                code += R"CODE(class AssetResource {
  /// Creates an object to hold the asset resource’s metadata.
  const AssetResource(this.assetName, {this.packageName});
  /// The name of the main asset from the set of asset resources to choose from.
  final String assetName;
  /// The name of the package from which the asset resource is included.
  final String? packageName;
  /// The name used to generate the key to obtain the asset resource. For local assets
  /// this is [assetName], and for assets from packages the [assetName] is
  /// prefixed 'packages/<package_name>/'.
  String get keyName => packageName == null ? assetName : "packages/$packageName/$assetName";
  /// The file basename of the asset resource.
  String get fileBasename {
    final basename = path.basename(assetName);
    return basename;
  }
  /// The no extension file basename of the asset resource.
  String get fileBasenameNoExtension {
    final basenameWithoutExtension = path.basenameWithoutExtension(assetName);
    return basenameWithoutExtension;
  }
  /// The file extension name of the asset resource.
  String get fileExtname {
    final extension = path.extension(assetName);
    return extension;
  }
  /// The directory path name of the asset resource.
  String get fileDirname {
    var dirname = assetName;
    if (packageName != null) {
      final packageStr = "packages/$packageName/";
      dirname = dirname.replaceAll(packageStr, "");
    }
    final filenameStr = "$fileBasename/";
    dirname = dirname.replaceAll(filenameStr, "");
    return dirname;
  }
}
)CODE";
            } else {
                code += R"CODE(class AssetResource {
  /// Creates an object to hold the asset resource’s metadata.
  const AssetResource(this.assetName, {this.packageName}) : assert(assetName != null);
  /// The name of the main asset from the set of asset resources to choose from.
  final String assetName;
  /// The name of the package from which the asset resource is included.
  final String packageName;
  /// The name used to generate the key to obtain the asset resource. For local assets
  /// this is [assetName], and for assets from packages the [assetName] is
  /// prefixed 'packages/<package_name>/'.
  String get keyName => packageName == null ? assetName : "packages/$packageName/$assetName";
  /// The file basename of the asset resource.
  String get fileBasename {
    final basename = path.basename(assetName);
    return basename;
  }
  /// The no extension file basename of the asset resource.
  String get fileBasenameNoExtension {
    final basenameWithoutExtension = path.basenameWithoutExtension(assetName);
    return basenameWithoutExtension;
  }
  /// The file extension name of the asset resource.
  String get fileExtname {
    final extension = path.extension(assetName);
    return extension;
  }
  /// The directory path name of the asset resource.
  String get fileDirname {
    var dirname = assetName;
    if (packageName != null) {
      final packageStr = "packages/$packageName/";
      dirname = dirname.replaceAll(packageStr, "");
    }
    final filenameStr = "$fileBasename/";
    dirname = dirname.replaceAll(filenameStr, "");
    return dirname;
  }
}
)CODE";
            }
            return code;
        }

        std::string generate_asset_id(std::string asset, std::vector<std::string> used_asset_id_array, std::string prior_asset_type) {
            fs::path file_path = fs::path(asset);

            std::string file_extname = file_path.extension().string();
            std::transform(file_extname.begin(),
                           file_extname.end(),
                           file_extname.begin(),
                           [](unsigned char c){ return std::tolower(c);}
            );

            std::string file_basename = file_path.filename().string();
            std::string file_basename_no_extension = file_path.stem();

            std::string asset_id = file_basename_no_extension;
            if (prior_asset_type == ".*" || file_extname == prior_asset_type) {
                std::string ext_info = file_extname;
                ext_info.replace(0, 1, "_");
                asset_id = asset_id + ext_info;
            }

            // 过滤非法字符
            std::regex good_char_regex("[^a-zA-Z0-9_$]");
            asset_id = regex_replace(asset_id, good_char_regex, "_");

            // 检测首字符是不是字母；
            // 若是字母，则检测其是不是大写字母，若是，则转换为小写字母；
            // 若不是字母，则添加一个前缀字母"a"
            char first_char = asset_id.at(0);
            if (std::isalpha(first_char)) {
                if (std::isupper(first_char)) {
                    first_char = std::tolower(first_char);
                    asset_id[0] = first_char;
                }
            } else {
                std::string first_char_str = "a";
                asset_id = first_char_str + asset_id;
            }

            // 处理 asset_id 重名的情况
            if (used_asset_id_array.empty() == false
            && std::find(used_asset_id_array.begin(), used_asset_id_array.end(), asset_id) != used_asset_id_array.end()) {
                // 当前asset_id重名次数，初始值为1
                int16_t repeat_count = 1;

                // 查找当前asset_id衍生出来的asset_id_brother（id兄弟）
                // asset_id_brother = #{asset_id}$#{repeat_count}
                // 其中，repeat_count >= 1
                //
                // Example：
                // asset_id = test
                // asset_id_brother = test$1
                //
                std::string id_brother_regex_str = fmt::format("^{}\\$[1-9][0-9]*$", asset_id);
                std::regex id_brother_regex("[^a-zA-Z0-9_$]");
                for (std::string &used_asset_id: used_asset_id_array) {
                    if (std::regex_match(used_asset_id, id_brother_regex)) {
                        repeat_count += 1;
                    }
                }

                asset_id = fmt::format("{0}${1}", asset_id, repeat_count);
            }

            return asset_id;
        }

        std::string generate_asset_comment(std::string asset, std::string package_name) {
            std::string packages_prefix = fmt::format("packages/{0}/", package_name);

            // check if asset startsWith packages_prefix
            if (asset.find(packages_prefix) == 0) {
                // asset: packages/flutter_r_demo/assets/images/test.png
                // to get asset_name: assets/images/test.png
                std::string asset_name = asset;
                asset_name.replace(0, packages_prefix.length(), "");

                std::string asset_comment = "asset: lib/" + asset_name;
                return asset_comment;
            } else {
                // asset: assets/images/test.png
                // to get asset_name: assets/images/test.png
                std::string asset_name = asset;

                std::string asset_comment = "asset: " + asset_name;
                return asset_comment;
            }
        }

        std::string generate_AssetResource_property(std::string asset, std::map<std::string, std::string> asset_id_dict, std::string package_name, bool is_package_project_type, std::string prior_asset_type) {
            std::string asset_id = asset_id_dict[asset];
            std::string asset_comment = generate_asset_comment(asset, package_name);

            std::string asset_name = "";
            bool need_package = false;

            std::string packages_prefix = fmt::format("packages/{}/", package_name);
            // check if asset startsWith packages_prefix
            if (asset.find(packages_prefix) == 0) {
                // asset: packages/flutter_r_demo/assets/images/test.png
                // to get asset_name: assets/images/test.png
                asset_name = asset;
                asset_name.replace(0, packages_prefix.length(), "");

                need_package = true;
            } else {
                // asset: assets/images/test.png
                // to get asset_name: assets/images/test.png
                asset_name = asset;

                if (is_package_project_type) {
                    need_package = true;
                } else {
                    need_package = false;
                }
            }

            // 对字符串中的 '$' 进行转义处理：'$' -> '\$'
            // asset_name: assets/images/test$.png
            // to get escaped_asset_name: assets/images/test\$.png
            std::string escaped_asset_name = regex_replace(asset_name, std::regex("[$]"), "\\$");

            if (need_package) {
                std::string code = fmt::format(R"CODE(
  /// {0}
  // ignore: non_constant_identifier_names
  final {1} = const AssetResource("{2}", packageName: R.package);
)CODE",
asset_comment,
asset_id,
escaped_asset_name);

                // 移除 "\n" 首字符
                code.erase(0, 1);
                return code;
            } else {
                std::string code = fmt::format(R"CODE(
  /// {0}
  // ignore: non_constant_identifier_names
  final {1} = const AssetResource("{2}", packageName: null);
)CODE",
asset_comment,
asset_id,
escaped_asset_name);
                // 移除 "\n" 首字符
                code.erase(0, 1);
                return code;
            }
        }

        std::string generate__R_Image_AssetResource_class(std::vector<std::string> non_svg_image_asset_array, std::map<std::string, std::string> non_svg_image_asset_id_dict, std::string package_name, bool is_package_project_type) {
            std::string all_g_AssetResource_property_code = "";

            for (std::string asset: non_svg_image_asset_array) {
                all_g_AssetResource_property_code += "\n";
                std::string g_AssetResource_property_code = generate_AssetResource_property(asset, non_svg_image_asset_id_dict, package_name, is_package_project_type, flr::constant::PRIOR_NON_SVG_IMAGE_FILE_TYPE);
                all_g_AssetResource_property_code += g_AssetResource_property_code;
            }

            // 转义序列 {{ 与 }} ，它们在输出中被分别替换成 { 与 }
            std::string code = fmt::format(R"CODE(
// ignore: camel_case_types
class _R_Image_AssetResource {{
  const _R_Image_AssetResource();
{0}
}}
)CODE",
all_g_AssetResource_property_code);

            // 移除 "\n" 首字符
            code.erase(0, 1);
            return code;
        }

        std::string generate__R_Svg_AssetResource_class(std::vector<std::string> svg_image_asset_array, std::map<std::string, std::string> svg_image_asset_id_dict, std::string package_name, bool is_package_project_type) {
            std::string all_g_AssetResource_property_code = "";

            for (std::string asset: svg_image_asset_array) {
                all_g_AssetResource_property_code += "\n";
                std::string g_AssetResource_property_code = generate_AssetResource_property(asset, svg_image_asset_id_dict, package_name, is_package_project_type, flr::constant::PRIOR_SVG_IMAGE_FILE_TYPE);
                all_g_AssetResource_property_code += g_AssetResource_property_code;
            }

            // 转义序列 {{ 与 }} ，它们在输出中被分别替换成 { 与 }
            std::string code = fmt::format(R"CODE(
// ignore: camel_case_types
class _R_Svg_AssetResource {{
  const _R_Svg_AssetResource();
{0}
}}
)CODE",
all_g_AssetResource_property_code);

            // 移除 "\n" 首字符
            code.erase(0, 1);
            return code;
        }

        std::string generate__R_Text_AssetResource_class(std::vector<std::string> text_asset_array, std::map<std::string, std::string> text_asset_id_dict, std::string package_name, bool is_package_project_type) {
            std::string all_g_AssetResource_property_code = "";

            for (std::string asset: text_asset_array) {
                all_g_AssetResource_property_code += "\n";
                std::string g_AssetResource_property_code = generate_AssetResource_property(asset, text_asset_id_dict, package_name, is_package_project_type, flr::constant::PRIOR_TEXT_FILE_TYPE);
                all_g_AssetResource_property_code += g_AssetResource_property_code;
            }

            // 转义序列 {{ 与 }} ，它们在输出中被分别替换成 { 与 }
            std::string code = fmt::format(R"CODE(
// ignore: camel_case_types
class _R_Text_AssetResource {{
  const _R_Text_AssetResource();
{0}
}}
)CODE",
all_g_AssetResource_property_code);

            // 移除 "\n" 首字符
            code.erase(0, 1);
            return code;
        }

        std::string generate__R_Image_class(std::vector<std::string> non_svg_image_asset_array, std::map<std::string, std::string> non_svg_image_asset_id_dict, std::string package_name) {
            std::string all_g_Asset_method_code = "";

            for (std::string asset: non_svg_image_asset_array) {
                all_g_Asset_method_code += "\n";

                std::string asset_id = non_svg_image_asset_id_dict[asset];
                std::string asset_comment = generate_asset_comment(asset, package_name);

                // 转义序列 {{ 与 }} ，它们在输出中被分别替换成 { 与 }
                std::string g_Asset_method_code = fmt::format(R"CODE(
  /// {0}
  // ignore: non_constant_identifier_names
  AssetImage {1}() {{
    return AssetImage(asset.{1}.keyName);
  }}
)CODE",
asset_comment,
asset_id);

                all_g_Asset_method_code += g_Asset_method_code;
            }

            // 转义序列 {{ 与 }} ，它们在输出中被分别替换成 { 与 }
            std::string code = fmt::format(R"CODE(
/// This `_R_Image` class is generated and contains references to static non-svg type image asset resources.
// ignore: camel_case_types
class _R_Image {{
  const _R_Image();
  final asset = const _R_Image_AssetResource();
{0}
}}
)CODE",
all_g_Asset_method_code);

            // 移除 "\n" 首字符
            code.erase(0, 1);
            return code;
        }

        std::string generate__R_Svg_class(std::vector<std::string> svg_image_asset_array, std::map<std::string, std::string> svg_image_asset_id_dict, std::string package_name, bool should_support_nullsafety) {
            std::string all_g_Asset_method_code = "";

            for (std::string asset: svg_image_asset_array) {
                all_g_Asset_method_code += "\n";

                std::string asset_id = svg_image_asset_id_dict[asset];
                std::string asset_comment = generate_asset_comment(asset, package_name);

                if (should_support_nullsafety) {
                    // 转义序列 {{ 与 }} ，它们在输出中被分别替换成 { 与 }
                    std::string g_Asset_method_code = fmt::format(R"CODE(
  /// {0}
  // ignore: non_constant_identifier_names
  AssetSvg {1}({required double width, required double height}) {{
    final imageProvider = AssetSvg(asset.{1}.keyName, width: width, height: height);
    return imageProvider;
  }}
)CODE",
asset_comment,
asset_id);

                    all_g_Asset_method_code += g_Asset_method_code;
                } else {
                    // 转义序列 {{ 与 }} ，它们在输出中被分别替换成 { 与 }
                    std::string g_Asset_method_code = fmt::format(R"CODE(
  /// {0}
  // ignore: non_constant_identifier_names
  AssetSvg {1}({@required double width, @required double height}) {{
    final imageProvider = AssetSvg(asset.{1}.keyName, width: width, height: height);
    return imageProvider;
  }}
)CODE",
asset_comment,
asset_id);

                    all_g_Asset_method_code += g_Asset_method_code;
                }
            }

            // 转义序列 {{ 与 }} ，它们在输出中被分别替换成 { 与 }
            std::string code = fmt::format(R"CODE(
/// This `_R_Svg` class is generated and contains references to static svg type image asset resources.
// ignore: camel_case_types
class _R_Svg {{
  const _R_Svg();
  final asset = const _R_Svg_AssetResource();
{0}
}}
)CODE",
all_g_Asset_method_code);

            // 移除 "\n" 首字符
            code.erase(0, 1);
            return code;
        }

        std::string generate__R_Text_class(std::vector<std::string> text_asset_array,std::map<std::string, std::string> text_asset_id_dict, std::string package_name) {
            std::string all_g_Asset_method_code = "";

            for (std::string asset: text_asset_array) {
                all_g_Asset_method_code += "\n";

                std::string asset_id = text_asset_id_dict[asset];
                std::string asset_comment = generate_asset_comment(asset, package_name);

                // 转义序列 {{ 与 }} ，它们在输出中被分别替换成 { 与 }
                std::string g_Asset_method_code = fmt::format(R"CODE(
  /// {0}
  // ignore: non_constant_identifier_names
  Future<String> {1}() {{
    final str = rootBundle.loadString(asset.{1}.keyName);
    return str;
  }}
)CODE",
asset_comment,
asset_id);

                all_g_Asset_method_code += g_Asset_method_code;
            }

            // 转义序列 {{ 与 }} ，它们在输出中被分别替换成 { 与 }
            std::string code = fmt::format(R"CODE(
/// This `_R_Text` class is generated and contains references to static text asset resources.
// ignore: camel_case_types
class _R_Text {{
  const _R_Text();
  final asset = const _R_Text_AssetResource();
{0}
}}
)CODE",
all_g_Asset_method_code);

            // 移除 "\n" 首字符
            code.erase(0, 1);
            return code;
        }

        std::string generate_font_family_id(std::string font_family_name) {
            std::string font_family_id = font_family_name;

            // 过滤非法字符
            std::regex good_char_regex("[^a-zA-Z0-9_$]");
            font_family_id = regex_replace(font_family_id, good_char_regex, "_");

            // 检测首字符是不是字母；
            // 若是字母，则检测其是不是大写字母，若是，则转换为小写字母；
            // 若不是字母，则添加一个前缀字母"a"
            char first_char = font_family_id.at(0);
            if (std::isalpha(first_char)) {
                if (std::isupper(first_char)) {
                    first_char = std::tolower(first_char);
                    font_family_id[0] = first_char;
                }
            } else {
                std::string first_char_str = "a";
                font_family_id = first_char_str + font_family_id;
            }

            return font_family_id;
        }

        std::string generate__R_FontFamily_class(std::vector<FontFamilyConfig> font_family_config_array, std::string package_name) {
            std::string all_g_Asset_method_code = "";

            for (FontFamilyConfig font_family_config: font_family_config_array) {
                all_g_Asset_method_code += "\n";

                std::string font_family_name = std::get<0>(font_family_config);
                std::string font_family_id = generate_font_family_id(font_family_name);
                std::string  font_family_comment = "font family: " + font_family_name;

                std::string g_Asset_method_code = fmt::format(R"CODE(
  /// {0}
  // ignore: non_constant_identifier_names
  final {1} = "{2}";
)CODE",
font_family_comment,
font_family_id,
font_family_name);

                all_g_Asset_method_code += g_Asset_method_code;
            }

            // 转义序列 {{ 与 }} ，它们在输出中被分别替换成 { 与 }
            std::string code = fmt::format(R"CODE(
/// This `_R_FontFamily` class is generated and contains references to static font asset resources.
// ignore: camel_case_types
class _R_FontFamily {{
  const _R_FontFamily();
{0}
}}
)CODE",
all_g_Asset_method_code);

            // 移除 "\n" 首字符
            code.erase(0, 1);
            return code;
        }
    }
}
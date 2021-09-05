//
// Created by York on 2021/7/24.
//

#ifndef FLR_CORE_ENGINE_EXAMPLE_CODE_UTIL_H
#define FLR_CORE_ENGINE_EXAMPLE_CODE_UTIL_H

#include <ryml_std.hpp>
#include <ryml.hpp>

#include <iostream>
#include <vector>
#include <map>

namespace flr {
    namespace code_util {
        /**
         * @verbatim
         * generate_R_class(package_name) -> R_class_code
         *
         * 根据模板生成 R class 的代码
         * @endverbatim
         *
         * @param packageName 包名
         * @return R_class_code
         */
        std::string generate_R_class(std::string package_name);

         /**
          * @verbatim
          * generate_AssetResource_class(std::string package_name, bool should_support_nullsafety) -> AssetResource_class_code
          *
          * 根据模板生成 AssetResource class 的代码
          * @endverbatim
          *
          * @param package_name 包名
          * @param should_support_nullsafety 生成的代码是否需要支持空安全
          * @return AssetResource_class_code
          */
        std::string generate_AssetResource_class(std::string package_name, bool should_support_nullsafety);

        /**
         * @verbatim
         * generate_asset_id (asset, used_asset_id_array, prior_asset_type) -> asset_id
         *
         * 为当前 asset 生成 asset_id（资产ID）；asset_id 一般为 asset 的 file_basename_no_extension；
         *
         * 为了保证 asset_id 的健壮性，flr 会对 file_basename_no_extension 做以下加工处理：
         * - 处理非法字符：把除了字母（a-z, A-Z）、数字（0-9）、'_' 字符、'$' 字符之外的字符转换为 '_' 字符
         * - 首字母转化为小写
         * - 处理首字符异常情况：检测首字符是不是数字、'_'、'$'，若是则添加前缀字符"a"
         * - 处理 asset_id 重名的情况
         *
         * prior_asset_type 用于指定优先的资产类型，其决定了当前 asset 的 asset_id 是否带有资产类型信息：
         * 如 prior_asset_type 为".png"，
         * 这时候若 asset 为 "packages/flutter_demo/assets/images/test.png"，这时其生成 asset_id 为 "test"，不带有类型信息；
         * 这时候若 asset 为 "packages/flutter_demo/assets/images/test.jpg"，这时其生成 asset_id 为 "test_jpg"，带有类型信息；
         *
         * === Examples
         *
   	 	 * ==== Example-1
	     * asset = "packages/flutter_r_demo/assets/images/test.png"
	     * asset = "packages/flutter_r_demo/assets/images/test.jpg"
	     * used_asset_id_array = []
	     * prior_asset_type = ".png"
	     * asset_id = "test"
	     *
	     * ===== Example-2
	     * asset = "packages/flutter_r_demo/assets/images/test.jpg"
	     * used_asset_id_array = [test]
	     * prior_asset_type = ".png"
	     * asset_id = "test_jpg"
	     *
	     * ===== Example-3
	     * asset = "packages/flutter_r_demo/assets/home-images/test.jpg"
	     * used_asset_id_array = [test, test_jpg]
	     * prior_asset_type = ".png"
	     * asset_id = "test_jpg_1"
	     *
	     * ===== Example-4
	     * asset = "packages/flutter_r_demo/assets/texts/test.json"
	     * used_asset_id_array = []
	     * prior_asset_type = ".*"
	     * asset_id = "test_json"
         *
         * @endverbatim
         *
         * @param asset 资产
         * @param used_asset_id_array 已使用的 asset_id 数组
         * @param prior_asset_type 优先的资源类型；默认值为 ".*"，意味当前不存在任何优先的资源类型
         * @return asset_id
         */
        std::string generate_asset_id(std::string asset, std::vector<std::string> used_asset_id_array, std::string prior_asset_type = ".*");

        /**
         * @verbatim
         * generate_asset_comment (asset, package_name) -> asset_comment
         *
         * 为当前 asset 生成注释
         *
         * === Examples
         * package_name = "flutter_r_demo"
         *
         * === Example-1
         * asset = "packages/flutter_r_demo/assets/images/test.png"
         * asset_comment = "asset: lib/assets/images/test.png"
         *
         * === Example-2
         * asset = "assets/images/test.png"
         * asset_comment = "asset: assets/images/test.png"
         *
         * @endverbatim
         *
         * @param asset 资产
         * @param package_name 包名
         * @return asset_comment
         */
        std::string generate_asset_comment(std::string asset, std::string package_name);

        /**
         * @verbatim
         * generate_AssetResource_property(asset, asset_id_dict, package_name, is_package_project_type, prior_asset_type) -> AssetResource_property_code
         *
         * 为当前 asset 生成 AssetResource property 的代码
         *
         * @endverbatim
         * @param asset 资产
         * @param asset_id_dict 资产ID字典
         * @param package_name 包名
         * @param is_package_project_type 当前资产是否在包内
         * @param prior_asset_type 优先的资产类型
         * @return AssetResource_property_code
         */
        std::string generate_AssetResource_property(std::string asset, std::map<std::string, std::string> asset_id_dict, std::string package_name, bool is_package_project_type, std::string prior_asset_type = ".*");

        /**
         * @verbatim
         * generate__R_Image_AssetResource_class(non_svg_image_asset_array, non_svg_image_asset_id_dict, package_name, is_package_project_type) -> _R_Image_AssetResource_class_code
         *
         * 根据模板，为 non_svg_image_asset_array（非svg类的图片资产数组）生成 _R_Image_AssetResource class 的代码
         *
         * @endverbatim
         *
         * @param non_svg_image_asset_array
         * @param non_svg_image_asset_id_dict
         * @param package_name
         * @param is_package_project_type
         * @return _R_Image_AssetResource_class_code
         */
        std::string generate__R_Image_AssetResource_class(std::vector<std::string> non_svg_image_asset_array, std::map<std::string, std::string> non_svg_image_asset_id_dict, std::string package_name, bool is_package_project_type);

        /**
         * @verbatim
         * generate__R_Svg_AssetResource_class(svg_image_asset_array, svg_image_asset_id_dict, package_name, is_package_project_type) -> _R_Svg_AssetResource_class_code
         *
         * 根据模板，为 svg_image_asset_array（svg类的图片资产数组）生成 _R_Svg_AssetResource class 的代码
         *
         * @endverbatim
         *
         * @param svg_image_asset_array
         * @param svg_image_asset_id_dict
         * @param package_name
         * @param is_package_project_type
         * @return _R_Svg_AssetResource_class_code
         */
        std::string generate__R_Svg_AssetResource_class(std::vector<std::string> svg_image_asset_array, std::map<std::string, std::string> svg_image_asset_id_dict, std::string package_name, bool is_package_project_type);

        /**
         * @verbatim
         * generate__R_Text_AssetResource_class(text_asset_array, text_asset_id_dict, package_name, is_package_project_typ) -> _R_Text_AssetResource_class_code
         *
         * 根据模板，为 text_asset_array（文本资产数组）生成 _R_Text_AssetResource class 的代码
         *
         * @endverbatim
         *
         * @param text_asset_array
         * @param text_asset_id_dict
         * @param package_name
         * @param is_package_project_type
         * @return _R_Text_AssetResource_class_code
         */
        std::string generate__R_Text_AssetResource_class(std::vector<std::string> text_asset_array, std::map<std::string, std::string> text_asset_id_dict, std::string package_name, bool is_package_project_type);

        /**
         * @verbatim
         * generate__R_Image_class(non_svg_image_asset_array, non_svg_image_asset_id_dict, package_name) -> _R_Image_class_code
         *
         * 根据模板，为 non_svg_image_asset_array（非svg类的图片资产数组）生成 _R_Image class 的代码
         *
         * @endverbatim
         *
         * @param non_svg_image_asset_array
         * @param non_svg_image_asset_id_dict
         * @param package_name
         * @return
         */
        std::string generate__R_Image_class(std::vector<std::string> non_svg_image_asset_array, std::map<std::string, std::string> non_svg_image_asset_id_dict, std::string package_name);

        /**
         * @verbatim
         * generate__R_Svg_class(svg_image_asset_array, svg_image_asset_id_dict, package_name, should_support_nullsafety) -> _R_Svg_class_code
         *
         * 根据模板，为 svg_image_asset_array（svg类的图片资产数组）生成 _R_Svg class 的代码
         *
         * @endverbatim
         *
         * @param svg_image_asset_array
         * @param svg_image_asset_id_dict
         * @param package_name
         * @param should_support_nullsafety 生成的代码是否需要支持空安全
         * @return _R_Svg_class_code
         */
        std::string generate__R_Svg_class(std::vector<std::string> svg_image_asset_array, std::map<std::string, std::string> svg_image_asset_id_dict, std::string package_name, bool should_support_nullsafety);

        /**
         * @verbatim
         * generate__R_Text_class(text_asset_array, text_asset_id_dict, package_name) -> _R_Text_class_code
         *
         * 根据模板，为 text_asset_array（文本资产数组）生成 _R_Text class 的代码
         *
         * @endverbatim
         *
         * @param text_asset_array
         * @param text_asset_id_dict
         * @param package_name
         * @return _R_Text_class_code
         */
        std::string generate__R_Text_class(std::vector<std::string> text_asset_array,std::map<std::string, std::string> text_asset_id_dict, std::string package_name);

        /**
         * @verbatim
         * generate_font_family_id(font_family_name) -> font_family_id
         *
         * 为当前 font_family_name 生成 font_family_id；font_family_id 一般为 asset 的 font_family_name；
         * 但是为了保证 font_family_id 的健壮性，需要对 font_family_name 做以下加工处理：
         *  - 处理非法字符：把除了字母（a-z, A-Z）、数字（0-9）、'_' 字符、'$' 字符之外的字符转换为 '_' 字符
         *  - 首字母转化为小写
         *  - 处理首字符异常情况：检测首字符是不是数字、'_'、'$'，若是则添加前缀字符"a"
         *
         * === Examples
         * a_font_family_name = "Amiri"
         * b_font_family_name = "Baloo-Thambi-2"
         * a_font_family_id = "amiri"
         * b_font_family_id = "baloo_Thambi_2"
         *
         * @endverbatim
         *
         * @param font_family_name
         * @return font_family_id
         */
        std::string generate_font_family_id(std::string font_family_name);

        /**
        * FontFamilyConfig=<font_family_name, font_asset_array>
        */
        using FontFamilyConfig = std::tuple<std::string, std::vector<std::string> >;

        /**
         * @verbatim
         * generate__R_FontFamily_class(font_family_config_array, package_name) -> _R_FontFamily_class_code
         *
         * 根据模板，为 font_family_config_array（字体家族配置数组）生成 _R_FontFamily class 的代码
         *
         * @endverbatim
         *
         * @param font_family_config_array
         * @param package_name
         * @return _R_FontFamily_class_code
         */
        std::string generate__R_FontFamily_class(std::vector<FontFamilyConfig> font_family_config_array, std::string package_name);
    }
}


#endif //FLR_CORE_ENGINE_EXAMPLE_CODE_UTIL_H

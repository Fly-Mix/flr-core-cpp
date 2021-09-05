//
// Created by York on 2021/7/18.
//

#ifndef FLR_CORE_ENGINE_EXAMPLE_CONSTANT_H
#define FLR_CORE_ENGINE_EXAMPLE_CONSTANT_H

#include <iostream>
# include <set>

namespace flr {
    namespace constant {
        /**
         * Flr的核心逻辑版本
         */
        const std::string CORE_VERSION = "3.2.0";

        /**
         * flutter工程的配置文件的名称：pubspec.yaml
         */
        const std::string PUBSPEC_FILE_NAME = "pubspec.yaml";

        /**
         * Flr支持的非SVG类图片文件类型
         */
        const std::set<std::string> NON_SVG_IMAGE_FILE_TYPES = {".png", ".jpg", ".jpeg", ".gif", ".webp", ".icon", ".bmp", ".wbmp"};

        /**
         * Flr支持的SVG类图片文件类型
         */
        const std::set<std::string> SVG_IMAGE_FILE_TYPES = {".svg"};

        /**
         * Flr支持的图片文件类型
         */
        const std::set<std::string> IMAGE_FILE_TYPES = {".png", ".jpg", ".jpeg", ".gif", ".webp", ".icon", ".bmp", ".wbmp", ".svg"};

        /**
         * Flr支持的文本文件类型
         */
        const std::set<std::string> TEXT_FILE_TYPES = {".txt", ".json", ".yaml", ".xml"};

        /**
         * Flr支持的字体文件类型
         */
        const std::set<std::string> FONT_FILE_TYPES = {".ttf", ".otf", ".ttc"};

        /**
         * Flr优先考虑的非SVG类图片文件类型
         */
        const std::string PRIOR_NON_SVG_IMAGE_FILE_TYPE = ".png";

        /**
         * Flr优先考虑的SVG类图片文件类型
         */
        const std::string PRIOR_SVG_IMAGE_FILE_TYPE = ".svg";

        /**
         * @verbatim
         * Flr优先考虑的文本文件类型
         * 当前值为 ".*"， 意味所有文本文件类型的优先级都一样
         * @endverbatim
         */
        const std::string PRIOR_TEXT_FILE_TYPE = ".*";

        /**
         * @verbatim
         * Flr优先考虑的字体文件类型
         * 当前值为 ".*"， 意味所有文本文件类型的优先级都一样
         * @endverbatim
         */
        const std::string PRIOR_FONT_FILE_TYPE = ".*";

        /**
         * dartfmt工具的默认行长
         * 仅用于flr-cli
         */
        const int DARTFMT_LINE_LENGTH = 80;
    }
}

#endif //FLR_CORE_ENGINE_EXAMPLE_CONSTANT_H

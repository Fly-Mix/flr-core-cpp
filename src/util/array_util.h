//
// Created by York on 2021/8/1.
//

#ifndef FLR_CORE_ENGINE_EXAMPLE_ARRAY_UTIL_H
#define FLR_CORE_ENGINE_EXAMPLE_ARRAY_UTIL_H

#include <iostream>
#include <vector>

namespace flr {
    namespace array_util {
        /**
    * @verbatim
    * 对字符串数组进行去重操作
    *
    * 注意：此处的"去重"会删除重复的元素
    * @endverbatim
    *
    * @param array
    */
        void unique_string_array(std::vector<std::string> &array);

        /**
         * @verbatim
         * 按照字典顺序对字符串数组做升序排列
         *
         * @endverbatim
         *
         * @param array
         */
        void sort_string_array(std::vector<std::string> &array);
    }
}


#endif //FLR_CORE_ENGINE_EXAMPLE_ARRAY_UTIL_H

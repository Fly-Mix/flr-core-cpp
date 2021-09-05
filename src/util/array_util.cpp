//
// Created by York on 2021/8/1.
//

#include "array_util.h"

namespace flr {
    namespace array_util {
        void unique_string_array(std::vector<std::string> &array)
        {
            /**
             * std::unique 的作用："去除"容器或者数组中相邻元素的重复出现的元素（此处的"去除"是将重复的元素放到容器的末尾，返回值是去重之后的尾地址）
             * 所以，去重处理的步骤：
             * 1. 通过 std::sort 对数组进行排序处理
             * 2. 通过 std::unique 查找重复元素的尾地址
             * 3. 删除重复元素
             * */
            std::sort(array.begin(), array.end());
            auto first_dup_it = std::unique(array.begin(), array.end());
            array.erase(first_dup_it, array.end());
        }

        void sort_string_array(std::vector<std::string> &array) {
            std::sort(array.begin(), array.end());
        }
    }
}

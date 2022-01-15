require 'mkmf'

# 声明追加的 CFLAGS，设置 ”-x c++“，告知 mkmf 使用 g++ 编译器代替 gcc
# 这样，have_header/find_header 才能识别 cpp 的头文件 
# 《Explicitly setting compiler to C++ in extconf.rb》：https://www.ruby-forum.com/t/explicitly-setting-compiler-to-c-in-extconf-rb/208668/10
$CFLAGS += " -x c++ "
puts "CFLAGS = #{$CFLAGS}"

# 声明追加的 CXXFLAGS
$CXXFLAGS += " -std=c++11 -Wno-error=reserved-user-defined-literal "
puts "CXXFLAGS = #{$CXXFLAGS}"

$libs_dir = "#{$srcdir}/../api-libs"
puts "libs_dir = #{$libs_dir}"

# 声明引入二方库
dir_config('flr-core', "#{$libs_dir}/flr-core")
# 导入库的二方库的头文件
abort "missing header flr.h" unless find_header("flr.h", "#{$libs_dir}/flr-core/include/flr-core")
# link二方库
abort "missing static library flr-core" unless have_library("flr-core")

# 声明引入二方库 flr-core 依赖的三方库 glog
dir_config('glog', "#{$libs_dir}/glog")
# link三方库
abort "missing static library glog" unless have_library("glog")

# 声明引入二方库 flr-core 依赖的三方库 fmt
dir_config('fmt', "#{$libs_dir}/fmt")
# link三方库
abort "missing static library fmt" unless have_library("fmt")

# 声明引入二方库 flr-core 依赖的三方库 ryml
dir_config('ryml', "#{$libs_dir}/ryml")
# link三方库
abort "missing static library ryml" unless have_library("ryml")

# 声明源码文件
$flr_src_dir = "#{$srcdir}/.."
$srcs = ["#{$flr_src_dir}/flr_wrap.cxx"]
puts "srcs = #{$srcs}"

create_makefile("Flr", "#{$flr_src_dir}")

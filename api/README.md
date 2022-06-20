## 编译环境

- macOS（>= 11.64）

- cmake（>=3.19.6）

- clang（from Xcode）

   

# How To Run

```shell
# 构建生成 flr-core 和其依赖库
$ cd path/to/flr-core-cpp
# 可选ARCHITECTURES: x86_64;i386;arm64
# 目前只选择支持：x86_64;arm64
$ cmake "-DCMAKE_OSX_ARCHITECTURES=x86_64;arm64" -B Products/build-bin
$ make -C Products/build-bin clean && make -C Products/build-bin

# 调用 swig 生成胶水代码
$ cd path/to/flr-core-cpp/api
$ rm -rf flr_wrap.h flr_wrap.cxx
$ swig -c++ -ruby flr.i

# 为目标高级语言 Ruby 生成动态库，并进行测试
$ cd path/to/flr-core-cpp/api/ruby
$ ruby extconf.rb
$ make clean && make
$ ruby runme.rb
```




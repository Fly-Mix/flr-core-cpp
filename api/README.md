# How To Run
```shell
# 构建生成 flr-core 和其依赖库
$ cd path/to/flr-core-cpp
$ cmake -B Products/build-bin
$ make -C Products/build-bin

# 调用 swig 生成胶水代码
$ cd path/to/flr-core-cpp/api
$ swig -c++ -ruby flr.i

# 为目标高级语言生成动态库，并进行测试
$ cd path/to/flr-core-cpp/api/ruby
$ ruby extconf.rb
$ make
$ ruby runme.rb
```




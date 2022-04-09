# 构建+打包+发布三方库指引

注意，所有构建都是在`3rdparty`目录下进行，若当前不在该目录，请先在终端切换到`3rdparty`目录：

```shell
# 切换到 3rdparty 目录
cd path/to/flr-core-cpp/3rdparty
```



## 编译环境

- macOS（>= 11.64）
- cmake（>=3.19.6）
- clang（from Xcode）

> 查看静态库支持的architecture：lipo -info path/to/libxxx.a



## 三方库—fmt

1. 构建并打包库（该工程的`CMakeLists.txt`默认生成的是静态库）
  ```shell
  # 创建 build 目录（若不存在）
  if [ ! -d fmt-lib/build ] ; then mkdir fmt-lib/build; fi;
  # 执行 camke 生成 makefile
  cmake "-DCMAKE_OSX_ARCHITECTURES=x86_64;arm64" -S fmt -B fmt-lib/build 
  # 执行 make clean 清理上次构建数据
  make clean -C fmt-lib/build
  # 执行 make package 构建和打包库
  make package -C fmt-lib/build
  ```
  > ``` shell
  >  # 若只需要构建生成库，直接执行 make 即可
  >  make -C fmt-lib/build
  > ```

2. 发布库到工程指定目录
  ```shell
  # 清理上次的库，重新生成 lib 和 include 目录
  rm -rf fmt-lib/lib && rm -rf fmt-lib/include
  # 接步骤1，解压库的压缩包（fmt-lib/build/fmt-8.0.1-Darwin.tar.gz）的内容（lib 和 include）到 fmt-lib
  tar zxvf fmt-lib/build/fmt-8.0.1-Darwin.tar.gz --strip-components 1 -C fmt-lib
  ```



## 三方库—rapidyaml

1. 构建并打包库（该工程的`CMakeLists.txt`默认生成的是静态库）
  ```shell
  # 创建 build 目录（若不存在）
  if [ ! -d rapidyaml-lib/build ] ; then mkdir rapidyaml-lib/build; fi;
  # 执行 camke 生成 makefile
  cmake "-DCMAKE_OSX_ARCHITECTURES=x86_64;arm64" -S rapidyaml -B rapidyaml-lib/build 
  # 执行 make clean 清理上次构建数据
  make clean -C rapidyaml-lib/build
  # 执行 make package 构建和打包库
  make package -C rapidyaml-lib/build
  ```
  > ``` shell
  >  # 若只需要构建生成库，直接执行 make 即可
  >  make -C rapidyaml-lib/build
  > ```

2. 发布库到工程指定目录
  ```shell
  # 清理上次的库，重新生成 lib 和 include 目录
  rm -rf rapidyaml-lib/lib && rm -rf rapidyaml-lib/include
  # 接步骤1，解压库的压缩包（rapidyaml-lib/build/ryml-0.2.3-apple64.tar.gz）的内容（lib 和 include）到 fmt-lib
  tar zxvf rapidyaml-lib/build/ryml-0.2.3-apple64.tar.gz --strip-components 1 -C rapidyaml-lib
  ```



## ~~三方库—glog~~

1. 构建并打包库（该工程的`CMakeLists.txt`默认生成的是动态库）

  ```shell
# 创建 build 目录（若不存在）
if [ ! -d glog-lib/build ] ; then mkdir glog-lib/build; fi;
# 执行 camke 生成 makefile（指定 BUILD_SHARED_LIBS=OFF 以生成静态库）；
cmake "-DCMAKE_OSX_ARCHITECTURES=x86_64;arm64" -S glog -B glog-lib/build -D BUILD_SHARED_LIBS=OFF
# 执行 make clean 清理上次构建数据
make clean -C glog-lib/build
# 执行 make package 构建和打包库
make package -C glog-lib/build
  ```

  > ``` shell
  > # 若只需要构建生成库，直接执行 make 即可
  > make -C glog-lib/build
  > ```

2. 发布库到工程指定目录

  ```shell
# 清理上次的库，重新生成 lib 和 include 目录
rm -rf glog-lib/lib && rm -rf glog-lib/include
# 接步骤1，解压库的压缩包（glog-lib/build/glog-0.5.0-Darwin.tar.gz）的内容（lib 和 include）到 glog-lib
tar zxvf glog-lib/build/glog-0.5.0-Darwin.tar.gz --strip-components 1 -C glog-lib
  ```


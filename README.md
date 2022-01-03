## flr-core 以源码方式接入 example 工程运行

1. 编辑工程根目录的`cmake_dev.cmake`中的`FLR_USE_SRC`值为`True`

   ```cmake
   set(FLR_USE_SRC True)
   ```

   > 源码方式引入三方库时，手动构建 flr-core 静态库的步骤：
   >
   > ```shell
   > # 创建 build 目录（若不存在）
   > if [ ! -d Products/build-src ] ; then mkdir Products/build-src; fi;
   > # 执行 camke 生成 makefile
   > cmake -B Products/build-src 
   > # 执行 make clean 清理上次构建数据
   > make clean -C Products/build-src
   > # 执行 make 构建生成库
   > make -C Products/build-src
   > ```


2. 进入`example`目录构建运行 example 工程



   


## flr-core 以二进制包方式接入 example 工程运行

1. 编辑工程根目录的`cmake_dev.cmake`中的`FLR_USE_SRC`值为`False`

   ```cmake
   set(FLR_USE_SRC False)
   ```

2. 进入`3rdparty`目录，按照`README`的指引构建和发布三方库（若已发布，则跳过该步骤）

3. 构建 flr-core 静态库

   ```shell
   # 创建 build 目录（若不存在）
   if [ ! -d Products/build-bin ] ; then mkdir Products/build-bin; fi;
   # 执行 camke 生成 makefile
   cmake -B Products/build-bin 
   # 执行 make clean 清理上次构建数据
   make clean -C Products/build-bin
   # 执行 make 构建生成库
   make -C Products/build-bin
   ```

4. 进入`example`目录构建运行 example 工程


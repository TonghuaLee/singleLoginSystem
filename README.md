# singleLoginSystem
a system for single device login

# 背景
综合设计题
￼
请设计一套单终端登录系统，具备以下功能
 
1. 具备注册登录功能
2. 一个用户只能在一个设备上登录，切换终端登录时，其他已登录的终端会被踢出
 
￼
请实现该系统的客户端及后台，要求:
 
1. 客户端登录模块需要采用C++实现iOS和android的跨平台
2. 终端app只要提供iOS或android一种客户端即可
3. 后台采用grpc实现，使用C++实现，用户信息存储上可以采用sql或nosql实现，设计时需要考虑敏感数据的安全性及传输安全性
4. 后台项目使用bazel编译及管理项目依赖
5. 项目设计实现周期两周左右，交付物包括可运行的客户端及后台项目代码，设计文档，使用github管理相关资源
6. 使用docker部署，docker compose管理项目
 
￼
参考资料
 
1. C++实现iOS和android跨平台 https://github.com/dropbox/djinni
2. grpc https://grpc.io/
3. bazel https://bazel.build/
4. docker compose https://docs.docker.com/compose/
5. docker file https://docs.docker.com/engine/reference/builder/

# 计划
1、 flutter LoginUI
2、 接入djinni HelloWorld
3、 部署docker
4、 学习grpc、bazel，运行helloworld接口
5、 部署mysql，创建user表
6、 后台接口链接数据库
7、 第一次联调，客户端调用后台接口存入数据库
8、 登录流程与接口设计，并记录文档
9、 接口接入,完整实现
10、 正式环境部署，系统测试

### 常用后端命令
| 命令 | 备注 |
|:--|----|
| bazel build //source:account_server | 构建目标文件 |
| sudo cp ./bazel-bin/source/account_server docker-src/ | 将构建成功的执行文件复制到本地docker目录 |
| sudo docker build -t grpcserver:1.0 . | 在根目录，根据Dockerfile编写的规则，生成服务镜像 |
| sudo docker-compose up grpcserver | 在根目录，根据docker-compose.yml编写的规则，启动并管理容器 |



 # broad
https://github.com/protocolbuffers/protobuf/releases?after=v3.11.0-rc1
cd protoc-3.6.1-osx-x86_64
cp -r include/ /usr/local/include/
cp -r bin/ /usr/local/bin/
 protoc --cpp_out=./ account.proto
 protoc --grpc_out=./ --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin account.proto
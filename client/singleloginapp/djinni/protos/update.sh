 # broad
protoc --cpp_out=./ account.proto
protoc --grpc_out=./ --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin account.proto
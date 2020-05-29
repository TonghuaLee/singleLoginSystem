// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: account.proto

#include "account.pb.h"
#include "account.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace account {

static const char* Account_method_names[] = {
  "/account.Account/requestUserLogin",
  "/account.Account/requestUserSign",
  "/account.Account/checkConnect",
  "/account.Account/requestLogout",
  "/account.Account/refreshToken",
  "/account.Account/requestAddCategory",
  "/account.Account/requestFetchCategory",
  "/account.Account/requestAddTodo",
  "/account.Account/requestUpdateTodo",
};

std::unique_ptr< Account::Stub> Account::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< Account::Stub> stub(new Account::Stub(channel));
  return stub;
}

Account::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_requestUserLogin_(Account_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_requestUserSign_(Account_method_names[1], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_checkConnect_(Account_method_names[2], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_requestLogout_(Account_method_names[3], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_refreshToken_(Account_method_names[4], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_requestAddCategory_(Account_method_names[5], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_requestFetchCategory_(Account_method_names[6], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_requestAddTodo_(Account_method_names[7], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_requestUpdateTodo_(Account_method_names[8], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status Account::Stub::requestUserLogin(::grpc::ClientContext* context, const ::account::LoginRequest& request, ::account::CodeReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_requestUserLogin_, context, request, response);
}

void Account::Stub::experimental_async::requestUserLogin(::grpc::ClientContext* context, const ::account::LoginRequest* request, ::account::CodeReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_requestUserLogin_, context, request, response, std::move(f));
}

void Account::Stub::experimental_async::requestUserLogin(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::account::CodeReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_requestUserLogin_, context, request, response, std::move(f));
}

void Account::Stub::experimental_async::requestUserLogin(::grpc::ClientContext* context, const ::account::LoginRequest* request, ::account::CodeReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_requestUserLogin_, context, request, response, reactor);
}

void Account::Stub::experimental_async::requestUserLogin(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::account::CodeReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_requestUserLogin_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::account::CodeReply>* Account::Stub::AsyncrequestUserLoginRaw(::grpc::ClientContext* context, const ::account::LoginRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::account::CodeReply>::Create(channel_.get(), cq, rpcmethod_requestUserLogin_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::account::CodeReply>* Account::Stub::PrepareAsyncrequestUserLoginRaw(::grpc::ClientContext* context, const ::account::LoginRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::account::CodeReply>::Create(channel_.get(), cq, rpcmethod_requestUserLogin_, context, request, false);
}

::grpc::Status Account::Stub::requestUserSign(::grpc::ClientContext* context, const ::account::SignRequest& request, ::account::CodeReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_requestUserSign_, context, request, response);
}

void Account::Stub::experimental_async::requestUserSign(::grpc::ClientContext* context, const ::account::SignRequest* request, ::account::CodeReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_requestUserSign_, context, request, response, std::move(f));
}

void Account::Stub::experimental_async::requestUserSign(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::account::CodeReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_requestUserSign_, context, request, response, std::move(f));
}

void Account::Stub::experimental_async::requestUserSign(::grpc::ClientContext* context, const ::account::SignRequest* request, ::account::CodeReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_requestUserSign_, context, request, response, reactor);
}

void Account::Stub::experimental_async::requestUserSign(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::account::CodeReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_requestUserSign_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::account::CodeReply>* Account::Stub::AsyncrequestUserSignRaw(::grpc::ClientContext* context, const ::account::SignRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::account::CodeReply>::Create(channel_.get(), cq, rpcmethod_requestUserSign_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::account::CodeReply>* Account::Stub::PrepareAsyncrequestUserSignRaw(::grpc::ClientContext* context, const ::account::SignRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::account::CodeReply>::Create(channel_.get(), cq, rpcmethod_requestUserSign_, context, request, false);
}

::grpc::Status Account::Stub::checkConnect(::grpc::ClientContext* context, const ::account::ConnectRequest& request, ::account::CodeReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_checkConnect_, context, request, response);
}

void Account::Stub::experimental_async::checkConnect(::grpc::ClientContext* context, const ::account::ConnectRequest* request, ::account::CodeReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_checkConnect_, context, request, response, std::move(f));
}

void Account::Stub::experimental_async::checkConnect(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::account::CodeReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_checkConnect_, context, request, response, std::move(f));
}

void Account::Stub::experimental_async::checkConnect(::grpc::ClientContext* context, const ::account::ConnectRequest* request, ::account::CodeReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_checkConnect_, context, request, response, reactor);
}

void Account::Stub::experimental_async::checkConnect(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::account::CodeReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_checkConnect_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::account::CodeReply>* Account::Stub::AsynccheckConnectRaw(::grpc::ClientContext* context, const ::account::ConnectRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::account::CodeReply>::Create(channel_.get(), cq, rpcmethod_checkConnect_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::account::CodeReply>* Account::Stub::PrepareAsynccheckConnectRaw(::grpc::ClientContext* context, const ::account::ConnectRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::account::CodeReply>::Create(channel_.get(), cq, rpcmethod_checkConnect_, context, request, false);
}

::grpc::Status Account::Stub::requestLogout(::grpc::ClientContext* context, const ::account::LogoutRequest& request, ::account::CodeReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_requestLogout_, context, request, response);
}

void Account::Stub::experimental_async::requestLogout(::grpc::ClientContext* context, const ::account::LogoutRequest* request, ::account::CodeReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_requestLogout_, context, request, response, std::move(f));
}

void Account::Stub::experimental_async::requestLogout(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::account::CodeReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_requestLogout_, context, request, response, std::move(f));
}

void Account::Stub::experimental_async::requestLogout(::grpc::ClientContext* context, const ::account::LogoutRequest* request, ::account::CodeReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_requestLogout_, context, request, response, reactor);
}

void Account::Stub::experimental_async::requestLogout(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::account::CodeReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_requestLogout_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::account::CodeReply>* Account::Stub::AsyncrequestLogoutRaw(::grpc::ClientContext* context, const ::account::LogoutRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::account::CodeReply>::Create(channel_.get(), cq, rpcmethod_requestLogout_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::account::CodeReply>* Account::Stub::PrepareAsyncrequestLogoutRaw(::grpc::ClientContext* context, const ::account::LogoutRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::account::CodeReply>::Create(channel_.get(), cq, rpcmethod_requestLogout_, context, request, false);
}

::grpc::Status Account::Stub::refreshToken(::grpc::ClientContext* context, const ::account::RefreshRequest& request, ::account::CodeReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_refreshToken_, context, request, response);
}

void Account::Stub::experimental_async::refreshToken(::grpc::ClientContext* context, const ::account::RefreshRequest* request, ::account::CodeReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_refreshToken_, context, request, response, std::move(f));
}

void Account::Stub::experimental_async::refreshToken(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::account::CodeReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_refreshToken_, context, request, response, std::move(f));
}

void Account::Stub::experimental_async::refreshToken(::grpc::ClientContext* context, const ::account::RefreshRequest* request, ::account::CodeReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_refreshToken_, context, request, response, reactor);
}

void Account::Stub::experimental_async::refreshToken(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::account::CodeReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_refreshToken_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::account::CodeReply>* Account::Stub::AsyncrefreshTokenRaw(::grpc::ClientContext* context, const ::account::RefreshRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::account::CodeReply>::Create(channel_.get(), cq, rpcmethod_refreshToken_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::account::CodeReply>* Account::Stub::PrepareAsyncrefreshTokenRaw(::grpc::ClientContext* context, const ::account::RefreshRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::account::CodeReply>::Create(channel_.get(), cq, rpcmethod_refreshToken_, context, request, false);
}

::grpc::Status Account::Stub::requestAddCategory(::grpc::ClientContext* context, const ::account::AddCategoryRequest& request, ::account::CodeReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_requestAddCategory_, context, request, response);
}

void Account::Stub::experimental_async::requestAddCategory(::grpc::ClientContext* context, const ::account::AddCategoryRequest* request, ::account::CodeReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_requestAddCategory_, context, request, response, std::move(f));
}

void Account::Stub::experimental_async::requestAddCategory(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::account::CodeReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_requestAddCategory_, context, request, response, std::move(f));
}

void Account::Stub::experimental_async::requestAddCategory(::grpc::ClientContext* context, const ::account::AddCategoryRequest* request, ::account::CodeReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_requestAddCategory_, context, request, response, reactor);
}

void Account::Stub::experimental_async::requestAddCategory(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::account::CodeReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_requestAddCategory_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::account::CodeReply>* Account::Stub::AsyncrequestAddCategoryRaw(::grpc::ClientContext* context, const ::account::AddCategoryRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::account::CodeReply>::Create(channel_.get(), cq, rpcmethod_requestAddCategory_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::account::CodeReply>* Account::Stub::PrepareAsyncrequestAddCategoryRaw(::grpc::ClientContext* context, const ::account::AddCategoryRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::account::CodeReply>::Create(channel_.get(), cq, rpcmethod_requestAddCategory_, context, request, false);
}

::grpc::Status Account::Stub::requestFetchCategory(::grpc::ClientContext* context, const ::account::FetchCategoryRequest& request, ::account::CodeReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_requestFetchCategory_, context, request, response);
}

void Account::Stub::experimental_async::requestFetchCategory(::grpc::ClientContext* context, const ::account::FetchCategoryRequest* request, ::account::CodeReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_requestFetchCategory_, context, request, response, std::move(f));
}

void Account::Stub::experimental_async::requestFetchCategory(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::account::CodeReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_requestFetchCategory_, context, request, response, std::move(f));
}

void Account::Stub::experimental_async::requestFetchCategory(::grpc::ClientContext* context, const ::account::FetchCategoryRequest* request, ::account::CodeReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_requestFetchCategory_, context, request, response, reactor);
}

void Account::Stub::experimental_async::requestFetchCategory(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::account::CodeReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_requestFetchCategory_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::account::CodeReply>* Account::Stub::AsyncrequestFetchCategoryRaw(::grpc::ClientContext* context, const ::account::FetchCategoryRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::account::CodeReply>::Create(channel_.get(), cq, rpcmethod_requestFetchCategory_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::account::CodeReply>* Account::Stub::PrepareAsyncrequestFetchCategoryRaw(::grpc::ClientContext* context, const ::account::FetchCategoryRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::account::CodeReply>::Create(channel_.get(), cq, rpcmethod_requestFetchCategory_, context, request, false);
}

::grpc::Status Account::Stub::requestAddTodo(::grpc::ClientContext* context, const ::account::AddTodoRequest& request, ::account::CodeReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_requestAddTodo_, context, request, response);
}

void Account::Stub::experimental_async::requestAddTodo(::grpc::ClientContext* context, const ::account::AddTodoRequest* request, ::account::CodeReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_requestAddTodo_, context, request, response, std::move(f));
}

void Account::Stub::experimental_async::requestAddTodo(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::account::CodeReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_requestAddTodo_, context, request, response, std::move(f));
}

void Account::Stub::experimental_async::requestAddTodo(::grpc::ClientContext* context, const ::account::AddTodoRequest* request, ::account::CodeReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_requestAddTodo_, context, request, response, reactor);
}

void Account::Stub::experimental_async::requestAddTodo(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::account::CodeReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_requestAddTodo_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::account::CodeReply>* Account::Stub::AsyncrequestAddTodoRaw(::grpc::ClientContext* context, const ::account::AddTodoRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::account::CodeReply>::Create(channel_.get(), cq, rpcmethod_requestAddTodo_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::account::CodeReply>* Account::Stub::PrepareAsyncrequestAddTodoRaw(::grpc::ClientContext* context, const ::account::AddTodoRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::account::CodeReply>::Create(channel_.get(), cq, rpcmethod_requestAddTodo_, context, request, false);
}

::grpc::Status Account::Stub::requestUpdateTodo(::grpc::ClientContext* context, const ::account::UpdateTodoRequest& request, ::account::CodeReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_requestUpdateTodo_, context, request, response);
}

void Account::Stub::experimental_async::requestUpdateTodo(::grpc::ClientContext* context, const ::account::UpdateTodoRequest* request, ::account::CodeReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_requestUpdateTodo_, context, request, response, std::move(f));
}

void Account::Stub::experimental_async::requestUpdateTodo(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::account::CodeReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_requestUpdateTodo_, context, request, response, std::move(f));
}

void Account::Stub::experimental_async::requestUpdateTodo(::grpc::ClientContext* context, const ::account::UpdateTodoRequest* request, ::account::CodeReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_requestUpdateTodo_, context, request, response, reactor);
}

void Account::Stub::experimental_async::requestUpdateTodo(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::account::CodeReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_requestUpdateTodo_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::account::CodeReply>* Account::Stub::AsyncrequestUpdateTodoRaw(::grpc::ClientContext* context, const ::account::UpdateTodoRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::account::CodeReply>::Create(channel_.get(), cq, rpcmethod_requestUpdateTodo_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::account::CodeReply>* Account::Stub::PrepareAsyncrequestUpdateTodoRaw(::grpc::ClientContext* context, const ::account::UpdateTodoRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::account::CodeReply>::Create(channel_.get(), cq, rpcmethod_requestUpdateTodo_, context, request, false);
}

Account::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Account_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Account::Service, ::account::LoginRequest, ::account::CodeReply>(
          [](Account::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::account::LoginRequest* req,
             ::account::CodeReply* resp) {
               return service->requestUserLogin(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Account_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Account::Service, ::account::SignRequest, ::account::CodeReply>(
          [](Account::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::account::SignRequest* req,
             ::account::CodeReply* resp) {
               return service->requestUserSign(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Account_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Account::Service, ::account::ConnectRequest, ::account::CodeReply>(
          [](Account::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::account::ConnectRequest* req,
             ::account::CodeReply* resp) {
               return service->checkConnect(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Account_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Account::Service, ::account::LogoutRequest, ::account::CodeReply>(
          [](Account::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::account::LogoutRequest* req,
             ::account::CodeReply* resp) {
               return service->requestLogout(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Account_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Account::Service, ::account::RefreshRequest, ::account::CodeReply>(
          [](Account::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::account::RefreshRequest* req,
             ::account::CodeReply* resp) {
               return service->refreshToken(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Account_method_names[5],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Account::Service, ::account::AddCategoryRequest, ::account::CodeReply>(
          [](Account::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::account::AddCategoryRequest* req,
             ::account::CodeReply* resp) {
               return service->requestAddCategory(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Account_method_names[6],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Account::Service, ::account::FetchCategoryRequest, ::account::CodeReply>(
          [](Account::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::account::FetchCategoryRequest* req,
             ::account::CodeReply* resp) {
               return service->requestFetchCategory(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Account_method_names[7],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Account::Service, ::account::AddTodoRequest, ::account::CodeReply>(
          [](Account::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::account::AddTodoRequest* req,
             ::account::CodeReply* resp) {
               return service->requestAddTodo(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Account_method_names[8],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Account::Service, ::account::UpdateTodoRequest, ::account::CodeReply>(
          [](Account::Service* service,
             ::grpc_impl::ServerContext* ctx,
             const ::account::UpdateTodoRequest* req,
             ::account::CodeReply* resp) {
               return service->requestUpdateTodo(ctx, req, resp);
             }, this)));
}

Account::Service::~Service() {
}

::grpc::Status Account::Service::requestUserLogin(::grpc::ServerContext* context, const ::account::LoginRequest* request, ::account::CodeReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Account::Service::requestUserSign(::grpc::ServerContext* context, const ::account::SignRequest* request, ::account::CodeReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Account::Service::checkConnect(::grpc::ServerContext* context, const ::account::ConnectRequest* request, ::account::CodeReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Account::Service::requestLogout(::grpc::ServerContext* context, const ::account::LogoutRequest* request, ::account::CodeReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Account::Service::refreshToken(::grpc::ServerContext* context, const ::account::RefreshRequest* request, ::account::CodeReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Account::Service::requestAddCategory(::grpc::ServerContext* context, const ::account::AddCategoryRequest* request, ::account::CodeReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Account::Service::requestFetchCategory(::grpc::ServerContext* context, const ::account::FetchCategoryRequest* request, ::account::CodeReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Account::Service::requestAddTodo(::grpc::ServerContext* context, const ::account::AddTodoRequest* request, ::account::CodeReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Account::Service::requestUpdateTodo(::grpc::ServerContext* context, const ::account::UpdateTodoRequest* request, ::account::CodeReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace account


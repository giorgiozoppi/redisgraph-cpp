// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: fraud_service.proto

#include "fraud_service.pb.h"
#include "fraud_service.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace fraudservice {

static const char* FraudDetectService_method_names[] = {
  "/fraudservice.FraudDetectService/Transact",
};

std::unique_ptr< FraudDetectService::Stub> FraudDetectService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< FraudDetectService::Stub> stub(new FraudDetectService::Stub(channel));
  return stub;
}

FraudDetectService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_Transact_(FraudDetectService_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status FraudDetectService::Stub::Transact(::grpc::ClientContext* context, const ::fraudservice::TransactRequest& request, ::fraudservice::TransactReply* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_Transact_, context, request, response);
}

void FraudDetectService::Stub::experimental_async::Transact(::grpc::ClientContext* context, const ::fraudservice::TransactRequest* request, ::fraudservice::TransactReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Transact_, context, request, response, std::move(f));
}

void FraudDetectService::Stub::experimental_async::Transact(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::fraudservice::TransactReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_Transact_, context, request, response, std::move(f));
}

void FraudDetectService::Stub::experimental_async::Transact(::grpc::ClientContext* context, const ::fraudservice::TransactRequest* request, ::fraudservice::TransactReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_Transact_, context, request, response, reactor);
}

void FraudDetectService::Stub::experimental_async::Transact(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::fraudservice::TransactReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_Transact_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::fraudservice::TransactReply>* FraudDetectService::Stub::AsyncTransactRaw(::grpc::ClientContext* context, const ::fraudservice::TransactRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::fraudservice::TransactReply>::Create(channel_.get(), cq, rpcmethod_Transact_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::fraudservice::TransactReply>* FraudDetectService::Stub::PrepareAsyncTransactRaw(::grpc::ClientContext* context, const ::fraudservice::TransactRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::fraudservice::TransactReply>::Create(channel_.get(), cq, rpcmethod_Transact_, context, request, false);
}

FraudDetectService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      FraudDetectService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< FraudDetectService::Service, ::fraudservice::TransactRequest, ::fraudservice::TransactReply>(
          std::mem_fn(&FraudDetectService::Service::Transact), this)));
}

FraudDetectService::Service::~Service() {
}

::grpc::Status FraudDetectService::Service::Transact(::grpc::ServerContext* context, const ::fraudservice::TransactRequest* request, ::fraudservice::TransactReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace fraudservice

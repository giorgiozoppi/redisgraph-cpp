# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
import grpc

import fraud_service_pb2 as fraud__service__pb2


class FraudDetectServiceStub(object):
  # missing associated documentation comment in .proto file
  pass

  def __init__(self, channel):
    """Constructor.

    Args:
      channel: A grpc.Channel.
    """
    self.Transact = channel.unary_unary(
        '/fraudservice.FraudDetectService/Transact',
        request_serializer=fraud__service__pb2.TransactRequest.SerializeToString,
        response_deserializer=fraud__service__pb2.TransactReply.FromString,
        )


class FraudDetectServiceServicer(object):
  # missing associated documentation comment in .proto file
  pass

  def Transact(self, request, context):
    # missing associated documentation comment in .proto file
    pass
    context.set_code(grpc.StatusCode.UNIMPLEMENTED)
    context.set_details('Method not implemented!')
    raise NotImplementedError('Method not implemented!')


def add_FraudDetectServiceServicer_to_server(servicer, server):
  rpc_method_handlers = {
      'Transact': grpc.unary_unary_rpc_method_handler(
          servicer.Transact,
          request_deserializer=fraud__service__pb2.TransactRequest.FromString,
          response_serializer=fraud__service__pb2.TransactReply.SerializeToString,
      ),
  }
  generic_handler = grpc.method_handlers_generic_handler(
      'fraudservice.FraudDetectService', rpc_method_handlers)
  server.add_generic_rpc_handlers((generic_handler,))
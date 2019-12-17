#!/usr/bin/env python
from __future__ import print_function
import logging
import grpc
import fraud_service_pb2
import fraud_service_pb2_grpc
def run(index=0):
    with grpc.insecure_channel('localhost:14100') as channel:
        stub = fraud_service_pb2_grpc.FraudDetectServiceStub(channel)
        request = fraud_service_pb2.TransactRequest(transactionId='190291029102910', 
                                                    ipAddress='192.129.1.2',
                                                    merchantId=55901,
                                                    userId=2391,
                                                    userAddress='Calle Ramon Turro 34',
                                                    shippingAddress='Calle Ramon Turro 34',
                                                    city='Barcelona',
                                                    country='Spain',
                                                    amount=120, 
                                                    latitude=41.2,
                                                    longitude =31.1,
                                                    cookie_hash='2320392039209x90322920380238x823204')

        response = stub.Transact(request)
        print("Fraud Score: %f" % response.fraud_score)
        request_t2 = fraud_service_pb2.TransactRequest(transactionId='190291029102910', 
                                                    ipAddress='192.129.1.2',
                                                    merchantId=55901,
                                                    userId=2391,
                                                    userAddress='Avenida Roma 34',
                                                    shippingAddress='Avenida Roma 34',
                                                    city='Barcelona',
                                                    country='Spain',
                                                    amount=120, 
                                                    latitude=41.2,
                                                    longitude =31.1,
                                                    cookie_hash='2320392039209x90322920380238x823204')
        response_t2 = stub.Transact(request_t2)
        print("Fraud Score: %f" % response_t2.fraud_score)
if __name__ == '__main__':
    logging.basicConfig()
    run()
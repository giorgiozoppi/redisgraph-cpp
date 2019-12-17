/*
* Copyright 2019, Giorgio Zoppi
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*         http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
*        limitations under the License
*/
#ifndef FDS_SERVER_IMPL
#define FDS_SERVER_IMPL
#pragma once
#include <string>
#include <memory>
#include <array>
#include <optional>
#include <thread>
#include <future>
#include <redisgraph.hpp>
#include <fraud_service.grpc.pb.h>

namespace redisgraph
{
    using namespace grpc;
    using namespace fraudservice;
    class fraud_service final : public FraudDetectService::Service
    {
    public:
        fraud_service() = default;
        fraud_service(redisgraph::graph<picojson::value>&& graph);
        bool load_rules(const std::string& path);
        void start();
        void shutdown();
        ::grpc::Status Transact(::grpc::ServerContext* context, 
            const ::fraudservice::TransactRequest* request, 
            ::fraudservice::TransactReply* response) override;
        
    private:
        double send(const std::string& query);
        std::vector<std::string> query_rules_;
        redisgraph::graph<picojson::value> api_;
        static const double THREASOLD;
    };
}
#endif
#include "fds.ipp"

#include <fstream>
#include <string>
#include <picojson/picojson.h>
#include <fds.hpp>
#pragma once

namespace redisgraph
{
    using namespace grpc;
    using namespace google::protobuf::util;
    using namespace fraudservice;

    fraud_service::fraud_service(redisgraph::graph<picojson::value>&& graph) : api_(std::move(graph))
    {
    }
    ::grpc::Status fraud_service::Transact(::grpc::ServerContext* context,
        const ::fraudservice::TransactRequest* request,
        ::fraudservice::TransactReply* response)
    {
        double fraudRate = 0;
        int i = 2;
        for (const auto& query : query_rules_)
        {
            try
            {
                fraudRate = send(query) + (fraudRate * 1 / i);
                i += 2;
            }
            catch (const std::exception & ex)
            {
                response->set_message_code(TransactionStatus::TRANSACTION_FAILED);
                return ::grpc::Status(StatusCode::INTERNAL, ex.what());
            }
        }
        if (fraudRate < 0)
        {
            response->set_message_code(TransactionStatus::TRANSACTION_FAILED);
        }
        else if (fraudRate > THREASOLD)
        {
            response->set_is_blocked(true);
            response->set_fraud_score(fraudRate);
            response->set_message_code(TransactionStatus::TRANSACTION_OK);
            response->set_reason("High risk of fraud");
        }
        else
        {
            response->set_is_blocked(false);
            response->set_fraud_score(fraudRate);
            response->set_message_code(TransactionStatus::TRANSACTION_OK);
            response->set_reason("Low fraud score");
        }
        return::grpc::Status::OK;
    }
    void fraud_service::start()
    {
        api_.start();
    }
    void fraud_service::shutdown()
    {
        api_.shutdown();
    }
    bool fraud_service::load_rules(const std::string& path)
    {
        std::ifstream in(path.c_str());
        if (!in.is_open()) return false;
        std::string line;
        while (std::getline(in, line))
        {
            query_rules_.emplace_back(line);
        }
        return true;
    }
    double fraud_service::send(const std::string& query)
    {
        std::promise<redisgraph::result_view> p;
        auto result = p.get_future();
        api_.query_async(query, std::move(p));
        auto view = result.get();
        view.parse_score();
        return view.score();
    }
    const double fraud_service::THREASOLD = 0.70;
}
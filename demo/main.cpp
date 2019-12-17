#include <redisgraph.hpp>
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
#include <iostream>
#include <memory>
#include <string>
#include <fds.hpp>
#include <grpc++/grpc++.h>
using namespace grpc;
using
  grpc::Server;
using
  grpc::ServerBuilder;
using
  grpc::ServerContext;
using
  grpc::Status;



void
RunServer (const std::string & address, const std::string& rules, redisgraph::graph<picojson::value>&& api)
{
  std::string server_address (address);
  redisgraph::fraud_service fraudService(std::move(api));
  fraudService.start();
  ServerBuilder builder;
//  builder.AddListeningPort (server_address,
//		    grpc::InsecureServerCredentials ());
//  builder.RegisterService (&fraudService);
 // std::unique_ptr <Server> server (builder.BuildAndStart ());
  std::cout << "Server listening on " << server_address << std::endl;
 // server->Wait ();
  fraudService.shutdown();
}

int main ()
{
 const int NUMBER_THREADS = 2;
 const std::string defaultHost("127.0.0.1");
 redisgraph::connection_context ctx{defaultHost,6379, NUMBER_THREADS};
 redisgraph::graph<picojson::value> api("Social", ctx);   
 RunServer ("0.0.0.0:14100", "link_analysis.txt", std::move(api));
 return 0;
}
/**
* Copyright 2019 RRD Software Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
**/

#ifndef REDISGRAPH_CPP_REDISEXECUTOR_H_
#define REDISGRAPH_CPP_REDISEXECUTOR_H_
#pragma once
#include <string>
#include <future>
#include <memory>
#include <optional>
#include <utility>
#include <thread>
#include <boost/asio.hpp>
#include <bredis.hpp>
#include "connection_context.hpp"
#include "result_view.hpp"
#include "redis_context.hpp"
#include "exception/connection_exception.hpp"

#if (defined(_MSC_VER) && (_MSC_VER >= 1400))
#define _CRT_SECURE_NO_WARNINGS
#endif // _MSC_VER >= 1400
namespace redis = bredis;
namespace asio = boost::asio;


namespace redisgraph {
	/*
	* Create a graph interface with nodes that contains data of type T
	*/
	class redis_executor 
	{
		public:
			static const int REDIS_CONNECTION_ERROR = 400;
			explicit redis_executor(const redisgraph::connection_context& context):context_(context)
			{
				started_ = false;
			}
			
			/**
			* Copy constructor. The graph is not copyable.
			* @param graph Graph to be copied.
			*/
			redis_executor(const redisgraph::redis_executor& executor) = delete;
			/**
			* Copy assignment operator. The graph is not copyable
			* @param Graph to be assigned.
			*/
			const redisgraph::redis_executor& operator=(const redisgraph::redis_executor& graph) = delete;
			/** 
			 * Move semantics 
			 */
			redis_executor(redis_executor&& g): context_(g.context_)
			{
				g.shutdown();
				started_ = false;
			}

			bool is_started() const
			{
				return started_;
			}
			~redis_executor() {
				if (started_)
				{
				    shutdown();
				}
			}

			/*
			* startup the connection pool to redis 
			*/
			bool start()
			{	
				auto workers_count = context_.n_threads();
				init_connection(context_);
				for (size_t i = 0; i < workers_count; i++) {
					threadpool_.emplace_back(std::thread([this]()
						{
							io_context_.run();
						}));
				} 
				started_ = true;
				return started_;
			}

			void send_message(const std::string& message, std::promise<redisgraph::result_view>&& result_promise)
			{
					redis::single_command_t cmd{ message };

					executor_context_ptr_->conn.async_write(
						executor_context_ptr_->tx_buff,
						cmd,
						asio::bind_executor(executor_context_ptr_->strand, [this, &result_promise](const sys::error_code& ec, std::size_t bytes_transferred)
							{
							if (!ec) {
								auto self = this;
								self->executor_context_ptr_->tx_buff.consume(bytes_transferred);
								executor_context_ptr_->conn.async_read(
									executor_context_ptr_->rx_buff,
									asio::bind_executor(executor_context_ptr_->strand, [this, &result_promise](const sys::error_code& ec, result_t&& r) {
										if (!ec) {
											auto self = this;
											auto extract = boost::apply_visitor(redis::extractor<Iterator>(), r.result);
											auto& reply_str = boost::get<r::extracts::string_t>(extract);
											redisgraph::result_view view(reply_str.str);
											result_promise.set_value(view);
											self->executor_context_ptr_->rx_buff.consume(r.consumed);
											// parse and set result.
										}
										}));
								//self->produce();
							}
							}));
					
					
			}		
			/**
			 *  shutdown the connection pool to redis 
			 */
			bool shutdown()
			{
				if (started_)
				{
					std::for_each(threadpool_.begin(), threadpool_.end(), [](std::thread& t)
						{
							t.join();
						});

					executor_context_ptr_.reset();
					io_context_.stop();
					threadpool_.clear();
					started_ = false;
				}
				return true;
			}
		    void init_connection(const redisgraph::connection_context context)
			{
				try
				{
					auto ip_address = asio::ip::address::from_string(context.host());
					asio::ip::tcp::endpoint end_point(ip_address, context.port());
					socket_t socket(io_context_, end_point.protocol());
					socket.connect(end_point);
					executor_context_ptr_ = std::make_unique<redis_executor_context_t>(std::move(socket), io_context_);
				}
				catch (const std::exception & ex)
				{
					started_ = false;
					auto code = std::error_code(redis_executor::REDIS_CONNECTION_ERROR, std::system_category());
					throw connection_exception(code, ex.what());
				}
			}
			redisgraph::connection_context context_;
			bool started_;
			std::vector<std::thread> threadpool_;
			asio::io_context io_context_;
			std::unique_ptr<redis_executor_context_t> executor_context_ptr_;
	};
}
#endif /* REDISGRAPH_CPP_REDISEXECUTOR_H_ */
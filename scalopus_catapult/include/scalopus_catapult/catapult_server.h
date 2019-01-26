/*
  Copyright (c) 2018-2019, Ivor Wanders
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  * Neither the name of the author nor the names of contributors may be used to
    endorse or promote products derived from this software without specific
    prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef SCALOPUS_CATAPULT_CATAPULT_SERVER_H
#define SCALOPUS_CATAPULT_CATAPULT_SERVER_H

#include <seasocks/PrintfLogger.h>
#include <seasocks/Server.h>
#include "scalopus_catapult/catapult_backend.h"
#include "scalopus_catapult/endpoint_manager_poll.h"

namespace scalopus
{
namespace ss = seasocks;

/**
 * @brief The catapult server, combines the seasocks server, worker thread and backend in a convenient way.
 */
class CatapultServer
{
public:
  using Ptr = std::shared_ptr<CatapultServer>;

  /**
   * @brief Construct the catapult server using the provided backend.
   */
  CatapultServer(CatapultBackend::Ptr backend);

  /**
   * @brief Set a logger to use.
   */
  void setLogger(std::shared_ptr<ss::Logger> logger);

  /**
   * @brief Same as calling setLogger(std::make_shared<ss::PrintfLogger>(ss::Logger::Level::Warning)).
   */
  void setDefaultLogger();

  /**
   * @brief This is passed directly to seasocks' setClientBufferSize method. This is the limit that can be present in
   *        the communication buffer. It is not necessarily allocated or used. Defaults to 128 mb.
   */
  void setMaxBuffersize(std::size_t max_buffer);

  /**
   * @brief Start the server and create the worker thread.
   * @note This MUST be the last method called.
   */
  void start(std::size_t port = 9222);

  ~CatapultServer();

private:
  CatapultBackend::Ptr backend_;
  std::shared_ptr<ss::Logger> logger_;
  std::shared_ptr<ss::Server> server_;
  std::thread thread_;
  std::size_t max_buffer_{ 128 * 1024 * 1024u };
};

}  // namespace scalopus
#endif  // SCALOPUS_CATAPULT_CATAPULT_SERVER_H

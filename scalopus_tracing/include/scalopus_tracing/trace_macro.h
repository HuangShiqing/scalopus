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
#ifndef SCALOPUS_TRACING_SCOPE_TRACING_H
#define SCALOPUS_TRACING_SCOPE_TRACING_H

#include <scalopus_tracing/internal/scope_trace_raii.h>
#include <scalopus_tracing/internal/static_string_tracker.h>
#include <scalopus_tracing/internal/trace_configuration_raii.h>
#include <scalopus_tracing/internal/trace_macro.h>
#include <scalopus_tracing/internal/compile_time_crc.hpp>
#include <scalopus_tracing/internal/run_time_crc.h>

/**
 * Public Macros
 * Tracked tracepoints store the actual tracepoint by ID, the name is tracked by the trace point tracking singleton.
 * The ID must be unique per tracing session, it can be automatically generated with the filename and line number.
 */

// Macro to create a tracker RAII tracepoint. The ID is automatically generated with the last part of the filename and
// the line number.
// TODO:
#define TRACE_SCOPE_RAII(name) TRACE_SCOPE_RAII_ID(name, SCALOPUS_TRACKED_TRACE_ID_CREATOR())

// Macro to create a traced RAII tracepoint using __PRETTY_FUNCTION__ as name.
#define TRACE_PRETTY_FUNCTION() TRACE_SCOPE_RAII_ID(__PRETTY_FUNCTION__, SCALOPUS_TRACKED_TRACE_ID_CREATOR())

// Macro to explicitly emit a start scope, needs to be paired with TRACE_SCOPE_END(name) with the same name.
// #define TRACE_SCOPE_START(name) TRACE_SCOPE_START_NAMED_ID(name, SCALOPUS_TRACKED_TRACE_ID_STRING(name))
#ifdef TRACE_PROFILE
  #define TRACE_SCOPE_START(name, wait_fun) \
  wait_fun; \
  TRACE_SCOPE_START_NAMED_ID(name, SCALOPUS_TRACKED_TRACE_ID_STRING(name))
#else
  #define TRACE_SCOPE_START(name, wait_fun) ;
#endif

// Macro to explicitly emit a start scope, needs to be paired with TRACE_SCOPE_START(name) with the same name.
// #define TRACE_SCOPE_END(name) TRACE_SCOPE_END_NAMED_ID(name, SCALOPUS_TRACKED_TRACE_ID_STRING(name))
#ifdef TRACE_PROFILE
  #define TRACE_SCOPE_END(name, wait_fun) \
  TRACE_SCOPE_END_NAMED_ID(name, SCALOPUS_TRACKED_TRACE_ID_STRING(name)); \
  wait_fun;
#else
  #define TRACE_SCOPE_END(name, wait_fun) ;
#endif

// The parameter name can be either a constant or a variable like string.c_str().
// But this will a little slower than TRACE_SCOPE_START because:
// 1. the function get id by name is no longer a constexpr
// 2. change the judgment methods whether the map contains name and id from static varable to map's method exit.
// #define RUNTIME_TRACE_SCOPE_START(name) RUNTIME_TRACE_SCOPE_START_NAMED_ID(name, RUNTIME_SCALOPUS_TRACKED_TRACE_ID_STRING(name))
// #define RUNTIME_TRACE_SCOPE_END(name) RUNTIME_TRACE_SCOPE_END_NAMED_ID(name, RUNTIME_SCALOPUS_TRACKED_TRACE_ID_STRING(name))
#ifdef TRACE_PROFILE
  #define RUNTIME_TRACE_SCOPE_START(name, wait_fun) \
  wait_fun; \
  RUNTIME_TRACE_SCOPE_START_NAMED_ID(name, RUNTIME_SCALOPUS_TRACKED_TRACE_ID_STRING(name))
#else
  #define RUNTIME_TRACE_SCOPE_START(name, wait_fun)
#endif

#ifdef TRACE_PROFILE
  #define RUNTIME_TRACE_SCOPE_END(name, wait_fun) \
  RUNTIME_TRACE_SCOPE_END_NAMED_ID(name, RUNTIME_SCALOPUS_TRACKED_TRACE_ID_STRING(name)); \
  wait_fun;
#else
  #define RUNTIME_TRACE_SCOPE_END(name, wait_fun)
#endif

// Macro to set the configuration of this thread's traces for this and any lower scopes; reverts to previous value.
#define TRACING_CONFIG_THREAD_STATE_RAII(boolean) TRACING_CONFIG_THREAD_PROCESS_STATE_RAII(false, boolean)

// Macro to set the configuration of this process' traces for this and any lower scopes; reverts to previous value.
#define TRACING_CONFIG_PROCESS_STATE_RAII(boolean) TRACING_CONFIG_THREAD_PROCESS_STATE_RAII(true, boolean)

// Macro to set global marker event.
#define TRACE_MARK_EVENT_GLOBAL(name) TRACE_MARK_EVENT_NAMED_ID(GLOBAL, name, SCALOPUS_TRACKED_TRACE_ID_STRING(name))
#define TRACE_MARK_EVENT_PROCESS(name) TRACE_MARK_EVENT_NAMED_ID(PROCESS, name, SCALOPUS_TRACKED_TRACE_ID_STRING(name))
#define TRACE_MARK_EVENT_THREAD(name) TRACE_MARK_EVENT_NAMED_ID(THREAD, name, SCALOPUS_TRACKED_TRACE_ID_STRING(name))

// Macro to set a counter series value.
#define TRACE_COUNT_SERIES(name, series_name, value) TRACE_COUNT_SERIES_EVENT_NAMED(name "/" series_name, value)

// Macro to set a counter value, just one series called 'count'.
#define TRACE_COUNT(name, value) TRACE_COUNT_SERIES(name, "", value)
#endif  // SCALOPUS_TRACING_SCOPE_TRACING_H


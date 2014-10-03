// Copyright 2014 Makoto Yano

#ifndef INCLUDE_DEBUG_LOG_H_
#define INCLUDE_DEBUG_LOG_H_

#include <cstdio>

namespace hydron_debugger {

#ifdef DEBUG

#define __FUNCTION_START_DEBUG_LOG__ \
                            hydron_debugger::logger("%s Start\n", __func__)
#define __FUNCTION_END_DEBUG_LOG__ \
                            hydron_debugger::logger("%s End\n", __func__)
#define __DEBUG_LOG__(format, ...) \
                            hydron_debugger::logger(format,  __VA_ARGS__)

void logger(const char* format, ...) {
  char *std;
  va_list arg;
  va_start(arg, format);
  vasprintf(&std, format, arg);
  va_end(arg);
  printf("%s", std);
  free(std);
}

#else

#define __FUNCTION_START_DEBUG_LOG__
#define __FUNCTION_END_DEBUG_LOG__
#define __DEBUG_LOG(format, ...)__

#endif  // DEBUG

}  // namespace hydron_debugger

#endif  // INCLUDE_DEBUG_LOG_H_

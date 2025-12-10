// linux_compat.cpp - Implementation of Linux compatibility layer
#include "stdafx.h"

#ifdef LINUX_PORT

#include <cstdarg>
#include <cstdio>

void CString::Format(const char *fmt, ...) {
  char buffer[4096];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);
  assign(buffer);
}

#endif // LINUX_PORT

// This file is used to not include os specific functions that might break other projects
// You should use it in sources
#pragma once

#ifdef PLATFORM_POSIX
#    include <cxxabi.h>
#    include <fcntl.h>
#    include <link.h>
#    include <sys/mman.h>
#    include <sys/stat.h>
#    include <thread>
#    include <unistd.h>
#elifdef  PLATFORM_WINDOWS
#    include <windows.h>
#    include <winternl.h>
#endif

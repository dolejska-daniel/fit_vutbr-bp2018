
include_directories(${ROOT_DIR}/include)

set(CMAKE_DEBUG_POSTFIX                 "d" CACHE STRING "add a postfix, usually d on windows")
set(CMAKE_RELEASE_POSTFIX               "" CACHE STRING "add a postfix, usually empty on windows")
set(CMAKE_RELEASEWITHDEBINFO_POSTFIX    "rd" CACHE STRING "add a postfix, usually empty on windows")
set(CMAKE_MINSIZEREL_POSTFIX            "s" CACHE STRING "add a postfix, usually empty on windows")

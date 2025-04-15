cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Limbogger")

include_directories("include")

add_library(Limbogger SHARED src/Limbogger.cpp)
target_compile_definitions(Limbogger PRIVATE LIMBOGGER_EXPORTS)

add_executable (LimboggerTest samples/main.cpp)
target_link_libraries(LimboggerTest Limbogger)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Limbogger PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

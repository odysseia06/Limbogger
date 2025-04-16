cmake_minimum_required (VERSION 3.10)

project (Limbogger
    VERSION 0.1.0
    DESCRIPTION "A Lightweight cross-platform logging library"
)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

find_package(Threads REQUIRED)

add_library(Limbogger SHARED src/Limbogger.cpp)

target_include_directories(Limbogger
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_definitions(Limbogger PRIVATE LIMBOGGER_EXPORTS)

if (NOT WIN32)
    target_compile_options(Limbogger PRIVATE -fvisibility=hidden)
    set_target_properties(Limbogger PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()
target_link_libraries(Limbogger PRIVATE Threads::Threads)

add_executable (LimboggerTest samples/main.cpp)
target_link_libraries(LimboggerTest PRIVATE Limbogger Threads::Threads)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Limbogger PROPERTY CXX_STANDARD 20)
endif()

include(GNUInstallDirs)

install(TARGETS Limbogger LimboggerTest
    EXPORT LimboggerTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT LimboggerTargets
    FILE LimboggerTargets.cmake 
    NAMESPACE Limbogger::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Limbogger)

option(LIMBOGGER_ENABLE_CLANGD "Export compile_commands.json for clangd" ON)
if(LIMBOGGER_ENABLE_CLANGD)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()


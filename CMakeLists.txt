cmake_minimum_required(VERSION 3.0.0)
project(
    umgmt
    VERSION 0.1.0
    LANGUAGES C
)

include(GNUInstallDirs)
include(CompileOptions.cmake)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/deps/uthash/include
)

set(
    UMGMT_SOURCES

    "src/umgmt/user.c"
    "src/umgmt/group.c"
    "src/umgmt/db.c"
    "src/umgmt/password.c"
	"src/umgmt/dyn_buffer.c"
)

add_library(
    ${CMAKE_PROJECT_NAME}
    SHARED
    ${UMGMT_SOURCES}
)

install(
    TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# project version
set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

install(
    FILES ${PROJECT_SOURCE_DIR}/src/umgmt.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    FILES
    ${PROJECT_SOURCE_DIR}/src/umgmt/types.h
    ${PROJECT_SOURCE_DIR}/src/umgmt/db.h
    ${PROJECT_SOURCE_DIR}/src/umgmt/user.h
    ${PROJECT_SOURCE_DIR}/src/umgmt/group.h

    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/umgmt
)

add_custom_target(
    uninstall

    COMMAND xargs rm < ${PROJECT_SOURCE_DIR}/build/install_manifest.txt
)

if(ENABLE_TESTS)
    find_package(CMOCKA REQUIRED)
    include(CTest)
    include(test/Tests.cmake)
endif()

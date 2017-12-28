cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
SET(VERSION_PATCH "$ENV{TRAVIS_BUILD_NUMBER}")
else()
SET(VERSION_PATCH "999")
endif()

project(ip_filter VERSION 1.0.${VERSION_PATCH})

#configure_file(version.h.in version.h)
add_executable(ip_filter ip_filter.cpp)
#add_executable(test_version test.cpp)
#add_library(versions lib.cpp)

set_target_properties(ip_filter PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
#    INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR}
)

#target_link_libraries(build_test versions)
#target_link_libraries(test_version versions)

INSTALL(TARGETS ip_filter RUNTIME DESTINATION bin)

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Ilia Sobolev")
SET(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
SET(CPACK_PACKAGE_VERSION_TWEAK "${PROJECT_VERSION_TWEAK}")

INCLUDE(CPack)

#enable_testing()
#add_test(simple_check test_version)

add_library(tw_base INTERFACE)
target_sources(tw_base INTERFACE Data.cpp)
target_include_directories(tw_base INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(tw_base PROPERTIES PUBLIC_HEADER Data.h)
set_property(SOURCE Data.cpp PROPERTY SKIP_UNITY_BUILD_INCLUSION ON)

add_library(tw::base ALIAS tw_base)

if (TW_UNIT_TESTS)
    add_library(tw_base_tests INTERFACE)
    target_sources(tw_base_tests INTERFACE tests/DataTests.cpp)
    add_library(tw::base_tests ALIAS tw_base_tests)
endif()

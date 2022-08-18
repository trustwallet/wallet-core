add_library(tw_base OBJECT)
target_sources(tw_base PRIVATE Data.cpp)
target_include_directories(tw_base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(tw_base PROPERTIES PUBLIC_HEADER Data.h)

add_library(TW::Base ALIAS tw_base)

if (TW_UNIT_TESTS)
    add_library(tw_base_tests INTERFACE)
    target_sources(tw_base_tests INTERFACE tests/DataTests.cpp)
    target_link_libraries(tw_base_tests INTERFACE TW::Base)
    add_library(TW::BaseTests ALIAS tw_base_tests)
endif()

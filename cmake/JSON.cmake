set(json_PREFIX ${PROJECT_SOURCE_DIR}/lib/json)
file(MAKE_DIRECTORY ${json_PREFIX}/include/nlohmann)

ExternalProject_Add(
    nlohmann_json

    PREFIX "nlohmann"
    INSTALL_DIR ${json_PREFIX}

    URL https://github.com/nlohmann/json/releases/download/v3.5.0/include.zip
    URL_HASH SHA256=3564da9c5b0cf2e032f97c69baedf10ddbc98030c337d0327a215ea72259ea21

    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/nlohmann <INSTALL_DIR>/include/nlohmann
)
ExternalProject_Get_Property(nlohmann_json source_dir)

set(json_SOURCE_DIR ${source_dir})
set(json_INCLUDE_DIRS ${json_SOURCE_DIR})

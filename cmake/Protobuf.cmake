ExternalProject_Add(
    protobuf_ext

    URL https://github.com/protocolbuffers/protobuf/releases/download/v3.6.1/protobuf-cpp-3.6.1.tar.gz
    URL_HASH SHA256=b3732e471a9bb7950f090fd0457ebd2536a9ba0891b7f3785919c654fe2a2529

    UPDATE_COMMAND ""
    PATCH_COMMAND ""

    CONFIGURE_COMMAND "<SOURCE_DIR>/configure" --prefix=<INSTALL_DIR>

    BUILD_COMMAND make
    INSTALL_COMMAND make install
)
ExternalProject_Get_Property(protobuf_ext source_dir)
ExternalProject_Get_Property(protobuf_ext install_dir)

set(protobuf_INCLUDE_DIRS ${install_dir}/include)

add_library(protobuf STATIC IMPORTED)
set_target_properties(protobuf
    PROPERTIES
    IMPORTED_LOCATION ${install_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}protobuf${CMAKE_STATIC_LIBRARY_SUFFIX}
)
add_dependencies(protobuf protobuf_ext)

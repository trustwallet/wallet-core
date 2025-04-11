# Install script for directory: /Users/enriquesouza/projects/enriquesouza/wallet-core

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/enriquesouza/projects/enriquesouza/wallet-core/trezor-crypto/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/enriquesouza/projects/enriquesouza/wallet-core/libprotobuf.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf.a")
    execute_process(COMMAND "/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf.a")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/protobuf" TYPE FILE FILES
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/any.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/any.pb.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/api.pb.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/arena.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/arena_impl.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/arenastring.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/compiler/importer.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/compiler/parser.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/descriptor.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/descriptor.pb.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/descriptor_database.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/duration.pb.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/dynamic_message.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/empty.pb.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/explicitly_constructed.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/extension_set.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/extension_set_inl.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/field_access_listener.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/field_mask.pb.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/generated_enum_reflection.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/generated_enum_util.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/generated_message_bases.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/generated_message_reflection.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/generated_message_table_driven.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/generated_message_table_driven_lite.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/generated_message_tctable_decl.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/generated_message_tctable_impl.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/generated_message_tctable_impl.inc"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/generated_message_util.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/has_bits.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/implicit_weak_message.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/inlined_string_field.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/io/coded_stream.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/io/gzip_stream.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/io/io_win32.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/io/printer.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/io/strtod.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/io/tokenizer.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/io/zero_copy_stream.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/io/zero_copy_stream_impl.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/io/zero_copy_stream_impl_lite.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/map.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/map_entry.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/map_entry_lite.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/map_field.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/map_field_inl.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/map_field_lite.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/map_type_handler.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/message.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/message_lite.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/metadata.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/metadata_lite.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/parse_context.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/port.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/reflection.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/reflection_ops.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/repeated_field.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/repeated_ptr_field.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/service.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/source_context.pb.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/struct.pb.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/stubs/bytestream.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/stubs/callback.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/stubs/casts.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/stubs/common.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/stubs/hash.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/stubs/logging.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/stubs/macros.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/stubs/map_util.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/stubs/mutex.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/stubs/once.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/stubs/platform_macros.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/stubs/port.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/stubs/status.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/stubs/stl_util.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/stubs/stringpiece.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/stubs/strutil.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/stubs/template_util.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/text_format.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/timestamp.pb.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/type.pb.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/unknown_field_set.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/util/delimited_message_util.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/util/field_comparator.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/util/field_mask_util.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/util/json_util.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/util/message_differencer.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/util/time_util.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/util/type_resolver.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/util/type_resolver_util.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/wire_format.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/wire_format_lite.h"
    "/Users/enriquesouza/projects/enriquesouza/wallet-core/cmake/../build/local/src/protobuf/protobuf-3.19.2/src/google/protobuf/wrappers.pb.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/enriquesouza/projects/enriquesouza/wallet-core/tests/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/enriquesouza/projects/enriquesouza/wallet-core/walletconsole/lib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/enriquesouza/projects/enriquesouza/wallet-core/walletconsole/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/enriquesouza/projects/enriquesouza/wallet-core/libTrustWalletCore.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTrustWalletCore.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTrustWalletCore.a")
    execute_process(COMMAND "/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTrustWalletCore.a")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WalletCore" TYPE DIRECTORY FILES "/Users/enriquesouza/projects/enriquesouza/wallet-core/src/" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/Users/enriquesouza/projects/enriquesouza/wallet-core/include/")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/enriquesouza/projects/enriquesouza/wallet-core/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
if(CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_COMPONENT MATCHES "^[a-zA-Z0-9_.+-]+$")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
  else()
    string(MD5 CMAKE_INST_COMP_HASH "${CMAKE_INSTALL_COMPONENT}")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INST_COMP_HASH}.txt")
    unset(CMAKE_INST_COMP_HASH)
  endif()
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/enriquesouza/projects/enriquesouza/wallet-core/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()

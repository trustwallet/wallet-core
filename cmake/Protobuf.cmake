# SPDX-License-Identifier: Apache-2.0
#
# Copyright © 2017 Trust Wallet.

set(protobuf_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../build/local/src/protobuf/protobuf-3.20.3)
set(protobuf_source_dir ${CMAKE_CURRENT_LIST_DIR}/../build/local/src/protobuf/protobuf-3.20.3)

# sort + uniq -u
# https://github.com/protocolbuffers/protobuf/blob/master/cmake/libprotobuf.cmake
# https://github.com/protocolbuffers/protobuf/blob/master/cmake/libprotobuf-lite.cmake

set(protobuf_SOURCE_FILES
  ${protobuf_source_dir}/src/google/protobuf/any.cc
  ${protobuf_source_dir}/src/google/protobuf/any.pb.cc
  ${protobuf_source_dir}/src/google/protobuf/any_lite.cc
  ${protobuf_source_dir}/src/google/protobuf/api.pb.cc
  ${protobuf_source_dir}/src/google/protobuf/arena.cc
  ${protobuf_source_dir}/src/google/protobuf/arenastring.cc
  ${protobuf_source_dir}/src/google/protobuf/arenaz_sampler.cc
  ${protobuf_source_dir}/src/google/protobuf/compiler/importer.cc
  ${protobuf_source_dir}/src/google/protobuf/compiler/parser.cc
  ${protobuf_source_dir}/src/google/protobuf/descriptor.cc
  ${protobuf_source_dir}/src/google/protobuf/descriptor.pb.cc
  ${protobuf_source_dir}/src/google/protobuf/descriptor_database.cc
  ${protobuf_source_dir}/src/google/protobuf/duration.pb.cc
  ${protobuf_source_dir}/src/google/protobuf/dynamic_message.cc
  ${protobuf_source_dir}/src/google/protobuf/empty.pb.cc
  ${protobuf_source_dir}/src/google/protobuf/extension_set.cc
  ${protobuf_source_dir}/src/google/protobuf/extension_set_heavy.cc
  ${protobuf_source_dir}/src/google/protobuf/field_mask.pb.cc
  ${protobuf_source_dir}/src/google/protobuf/generated_enum_util.cc
  ${protobuf_source_dir}/src/google/protobuf/generated_message_bases.cc
  ${protobuf_source_dir}/src/google/protobuf/generated_message_reflection.cc
  ${protobuf_source_dir}/src/google/protobuf/generated_message_tctable_full.cc
  ${protobuf_source_dir}/src/google/protobuf/generated_message_tctable_lite.cc
  ${protobuf_source_dir}/src/google/protobuf/generated_message_util.cc
  ${protobuf_source_dir}/src/google/protobuf/implicit_weak_message.cc
  ${protobuf_source_dir}/src/google/protobuf/inlined_string_field.cc
  ${protobuf_source_dir}/src/google/protobuf/io/coded_stream.cc
  ${protobuf_source_dir}/src/google/protobuf/io/gzip_stream.cc
  ${protobuf_source_dir}/src/google/protobuf/io/io_win32.cc
  ${protobuf_source_dir}/src/google/protobuf/io/printer.cc
  ${protobuf_source_dir}/src/google/protobuf/io/strtod.cc
  ${protobuf_source_dir}/src/google/protobuf/io/tokenizer.cc
  ${protobuf_source_dir}/src/google/protobuf/io/zero_copy_stream.cc
  ${protobuf_source_dir}/src/google/protobuf/io/zero_copy_stream_impl.cc
  ${protobuf_source_dir}/src/google/protobuf/io/zero_copy_stream_impl_lite.cc
  ${protobuf_source_dir}/src/google/protobuf/map.cc
  ${protobuf_source_dir}/src/google/protobuf/map_field.cc
  ${protobuf_source_dir}/src/google/protobuf/message.cc
  ${protobuf_source_dir}/src/google/protobuf/message_lite.cc
  ${protobuf_source_dir}/src/google/protobuf/parse_context.cc
  ${protobuf_source_dir}/src/google/protobuf/reflection_internal.h
  ${protobuf_source_dir}/src/google/protobuf/reflection_ops.cc
  ${protobuf_source_dir}/src/google/protobuf/repeated_field.cc
  ${protobuf_source_dir}/src/google/protobuf/repeated_ptr_field.cc
  ${protobuf_source_dir}/src/google/protobuf/service.cc
  ${protobuf_source_dir}/src/google/protobuf/source_context.pb.cc
  ${protobuf_source_dir}/src/google/protobuf/struct.pb.cc
  ${protobuf_source_dir}/src/google/protobuf/stubs/bytestream.cc
  ${protobuf_source_dir}/src/google/protobuf/stubs/common.cc
  ${protobuf_source_dir}/src/google/protobuf/stubs/int128.cc
  ${protobuf_source_dir}/src/google/protobuf/stubs/status.cc
  ${protobuf_source_dir}/src/google/protobuf/stubs/statusor.cc
  ${protobuf_source_dir}/src/google/protobuf/stubs/stringpiece.cc
  ${protobuf_source_dir}/src/google/protobuf/stubs/stringprintf.cc
  ${protobuf_source_dir}/src/google/protobuf/stubs/structurally_valid.cc
  ${protobuf_source_dir}/src/google/protobuf/stubs/strutil.cc
  ${protobuf_source_dir}/src/google/protobuf/stubs/substitute.cc
  ${protobuf_source_dir}/src/google/protobuf/stubs/time.cc
  ${protobuf_source_dir}/src/google/protobuf/text_format.cc
  ${protobuf_source_dir}/src/google/protobuf/timestamp.pb.cc
  ${protobuf_source_dir}/src/google/protobuf/type.pb.cc
  ${protobuf_source_dir}/src/google/protobuf/unknown_field_set.cc
  ${protobuf_source_dir}/src/google/protobuf/util/delimited_message_util.cc
  ${protobuf_source_dir}/src/google/protobuf/util/field_comparator.cc
  ${protobuf_source_dir}/src/google/protobuf/util/field_mask_util.cc
  ${protobuf_source_dir}/src/google/protobuf/util/internal/datapiece.cc
  ${protobuf_source_dir}/src/google/protobuf/util/internal/default_value_objectwriter.cc
  ${protobuf_source_dir}/src/google/protobuf/util/internal/error_listener.cc
  ${protobuf_source_dir}/src/google/protobuf/util/internal/field_mask_utility.cc
  ${protobuf_source_dir}/src/google/protobuf/util/internal/json_escaping.cc
  ${protobuf_source_dir}/src/google/protobuf/util/internal/json_objectwriter.cc
  ${protobuf_source_dir}/src/google/protobuf/util/internal/json_stream_parser.cc
  ${protobuf_source_dir}/src/google/protobuf/util/internal/object_writer.cc
  ${protobuf_source_dir}/src/google/protobuf/util/internal/proto_writer.cc
  ${protobuf_source_dir}/src/google/protobuf/util/internal/protostream_objectsource.cc
  ${protobuf_source_dir}/src/google/protobuf/util/internal/protostream_objectwriter.cc
  ${protobuf_source_dir}/src/google/protobuf/util/internal/type_info.cc
  ${protobuf_source_dir}/src/google/protobuf/util/internal/utility.cc
  ${protobuf_source_dir}/src/google/protobuf/util/json_util.cc
  ${protobuf_source_dir}/src/google/protobuf/util/message_differencer.cc
  ${protobuf_source_dir}/src/google/protobuf/util/time_util.cc
  ${protobuf_source_dir}/src/google/protobuf/util/type_resolver_util.cc
  ${protobuf_source_dir}/src/google/protobuf/wire_format.cc
  ${protobuf_source_dir}/src/google/protobuf/wire_format_lite.cc
  ${protobuf_source_dir}/src/google/protobuf/wrappers.pb.cc
)

set(protobuf_HEADER_FILES
  ${protobuf_source_dir}/src/google/protobuf/any.h
  ${protobuf_source_dir}/src/google/protobuf/any.pb.h
  ${protobuf_source_dir}/src/google/protobuf/api.pb.h
  ${protobuf_source_dir}/src/google/protobuf/arena.h
  ${protobuf_source_dir}/src/google/protobuf/arena_impl.h
  ${protobuf_source_dir}/src/google/protobuf/arenastring.h
  ${protobuf_source_dir}/src/google/protobuf/arenaz_sampler.h
  ${protobuf_source_dir}/src/google/protobuf/compiler/importer.h
  ${protobuf_source_dir}/src/google/protobuf/compiler/parser.h
  ${protobuf_source_dir}/src/google/protobuf/descriptor.h
  ${protobuf_source_dir}/src/google/protobuf/descriptor.pb.h
  ${protobuf_source_dir}/src/google/protobuf/descriptor_database.h
  ${protobuf_source_dir}/src/google/protobuf/duration.pb.h
  ${protobuf_source_dir}/src/google/protobuf/dynamic_message.h
  ${protobuf_source_dir}/src/google/protobuf/empty.pb.h
  ${protobuf_source_dir}/src/google/protobuf/explicitly_constructed.h
  ${protobuf_source_dir}/src/google/protobuf/extension_set.h
  ${protobuf_source_dir}/src/google/protobuf/extension_set_inl.h
  ${protobuf_source_dir}/src/google/protobuf/field_access_listener.h
  ${protobuf_source_dir}/src/google/protobuf/field_mask.pb.h
  ${protobuf_source_dir}/src/google/protobuf/generated_enum_reflection.h
  ${protobuf_source_dir}/src/google/protobuf/generated_enum_util.h
  ${protobuf_source_dir}/src/google/protobuf/generated_message_bases.h
  ${protobuf_source_dir}/src/google/protobuf/generated_message_reflection.h
  ${protobuf_source_dir}/src/google/protobuf/generated_message_tctable_decl.h
  ${protobuf_source_dir}/src/google/protobuf/generated_message_tctable_impl.h
  ${protobuf_source_dir}/src/google/protobuf/generated_message_util.h
  ${protobuf_source_dir}/src/google/protobuf/has_bits.h
  ${protobuf_source_dir}/src/google/protobuf/implicit_weak_message.h
  ${protobuf_source_dir}/src/google/protobuf/inlined_string_field.h
  ${protobuf_source_dir}/src/google/protobuf/io/coded_stream.h
  ${protobuf_source_dir}/src/google/protobuf/io/gzip_stream.h
  ${protobuf_source_dir}/src/google/protobuf/io/io_win32.h
  ${protobuf_source_dir}/src/google/protobuf/io/printer.h
  ${protobuf_source_dir}/src/google/protobuf/io/strtod.h
  ${protobuf_source_dir}/src/google/protobuf/io/tokenizer.h
  ${protobuf_source_dir}/src/google/protobuf/io/zero_copy_stream.h
  ${protobuf_source_dir}/src/google/protobuf/io/zero_copy_stream_impl.h
  ${protobuf_source_dir}/src/google/protobuf/io/zero_copy_stream_impl_lite.h
  ${protobuf_source_dir}/src/google/protobuf/map.h
  ${protobuf_source_dir}/src/google/protobuf/map_entry.h
  ${protobuf_source_dir}/src/google/protobuf/map_entry_lite.h
  ${protobuf_source_dir}/src/google/protobuf/map_field.h
  ${protobuf_source_dir}/src/google/protobuf/map_field_inl.h
  ${protobuf_source_dir}/src/google/protobuf/map_field_lite.h
  ${protobuf_source_dir}/src/google/protobuf/map_type_handler.h
  ${protobuf_source_dir}/src/google/protobuf/message.h
  ${protobuf_source_dir}/src/google/protobuf/message_lite.h
  ${protobuf_source_dir}/src/google/protobuf/metadata.h
  ${protobuf_source_dir}/src/google/protobuf/metadata_lite.h
  ${protobuf_source_dir}/src/google/protobuf/parse_context.h
  ${protobuf_source_dir}/src/google/protobuf/port.h
  ${protobuf_source_dir}/src/google/protobuf/reflection.h
  ${protobuf_source_dir}/src/google/protobuf/reflection_ops.h
  ${protobuf_source_dir}/src/google/protobuf/repeated_field.h
  ${protobuf_source_dir}/src/google/protobuf/repeated_ptr_field.h
  ${protobuf_source_dir}/src/google/protobuf/service.h
  ${protobuf_source_dir}/src/google/protobuf/source_context.pb.h
  ${protobuf_source_dir}/src/google/protobuf/struct.pb.h
  ${protobuf_source_dir}/src/google/protobuf/stubs/bytestream.h
  ${protobuf_source_dir}/src/google/protobuf/stubs/callback.h
  ${protobuf_source_dir}/src/google/protobuf/stubs/casts.h
  ${protobuf_source_dir}/src/google/protobuf/stubs/common.h
  ${protobuf_source_dir}/src/google/protobuf/stubs/hash.h
  ${protobuf_source_dir}/src/google/protobuf/stubs/logging.h
  ${protobuf_source_dir}/src/google/protobuf/stubs/macros.h
  ${protobuf_source_dir}/src/google/protobuf/stubs/map_util.h
  ${protobuf_source_dir}/src/google/protobuf/stubs/mutex.h
  ${protobuf_source_dir}/src/google/protobuf/stubs/once.h
  ${protobuf_source_dir}/src/google/protobuf/stubs/platform_macros.h
  ${protobuf_source_dir}/src/google/protobuf/stubs/port.h
  ${protobuf_source_dir}/src/google/protobuf/stubs/status.h
  ${protobuf_source_dir}/src/google/protobuf/stubs/stl_util.h
  ${protobuf_source_dir}/src/google/protobuf/stubs/stringpiece.h
  ${protobuf_source_dir}/src/google/protobuf/stubs/strutil.h
  ${protobuf_source_dir}/src/google/protobuf/stubs/template_util.h
  ${protobuf_source_dir}/src/google/protobuf/text_format.h
  ${protobuf_source_dir}/src/google/protobuf/timestamp.pb.h
  ${protobuf_source_dir}/src/google/protobuf/type.pb.h
  ${protobuf_source_dir}/src/google/protobuf/unknown_field_set.h
  ${protobuf_source_dir}/src/google/protobuf/util/delimited_message_util.h
  ${protobuf_source_dir}/src/google/protobuf/util/field_comparator.h
  ${protobuf_source_dir}/src/google/protobuf/util/field_mask_util.h
  ${protobuf_source_dir}/src/google/protobuf/util/json_util.h
  ${protobuf_source_dir}/src/google/protobuf/util/message_differencer.h
  ${protobuf_source_dir}/src/google/protobuf/util/time_util.h
  ${protobuf_source_dir}/src/google/protobuf/util/type_resolver.h
  ${protobuf_source_dir}/src/google/protobuf/util/type_resolver_util.h
  ${protobuf_source_dir}/src/google/protobuf/wire_format.h
  ${protobuf_source_dir}/src/google/protobuf/wire_format_lite.h
  ${protobuf_source_dir}/src/google/protobuf/wrappers.pb.h
)

#file(GLOB_RECURSE protobuf_HEADER_FILES ${protobuf_SOURCE_DIR}/src/**/*.h)
include_directories(${protobuf_source_dir}/src)

add_library(protobuf ${protobuf_SOURCE_FILES} ${protobuf_HEADER_FILES})
set_target_properties(
    protobuf
    PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    IMPORTED_CONFIGURATIONS Release
    INCLUDE_DIRECTORIES ${protobuf_source_dir}/src
    PUBLIC_HEADER "${protobuf_HEADER_FILES}"
    LINK_FLAGS -no-undefined
)

target_compile_options(protobuf PRIVATE -DHAVE_PTHREAD=1 -Wno-inconsistent-missing-override -Wno-shorten-64-to-32 -Wno-invalid-noreturn)

install(TARGETS protobuf
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/protobuf
)

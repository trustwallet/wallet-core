version = '0.4.0'

Pod::Spec.new do |s|
  s.name         = 'TrustWalletCore'
  s.version      = version
  s.summary      = 'Trust Wallet core data structures and algorithms.'
  s.homepage     = 'https://github.com/TrustWallet/wallet-core'
  s.license      = 'MIT'
  s.authors      = { 'Alejandro Isaza' => 'al@isaza.ca' }

  s.ios.deployment_target = '10.0'
  s.osx.deployment_target = '10.12'
  s.swift_version = '4.2'

  s.source = {
    git: 'git@github.com:TrustWallet/wallet-core.git',
    tag: version,
    submodules: true
  }

  s.source_files =
    'src/**/*.{c,cc,cpp,h}',
    'include/**/*.h',
    'swift/Sources/**/*.{swift,h,m,cpp}',
    'trezor-crypto/src/**/*.{c,h}',
    'trezor-crypto/include/**/*.{h}',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/stubs/bytestream.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/stubs/callback.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/stubs/common.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/stubs/int128.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/stubs/status.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/stubs/statusor.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/stubs/stringpiece.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/stubs/stringprintf.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/stubs/structurally_valid.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/stubs/strutil.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/stubs/time.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/arena.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/extension_set.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/generated_message_util.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/generated_message_table_driven_lite.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/implicit_weak_message.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/message_lite.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/repeated_field.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/wire_format_lite.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/io/coded_stream.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/io/strtod.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/io/zero_copy_stream.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/io/zero_copy_stream_impl_lite.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/any.pb.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/api.pb.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/stubs/mathlimits.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/stubs/mathlimits.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/any.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/descriptor.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/descriptor_database.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/descriptor.pb.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/duration.pb.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/dynamic_message.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/empty.pb.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/extension_set_heavy.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/field_mask.pb.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/generated_message_reflection.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/generated_message_table_driven_lite.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/generated_message_table_driven.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/map_field.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/message.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/reflection_internal.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/reflection_ops.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/service.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/source_context.pb.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/struct.pb.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/stubs/substitute.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/stubs/substitute.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/text_format.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/timestamp.pb.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/type.pb.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/unknown_field_set.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/wire_format.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/wrappers.pb.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/io/gzip_stream.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/io/printer.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/io/strtod.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/io/tokenizer.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/io/zero_copy_stream_impl.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/compiler/importer.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/compiler/parser.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/delimited_message_util.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/field_comparator.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/field_mask_util.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/constants.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/datapiece.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/datapiece.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/default_value_objectwriter.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/default_value_objectwriter.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/error_listener.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/error_listener.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/expecting_objectwriter.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/field_mask_utility.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/field_mask_utility.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/json_escaping.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/json_escaping.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/json_objectwriter.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/json_objectwriter.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/json_stream_parser.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/json_stream_parser.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/location_tracker.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/mock_error_listener.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/object_location_tracker.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/object_source.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/object_writer.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/object_writer.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/protostream_objectsource.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/protostream_objectsource.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/protostream_objectwriter.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/protostream_objectwriter.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/proto_writer.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/proto_writer.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/structured_objectwriter.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/type_info.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/type_info.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/type_info_test_helper.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/type_info_test_helper.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/utility.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/internal/utility.h',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/json_util.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/message_differencer.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/time_util.cc',
    'build/protobuf/src/protobuf_ext/src/google/protobuf/util/type_resolver_util.cc'
  s.exclude_files =
    'trezor-crypto/src/rand.c'
  s.public_header_files =
    'include/**/*.h',
    'swift/Sources/*.h'

  s.preserve_paths =
    'trezor-crypto/src/*.{table}',
    'lib/protobuf/include/**/*.h',
    'lib/json/include/**/*.hpp',
    'src/proto/*.proto'
  s.xcconfig = {
    'HEADER_SEARCH_PATHS' => '$(inherited) ' \
      '${PODS_ROOT}/TrustWalletCore/src ' \
      '${PODS_ROOT}/TrustWalletCore/trezor-crypto/src',
    'SYSTEM_HEADER_SEARCH_PATHS' => '$(inherited) ' \
      '/usr/local/include' \
      '${PODS_ROOT}/TrustWalletCore/include ' \
      '${PODS_ROOT}/TrustWalletCore/trezor-crypto/include ' \
      '${PODS_ROOT}/TrustWalletCore/lib/protobuf/src ' \
      '${PODS_ROOT}/TrustWalletCore/build/nlohmann/src/nlohmann_json',
    'GCC_WARN_UNUSED_FUNCTION' => 'NO',
    'GCC_WARN_64_TO_32_BIT_CONVERSION' => 'NO',
    'CLANG_CXX_LANGUAGE_STANDARD' => 'c++17',
    'OTHER_CFLAGS' => '-DHAVE_PTHREAD=1'
  }
  s.pod_target_xcconfig = {
    'SYSTEM_HEADER_SEARCH_PATHS' => '$(inherited) /usr/local/include'
  }
  s.prepare_command = 'tools/generate-files && cmake -H. -Bbuild && make -Cbuild'

  s.dependency 'SwiftProtobuf', '~> 1.3.0'
end

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
    'lib/protobuf/src/google/protobuf/stubs/bytestream.cc',
    'lib/protobuf/src/google/protobuf/stubs/callback.cc',
    'lib/protobuf/src/google/protobuf/stubs/common.cc',
    'lib/protobuf/src/google/protobuf/stubs/int128.cc',
    'lib/protobuf/src/google/protobuf/stubs/status.cc',
    'lib/protobuf/src/google/protobuf/stubs/statusor.cc',
    'lib/protobuf/src/google/protobuf/stubs/stringpiece.cc',
    'lib/protobuf/src/google/protobuf/stubs/stringprintf.cc',
    'lib/protobuf/src/google/protobuf/stubs/structurally_valid.cc',
    'lib/protobuf/src/google/protobuf/stubs/strutil.cc',
    'lib/protobuf/src/google/protobuf/stubs/time.cc',
    'lib/protobuf/src/google/protobuf/arena.cc',
    'lib/protobuf/src/google/protobuf/extension_set.cc',
    'lib/protobuf/src/google/protobuf/generated_message_util.cc',
    'lib/protobuf/src/google/protobuf/generated_message_table_driven_lite.cc',
    'lib/protobuf/src/google/protobuf/implicit_weak_message.cc',
    'lib/protobuf/src/google/protobuf/message_lite.cc',
    'lib/protobuf/src/google/protobuf/repeated_field.cc',
    'lib/protobuf/src/google/protobuf/wire_format_lite.cc',
    'lib/protobuf/src/google/protobuf/io/coded_stream.cc',
    'lib/protobuf/src/google/protobuf/io/strtod.cc',
    'lib/protobuf/src/google/protobuf/io/zero_copy_stream.cc',
    'lib/protobuf/src/google/protobuf/io/zero_copy_stream_impl_lite.cc',
    'lib/protobuf/src/google/protobuf/any.pb.cc',
    'lib/protobuf/src/google/protobuf/api.pb.cc',
    'lib/protobuf/src/google/protobuf/stubs/mathlimits.cc',
    'lib/protobuf/src/google/protobuf/stubs/mathlimits.h',
    'lib/protobuf/src/google/protobuf/any.cc',
    'lib/protobuf/src/google/protobuf/descriptor.cc',
    'lib/protobuf/src/google/protobuf/descriptor_database.cc',
    'lib/protobuf/src/google/protobuf/descriptor.pb.cc',
    'lib/protobuf/src/google/protobuf/duration.pb.cc',
    'lib/protobuf/src/google/protobuf/dynamic_message.cc',
    'lib/protobuf/src/google/protobuf/empty.pb.cc',
    'lib/protobuf/src/google/protobuf/extension_set_heavy.cc',
    'lib/protobuf/src/google/protobuf/field_mask.pb.cc',
    'lib/protobuf/src/google/protobuf/generated_message_reflection.cc',
    'lib/protobuf/src/google/protobuf/generated_message_table_driven_lite.h',
    'lib/protobuf/src/google/protobuf/generated_message_table_driven.cc',
    'lib/protobuf/src/google/protobuf/map_field.cc',
    'lib/protobuf/src/google/protobuf/message.cc',
    'lib/protobuf/src/google/protobuf/reflection_internal.h',
    'lib/protobuf/src/google/protobuf/reflection_ops.cc',
    'lib/protobuf/src/google/protobuf/service.cc',
    'lib/protobuf/src/google/protobuf/source_context.pb.cc',
    'lib/protobuf/src/google/protobuf/struct.pb.cc',
    'lib/protobuf/src/google/protobuf/stubs/substitute.cc',
    'lib/protobuf/src/google/protobuf/stubs/substitute.h',
    'lib/protobuf/src/google/protobuf/text_format.cc',
    'lib/protobuf/src/google/protobuf/timestamp.pb.cc',
    'lib/protobuf/src/google/protobuf/type.pb.cc',
    'lib/protobuf/src/google/protobuf/unknown_field_set.cc',
    'lib/protobuf/src/google/protobuf/wire_format.cc',
    'lib/protobuf/src/google/protobuf/wrappers.pb.cc',
    'lib/protobuf/src/google/protobuf/io/gzip_stream.cc',
    'lib/protobuf/src/google/protobuf/io/printer.cc',
    'lib/protobuf/src/google/protobuf/io/strtod.cc',
    'lib/protobuf/src/google/protobuf/io/tokenizer.cc',
    'lib/protobuf/src/google/protobuf/io/zero_copy_stream_impl.cc',
    'lib/protobuf/src/google/protobuf/compiler/importer.cc',
    'lib/protobuf/src/google/protobuf/compiler/parser.cc',
    'lib/protobuf/src/google/protobuf/util/delimited_message_util.cc',
    'lib/protobuf/src/google/protobuf/util/field_comparator.cc',
    'lib/protobuf/src/google/protobuf/util/field_mask_util.cc',
    'lib/protobuf/src/google/protobuf/util/internal/constants.h',
    'lib/protobuf/src/google/protobuf/util/internal/datapiece.cc',
    'lib/protobuf/src/google/protobuf/util/internal/datapiece.h',
    'lib/protobuf/src/google/protobuf/util/internal/default_value_objectwriter.cc',
    'lib/protobuf/src/google/protobuf/util/internal/default_value_objectwriter.h',
    'lib/protobuf/src/google/protobuf/util/internal/error_listener.cc',
    'lib/protobuf/src/google/protobuf/util/internal/error_listener.h',
    'lib/protobuf/src/google/protobuf/util/internal/expecting_objectwriter.h',
    'lib/protobuf/src/google/protobuf/util/internal/field_mask_utility.cc',
    'lib/protobuf/src/google/protobuf/util/internal/field_mask_utility.h',
    'lib/protobuf/src/google/protobuf/util/internal/json_escaping.cc',
    'lib/protobuf/src/google/protobuf/util/internal/json_escaping.h',
    'lib/protobuf/src/google/protobuf/util/internal/json_objectwriter.cc',
    'lib/protobuf/src/google/protobuf/util/internal/json_objectwriter.h',
    'lib/protobuf/src/google/protobuf/util/internal/json_stream_parser.cc',
    'lib/protobuf/src/google/protobuf/util/internal/json_stream_parser.h',
    'lib/protobuf/src/google/protobuf/util/internal/location_tracker.h',
    'lib/protobuf/src/google/protobuf/util/internal/mock_error_listener.h',
    'lib/protobuf/src/google/protobuf/util/internal/object_location_tracker.h',
    'lib/protobuf/src/google/protobuf/util/internal/object_source.h',
    'lib/protobuf/src/google/protobuf/util/internal/object_writer.cc',
    'lib/protobuf/src/google/protobuf/util/internal/object_writer.h',
    'lib/protobuf/src/google/protobuf/util/internal/protostream_objectsource.cc',
    'lib/protobuf/src/google/protobuf/util/internal/protostream_objectsource.h',
    'lib/protobuf/src/google/protobuf/util/internal/protostream_objectwriter.cc',
    'lib/protobuf/src/google/protobuf/util/internal/protostream_objectwriter.h',
    'lib/protobuf/src/google/protobuf/util/internal/proto_writer.cc',
    'lib/protobuf/src/google/protobuf/util/internal/proto_writer.h',
    'lib/protobuf/src/google/protobuf/util/internal/structured_objectwriter.h',
    'lib/protobuf/src/google/protobuf/util/internal/type_info.cc',
    'lib/protobuf/src/google/protobuf/util/internal/type_info.h',
    'lib/protobuf/src/google/protobuf/util/internal/type_info_test_helper.cc',
    'lib/protobuf/src/google/protobuf/util/internal/type_info_test_helper.h',
    'lib/protobuf/src/google/protobuf/util/internal/utility.cc',
    'lib/protobuf/src/google/protobuf/util/internal/utility.h',
    'lib/protobuf/src/google/protobuf/util/json_util.cc',
    'lib/protobuf/src/google/protobuf/util/message_differencer.cc',
    'lib/protobuf/src/google/protobuf/util/time_util.cc',
    'lib/protobuf/src/google/protobuf/util/type_resolver_util.cc'
  s.exclude_files =
    'trezor-crypto/src/gui/*.{c,h}',
    'trezor-crypto/src/sqlite/*.{c,h}',
    'trezor-crypto/src/rand.c'
  s.public_header_files =
    'include/**/*.h',
    'swift/Sources/*.h'

  s.preserve_paths =
    'trezor-crypto/src/*.{table}',
    'lib/protobuf/src/google/protobuf/**/*.h',
    'lib/json/include/**/*.hpp'
  s.xcconfig = {
    'HEADER_SEARCH_PATHS' => '$(inherited) ' \
      '${PODS_ROOT}/TrustWalletCore/src ' \
      '${PODS_ROOT}/TrustWalletCore/trezor-crypto/src',
    'SYSTEM_HEADER_SEARCH_PATHS' => '$(inherited) ' \
      '/usr/local/include' \
      '${PODS_ROOT}/TrustWalletCore/include ' \
      '${PODS_ROOT}/TrustWalletCore/trezor-crypto/include ' \
      '${PODS_ROOT}/TrustWalletCore/lib/protobuf/src ' \
      '${PODS_ROOT}/TrustWalletCore/lib/json/include',
    'GCC_WARN_UNUSED_FUNCTION' => 'NO',
    'GCC_WARN_64_TO_32_BIT_CONVERSION' => 'NO',
    'CLANG_CXX_LANGUAGE_STANDARD' => 'c++17',
    'OTHER_CFLAGS' => '-DHAVE_PTHREAD=1'
  }
  s.pod_target_xcconfig = {
    'SYSTEM_HEADER_SEARCH_PATHS' => '$(inherited) /usr/local/include'
  }
  s.dependency 'SwiftProtobuf', '~> 1.3.0'
end

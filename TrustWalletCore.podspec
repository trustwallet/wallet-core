version = '0.10.1'

Pod::Spec.new do |s|
  s.name         = 'TrustWalletCore'
  s.version      = version
  s.summary      = 'Trust Wallet core data structures and algorithms.'
  s.homepage     = 'https://github.com/TrustWallet/wallet-core'
  s.license      = 'MIT'
  s.authors      = { 'Alejandro Isaza' => 'al@isaza.ca' }

  s.ios.deployment_target = '11.0'
  s.osx.deployment_target = '10.12'
  s.swift_version = '5.0'

  s.source = {
    git: 'git@github.com:TrustWallet/wallet-core.git',
    tag: version,
    submodules: true
  }

  protobuf_dir = 'build/protobuf/staging/protobuf-3.7.0'
  include_dir = 'build/local/include'
  s.source_files =
    'src/**/*.{c,cc,cpp,h}',
    'include/**/*.h',
    'swift/Sources/**/*.{swift,h,m,cpp}',
    'trezor-crypto/src/**/*.{c,h}',
    'trezor-crypto/include/**/*.{h}',
    "#{protobuf_dir}/src/google/protobuf/any.cc",
    "#{protobuf_dir}/src/google/protobuf/any.pb.cc",
    "#{protobuf_dir}/src/google/protobuf/api.pb.cc",
    "#{protobuf_dir}/src/google/protobuf/arena.cc",
    "#{protobuf_dir}/src/google/protobuf/compiler/importer.cc",
    "#{protobuf_dir}/src/google/protobuf/compiler/parser.cc",
    "#{protobuf_dir}/src/google/protobuf/descriptor_database.cc",
    "#{protobuf_dir}/src/google/protobuf/descriptor.cc",
    "#{protobuf_dir}/src/google/protobuf/descriptor.pb.cc",
    "#{protobuf_dir}/src/google/protobuf/duration.pb.cc",
    "#{protobuf_dir}/src/google/protobuf/dynamic_message.cc",
    "#{protobuf_dir}/src/google/protobuf/empty.pb.cc",
    "#{protobuf_dir}/src/google/protobuf/extension_set_heavy.cc",
    "#{protobuf_dir}/src/google/protobuf/extension_set.cc",
    "#{protobuf_dir}/src/google/protobuf/field_mask.pb.cc",
    "#{protobuf_dir}/src/google/protobuf/generated_message_reflection.cc",
    "#{protobuf_dir}/src/google/protobuf/generated_message_table_driven_lite.cc",
    "#{protobuf_dir}/src/google/protobuf/generated_message_table_driven_lite.h",
    "#{protobuf_dir}/src/google/protobuf/generated_message_table_driven.cc",
    "#{protobuf_dir}/src/google/protobuf/generated_message_util.cc",
    "#{protobuf_dir}/src/google/protobuf/implicit_weak_message.cc",
    "#{protobuf_dir}/src/google/protobuf/io/coded_stream.cc",
    "#{protobuf_dir}/src/google/protobuf/io/gzip_stream.cc",
    "#{protobuf_dir}/src/google/protobuf/io/printer.cc",
    "#{protobuf_dir}/src/google/protobuf/io/strtod.cc",
    "#{protobuf_dir}/src/google/protobuf/io/strtod.cc",
    "#{protobuf_dir}/src/google/protobuf/io/tokenizer.cc",
    "#{protobuf_dir}/src/google/protobuf/io/zero_copy_stream_impl_lite.cc",
    "#{protobuf_dir}/src/google/protobuf/io/zero_copy_stream_impl.cc",
    "#{protobuf_dir}/src/google/protobuf/io/zero_copy_stream.cc",
    "#{protobuf_dir}/src/google/protobuf/map_field.cc",
    "#{protobuf_dir}/src/google/protobuf/message_lite.cc",
    "#{protobuf_dir}/src/google/protobuf/message.cc",
    "#{protobuf_dir}/src/google/protobuf/reflection_internal.h",
    "#{protobuf_dir}/src/google/protobuf/reflection_ops.cc",
    "#{protobuf_dir}/src/google/protobuf/repeated_field.cc",
    "#{protobuf_dir}/src/google/protobuf/service.cc",
    "#{protobuf_dir}/src/google/protobuf/source_context.pb.cc",
    "#{protobuf_dir}/src/google/protobuf/struct.pb.cc",
    "#{protobuf_dir}/src/google/protobuf/stubs/bytestream.cc",
    "#{protobuf_dir}/src/google/protobuf/stubs/callback.cc",
    "#{protobuf_dir}/src/google/protobuf/stubs/common.cc",
    "#{protobuf_dir}/src/google/protobuf/stubs/int128.cc",
    "#{protobuf_dir}/src/google/protobuf/stubs/mathlimits.cc",
    "#{protobuf_dir}/src/google/protobuf/stubs/mathlimits.h",
    "#{protobuf_dir}/src/google/protobuf/stubs/status.cc",
    "#{protobuf_dir}/src/google/protobuf/stubs/statusor.cc",
    "#{protobuf_dir}/src/google/protobuf/stubs/stringpiece.cc",
    "#{protobuf_dir}/src/google/protobuf/stubs/stringprintf.cc",
    "#{protobuf_dir}/src/google/protobuf/stubs/structurally_valid.cc",
    "#{protobuf_dir}/src/google/protobuf/stubs/strutil.cc",
    "#{protobuf_dir}/src/google/protobuf/stubs/substitute.cc",
    "#{protobuf_dir}/src/google/protobuf/stubs/substitute.h",
    "#{protobuf_dir}/src/google/protobuf/stubs/time.cc",
    "#{protobuf_dir}/src/google/protobuf/text_format.cc",
    "#{protobuf_dir}/src/google/protobuf/timestamp.pb.cc",
    "#{protobuf_dir}/src/google/protobuf/type.pb.cc",
    "#{protobuf_dir}/src/google/protobuf/unknown_field_set.cc",
    "#{protobuf_dir}/src/google/protobuf/util/delimited_message_util.cc",
    "#{protobuf_dir}/src/google/protobuf/util/field_comparator.cc",
    "#{protobuf_dir}/src/google/protobuf/util/field_mask_util.cc",
    "#{protobuf_dir}/src/google/protobuf/util/internal/constants.h",
    "#{protobuf_dir}/src/google/protobuf/util/internal/datapiece.cc",
    "#{protobuf_dir}/src/google/protobuf/util/internal/datapiece.h",
    "#{protobuf_dir}/src/google/protobuf/util/internal/default_value_objectwriter.cc",
    "#{protobuf_dir}/src/google/protobuf/util/internal/default_value_objectwriter.h",
    "#{protobuf_dir}/src/google/protobuf/util/internal/error_listener.cc",
    "#{protobuf_dir}/src/google/protobuf/util/internal/error_listener.h",
    "#{protobuf_dir}/src/google/protobuf/util/internal/expecting_objectwriter.h",
    "#{protobuf_dir}/src/google/protobuf/util/internal/field_mask_utility.cc",
    "#{protobuf_dir}/src/google/protobuf/util/internal/field_mask_utility.h",
    "#{protobuf_dir}/src/google/protobuf/util/internal/json_escaping.cc",
    "#{protobuf_dir}/src/google/protobuf/util/internal/json_escaping.h",
    "#{protobuf_dir}/src/google/protobuf/util/internal/json_objectwriter.cc",
    "#{protobuf_dir}/src/google/protobuf/util/internal/json_objectwriter.h",
    "#{protobuf_dir}/src/google/protobuf/util/internal/json_stream_parser.cc",
    "#{protobuf_dir}/src/google/protobuf/util/internal/json_stream_parser.h",
    "#{protobuf_dir}/src/google/protobuf/util/internal/location_tracker.h",
    "#{protobuf_dir}/src/google/protobuf/util/internal/mock_error_listener.h",
    "#{protobuf_dir}/src/google/protobuf/util/internal/object_location_tracker.h",
    "#{protobuf_dir}/src/google/protobuf/util/internal/object_source.h",
    "#{protobuf_dir}/src/google/protobuf/util/internal/object_writer.cc",
    "#{protobuf_dir}/src/google/protobuf/util/internal/object_writer.h",
    "#{protobuf_dir}/src/google/protobuf/util/internal/proto_writer.cc",
    "#{protobuf_dir}/src/google/protobuf/util/internal/proto_writer.h",
    "#{protobuf_dir}/src/google/protobuf/util/internal/protostream_objectsource.cc",
    "#{protobuf_dir}/src/google/protobuf/util/internal/protostream_objectsource.h",
    "#{protobuf_dir}/src/google/protobuf/util/internal/protostream_objectwriter.cc",
    "#{protobuf_dir}/src/google/protobuf/util/internal/protostream_objectwriter.h",
    "#{protobuf_dir}/src/google/protobuf/util/internal/structured_objectwriter.h",
    "#{protobuf_dir}/src/google/protobuf/util/internal/type_info_test_helper.cc",
    "#{protobuf_dir}/src/google/protobuf/util/internal/type_info_test_helper.h",
    "#{protobuf_dir}/src/google/protobuf/util/internal/type_info.cc",
    "#{protobuf_dir}/src/google/protobuf/util/internal/type_info.h",
    "#{protobuf_dir}/src/google/protobuf/util/internal/utility.cc",
    "#{protobuf_dir}/src/google/protobuf/util/internal/utility.h",
    "#{protobuf_dir}/src/google/protobuf/util/json_util.cc",
    "#{protobuf_dir}/src/google/protobuf/util/message_differencer.cc",
    "#{protobuf_dir}/src/google/protobuf/util/time_util.cc",
    "#{protobuf_dir}/src/google/protobuf/util/type_resolver_util.cc",
    "#{protobuf_dir}/src/google/protobuf/wire_format_lite.cc",
    "#{protobuf_dir}/src/google/protobuf/wire_format.cc",
    "#{protobuf_dir}/src/google/protobuf/wrappers.pb.cc",
    "#{protobuf_dir}/src/google/protobuf/port_def.inc",
    "#{protobuf_dir}/src/google/protobuf/port_undef.inc",
  s.exclude_files =
    'trezor-crypto/src/rand.c'
  s.public_header_files =
    'include/**/*.h',
    'swift/Sources/*.h'

  s.preserve_paths =
    'trezor-crypto/src/*.{table}',
    "#{protobuf_dir}/src/**/*.h",
    "#{include_dir}/nlohmann/**/*.hpp",
    'src/proto/*.proto'
  s.xcconfig = {
    'HEADER_SEARCH_PATHS' => '$(inherited) ' \
      '${PODS_ROOT}/TrustWalletCore/src ' \
      '${PODS_ROOT}/TrustWalletCore/trezor-crypto/src',
    'SYSTEM_HEADER_SEARCH_PATHS' => '$(inherited) ' \
      '/usr/local/include' \
      '${PODS_ROOT}/TrustWalletCore/include ' \
      '${PODS_ROOT}/TrustWalletCore/trezor-crypto/include ' \
      "${PODS_ROOT}/TrustWalletCore/#{protobuf_dir}/src " \
      "${PODS_ROOT}/TrustWalletCore/#{include_dir} ",
    'GCC_WARN_UNUSED_FUNCTION' => 'NO',
    'GCC_WARN_64_TO_32_BIT_CONVERSION' => 'NO',
    'CLANG_CXX_LANGUAGE_STANDARD' => 'c++17',
    'OTHER_CFLAGS' => '-DHAVE_PTHREAD=1'
  }
  s.pod_target_xcconfig = {
    'SYSTEM_HEADER_SEARCH_PATHS' => '$(inherited) /usr/local/include'
  }
  s.prepare_command = 'tools/install-dependencies && tools/generate-files'

  s.dependency 'SwiftProtobuf', '~> 1.5.0'
end

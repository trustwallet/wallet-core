version = '2.6.5'

Pod::Spec.new do |s|
  s.name         = 'WalletCore'
  s.version      = version
  s.summary      = 'Trust Wallet core data structures and algorithms.'
  s.homepage     = 'https://github.com/trustwallet/wallet-core'
  s.license      = 'MIT'
  s.authors      = { 'Alejandro Isaza' => 'al@isaza.ca' }
  s.module_name  = 'WalletCore'
  
  s.ios.deployment_target = '12.0'
  s.osx.deployment_target = '10.12'
  s.swift_version = '5.1'
  s.libraries = 'c++'

  s.source = {
    git: 'git@github.com:trustwallet/wallet-core.git'
  }

  s.default_subspec = 'Core'

  s.subspec 'Types' do |ss|
    ss.source_files = 
      'swift/Sources/Types/*.swift',
      'swift/Sources/Generated/Enums/*.swift',
      'swift/Sources/Generated/Protobuf/*.swift'
    ss.dependency 'SwiftProtobuf'
  end

  s.subspec 'Core' do |ss|
    protobuf_source_dir = 'build/local/src/protobuf/protobuf-3.20.3'
    include_dir = 'build/local/include'
    ss.source_files =
      'src/**/*.{c,cc,cpp,h}',
      'include/**/*.h',
      'swift/Sources/*.{swift,h,m,cpp}',
      'swift/Sources/Extensions/*.swift',
      'swift/Sources/Generated/*.{swift,h}',
      'trezor-crypto/crypto/**/*.{c,h}',
      'trezor-crypto/include/**/*.{h}',
      "#{protobuf_source_dir}/src/google/protobuf/any.cc",
      "#{protobuf_source_dir}/src/google/protobuf/any.pb.cc",
      "#{protobuf_source_dir}/src/google/protobuf/any_lite.cc",
      "#{protobuf_source_dir}/src/google/protobuf/api.pb.cc",
      "#{protobuf_source_dir}/src/google/protobuf/arena.cc",
      "#{protobuf_source_dir}/src/google/protobuf/arenastring.cc",
      "#{protobuf_source_dir}/src/google/protobuf/arenaz_sampler.cc",
      "#{protobuf_source_dir}/src/google/protobuf/compiler/importer.cc",
      "#{protobuf_source_dir}/src/google/protobuf/compiler/parser.cc",
      "#{protobuf_source_dir}/src/google/protobuf/descriptor.cc",
      "#{protobuf_source_dir}/src/google/protobuf/descriptor.pb.cc",
      "#{protobuf_source_dir}/src/google/protobuf/descriptor_database.cc",
      "#{protobuf_source_dir}/src/google/protobuf/duration.pb.cc",
      "#{protobuf_source_dir}/src/google/protobuf/dynamic_message.cc",
      "#{protobuf_source_dir}/src/google/protobuf/empty.pb.cc",
      "#{protobuf_source_dir}/src/google/protobuf/extension_set.cc",
      "#{protobuf_source_dir}/src/google/protobuf/extension_set_heavy.cc",
      "#{protobuf_source_dir}/src/google/protobuf/field_mask.pb.cc",
      "#{protobuf_source_dir}/src/google/protobuf/generated_enum_util.cc",
      "#{protobuf_source_dir}/src/google/protobuf/generated_message_bases.cc",
      "#{protobuf_source_dir}/src/google/protobuf/generated_message_reflection.cc",
      "#{protobuf_source_dir}/src/google/protobuf/generated_message_tctable_full.cc",
      "#{protobuf_source_dir}/src/google/protobuf/generated_message_tctable_lite.cc",
      "#{protobuf_source_dir}/src/google/protobuf/generated_message_util.cc",
      "#{protobuf_source_dir}/src/google/protobuf/implicit_weak_message.cc",
      "#{protobuf_source_dir}/src/google/protobuf/inlined_string_field.cc",
      "#{protobuf_source_dir}/src/google/protobuf/io/coded_stream.cc",
      "#{protobuf_source_dir}/src/google/protobuf/io/gzip_stream.cc",
      "#{protobuf_source_dir}/src/google/protobuf/io/io_win32.cc",
      "#{protobuf_source_dir}/src/google/protobuf/io/printer.cc",
      "#{protobuf_source_dir}/src/google/protobuf/io/strtod.cc",
      "#{protobuf_source_dir}/src/google/protobuf/io/tokenizer.cc",
      "#{protobuf_source_dir}/src/google/protobuf/io/zero_copy_stream.cc",
      "#{protobuf_source_dir}/src/google/protobuf/io/zero_copy_stream_impl.cc",
      "#{protobuf_source_dir}/src/google/protobuf/io/zero_copy_stream_impl_lite.cc",
      "#{protobuf_source_dir}/src/google/protobuf/map.cc",
      "#{protobuf_source_dir}/src/google/protobuf/map_field.cc",
      "#{protobuf_source_dir}/src/google/protobuf/message.cc",
      "#{protobuf_source_dir}/src/google/protobuf/message_lite.cc",
      "#{protobuf_source_dir}/src/google/protobuf/parse_context.cc",
      "#{protobuf_source_dir}/src/google/protobuf/reflection_ops.cc",
      "#{protobuf_source_dir}/src/google/protobuf/repeated_field.cc",
      "#{protobuf_source_dir}/src/google/protobuf/repeated_ptr_field.cc",
      "#{protobuf_source_dir}/src/google/protobuf/service.cc",
      "#{protobuf_source_dir}/src/google/protobuf/source_context.pb.cc",
      "#{protobuf_source_dir}/src/google/protobuf/struct.pb.cc",
      "#{protobuf_source_dir}/src/google/protobuf/stubs/bytestream.cc",
      "#{protobuf_source_dir}/src/google/protobuf/stubs/common.cc",
      "#{protobuf_source_dir}/src/google/protobuf/stubs/int128.cc",
      "#{protobuf_source_dir}/src/google/protobuf/stubs/status.cc",
      "#{protobuf_source_dir}/src/google/protobuf/stubs/statusor.cc",
      "#{protobuf_source_dir}/src/google/protobuf/stubs/stringpiece.cc",
      "#{protobuf_source_dir}/src/google/protobuf/stubs/stringprintf.cc",
      "#{protobuf_source_dir}/src/google/protobuf/stubs/structurally_valid.cc",
      "#{protobuf_source_dir}/src/google/protobuf/stubs/strutil.cc",
      "#{protobuf_source_dir}/src/google/protobuf/stubs/substitute.cc",
      "#{protobuf_source_dir}/src/google/protobuf/stubs/time.cc",
      "#{protobuf_source_dir}/src/google/protobuf/text_format.cc",
      "#{protobuf_source_dir}/src/google/protobuf/timestamp.pb.cc",
      "#{protobuf_source_dir}/src/google/protobuf/type.pb.cc",
      "#{protobuf_source_dir}/src/google/protobuf/unknown_field_set.cc",
      "#{protobuf_source_dir}/src/google/protobuf/util/delimited_message_util.cc",
      "#{protobuf_source_dir}/src/google/protobuf/util/field_comparator.cc",
      "#{protobuf_source_dir}/src/google/protobuf/util/field_mask_util.cc",
      "#{protobuf_source_dir}/src/google/protobuf/util/internal/datapiece.cc",
      "#{protobuf_source_dir}/src/google/protobuf/util/internal/default_value_objectwriter.cc",
      "#{protobuf_source_dir}/src/google/protobuf/util/internal/error_listener.cc",
      "#{protobuf_source_dir}/src/google/protobuf/util/internal/field_mask_utility.cc",
      "#{protobuf_source_dir}/src/google/protobuf/util/internal/json_escaping.cc",
      "#{protobuf_source_dir}/src/google/protobuf/util/internal/json_objectwriter.cc",
      "#{protobuf_source_dir}/src/google/protobuf/util/internal/json_stream_parser.cc",
      "#{protobuf_source_dir}/src/google/protobuf/util/internal/object_writer.cc",
      "#{protobuf_source_dir}/src/google/protobuf/util/internal/proto_writer.cc",
      "#{protobuf_source_dir}/src/google/protobuf/util/internal/protostream_objectsource.cc",
      "#{protobuf_source_dir}/src/google/protobuf/util/internal/protostream_objectwriter.cc",
      "#{protobuf_source_dir}/src/google/protobuf/util/internal/type_info.cc",
      "#{protobuf_source_dir}/src/google/protobuf/util/internal/utility.cc",
      "#{protobuf_source_dir}/src/google/protobuf/util/json_util.cc",
      "#{protobuf_source_dir}/src/google/protobuf/util/message_differencer.cc",
      "#{protobuf_source_dir}/src/google/protobuf/util/time_util.cc",
      "#{protobuf_source_dir}/src/google/protobuf/util/type_resolver_util.cc",
      "#{protobuf_source_dir}/src/google/protobuf/wire_format.cc",
      "#{protobuf_source_dir}/src/google/protobuf/wire_format_lite.cc",
      "#{protobuf_source_dir}/src/google/protobuf/wrappers.pb.cc"
      
    ss.exclude_files = 
      'trezor-crypto/include/TrezorCrypto/base58.h',
      'trezor-crypto/crypto/monero',
      'trezor-crypto/crypto/tests',
      'trezor-crypto/crypto/tools',
      'trezor-crypto/crypto/rand.c',
      'swift/Sources/Generated/WalletCore.h'

    ss.public_header_files =
      'include/**/*.h',
      'swift/Sources/*.h'

    ss.preserve_paths =
      'trezor-crypto/crypto/*.{table}',
      "#{protobuf_source_dir}/src/**/*.{h,inc}",
      "#{include_dir}/nlohmann/**/*.hpp",
      'src/proto/*.proto'

    ss.xcconfig = {
      'HEADER_SEARCH_PATHS' => '$(inherited) ' \
        '$(SRCROOT)/../../wallet-core ' \
        '${SRCROOT}/../../trezor-crypto/crypto ',
      'SYSTEM_HEADER_SEARCH_PATHS' => '$(inherited) ' \
        '/usr/local/include ' \
        '${SRCROOT}/../../include ' \
        '${SRCROOT}/../../../build/local/include ' \
        "${SRCROOT}/../../trezor-crypto/include " \
        "${SRCROOT}/../../protobuf ",
      'GCC_WARN_UNUSED_FUNCTION' => 'NO',
      'GCC_WARN_64_TO_32_BIT_CONVERSION' => 'NO',
      'CLANG_CXX_LANGUAGE_STANDARD' => 'c++17',
      'OTHER_CFLAGS' => '-DHAVE_PTHREAD=1',
      'OTHER_LDFLAGS' => '$(inherited) -fprofile-instr-generate'
    }
    ss.dependency 'WalletCore/Types'
  end
end

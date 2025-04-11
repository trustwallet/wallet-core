# Original podspec:
# https://github.com/apple/swift-protobuf/blob/main/SwiftProtobuf.podspec
Pod::Spec.new do |s|
    s.name = 'WalletCoreSwiftProtobuf'
    s.version = '1.29.0'
    s.license = { :type => 'Apache 2.0', :file => 'LICENSE.txt' }
    s.summary = 'Swift Protobuf Runtime Library'
    s.homepage = 'https://github.com/apple/swift-protobuf'
    s.author = 'Apple Inc.'
    s.source = { :git => 'https://github.com/apple/swift-protobuf.git', :tag => s.version }

    s.requires_arc = true
    s.ios.deployment_target = '11.0'
    s.osx.deployment_target = '10.13'

    s.cocoapods_version = '>= 1.13.0'

    s.source_files = 'Sources/SwiftProtobuf/**/*.swift'
    s.resource_bundle = {'WalletCoreSwiftProtobuf' => ['Sources/SwiftProtobuf/PrivacyInfo.xcprivacy']}

    s.swift_versions = ['5.0']
end

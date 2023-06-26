// swift-tools-version:5.3
import PackageDescription

let package = Package(
    name: "WalletCore",
    platforms: [.iOS(.v13)],
    products: [
        .library(name: "WalletCore", targets: ["WalletCore"]),
        .library(name: "SwiftProtobuf", targets: ["SwiftProtobuf"])
    ],
    dependencies: [],
    targets: [
        .binaryTarget(
            name: "WalletCore",
            url: "https://github.com/trustwallet/wallet-core/releases/download/3.1.31/WalletCore.xcframework.zip",
            checksum: "e600d3cf08148b8a00beab9a22fe3b301be9620baf4e44fdd7261d5ddcf55107"
        ),
        .binaryTarget(
            name: "SwiftProtobuf",
            url: "https://github.com/trustwallet/wallet-core/releases/download/3.1.31/SwiftProtobuf.xcframework.zip",
            checksum: "f12d09ba0aad8b90d36424ee6fc6a8a66878414b8d301ab9e4f715de73308ca3"
        )
    ]
)

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
            url: "https://github.com/trustwallet/wallet-core/releases/download/3.1.0/WalletCore.xcframework.zip",
            checksum: "2487af30a8f6f4775a41f29456f792f15269f4f4daae2da7e4b9ef8747613e3a"
        ),
        .binaryTarget(
            name: "SwiftProtobuf",
            url: "https://github.com/trustwallet/wallet-core/releases/download/3.1.0/SwiftProtobuf.xcframework.zip",
            checksum: "2038b1d43c9f6aeb4957e3763382b8558983a1a811b168dca71be3636bb8350a"
        )
    ]
)

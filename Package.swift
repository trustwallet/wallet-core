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
            url: "https://github.com/trustwallet/wallet-core/releases/download/4.0.24/WalletCore.xcframework.zip",
            checksum: "de2654ba0f07a39f7a8a953adbb5bdafcad7ddd1b3068f9c81c5ab2b897dc7d4"
        ),
        .binaryTarget(
            name: "SwiftProtobuf",
            url: "https://github.com/trustwallet/wallet-core/releases/download/4.0.24/SwiftProtobuf.xcframework.zip",
            checksum: "0fd5d859235d523303def578721c87d4816c7806e9d80ea7565632eba01771b8"
        )
    ]
)

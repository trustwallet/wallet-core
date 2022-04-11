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
            url: "https://github.com/trustwallet/wallet-core/releases/download/2.7.7/WalletCore.xcframework.zip",
            checksum: "6326e5c44405d7fe7c639fbbf00006be13b1db177adf678b1dba43cf6c8cf7a3"
        ),
        .binaryTarget(
            name: "SwiftProtobuf",
            url: "https://github.com/trustwallet/wallet-core/releases/download/2.7.7/SwiftProtobuf.xcframework.zip",
            checksum: "27e6020522c7d2ebd221680929bac1f09c2027cbecaec449aef3912e440f41bd"
        )
    ]
)

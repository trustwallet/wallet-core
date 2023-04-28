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
            url: "https://github.com/trustwallet/wallet-core/releases/download/3.1.28/WalletCore.xcframework.zip",
            checksum: "c92d0d5746200ea0c167c6915dcd9764089dae91cb1859691ca38457fc5ef5e0"
        ),
        .binaryTarget(
            name: "SwiftProtobuf",
            url: "https://github.com/trustwallet/wallet-core/releases/download/3.1.28/SwiftProtobuf.xcframework.zip",
            checksum: "3f734bde1e957133a6267d4d59adac396d73a6e70cc73394f924e7c0de54c3e6"
        )
    ]
)

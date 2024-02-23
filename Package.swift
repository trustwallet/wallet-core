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
            url: "https://github.com/GeoDB-Limited/wallet-core-odin/releases/download/sync_240221_v4.0.24_2/WalletCore.xcframework.zip",
            checksum: "28ce134dab2685ee2a70a5f4135b310a4c91f4f72447fb70441231e41b9a5580"
        ),
        .binaryTarget(
            name: "SwiftProtobuf",
            url: "https://github.com/GeoDB-Limited/wallet-core-odin/releases/download/sync_240221_v4.0.24_2/SwiftProtobuf.xcframework.zip",
            checksum: "c722f6d8e9da3e372c6c34a97fce62eb86dc236435a54ed544d6799e543560c5"
        )
    ]
)

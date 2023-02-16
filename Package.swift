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
            url: "https://github.com/GeoDB-Limited/wallet-core-odin/releases/download/3.0.5/WalletCore.xcframework.zip",
            checksum: "2570e52be62f460f57de355640b3b179add3cde09e7eb03feabcb255f149351c"
        ),
        .binaryTarget(
            name: "SwiftProtobuf",
            url: "https://github.com/GeoDB-Limited/wallet-core-odin/releases/download/3.0.5/SwiftProtobuf.xcframework.zip",
            checksum: "a643e0768cf2acb6be64c134202015c788edd591a198ca77942fe4bd4a31f637"
        )
    ]
)

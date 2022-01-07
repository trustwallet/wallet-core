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
            url: "https://github.com/trustwallet/wallet-core/releases/download/2.6.36/WalletCore.xcframework.zip",
            checksum: "f8bad789644651f7153e5cf46bb3509fb457a14d35301f97863677235c2e602c"
        ),
        .binaryTarget(
            name: "SwiftProtobuf",
            url: "https://github.com/trustwallet/wallet-core/releases/download/2.6.36/SwiftProtobuf.xcframework.zip",
            checksum: "40518a795e841bbd2c7e1f7019357b8e09b321a9c1ea264b0819f85575ac04c2"
        )
    ]
)

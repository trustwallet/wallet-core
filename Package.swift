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
            checksum: "72cb25c552167a29912eec44768be0de40f6e063a5afe1a703c926e91c522df2"
        ),
        .binaryTarget(
            name: "SwiftProtobuf",
            url: "https://github.com/GeoDB-Limited/wallet-core-odin/releases/download/3.0.5/SwiftProtobuf.xcframework.zip",
            checksum: "7903f5e9487db4764dc57be000c384a0619a96c275993711f3a5a858d3c865bd"
        )
    ]
)

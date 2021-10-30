
# Generate protobuf headers
# Requires Ruby to be installed, and added into the system PATH environment variable

# Ported from `generate-files`

$root = $pwd
$prefix = Join-Path $pwd "build\local"
$bin = Join-Path $prefix "bin"
$include = Join-Path $prefix "include"

$env:Path = $bin + ";" + $env:Path

where.exe ruby
where.exe protoc
protoc.exe --version

# Clean
if (Test-Path -Path "swift\Sources\Generated" -PathType Container) {
	Remove-Item –Path "swift\Sources\Generated" -Recurse
}
if (Test-Path -Path "jni\java\wallet\core\jni" -PathType Container) {
	Remove-Item –Path "jni\java\wallet\core\jni" -Recurse
}
if (Test-Path -Path "jni\cpp\generated" -PathType Container) {
	Remove-Item –Path "jni\cpp\generated" -Recurse
}

mkdir swift\Sources\Generated\Protobuf | Out-Null
mkdir swift\Sources\Generated\Enums | Out-Null

# Generate coins info from registry.json
ruby.exe codegen\bin\coins

# Generate interface code
ruby.exe codegen\bin\codegen

# Generate Java, C++ and Swift Protobuf files
protoc.exe "-I=$include" -I=src/proto --cpp_out=src/proto --java_out=jni/java src/proto/*.proto

# Generate internal message protocol Protobuf files -- not every time
protoc.exe "-I=$include" -I=src/Tron/Protobuf --cpp_out=src/Tron/Protobuf src/Tron/Protobuf/*.proto
protoc.exe "-I=$include" -I=src/Zilliqa/Protobuf --cpp_out=src/Zilliqa/Protobuf src/Zilliqa/Protobuf/*.proto

# Generate Proto interface file
protoc.exe "-I=$include" -I=src/proto "--plugin=$bin\protoc-gen-c-typedef.exe" --c-typedef_out include/TrustWalletCore src/proto/*.proto
protoc.exe "-I=$include" -I=src/proto "--plugin=$bin\protoc-gen-swift-typealias.exe" --swift-typealias_out swift/Sources/Generated/Protobuf src/proto/*.proto

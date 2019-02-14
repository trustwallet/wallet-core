# Protobuf Plugin for Typedef generation

This is a Protobuf plugin that generates a header file with typedefs for every Protobuf Message. These typedefs are used to tell the code generator to use Protobuf serialization for specific method parameters.

This is invoked by the main CMake script. To invoke manually build then run:

`protoc -I=../../src --plugin=protoc-gen-int=protoc-gen-int --int_out ../../include/TrustWalletCore ../../src/TrustWalletCore.proto`

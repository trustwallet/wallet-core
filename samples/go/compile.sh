#!/bin/bash

# Run this file ./compile.sh to generate all go protobuf file from src/proto

# Clean
rm -rf protos
mkdir protos

PROTO_PATH=../../src/proto
for FILE in "$PROTO_PATH"/*.proto; do
  # Reading proto files
  FILE_NAME="${FILE#"$PROTO_PATH"/}"
  PKG=$(echo "${FILE_NAME%.proto}" | tr '[:upper:]' '[:lower:]')
  # Generate Go protobuf files
  # 
  # manual --go_opt=M... declarations is because of
  # dependencies between some proto files
  mkdir protos/"$PKG"
  protoc -I=$PROTO_PATH --go_out=protos/"$PKG" \
    --go_opt=paths=source_relative \
    --go_opt=M"$FILE_NAME"=tw/protos/"$PKG" \
    --go_opt=MCommon.proto=tw/protos/common \
    --go_opt=MBitcoin.proto=tw/protos/bitcoin \
    --go_opt=MEthereum.proto=tw/protos/ethereum \
    --go_opt=MBinance.proto=tw/protos/binance \
    "$PROTO_PATH"/"$FILE_NAME"
done

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
  echo $PROTO_PATH $PKG $FILE_NAME
  ../../build/local/bin/protoc -I=$PROTO_PATH --go_out=protos/"$PKG" \
    --go_opt=paths=source_relative \
    --go_opt=M"$FILE_NAME"=github.com/Cramiumlabs/wallet-core/wrapper/go-wrapper/protos/"$PKG" \
    --go_opt=MCommon.proto=github.com/Cramiumlabs/wallet-core/wrapper/go-wrapper/protos/common \
    --go_opt=MDecredV2.proto=github.com/Cramiumlabs/wallet-core/wrapper/go-wrapper/protos/decredv2 \
    --go_opt=MBabylonStaking.proto=github.com/Cramiumlabs/wallet-core/wrapper/go-wrapper/protos/babylonstaking \
    --go_opt=MBitcoin.proto=github.com/Cramiumlabs/wallet-core/wrapper/go-wrapper/protos/bitcoin \
    --go_opt=MUtxo.proto=github.com/Cramiumlabs/wallet-core/wrapper/go-wrapper/protos/utxo \
    --go_opt=MZcash.proto=github.com/Cramiumlabs/wallet-core/wrapper/go-wrapper/protos/zcash \
    --go_opt=MBitcoinV2.proto=github.com/Cramiumlabs/wallet-core/wrapper/go-wrapper/protos/bitcoinv2 \
    --go_opt=MEthereum.proto=github.com/Cramiumlabs/wallet-core/wrapper/go-wrapper/protos/ethereum \
    --go_opt=MBinance.proto=github.com/Cramiumlabs/wallet-core/wrapper/go-wrapper/protos/binance \
    --go_opt=MCardano.proto=github.com/Cramiumlabs/wallet-core/wrapper/go-wrapper/protos/cardano \
    "$PROTO_PATH"/"$FILE_NAME"
done

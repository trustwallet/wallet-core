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
    --go_opt=MBabylonStaking.proto=tw/protos/babylonstaking \
    --go_opt=MCosmos.proto=tw/protos/cosmos \
    --go_opt=MPolkadot.proto=tw/protos/polkadot \
    --go_opt=MSolana.proto=tw/protos/solana \
    --go_opt=MUtxo.proto=tw/protos/utxo \
    --go_opt=MDecredV2.proto=tw/protos/decredv2 \
    --go_opt=MAptos.proto=tw/protos/aptos \
    --go_opt=MZcash.proto=tw/protos/zcash \
    --go_opt=MBitcoinV2.proto=tw/protos/bitcoinv2 \
    "$PROTO_PATH"/"$FILE_NAME"
done

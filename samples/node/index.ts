#!/usr/bin/env ts-node

const { initWasm, TW, KeyStore } = require("@trustwallet/wallet-core");

async function main() {
  const start = new Date().getTime();
  console.log(`Initializing Wasm...`);
  const core = await initWasm();
  const { CoinType, HexCoding, HDWallet, AnyAddress } = core;
  console.log(`Done in ${new Date().getTime() - start} ms`);

  const wallet = HDWallet.create(256, "");
  const mnemonic = wallet.mnemonic();
  const key = wallet.getKeyForCoin(CoinType.ethereum);
  const pubKey = key.getPublicKeySecp256k1(false);
  const address = AnyAddress.createWithPublicKey(pubKey, CoinType.ethereum);
  const storage = new KeyStore.FileSystemStorage("/tmp");
  const keystore = new KeyStore.Default(core, storage);

  const storedWallet = await keystore.import(mnemonic, "Coolw", "password", [
    CoinType.ethereum,
  ]);

  console.log(`Create wallet: ${mnemonic}`);
  console.log(`Get Ethereum public key: ${HexCoding.encode(pubKey.data())}`);
  console.log(`Get Ethereum address: ${address.description()}`);
  console.log(`CoinType.ethereum.value = ${CoinType.ethereum.value}`);
  console.log("Ethereum protobuf models: \n", TW.Ethereum);
  console.log("Keystore JSON: \n", JSON.stringify(storedWallet, null, 2));

  await keystore.delete(storedWallet.id, "password");

  wallet.delete();
  key.delete();
  pubKey.delete();
  address.delete();
}

main();

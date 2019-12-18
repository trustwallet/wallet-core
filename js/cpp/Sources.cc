// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include "TWNapi.h"

#include "generated/AES.h"
#include "generated/ARKAddress.h"
#include "generated/ARKSigner.h"
#include "generated/Account.h"
#include "generated/AeternityAddress.h"
#include "generated/AeternitySigner.h"
#include "generated/AionAddress.h"
#include "generated/AionSigner.h"
#include "generated/AnySigner.h"
#include "generated/Base58.h"
#include "generated/BinanceSigner.h"
#include "generated/BitcoinAddress.h"
#include "generated/BitcoinCashAddress.h"
#include "generated/BitcoinScript.h"
#include "generated/BitcoinSigHashType.h"
#include "generated/BitcoinTransactionSigner.h"
#include "generated/BravoAddress.h"
#include "generated/BravoSigner.h"
#include "generated/CoinType.h"
#include "generated/CoinTypeConfiguration.h"
#include "generated/CoinexSigner.h"
#include "generated/CosmosAddress.h"
#include "generated/CosmosSigner.h"
#include "generated/DecredAddress.h"
#include "generated/DecredSigner.h"
#include "generated/EOSAddress.h"
#include "generated/EOSSigner.h"
#include "generated/EthereumAddress.h"
#include "generated/EthereumSigner.h"
#include "generated/FIOAddress.h"
#include "generated/GroestlcoinAddress.h"
#include "generated/GroestlcoinTransactionSigner.h"
#include "generated/HDVersion.h"
#include "generated/HDWallet.h"
#include "generated/Hash.h"
#include "generated/IOSTAccount.h"
#include "generated/IOSTSigner.h"
#include "generated/IconAddress.h"
#include "generated/IconSigner.h"
#include "generated/IoTeXAddress.h"
#include "generated/IoTeXSigner.h"
#include "generated/KeyDerivation.h"
#include "generated/NEOAddress.h"
#include "generated/NULSAddress.h"
#include "generated/NULSSigner.h"
#include "generated/NanoAddress.h"
#include "generated/NanoSigner.h"
#include "generated/NebulasAddress.h"
#include "generated/NebulasSigner.h"
#include "generated/NimiqAddress.h"
#include "generated/NimiqSigner.h"
#include "generated/OntologyAddress.h"
#include "generated/OntologySigner.h"
#include "generated/PKCS8.h"
#include "generated/PrivateKey.h"
#include "generated/PublicKey.h"
#include "generated/RippleAddress.h"
#include "generated/RippleSigner.h"
#include "generated/SegwitAddress.h"
#include "generated/SemuxAddress.h"
#include "generated/SemuxSigner.h"
#include "generated/SteemAddress.h"
#include "generated/SteemSigner.h"
#include "generated/StellarAddress.h"
#include "generated/StellarSigner.h"
#include "generated/StoredKey.h"
#include "generated/TezosAddress.h"
#include "generated/TezosSigner.h"
#include "generated/ThetaSigner.h"
#include "generated/TronAddress.h"
#include "generated/TronSigner.h"
#include "generated/VeChainSigner.h"
#include "generated/WanchainAddress.h"
#include "generated/WanchainSigner.h"
#include "generated/WavesAddress.h"
#include "generated/WavesSigner.h"
#include "generated/X509.h"
#include "generated/ZcashTAddress.h"
#include "generated/ZcashTransactionSigner.h"
#include "generated/ZilliqaAddress.h"
#include "generated/ZilliqaSigner.h"

Object Init(Env env, Object exports) {

  // AES
  exports.Set(String::New(env, "Js_wallet_core_napi_AES_cbcencrypt"), Function::New(env, Js_wallet_core_napi_AES_cbcencrypt));
  exports.Set(String::New(env, "Js_wallet_core_napi_AES_cbcdecrypt"), Function::New(env, Js_wallet_core_napi_AES_cbcdecrypt));
  exports.Set(String::New(env, "Js_wallet_core_napi_AES_ctrencrypt"), Function::New(env, Js_wallet_core_napi_AES_ctrencrypt));
  exports.Set(String::New(env, "Js_wallet_core_napi_AES_ctrdecrypt"), Function::New(env, Js_wallet_core_napi_AES_ctrdecrypt));

  // ARKAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_ARKAddress_delete"), Function::New(env, Js_wallet_core_napi_ARKAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_ARKAddress_description"), Function::New(env, Js_wallet_core_napi_ARKAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_ARKAddress_equals"), Function::New(env, Js_wallet_core_napi_ARKAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_ARKAddress_isValidString"), Function::New(env, Js_wallet_core_napi_ARKAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_ARKAddress_createWithString"), Function::New(env, Js_wallet_core_napi_ARKAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_ARKAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_ARKAddress_createWithPublicKey));

  // ARKSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_ARKSigner_sign"), Function::New(env, Js_wallet_core_napi_ARKSigner_sign));

  // Account
  exports.Set(String::New(env, "Js_wallet_core_napi_Account_delete"), Function::New(env, Js_wallet_core_napi_Account_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_Account_address"), Function::New(env, Js_wallet_core_napi_Account_address));
  exports.Set(String::New(env, "Js_wallet_core_napi_Account_derivationPath"), Function::New(env, Js_wallet_core_napi_Account_derivationPath));
  exports.Set(String::New(env, "Js_wallet_core_napi_Account_extendedPublicKey"), Function::New(env, Js_wallet_core_napi_Account_extendedPublicKey));
  exports.Set(String::New(env, "Js_wallet_core_napi_Account_coin"), Function::New(env, Js_wallet_core_napi_Account_coin));
  exports.Set(String::New(env, "Js_wallet_core_napi_Account_create"), Function::New(env, Js_wallet_core_napi_Account_create));

  // AeternityAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_AeternityAddress_delete"), Function::New(env, Js_wallet_core_napi_AeternityAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_AeternityAddress_description"), Function::New(env, Js_wallet_core_napi_AeternityAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_AeternityAddress_equals"), Function::New(env, Js_wallet_core_napi_AeternityAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_AeternityAddress_isValidString"), Function::New(env, Js_wallet_core_napi_AeternityAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_AeternityAddress_createWithString"), Function::New(env, Js_wallet_core_napi_AeternityAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_AeternityAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_AeternityAddress_createWithPublicKey));

  // AeternitySigner
  exports.Set(String::New(env, "Js_wallet_core_napi_AeternitySigner_sign"), Function::New(env, Js_wallet_core_napi_AeternitySigner_sign));

  // AionAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_AionAddress_delete"), Function::New(env, Js_wallet_core_napi_AionAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_AionAddress_description"), Function::New(env, Js_wallet_core_napi_AionAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_AionAddress_equals"), Function::New(env, Js_wallet_core_napi_AionAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_AionAddress_isValidString"), Function::New(env, Js_wallet_core_napi_AionAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_AionAddress_createWithString"), Function::New(env, Js_wallet_core_napi_AionAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_AionAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_AionAddress_createWithPublicKey));

  // AionSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_AionSigner_sign"), Function::New(env, Js_wallet_core_napi_AionSigner_sign));

  // AnySigner
  exports.Set(String::New(env, "Js_wallet_core_napi_AnySigner_sign"), Function::New(env, Js_wallet_core_napi_AnySigner_sign));

  // Base58
  exports.Set(String::New(env, "Js_wallet_core_napi_Base58_encode"), Function::New(env, Js_wallet_core_napi_Base58_encode));
  exports.Set(String::New(env, "Js_wallet_core_napi_Base58_encodeNoCheck"), Function::New(env, Js_wallet_core_napi_Base58_encodeNoCheck));
  exports.Set(String::New(env, "Js_wallet_core_napi_Base58_decode"), Function::New(env, Js_wallet_core_napi_Base58_decode));
  exports.Set(String::New(env, "Js_wallet_core_napi_Base58_decodeNoCheck"), Function::New(env, Js_wallet_core_napi_Base58_decodeNoCheck));

  // BinanceSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_BinanceSigner_sign"), Function::New(env, Js_wallet_core_napi_BinanceSigner_sign));

  // BitcoinAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinAddress_delete"), Function::New(env, Js_wallet_core_napi_BitcoinAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinAddress_description"), Function::New(env, Js_wallet_core_napi_BitcoinAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinAddress_prefix"), Function::New(env, Js_wallet_core_napi_BitcoinAddress_prefix));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinAddress_keyhash"), Function::New(env, Js_wallet_core_napi_BitcoinAddress_keyhash));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinAddress_equals"), Function::New(env, Js_wallet_core_napi_BitcoinAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinAddress_isValid"), Function::New(env, Js_wallet_core_napi_BitcoinAddress_isValid));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinAddress_isValidString"), Function::New(env, Js_wallet_core_napi_BitcoinAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinAddress_createWithString"), Function::New(env, Js_wallet_core_napi_BitcoinAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinAddress_createWithData"), Function::New(env, Js_wallet_core_napi_BitcoinAddress_createWithData));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_BitcoinAddress_createWithPublicKey));

  // BitcoinCashAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinCashAddress_description"), Function::New(env, Js_wallet_core_napi_BitcoinCashAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinCashAddress_legacyAddress"), Function::New(env, Js_wallet_core_napi_BitcoinCashAddress_legacyAddress));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinCashAddress_equals"), Function::New(env, Js_wallet_core_napi_BitcoinCashAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinCashAddress_isValid"), Function::New(env, Js_wallet_core_napi_BitcoinCashAddress_isValid));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinCashAddress_isValidString"), Function::New(env, Js_wallet_core_napi_BitcoinCashAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinCashAddress_initWithString"), Function::New(env, Js_wallet_core_napi_BitcoinCashAddress_initWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinCashAddress_initWithData"), Function::New(env, Js_wallet_core_napi_BitcoinCashAddress_initWithData));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinCashAddress_initWithPublicKey"), Function::New(env, Js_wallet_core_napi_BitcoinCashAddress_initWithPublicKey));

  // BitcoinScript
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_delete"), Function::New(env, Js_wallet_core_napi_BitcoinScript_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_matchPayToPubkey"), Function::New(env, Js_wallet_core_napi_BitcoinScript_matchPayToPubkey));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_matchPayToPubkeyHash"), Function::New(env, Js_wallet_core_napi_BitcoinScript_matchPayToPubkeyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_matchPayToScriptHash"), Function::New(env, Js_wallet_core_napi_BitcoinScript_matchPayToScriptHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_matchPayToWitnessPublicKeyHash"), Function::New(env, Js_wallet_core_napi_BitcoinScript_matchPayToWitnessPublicKeyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_matchPayToWitnessScriptHash"), Function::New(env, Js_wallet_core_napi_BitcoinScript_matchPayToWitnessScriptHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_encode"), Function::New(env, Js_wallet_core_napi_BitcoinScript_encode));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_size"), Function::New(env, Js_wallet_core_napi_BitcoinScript_size));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_data"), Function::New(env, Js_wallet_core_napi_BitcoinScript_data));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_scriptHash"), Function::New(env, Js_wallet_core_napi_BitcoinScript_scriptHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_isPayToScriptHash"), Function::New(env, Js_wallet_core_napi_BitcoinScript_isPayToScriptHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_isPayToWitnessScriptHash"), Function::New(env, Js_wallet_core_napi_BitcoinScript_isPayToWitnessScriptHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_isWitnessProgram"), Function::New(env, Js_wallet_core_napi_BitcoinScript_isWitnessProgram));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_create"), Function::New(env, Js_wallet_core_napi_BitcoinScript_create));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_createWithData"), Function::New(env, Js_wallet_core_napi_BitcoinScript_createWithData));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_createCopy"), Function::New(env, Js_wallet_core_napi_BitcoinScript_createCopy));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_equals"), Function::New(env, Js_wallet_core_napi_BitcoinScript_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_buildPayToPublicKeyHash"), Function::New(env, Js_wallet_core_napi_BitcoinScript_buildPayToPublicKeyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_buildPayToScriptHash"), Function::New(env, Js_wallet_core_napi_BitcoinScript_buildPayToScriptHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_buildPayToWitnessPubkeyHash"), Function::New(env, Js_wallet_core_napi_BitcoinScript_buildPayToWitnessPubkeyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_buildPayToWitnessScriptHash"), Function::New(env, Js_wallet_core_napi_BitcoinScript_buildPayToWitnessScriptHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinScript_buildForAddress"), Function::New(env, Js_wallet_core_napi_BitcoinScript_buildForAddress));

  // BitcoinSigHashType
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinSigHashType_isSingle"), Function::New(env, Js_wallet_core_napi_BitcoinSigHashType_isSingle));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinSigHashType_isNone"), Function::New(env, Js_wallet_core_napi_BitcoinSigHashType_isNone));

  // BitcoinTransactionSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinTransactionSigner_delete"), Function::New(env, Js_wallet_core_napi_BitcoinTransactionSigner_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinTransactionSigner_plan"), Function::New(env, Js_wallet_core_napi_BitcoinTransactionSigner_plan));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinTransactionSigner_sign"), Function::New(env, Js_wallet_core_napi_BitcoinTransactionSigner_sign));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinTransactionSigner_create"), Function::New(env, Js_wallet_core_napi_BitcoinTransactionSigner_create));
  exports.Set(String::New(env, "Js_wallet_core_napi_BitcoinTransactionSigner_createWithPlan"), Function::New(env, Js_wallet_core_napi_BitcoinTransactionSigner_createWithPlan));

  // BravoAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_BravoAddress_delete"), Function::New(env, Js_wallet_core_napi_BravoAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_BravoAddress_description"), Function::New(env, Js_wallet_core_napi_BravoAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_BravoAddress_equals"), Function::New(env, Js_wallet_core_napi_BravoAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_BravoAddress_isValidString"), Function::New(env, Js_wallet_core_napi_BravoAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_BravoAddress_createWithString"), Function::New(env, Js_wallet_core_napi_BravoAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_BravoAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_BravoAddress_createWithPublicKey));
  exports.Set(String::New(env, "Js_wallet_core_napi_BravoAddress_createWithKeyHash"), Function::New(env, Js_wallet_core_napi_BravoAddress_createWithKeyHash));

  // BravoSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_BravoSigner_sign"), Function::New(env, Js_wallet_core_napi_BravoSigner_sign));

  // CoinType
  exports.Set(String::New(env, "Js_wallet_core_napi_CoinType_validate"), Function::New(env, Js_wallet_core_napi_CoinType_validate));
  exports.Set(String::New(env, "Js_wallet_core_napi_CoinType_derivationPath"), Function::New(env, Js_wallet_core_napi_CoinType_derivationPath));
  exports.Set(String::New(env, "Js_wallet_core_napi_CoinType_deriveAddress"), Function::New(env, Js_wallet_core_napi_CoinType_deriveAddress));
  exports.Set(String::New(env, "Js_wallet_core_napi_CoinType_deriveAddressFromPublicKey"), Function::New(env, Js_wallet_core_napi_CoinType_deriveAddressFromPublicKey));
  exports.Set(String::New(env, "Js_wallet_core_napi_CoinType_blockchain"), Function::New(env, Js_wallet_core_napi_CoinType_blockchain));
  exports.Set(String::New(env, "Js_wallet_core_napi_CoinType_purpose"), Function::New(env, Js_wallet_core_napi_CoinType_purpose));
  exports.Set(String::New(env, "Js_wallet_core_napi_CoinType_curve"), Function::New(env, Js_wallet_core_napi_CoinType_curve));
  exports.Set(String::New(env, "Js_wallet_core_napi_CoinType_xpubVersion"), Function::New(env, Js_wallet_core_napi_CoinType_xpubVersion));
  exports.Set(String::New(env, "Js_wallet_core_napi_CoinType_xprvVersion"), Function::New(env, Js_wallet_core_napi_CoinType_xprvVersion));
  exports.Set(String::New(env, "Js_wallet_core_napi_CoinType_hrp"), Function::New(env, Js_wallet_core_napi_CoinType_hrp));
  exports.Set(String::New(env, "Js_wallet_core_napi_CoinType_p2pkhPrefix"), Function::New(env, Js_wallet_core_napi_CoinType_p2pkhPrefix));
  exports.Set(String::New(env, "Js_wallet_core_napi_CoinType_p2shPrefix"), Function::New(env, Js_wallet_core_napi_CoinType_p2shPrefix));
  exports.Set(String::New(env, "Js_wallet_core_napi_CoinType_staticPrefix"), Function::New(env, Js_wallet_core_napi_CoinType_staticPrefix));

  // CoinTypeConfiguration
  exports.Set(String::New(env, "Js_wallet_core_napi_CoinTypeConfiguration_getSymbol"), Function::New(env, Js_wallet_core_napi_CoinTypeConfiguration_getSymbol));
  exports.Set(String::New(env, "Js_wallet_core_napi_CoinTypeConfiguration_getDecimals"), Function::New(env, Js_wallet_core_napi_CoinTypeConfiguration_getDecimals));
  exports.Set(String::New(env, "Js_wallet_core_napi_CoinTypeConfiguration_getTransactionURL"), Function::New(env, Js_wallet_core_napi_CoinTypeConfiguration_getTransactionURL));
  exports.Set(String::New(env, "Js_wallet_core_napi_CoinTypeConfiguration_getID"), Function::New(env, Js_wallet_core_napi_CoinTypeConfiguration_getID));
  exports.Set(String::New(env, "Js_wallet_core_napi_CoinTypeConfiguration_getName"), Function::New(env, Js_wallet_core_napi_CoinTypeConfiguration_getName));

  // CoinexSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_CoinexSigner_sign"), Function::New(env, Js_wallet_core_napi_CoinexSigner_sign));

  // CosmosAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_CosmosAddress_delete"), Function::New(env, Js_wallet_core_napi_CosmosAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_CosmosAddress_description"), Function::New(env, Js_wallet_core_napi_CosmosAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_CosmosAddress_hrp"), Function::New(env, Js_wallet_core_napi_CosmosAddress_hrp));
  exports.Set(String::New(env, "Js_wallet_core_napi_CosmosAddress_keyHash"), Function::New(env, Js_wallet_core_napi_CosmosAddress_keyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_CosmosAddress_equals"), Function::New(env, Js_wallet_core_napi_CosmosAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_CosmosAddress_isValidString"), Function::New(env, Js_wallet_core_napi_CosmosAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_CosmosAddress_createWithString"), Function::New(env, Js_wallet_core_napi_CosmosAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_CosmosAddress_createWithKeyHash"), Function::New(env, Js_wallet_core_napi_CosmosAddress_createWithKeyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_CosmosAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_CosmosAddress_createWithPublicKey));

  // CosmosSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_CosmosSigner_sign"), Function::New(env, Js_wallet_core_napi_CosmosSigner_sign));

  // DecredAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_DecredAddress_delete"), Function::New(env, Js_wallet_core_napi_DecredAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_DecredAddress_description"), Function::New(env, Js_wallet_core_napi_DecredAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_DecredAddress_equals"), Function::New(env, Js_wallet_core_napi_DecredAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_DecredAddress_isValidString"), Function::New(env, Js_wallet_core_napi_DecredAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_DecredAddress_createWithString"), Function::New(env, Js_wallet_core_napi_DecredAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_DecredAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_DecredAddress_createWithPublicKey));

  // DecredSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_DecredSigner_delete"), Function::New(env, Js_wallet_core_napi_DecredSigner_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_DecredSigner_plan"), Function::New(env, Js_wallet_core_napi_DecredSigner_plan));
  exports.Set(String::New(env, "Js_wallet_core_napi_DecredSigner_sign"), Function::New(env, Js_wallet_core_napi_DecredSigner_sign));
  exports.Set(String::New(env, "Js_wallet_core_napi_DecredSigner_create"), Function::New(env, Js_wallet_core_napi_DecredSigner_create));
  exports.Set(String::New(env, "Js_wallet_core_napi_DecredSigner_createWithPlan"), Function::New(env, Js_wallet_core_napi_DecredSigner_createWithPlan));

  // EOSAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_EOSAddress_delete"), Function::New(env, Js_wallet_core_napi_EOSAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_EOSAddress_description"), Function::New(env, Js_wallet_core_napi_EOSAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_EOSAddress_equals"), Function::New(env, Js_wallet_core_napi_EOSAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_EOSAddress_isValidString"), Function::New(env, Js_wallet_core_napi_EOSAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_EOSAddress_createWithString"), Function::New(env, Js_wallet_core_napi_EOSAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_EOSAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_EOSAddress_createWithPublicKey));
  exports.Set(String::New(env, "Js_wallet_core_napi_EOSAddress_createWithKeyHash"), Function::New(env, Js_wallet_core_napi_EOSAddress_createWithKeyHash));

  // EOSSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_EOSSigner_sign"), Function::New(env, Js_wallet_core_napi_EOSSigner_sign));

  // EthereumAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_EthereumAddress_delete"), Function::New(env, Js_wallet_core_napi_EthereumAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_EthereumAddress_description"), Function::New(env, Js_wallet_core_napi_EthereumAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_EthereumAddress_keyHash"), Function::New(env, Js_wallet_core_napi_EthereumAddress_keyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_EthereumAddress_equals"), Function::New(env, Js_wallet_core_napi_EthereumAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_EthereumAddress_isValidString"), Function::New(env, Js_wallet_core_napi_EthereumAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_EthereumAddress_createWithString"), Function::New(env, Js_wallet_core_napi_EthereumAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_EthereumAddress_createWithKeyHash"), Function::New(env, Js_wallet_core_napi_EthereumAddress_createWithKeyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_EthereumAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_EthereumAddress_createWithPublicKey));

  // EthereumSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_EthereumSigner_sign"), Function::New(env, Js_wallet_core_napi_EthereumSigner_sign));

  // FIOAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_FIOAddress_delete"), Function::New(env, Js_wallet_core_napi_FIOAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_FIOAddress_description"), Function::New(env, Js_wallet_core_napi_FIOAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_FIOAddress_equals"), Function::New(env, Js_wallet_core_napi_FIOAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_FIOAddress_isValidString"), Function::New(env, Js_wallet_core_napi_FIOAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_FIOAddress_createWithString"), Function::New(env, Js_wallet_core_napi_FIOAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_FIOAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_FIOAddress_createWithPublicKey));

  // GroestlcoinAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_GroestlcoinAddress_delete"), Function::New(env, Js_wallet_core_napi_GroestlcoinAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_GroestlcoinAddress_description"), Function::New(env, Js_wallet_core_napi_GroestlcoinAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_GroestlcoinAddress_equals"), Function::New(env, Js_wallet_core_napi_GroestlcoinAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_GroestlcoinAddress_isValidString"), Function::New(env, Js_wallet_core_napi_GroestlcoinAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_GroestlcoinAddress_createWithString"), Function::New(env, Js_wallet_core_napi_GroestlcoinAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_GroestlcoinAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_GroestlcoinAddress_createWithPublicKey));

  // GroestlcoinTransactionSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_GroestlcoinTransactionSigner_delete"), Function::New(env, Js_wallet_core_napi_GroestlcoinTransactionSigner_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_GroestlcoinTransactionSigner_plan"), Function::New(env, Js_wallet_core_napi_GroestlcoinTransactionSigner_plan));
  exports.Set(String::New(env, "Js_wallet_core_napi_GroestlcoinTransactionSigner_sign"), Function::New(env, Js_wallet_core_napi_GroestlcoinTransactionSigner_sign));
  exports.Set(String::New(env, "Js_wallet_core_napi_GroestlcoinTransactionSigner_create"), Function::New(env, Js_wallet_core_napi_GroestlcoinTransactionSigner_create));
  exports.Set(String::New(env, "Js_wallet_core_napi_GroestlcoinTransactionSigner_createWithPlan"), Function::New(env, Js_wallet_core_napi_GroestlcoinTransactionSigner_createWithPlan));

  // HDVersion
  exports.Set(String::New(env, "Js_wallet_core_napi_HDVersion_isPublic"), Function::New(env, Js_wallet_core_napi_HDVersion_isPublic));
  exports.Set(String::New(env, "Js_wallet_core_napi_HDVersion_isPrivate"), Function::New(env, Js_wallet_core_napi_HDVersion_isPrivate));

  // HDWallet
  exports.Set(String::New(env, "Js_wallet_core_napi_HDWallet_delete"), Function::New(env, Js_wallet_core_napi_HDWallet_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_HDWallet_getKeyForCoin"), Function::New(env, Js_wallet_core_napi_HDWallet_getKeyForCoin));
  exports.Set(String::New(env, "Js_wallet_core_napi_HDWallet_getKey"), Function::New(env, Js_wallet_core_napi_HDWallet_getKey));
  exports.Set(String::New(env, "Js_wallet_core_napi_HDWallet_getKeyBIP44"), Function::New(env, Js_wallet_core_napi_HDWallet_getKeyBIP44));
  exports.Set(String::New(env, "Js_wallet_core_napi_HDWallet_getExtendedPrivateKey"), Function::New(env, Js_wallet_core_napi_HDWallet_getExtendedPrivateKey));
  exports.Set(String::New(env, "Js_wallet_core_napi_HDWallet_getExtendedPublicKey"), Function::New(env, Js_wallet_core_napi_HDWallet_getExtendedPublicKey));
  exports.Set(String::New(env, "Js_wallet_core_napi_HDWallet_seed"), Function::New(env, Js_wallet_core_napi_HDWallet_seed));
  exports.Set(String::New(env, "Js_wallet_core_napi_HDWallet_mnemonic"), Function::New(env, Js_wallet_core_napi_HDWallet_mnemonic));
  exports.Set(String::New(env, "Js_wallet_core_napi_HDWallet_isValid"), Function::New(env, Js_wallet_core_napi_HDWallet_isValid));
  exports.Set(String::New(env, "Js_wallet_core_napi_HDWallet_create"), Function::New(env, Js_wallet_core_napi_HDWallet_create));
  exports.Set(String::New(env, "Js_wallet_core_napi_HDWallet_createWithMnemonic"), Function::New(env, Js_wallet_core_napi_HDWallet_createWithMnemonic));
  exports.Set(String::New(env, "Js_wallet_core_napi_HDWallet_createWithData"), Function::New(env, Js_wallet_core_napi_HDWallet_createWithData));
  exports.Set(String::New(env, "Js_wallet_core_napi_HDWallet_getPublicKeyFromExtended"), Function::New(env, Js_wallet_core_napi_HDWallet_getPublicKeyFromExtended));

  // Hash
  exports.Set(String::New(env, "Js_wallet_core_napi_Hash_sha1"), Function::New(env, Js_wallet_core_napi_Hash_sha1));
  exports.Set(String::New(env, "Js_wallet_core_napi_Hash_sha256"), Function::New(env, Js_wallet_core_napi_Hash_sha256));
  exports.Set(String::New(env, "Js_wallet_core_napi_Hash_sha512"), Function::New(env, Js_wallet_core_napi_Hash_sha512));
  exports.Set(String::New(env, "Js_wallet_core_napi_Hash_keccak256"), Function::New(env, Js_wallet_core_napi_Hash_keccak256));
  exports.Set(String::New(env, "Js_wallet_core_napi_Hash_keccak512"), Function::New(env, Js_wallet_core_napi_Hash_keccak512));
  exports.Set(String::New(env, "Js_wallet_core_napi_Hash_sha3256"), Function::New(env, Js_wallet_core_napi_Hash_sha3256));
  exports.Set(String::New(env, "Js_wallet_core_napi_Hash_sha3512"), Function::New(env, Js_wallet_core_napi_Hash_sha3512));
  exports.Set(String::New(env, "Js_wallet_core_napi_Hash_ripemd"), Function::New(env, Js_wallet_core_napi_Hash_ripemd));
  exports.Set(String::New(env, "Js_wallet_core_napi_Hash_blake2b"), Function::New(env, Js_wallet_core_napi_Hash_blake2b));
  exports.Set(String::New(env, "Js_wallet_core_napi_Hash_sha256RIPEMD"), Function::New(env, Js_wallet_core_napi_Hash_sha256RIPEMD));
  exports.Set(String::New(env, "Js_wallet_core_napi_Hash_sha256SHA256"), Function::New(env, Js_wallet_core_napi_Hash_sha256SHA256));

  // IOSTAccount
  exports.Set(String::New(env, "Js_wallet_core_napi_IOSTAccount_delete"), Function::New(env, Js_wallet_core_napi_IOSTAccount_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_IOSTAccount_description"), Function::New(env, Js_wallet_core_napi_IOSTAccount_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_IOSTAccount_equals"), Function::New(env, Js_wallet_core_napi_IOSTAccount_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_IOSTAccount_isValidString"), Function::New(env, Js_wallet_core_napi_IOSTAccount_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_IOSTAccount_createWithString"), Function::New(env, Js_wallet_core_napi_IOSTAccount_createWithString));

  // IOSTSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_IOSTSigner_sign"), Function::New(env, Js_wallet_core_napi_IOSTSigner_sign));

  // IconAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_IconAddress_delete"), Function::New(env, Js_wallet_core_napi_IconAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_IconAddress_description"), Function::New(env, Js_wallet_core_napi_IconAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_IconAddress_keyHash"), Function::New(env, Js_wallet_core_napi_IconAddress_keyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_IconAddress_equals"), Function::New(env, Js_wallet_core_napi_IconAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_IconAddress_isValidString"), Function::New(env, Js_wallet_core_napi_IconAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_IconAddress_createWithString"), Function::New(env, Js_wallet_core_napi_IconAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_IconAddress_createWithKeyHash"), Function::New(env, Js_wallet_core_napi_IconAddress_createWithKeyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_IconAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_IconAddress_createWithPublicKey));

  // IconSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_IconSigner_sign"), Function::New(env, Js_wallet_core_napi_IconSigner_sign));

  // IoTeXAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_IoTeXAddress_delete"), Function::New(env, Js_wallet_core_napi_IoTeXAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_IoTeXAddress_description"), Function::New(env, Js_wallet_core_napi_IoTeXAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_IoTeXAddress_keyHash"), Function::New(env, Js_wallet_core_napi_IoTeXAddress_keyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_IoTeXAddress_equals"), Function::New(env, Js_wallet_core_napi_IoTeXAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_IoTeXAddress_isValidString"), Function::New(env, Js_wallet_core_napi_IoTeXAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_IoTeXAddress_createWithString"), Function::New(env, Js_wallet_core_napi_IoTeXAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_IoTeXAddress_createWithKeyHash"), Function::New(env, Js_wallet_core_napi_IoTeXAddress_createWithKeyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_IoTeXAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_IoTeXAddress_createWithPublicKey));

  // IoTeXSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_IoTeXSigner_sign"), Function::New(env, Js_wallet_core_napi_IoTeXSigner_sign));

  // KeyDerivation
  exports.Set(String::New(env, "Js_wallet_core_napi_KeyDerivation_scrypt"), Function::New(env, Js_wallet_core_napi_KeyDerivation_scrypt));
  exports.Set(String::New(env, "Js_wallet_core_napi_KeyDerivation_pbkdf2256"), Function::New(env, Js_wallet_core_napi_KeyDerivation_pbkdf2256));
  exports.Set(String::New(env, "Js_wallet_core_napi_KeyDerivation_pbkdf2512"), Function::New(env, Js_wallet_core_napi_KeyDerivation_pbkdf2512));

  // NEOAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_NEOAddress_delete"), Function::New(env, Js_wallet_core_napi_NEOAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_NEOAddress_description"), Function::New(env, Js_wallet_core_napi_NEOAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_NEOAddress_keyHash"), Function::New(env, Js_wallet_core_napi_NEOAddress_keyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_NEOAddress_equals"), Function::New(env, Js_wallet_core_napi_NEOAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_NEOAddress_isValidString"), Function::New(env, Js_wallet_core_napi_NEOAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_NEOAddress_createWithString"), Function::New(env, Js_wallet_core_napi_NEOAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_NEOAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_NEOAddress_createWithPublicKey));

  // NULSAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_NULSAddress_delete"), Function::New(env, Js_wallet_core_napi_NULSAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_NULSAddress_description"), Function::New(env, Js_wallet_core_napi_NULSAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_NULSAddress_keyHash"), Function::New(env, Js_wallet_core_napi_NULSAddress_keyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_NULSAddress_equals"), Function::New(env, Js_wallet_core_napi_NULSAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_NULSAddress_isValidString"), Function::New(env, Js_wallet_core_napi_NULSAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_NULSAddress_createWithString"), Function::New(env, Js_wallet_core_napi_NULSAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_NULSAddress_createWithData"), Function::New(env, Js_wallet_core_napi_NULSAddress_createWithData));
  exports.Set(String::New(env, "Js_wallet_core_napi_NULSAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_NULSAddress_createWithPublicKey));

  // NULSSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_NULSSigner_plan"), Function::New(env, Js_wallet_core_napi_NULSSigner_plan));
  exports.Set(String::New(env, "Js_wallet_core_napi_NULSSigner_sign"), Function::New(env, Js_wallet_core_napi_NULSSigner_sign));

  // NanoAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_NanoAddress_delete"), Function::New(env, Js_wallet_core_napi_NanoAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_NanoAddress_description"), Function::New(env, Js_wallet_core_napi_NanoAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_NanoAddress_keyHash"), Function::New(env, Js_wallet_core_napi_NanoAddress_keyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_NanoAddress_equals"), Function::New(env, Js_wallet_core_napi_NanoAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_NanoAddress_isValidString"), Function::New(env, Js_wallet_core_napi_NanoAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_NanoAddress_createWithString"), Function::New(env, Js_wallet_core_napi_NanoAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_NanoAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_NanoAddress_createWithPublicKey));

  // NanoSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_NanoSigner_sign"), Function::New(env, Js_wallet_core_napi_NanoSigner_sign));

  // NebulasAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_NebulasAddress_delete"), Function::New(env, Js_wallet_core_napi_NebulasAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_NebulasAddress_description"), Function::New(env, Js_wallet_core_napi_NebulasAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_NebulasAddress_keyHash"), Function::New(env, Js_wallet_core_napi_NebulasAddress_keyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_NebulasAddress_equals"), Function::New(env, Js_wallet_core_napi_NebulasAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_NebulasAddress_isValidString"), Function::New(env, Js_wallet_core_napi_NebulasAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_NebulasAddress_createWithString"), Function::New(env, Js_wallet_core_napi_NebulasAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_NebulasAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_NebulasAddress_createWithPublicKey));

  // NebulasSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_NebulasSigner_sign"), Function::New(env, Js_wallet_core_napi_NebulasSigner_sign));

  // NimiqAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_NimiqAddress_delete"), Function::New(env, Js_wallet_core_napi_NimiqAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_NimiqAddress_description"), Function::New(env, Js_wallet_core_napi_NimiqAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_NimiqAddress_keyHash"), Function::New(env, Js_wallet_core_napi_NimiqAddress_keyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_NimiqAddress_equals"), Function::New(env, Js_wallet_core_napi_NimiqAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_NimiqAddress_isValidString"), Function::New(env, Js_wallet_core_napi_NimiqAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_NimiqAddress_createWithString"), Function::New(env, Js_wallet_core_napi_NimiqAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_NimiqAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_NimiqAddress_createWithPublicKey));

  // NimiqSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_NimiqSigner_sign"), Function::New(env, Js_wallet_core_napi_NimiqSigner_sign));

  // OntologyAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_OntologyAddress_delete"), Function::New(env, Js_wallet_core_napi_OntologyAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_OntologyAddress_description"), Function::New(env, Js_wallet_core_napi_OntologyAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_OntologyAddress_keyHash"), Function::New(env, Js_wallet_core_napi_OntologyAddress_keyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_OntologyAddress_equals"), Function::New(env, Js_wallet_core_napi_OntologyAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_OntologyAddress_isValidString"), Function::New(env, Js_wallet_core_napi_OntologyAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_OntologyAddress_createWithString"), Function::New(env, Js_wallet_core_napi_OntologyAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_OntologyAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_OntologyAddress_createWithPublicKey));

  // OntologySigner
  exports.Set(String::New(env, "Js_wallet_core_napi_OntologySigner_sign"), Function::New(env, Js_wallet_core_napi_OntologySigner_sign));

  // PKCS8
  exports.Set(String::New(env, "Js_wallet_core_napi_PKCS8_encodeED25519PrivateKey"), Function::New(env, Js_wallet_core_napi_PKCS8_encodeED25519PrivateKey));
  exports.Set(String::New(env, "Js_wallet_core_napi_PKCS8_decodeED25519PrivateKey"), Function::New(env, Js_wallet_core_napi_PKCS8_decodeED25519PrivateKey));

  // PrivateKey
  exports.Set(String::New(env, "Js_wallet_core_napi_PrivateKey_delete"), Function::New(env, Js_wallet_core_napi_PrivateKey_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_PrivateKey_getPublicKeySecp256k1"), Function::New(env, Js_wallet_core_napi_PrivateKey_getPublicKeySecp256k1));
  exports.Set(String::New(env, "Js_wallet_core_napi_PrivateKey_getPublicKeyNist256p1"), Function::New(env, Js_wallet_core_napi_PrivateKey_getPublicKeyNist256p1));
  exports.Set(String::New(env, "Js_wallet_core_napi_PrivateKey_getPublicKeyEd25519"), Function::New(env, Js_wallet_core_napi_PrivateKey_getPublicKeyEd25519));
  exports.Set(String::New(env, "Js_wallet_core_napi_PrivateKey_getPublicKeyEd25519Blake2b"), Function::New(env, Js_wallet_core_napi_PrivateKey_getPublicKeyEd25519Blake2b));
  exports.Set(String::New(env, "Js_wallet_core_napi_PrivateKey_getPublicKeyCurve25519"), Function::New(env, Js_wallet_core_napi_PrivateKey_getPublicKeyCurve25519));
  exports.Set(String::New(env, "Js_wallet_core_napi_PrivateKey_sign"), Function::New(env, Js_wallet_core_napi_PrivateKey_sign));
  exports.Set(String::New(env, "Js_wallet_core_napi_PrivateKey_signAsDER"), Function::New(env, Js_wallet_core_napi_PrivateKey_signAsDER));
  exports.Set(String::New(env, "Js_wallet_core_napi_PrivateKey_signSchnorr"), Function::New(env, Js_wallet_core_napi_PrivateKey_signSchnorr));
  exports.Set(String::New(env, "Js_wallet_core_napi_PrivateKey_data"), Function::New(env, Js_wallet_core_napi_PrivateKey_data));
  exports.Set(String::New(env, "Js_wallet_core_napi_PrivateKey_create"), Function::New(env, Js_wallet_core_napi_PrivateKey_create));
  exports.Set(String::New(env, "Js_wallet_core_napi_PrivateKey_createWithData"), Function::New(env, Js_wallet_core_napi_PrivateKey_createWithData));
  exports.Set(String::New(env, "Js_wallet_core_napi_PrivateKey_createCopy"), Function::New(env, Js_wallet_core_napi_PrivateKey_createCopy));
  exports.Set(String::New(env, "Js_wallet_core_napi_PrivateKey_isValid"), Function::New(env, Js_wallet_core_napi_PrivateKey_isValid));

  // PublicKey
  exports.Set(String::New(env, "Js_wallet_core_napi_PublicKey_delete"), Function::New(env, Js_wallet_core_napi_PublicKey_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_PublicKey_verify"), Function::New(env, Js_wallet_core_napi_PublicKey_verify));
  exports.Set(String::New(env, "Js_wallet_core_napi_PublicKey_verifySchnorr"), Function::New(env, Js_wallet_core_napi_PublicKey_verifySchnorr));
  exports.Set(String::New(env, "Js_wallet_core_napi_PublicKey_isCompressed"), Function::New(env, Js_wallet_core_napi_PublicKey_isCompressed));
  exports.Set(String::New(env, "Js_wallet_core_napi_PublicKey_compressed"), Function::New(env, Js_wallet_core_napi_PublicKey_compressed));
  exports.Set(String::New(env, "Js_wallet_core_napi_PublicKey_uncompressed"), Function::New(env, Js_wallet_core_napi_PublicKey_uncompressed));
  exports.Set(String::New(env, "Js_wallet_core_napi_PublicKey_data"), Function::New(env, Js_wallet_core_napi_PublicKey_data));
  exports.Set(String::New(env, "Js_wallet_core_napi_PublicKey_keyType"), Function::New(env, Js_wallet_core_napi_PublicKey_keyType));
  exports.Set(String::New(env, "Js_wallet_core_napi_PublicKey_description"), Function::New(env, Js_wallet_core_napi_PublicKey_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_PublicKey_createWithData"), Function::New(env, Js_wallet_core_napi_PublicKey_createWithData));
  exports.Set(String::New(env, "Js_wallet_core_napi_PublicKey_isValid"), Function::New(env, Js_wallet_core_napi_PublicKey_isValid));
  exports.Set(String::New(env, "Js_wallet_core_napi_PublicKey_recover"), Function::New(env, Js_wallet_core_napi_PublicKey_recover));

  // RippleAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_RippleAddress_delete"), Function::New(env, Js_wallet_core_napi_RippleAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_RippleAddress_description"), Function::New(env, Js_wallet_core_napi_RippleAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_RippleAddress_keyHash"), Function::New(env, Js_wallet_core_napi_RippleAddress_keyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_RippleAddress_equals"), Function::New(env, Js_wallet_core_napi_RippleAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_RippleAddress_isValidString"), Function::New(env, Js_wallet_core_napi_RippleAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_RippleAddress_createWithString"), Function::New(env, Js_wallet_core_napi_RippleAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_RippleAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_RippleAddress_createWithPublicKey));

  // RippleSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_RippleSigner_sign"), Function::New(env, Js_wallet_core_napi_RippleSigner_sign));

  // SegwitAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_SegwitAddress_delete"), Function::New(env, Js_wallet_core_napi_SegwitAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_SegwitAddress_description"), Function::New(env, Js_wallet_core_napi_SegwitAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_SegwitAddress_hrp"), Function::New(env, Js_wallet_core_napi_SegwitAddress_hrp));
  exports.Set(String::New(env, "Js_wallet_core_napi_SegwitAddress_witnessProgram"), Function::New(env, Js_wallet_core_napi_SegwitAddress_witnessProgram));
  exports.Set(String::New(env, "Js_wallet_core_napi_SegwitAddress_equals"), Function::New(env, Js_wallet_core_napi_SegwitAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_SegwitAddress_isValidString"), Function::New(env, Js_wallet_core_napi_SegwitAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_SegwitAddress_createWithString"), Function::New(env, Js_wallet_core_napi_SegwitAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_SegwitAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_SegwitAddress_createWithPublicKey));

  // SemuxAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_SemuxAddress_delete"), Function::New(env, Js_wallet_core_napi_SemuxAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_SemuxAddress_description"), Function::New(env, Js_wallet_core_napi_SemuxAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_SemuxAddress_equals"), Function::New(env, Js_wallet_core_napi_SemuxAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_SemuxAddress_isValidString"), Function::New(env, Js_wallet_core_napi_SemuxAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_SemuxAddress_createWithString"), Function::New(env, Js_wallet_core_napi_SemuxAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_SemuxAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_SemuxAddress_createWithPublicKey));

  // SemuxSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_SemuxSigner_sign"), Function::New(env, Js_wallet_core_napi_SemuxSigner_sign));

  // SteemAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_SteemAddress_delete"), Function::New(env, Js_wallet_core_napi_SteemAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_SteemAddress_description"), Function::New(env, Js_wallet_core_napi_SteemAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_SteemAddress_equals"), Function::New(env, Js_wallet_core_napi_SteemAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_SteemAddress_isValidString"), Function::New(env, Js_wallet_core_napi_SteemAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_SteemAddress_createWithString"), Function::New(env, Js_wallet_core_napi_SteemAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_SteemAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_SteemAddress_createWithPublicKey));
  exports.Set(String::New(env, "Js_wallet_core_napi_SteemAddress_createWithKeyHash"), Function::New(env, Js_wallet_core_napi_SteemAddress_createWithKeyHash));

  // SteemSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_SteemSigner_sign"), Function::New(env, Js_wallet_core_napi_SteemSigner_sign));

  // StellarAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_StellarAddress_delete"), Function::New(env, Js_wallet_core_napi_StellarAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_StellarAddress_description"), Function::New(env, Js_wallet_core_napi_StellarAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_StellarAddress_keyHash"), Function::New(env, Js_wallet_core_napi_StellarAddress_keyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_StellarAddress_equals"), Function::New(env, Js_wallet_core_napi_StellarAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_StellarAddress_isValidString"), Function::New(env, Js_wallet_core_napi_StellarAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_StellarAddress_createWithString"), Function::New(env, Js_wallet_core_napi_StellarAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_StellarAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_StellarAddress_createWithPublicKey));

  // StellarSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_StellarSigner_sign"), Function::New(env, Js_wallet_core_napi_StellarSigner_sign));

  // StoredKey
  exports.Set(String::New(env, "Js_wallet_core_napi_StoredKey_delete"), Function::New(env, Js_wallet_core_napi_StoredKey_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_StoredKey_account"), Function::New(env, Js_wallet_core_napi_StoredKey_account));
  exports.Set(String::New(env, "Js_wallet_core_napi_StoredKey_accountForCoin"), Function::New(env, Js_wallet_core_napi_StoredKey_accountForCoin));
  exports.Set(String::New(env, "Js_wallet_core_napi_StoredKey_addAccount"), Function::New(env, Js_wallet_core_napi_StoredKey_addAccount));
  exports.Set(String::New(env, "Js_wallet_core_napi_StoredKey_store"), Function::New(env, Js_wallet_core_napi_StoredKey_store));
  exports.Set(String::New(env, "Js_wallet_core_napi_StoredKey_decryptPrivateKey"), Function::New(env, Js_wallet_core_napi_StoredKey_decryptPrivateKey));
  exports.Set(String::New(env, "Js_wallet_core_napi_StoredKey_decryptMnemonic"), Function::New(env, Js_wallet_core_napi_StoredKey_decryptMnemonic));
  exports.Set(String::New(env, "Js_wallet_core_napi_StoredKey_privateKey"), Function::New(env, Js_wallet_core_napi_StoredKey_privateKey));
  exports.Set(String::New(env, "Js_wallet_core_napi_StoredKey_wallet"), Function::New(env, Js_wallet_core_napi_StoredKey_wallet));
  exports.Set(String::New(env, "Js_wallet_core_napi_StoredKey_exportJSON"), Function::New(env, Js_wallet_core_napi_StoredKey_exportJSON));
  exports.Set(String::New(env, "Js_wallet_core_napi_StoredKey_fixAddresses"), Function::New(env, Js_wallet_core_napi_StoredKey_fixAddresses));
  exports.Set(String::New(env, "Js_wallet_core_napi_StoredKey_identifier"), Function::New(env, Js_wallet_core_napi_StoredKey_identifier));
  exports.Set(String::New(env, "Js_wallet_core_napi_StoredKey_name"), Function::New(env, Js_wallet_core_napi_StoredKey_name));
  exports.Set(String::New(env, "Js_wallet_core_napi_StoredKey_isMnemonic"), Function::New(env, Js_wallet_core_napi_StoredKey_isMnemonic));
  exports.Set(String::New(env, "Js_wallet_core_napi_StoredKey_accountCount"), Function::New(env, Js_wallet_core_napi_StoredKey_accountCount));
  exports.Set(String::New(env, "Js_wallet_core_napi_StoredKey_load"), Function::New(env, Js_wallet_core_napi_StoredKey_load));
  exports.Set(String::New(env, "Js_wallet_core_napi_StoredKey_importPrivateKey"), Function::New(env, Js_wallet_core_napi_StoredKey_importPrivateKey));
  exports.Set(String::New(env, "Js_wallet_core_napi_StoredKey_importHDWallet"), Function::New(env, Js_wallet_core_napi_StoredKey_importHDWallet));
  exports.Set(String::New(env, "Js_wallet_core_napi_StoredKey_importJSON"), Function::New(env, Js_wallet_core_napi_StoredKey_importJSON));
  exports.Set(String::New(env, "Js_wallet_core_napi_StoredKey_create"), Function::New(env, Js_wallet_core_napi_StoredKey_create));

  // TezosAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_TezosAddress_delete"), Function::New(env, Js_wallet_core_napi_TezosAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_TezosAddress_description"), Function::New(env, Js_wallet_core_napi_TezosAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_TezosAddress_keyHash"), Function::New(env, Js_wallet_core_napi_TezosAddress_keyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_TezosAddress_equals"), Function::New(env, Js_wallet_core_napi_TezosAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_TezosAddress_isValidString"), Function::New(env, Js_wallet_core_napi_TezosAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_TezosAddress_deriveOriginatedAddress"), Function::New(env, Js_wallet_core_napi_TezosAddress_deriveOriginatedAddress));
  exports.Set(String::New(env, "Js_wallet_core_napi_TezosAddress_createWithString"), Function::New(env, Js_wallet_core_napi_TezosAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_TezosAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_TezosAddress_createWithPublicKey));

  // TezosSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_TezosSigner_sign"), Function::New(env, Js_wallet_core_napi_TezosSigner_sign));

  // ThetaSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_ThetaSigner_sign"), Function::New(env, Js_wallet_core_napi_ThetaSigner_sign));

  // TronAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_TronAddress_delete"), Function::New(env, Js_wallet_core_napi_TronAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_TronAddress_description"), Function::New(env, Js_wallet_core_napi_TronAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_TronAddress_equals"), Function::New(env, Js_wallet_core_napi_TronAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_TronAddress_isValidString"), Function::New(env, Js_wallet_core_napi_TronAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_TronAddress_createWithString"), Function::New(env, Js_wallet_core_napi_TronAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_TronAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_TronAddress_createWithPublicKey));

  // TronSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_TronSigner_sign"), Function::New(env, Js_wallet_core_napi_TronSigner_sign));

  // VeChainSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_VeChainSigner_sign"), Function::New(env, Js_wallet_core_napi_VeChainSigner_sign));

  // WanchainAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_WanchainAddress_delete"), Function::New(env, Js_wallet_core_napi_WanchainAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_WanchainAddress_description"), Function::New(env, Js_wallet_core_napi_WanchainAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_WanchainAddress_keyHash"), Function::New(env, Js_wallet_core_napi_WanchainAddress_keyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_WanchainAddress_equals"), Function::New(env, Js_wallet_core_napi_WanchainAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_WanchainAddress_isValidString"), Function::New(env, Js_wallet_core_napi_WanchainAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_WanchainAddress_createWithString"), Function::New(env, Js_wallet_core_napi_WanchainAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_WanchainAddress_createWithKeyHash"), Function::New(env, Js_wallet_core_napi_WanchainAddress_createWithKeyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_WanchainAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_WanchainAddress_createWithPublicKey));

  // WanchainSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_WanchainSigner_sign"), Function::New(env, Js_wallet_core_napi_WanchainSigner_sign));

  // WavesAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_WavesAddress_delete"), Function::New(env, Js_wallet_core_napi_WavesAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_WavesAddress_description"), Function::New(env, Js_wallet_core_napi_WavesAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_WavesAddress_keyHash"), Function::New(env, Js_wallet_core_napi_WavesAddress_keyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_WavesAddress_equals"), Function::New(env, Js_wallet_core_napi_WavesAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_WavesAddress_isValidString"), Function::New(env, Js_wallet_core_napi_WavesAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_WavesAddress_createWithString"), Function::New(env, Js_wallet_core_napi_WavesAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_WavesAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_WavesAddress_createWithPublicKey));

  // WavesSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_WavesSigner_sign"), Function::New(env, Js_wallet_core_napi_WavesSigner_sign));

  // X509
  exports.Set(String::New(env, "Js_wallet_core_napi_X509_encodeED25519PublicKey"), Function::New(env, Js_wallet_core_napi_X509_encodeED25519PublicKey));
  exports.Set(String::New(env, "Js_wallet_core_napi_X509_decodeED25519PublicKey"), Function::New(env, Js_wallet_core_napi_X509_decodeED25519PublicKey));

  // ZcashTAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_ZcashTAddress_delete"), Function::New(env, Js_wallet_core_napi_ZcashTAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_ZcashTAddress_description"), Function::New(env, Js_wallet_core_napi_ZcashTAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_ZcashTAddress_equals"), Function::New(env, Js_wallet_core_napi_ZcashTAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_ZcashTAddress_isValid"), Function::New(env, Js_wallet_core_napi_ZcashTAddress_isValid));
  exports.Set(String::New(env, "Js_wallet_core_napi_ZcashTAddress_isValidString"), Function::New(env, Js_wallet_core_napi_ZcashTAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_ZcashTAddress_createWithString"), Function::New(env, Js_wallet_core_napi_ZcashTAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_ZcashTAddress_createWithData"), Function::New(env, Js_wallet_core_napi_ZcashTAddress_createWithData));
  exports.Set(String::New(env, "Js_wallet_core_napi_ZcashTAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_ZcashTAddress_createWithPublicKey));

  // ZcashTransactionSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_ZcashTransactionSigner_delete"), Function::New(env, Js_wallet_core_napi_ZcashTransactionSigner_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_ZcashTransactionSigner_plan"), Function::New(env, Js_wallet_core_napi_ZcashTransactionSigner_plan));
  exports.Set(String::New(env, "Js_wallet_core_napi_ZcashTransactionSigner_sign"), Function::New(env, Js_wallet_core_napi_ZcashTransactionSigner_sign));
  exports.Set(String::New(env, "Js_wallet_core_napi_ZcashTransactionSigner_create"), Function::New(env, Js_wallet_core_napi_ZcashTransactionSigner_create));
  exports.Set(String::New(env, "Js_wallet_core_napi_ZcashTransactionSigner_createWithPlan"), Function::New(env, Js_wallet_core_napi_ZcashTransactionSigner_createWithPlan));

  // ZilliqaAddress
  exports.Set(String::New(env, "Js_wallet_core_napi_ZilliqaAddress_delete"), Function::New(env, Js_wallet_core_napi_ZilliqaAddress_delete));
  exports.Set(String::New(env, "Js_wallet_core_napi_ZilliqaAddress_description"), Function::New(env, Js_wallet_core_napi_ZilliqaAddress_description));
  exports.Set(String::New(env, "Js_wallet_core_napi_ZilliqaAddress_keyHash"), Function::New(env, Js_wallet_core_napi_ZilliqaAddress_keyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_ZilliqaAddress_equals"), Function::New(env, Js_wallet_core_napi_ZilliqaAddress_equals));
  exports.Set(String::New(env, "Js_wallet_core_napi_ZilliqaAddress_isValidString"), Function::New(env, Js_wallet_core_napi_ZilliqaAddress_isValidString));
  exports.Set(String::New(env, "Js_wallet_core_napi_ZilliqaAddress_createWithString"), Function::New(env, Js_wallet_core_napi_ZilliqaAddress_createWithString));
  exports.Set(String::New(env, "Js_wallet_core_napi_ZilliqaAddress_createWithKeyHash"), Function::New(env, Js_wallet_core_napi_ZilliqaAddress_createWithKeyHash));
  exports.Set(String::New(env, "Js_wallet_core_napi_ZilliqaAddress_createWithPublicKey"), Function::New(env, Js_wallet_core_napi_ZilliqaAddress_createWithPublicKey));

  // ZilliqaSigner
  exports.Set(String::New(env, "Js_wallet_core_napi_ZilliqaSigner_sign"), Function::New(env, Js_wallet_core_napi_ZilliqaSigner_sign));

  return exports;
}

NODE_API_MODULE(addon, Init)
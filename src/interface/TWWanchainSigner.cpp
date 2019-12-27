// // Copyright © 2017-2019 Trust Wallet.
// //
// // This file is part of Trust. The full Trust copyright notice, including
// // terms governing use, modification, and redistribution, is contained in the
// // file LICENSE at the root of the source code distribution tree.

// #include <TrustWalletCore/TWWanchainSigner.h>

// #include "../Wanchain/Signer.h"
// #include "../Ethereum/Signer.h"
// #include "../proto/Ethereum.pb.h"
// #include "../uint256.h"

// using namespace TW;
// using namespace TW::Wanchain;

// TW_Ethereum_Proto_SigningOutput TWWanchainSignerSign(TW_Ethereum_Proto_SigningInput data) {
//     Ethereum::Proto::SigningInput input;
//     input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

//     auto signer = Signer(load(input.chain_id()));
//     auto protoOutput = signer.sign(input);

//     auto serialized = protoOutput.SerializeAsString();
//     return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
// }

// TWData *_Nonnull TWWanchainSignerMessage(TW_Ethereum_Proto_SigningInput data) {
//     Ethereum::Proto::SigningInput input;
//     input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

//     auto chainId = load(input.chain_id());
//     auto signer = Signer(chainId);

//     auto transaction = Ethereum::Transaction(
//             /* nonce: */ load(input.nonce()),
//             /* gasPrice: */ load(input.gas_price()),
//             /* gasLimit: */ load(input.gas_limit()),
//             /* to: */ Ethereum::Address(input.to_address()),
//             /* amount: */ load(input.amount()),
//             /* payload: */ Data(input.payload().begin(), input.payload().end())
//     );
//     transaction.v = chainId;
//     transaction.r = 0;
//     transaction.s = 0;

//     auto serialized = signer.encode(transaction);
//     return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
// }

// TWData *_Nonnull TWWanchainSignerTransaction(TW_Ethereum_Proto_SigningInput data, TWData *_Nonnull signature) {
//     Ethereum::Proto::SigningInput input;
//     input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

//     Data sig(TWDataBytes(signature), TWDataBytes(signature) + TWDataSize(signature));

//     auto chainId = load(input.chain_id());
//     auto signer = Signer(chainId);
//     auto transaction = Ethereum::Transaction(
//             /* nonce: */ load(input.nonce()),
//             /* gasPrice: */ load(input.gas_price()),
//             /* gasLimit: */ load(input.gas_limit()),
//             /* to: */ Ethereum::Address(input.to_address()),
//             /* amount: */ load(input.amount()),
//             /* payload: */ Data(input.payload().begin(), input.payload().end())
//     );

//     auto tuple = Ethereum::Signer::values(chainId, sig);
//     transaction.r = std::get<0>(tuple);
//     transaction.s = std::get<1>(tuple);
//     transaction.v = std::get<2>(tuple);

//     auto encoded = signer.encode(transaction);
//     return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(encoded.data()), encoded.size());
// }


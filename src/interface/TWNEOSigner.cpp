// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWNEOSigner.h>

#include "../NEO/Signer.h"
#include "../proto/NEO.pb.h"
#include "../HexCoding.h"

using namespace TW;
using namespace TW::NEO;

TW_NEO_Proto_SigningOutput TWNEOSignerSign(TW_NEO_Proto_SigningInput data) {
  Proto::SigningInput input;
  input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
  auto output = Proto::SigningOutput();
  try {
	 auto signer = Signer(PrivateKey(Data(input.private_key().begin(), input.private_key().end())));
	 auto transaction = Transaction();
	 transaction.type = TransactionType::TT_ContractTransaction;
	 transaction.version = 0;

	 for (int i = 0; i < input.inputs_size(); i++) {
		CoinReference coin;
		Data prevHashReverse(input.inputs(i).prev_hash().begin(), input.inputs(i).prev_hash().end());
		std::reverse(prevHashReverse.begin(), prevHashReverse.end());
		coin.prevHash = load<uint256_t>(prevHashReverse);
		coin.prevIndex = (uint16_t)input.inputs(i).prev_index();
		transaction.inInputs.push_back(coin);
	 }

	 for (int i = 0; i < input.outputs_size(); i++) {
		TransactionOutput out;
		out.assetId = load<uint256_t>(parse_hex(input.outputs(i).asset_id()));
		out.value = (int64_t)input.outputs(i).value();
		auto scriptHash = TW::NEO::Address(input.outputs(i).address()).toScriptHash();
		out.scriptHash = load<uint256_t>(scriptHash);
		transaction.outputs.push_back(out);
	 }

	 signer.sign(transaction);
	 auto signedTx = transaction.serialize();

	 output.set_encoded(signedTx.data(), signedTx.size());
  } catch (...) {
  }

  auto serialized = output.SerializeAsString();
  return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

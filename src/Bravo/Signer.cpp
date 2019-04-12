#include "Signer.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/secp256k1.h>

using namespace TW::Bravo;

void Signer::sign(const PrivateKey& privateKey, Transaction& transaction, int(*canonicalChecker)(uint8_t by, uint8_t sig[64])) const {
    if (!transaction.isValid()) {
        throw std::invalid_argument("Invalid Transaction!");;
    }

    Data result;
    result.resize(65);
    if (ecdsa_sign_digest(&secp256k1, privateKey.bytes.data(), hash(transaction).data(), result.data() + 1, result.data(), canonicalChecker) != 0) {
        throw std::runtime_error("Signing failed.");
    }

    result[0] += 31;

    transaction.addSignature(Signature(result));
}


bool Signer::verify(const PrivateKey& privateKey, const Signature& signature, Transaction& transaction) const noexcept {
    return ecdsa_verify_digest(&secp256k1, 
                                privateKey.getPublicKey(PublicKeyType::secp256k1).bytes.data(), 
                                signature.data.data() + 1,
                                hash(transaction).data()) == 0;
}

TW::Data Signer::hash(const Transaction& transaction) const noexcept {
    Data hashInput(chainID);
    transaction.serialize(hashInput);
    return Hash::sha256(hashInput);
}

// canonical check for Bravo
int Signer::is_canonical(uint8_t by, uint8_t sig[64]) {
	return !(sig[0] & 0x80)
		&& !(sig[0] == 0 && !(sig[1] & 0x80))
		&& !(sig[32] & 0x80)
		&& !(sig[32] == 0 && !(sig[33] & 0x80));
}

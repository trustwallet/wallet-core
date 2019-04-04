#include "Signer.h"
#include "../HexCoding.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/secp256k1.h>
#include <TrezorCrypto/nist256p1.h>

using namespace TW::EOS;

void Signer::sign(const PrivateKey& privateKey, Type type, Transaction& transaction) const {
    if (!transaction.isValid()) {
        throw std::invalid_argument("Invalid transaction!");;
    }

    Data result;
    result.resize(65);

    const ecdsa_curve *curve = nullptr;
    auto canonicalChecker = is_canonical;

    switch (type) {
    case Type::Legacy:
        type = Type::ModernK1;
    case Type::ModernK1:
        curve = &secp256k1;
        break;

    case Type::ModernR1:
        curve = &nist256p1;
        canonicalChecker = nullptr;
        break;
    }

    if (ecdsa_sign_digest(curve, privateKey.bytes.data(), hash(transaction).data(), result.data() + 1, result.data(), canonicalChecker) != 0) {
        throw std::runtime_error("Signing failed.");
    }

    result[0] += 31;

    transaction.signatures.push_back(Signature(result, type));
}


bool Signer::verify(const PrivateKey& privateKey, const Signature& signature, Transaction& transaction) const noexcept {
    switch (signature.type) {
    case Type::Legacy: case Type::ModernK1:
        return ecdsa_verify_digest(&secp256k1, 
                                    privateKey.getPublicKey(PublicKeyType::secp256k1).bytes.data(), 
                                    signature.data.data() + 1,
                                    hash(transaction).data()) == 0;

    case Type::ModernR1:
        return ecdsa_verify_digest(&nist256p1, 
                                    privateKey.getPublicKey(PublicKeyType::nist256p1).bytes.data(), 
                                    signature.data.data() + 1,
                                    hash(transaction).data()) == 0;
    }
}

TW::Data Signer::hash(const Transaction& transaction) const noexcept {
    Data hashInput(chainID);
    transaction.serialize(hashInput);

    Data cfdHash;
    if (transaction.contextFreeData.size()) {
        cfdHash = Hash::sha256(transaction.contextFreeData);
    } else {
        cfdHash.assign(Hash::sha256Size, 0);
    }

    append(hashInput, cfdHash);
    return Hash::sha256(hashInput);
}

// canonical check for EOS
int Signer::is_canonical(uint8_t by, uint8_t sig[64]) {
	return !(sig[0] & 0x80)
		&& !(sig[0] == 0 && !(sig[1] & 0x80))
		&& !(sig[32] & 0x80)
		&& !(sig[32] == 0 && !(sig[33] & 0x80));
}

#include <TrustWalletCore/TWCoinType.h>
#include "WasmString.h"
#include "generated/PrivateKey.h"
#include <emscripten/bind.h>

using namespace emscripten;

namespace TW::Wasm {
    class CoinTypeExtension {
    public:
        TWCoinType mValue;
        CoinTypeExtension(TWCoinType value): mValue(value) {}

        auto value() {
            return mValue;
        }

        auto blockchain() {
            return TWCoinTypeBlockchain(mValue);
        }

        auto purpose() {
            return TWCoinTypePurpose(mValue);
        }

        auto curve() {
            return TWCoinTypeCurve(mValue);
        }

        auto xpubVersion() {
            return TWCoinTypeXpubVersion(mValue);
        }

        auto xprvVersion() {
            return TWCoinTypeXpubVersion(mValue);
        }

        auto validate(const std::string& address) {
            return TWCoinTypeValidate(mValue, &address);
        }

        auto derivationPath() {
            return TWStringToStd(TWCoinTypeDerivationPath(mValue));
        }

        auto derivationPathWithDerivation(TWDerivation derivation) {
            return TWStringToStd(TWCoinTypeDerivationPathWithDerivation(mValue, derivation));
        }

        auto deriveAddress(WasmPrivateKey* privateKey) {
            return TWStringToStd(TWCoinTypeDeriveAddress(mValue, privateKey->instance));
        }

        auto deriveAddressFromPublicKey(WasmPublicKey* publicKey) {
            return TWStringToStd(TWCoinTypeDeriveAddressFromPublicKey(mValue, publicKey->instance));
        }

        auto HRP() {
            return TWCoinTypeHRP(mValue);
        }

        auto P2pkhPrefix() {
            return TWCoinTypeP2pkhPrefix(mValue);
        }

        auto P2shPrefix() {
            return TWCoinTypeP2shPrefix(mValue);
        }

        auto staticPrefix() {
            return TWCoinTypeStaticPrefix(mValue);
        }

        auto chainID() {
            return TWStringToStd(TWCoinTypeChainId(mValue));
        }

        auto slip44ID() {
            return TWCoinTypeSlip44Id(mValue);
        }

        auto SS58Prefix() {
            return TWCoinTypeSS58Prefix(mValue);
        }

        auto publicKeyType() {
            return TWCoinTypePublicKeyType(mValue);
        }
    };

    EMSCRIPTEN_BINDINGS(Wasm_CoinTypeExtension) {
        class_<CoinTypeExtension>("CoinTypeExtension")
            .constructor<TWCoinType>()
            .function("blockchain", &CoinTypeExtension::blockchain)
            .function("purpose", &CoinTypeExtension::purpose)
            .function("curve", &CoinTypeExtension::curve)
            .function("xpubVersion", &CoinTypeExtension::xpubVersion)
            .function("xprvVersion", &CoinTypeExtension::xprvVersion)
            .function("validate", &CoinTypeExtension::validate)
            .function("derivationPath", &CoinTypeExtension::derivationPath)
            .function("derivationPathWithDerivation", &CoinTypeExtension::derivationPathWithDerivation)
            .function("deriveAddress", &CoinTypeExtension::deriveAddress, allow_raw_pointers())
            .function("deriveAddressFromPublicKey", &CoinTypeExtension::deriveAddressFromPublicKey, allow_raw_pointers())
            .function("hrp", &CoinTypeExtension::HRP)
            .function("P2pkhPrefix", &CoinTypeExtension::P2pkhPrefix)
            .function("P2shPrefix", &CoinTypeExtension::P2shPrefix)
            .function("staticPrefix", &CoinTypeExtension::staticPrefix)
            .function("chainID", &CoinTypeExtension::chainID)
            .function("slip44ID", &CoinTypeExtension::slip44ID)
            .function("SS58Prefix", &CoinTypeExtension::SS58Prefix)
            .function("publicKeyType", &CoinTypeExtension::publicKeyType)
            .function("value", &CoinTypeExtension::value);
    }
}

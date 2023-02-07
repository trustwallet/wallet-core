#include <TrustWalletCore/TWCoinType.h>
#include "WasmString.h"
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
            .function("value", &CoinTypeExtension::value);
    }
}

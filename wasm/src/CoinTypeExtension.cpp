#include <TrustWalletCore/TWCoinType.h>
#include <emscripten/bind.h>

using namespace emscripten;

namespace TW::Wasm {
    class CoinTypeExtension {
    public:
        TWCoinType mValue;
        CoinTypeExtension(TWCoinType value): mValue(value) {}
        TWBlockchain blockchain() {
            return TWCoinTypeBlockchain(mValue);
        }

        TWPurpose purpose() {
            return TWCoinTypePurpose(mValue);
        }

        TWCoinType value() {
            return mValue;
        }
    };

    EMSCRIPTEN_BINDINGS(Wasm_CoinTypeExtension) {
        class_<CoinTypeExtension>("CoinTypeExtension")
            .constructor<TWCoinType>()
            .function("blockchain", &CoinTypeExtension::blockchain)
            .function("purpose", &CoinTypeExtension::purpose)
            .function("value", &CoinTypeExtension::value);
    }
}

#include <TrustWalletCore/TWEOSSigner.h>

#include "../EOS/Signer.h"
#include "../Data.h"

#include <string>

using namespace TW;
using namespace TW::EOS;

struct TWEOSSigner *_Nullable TWEOSSignerCreate(TWData *_Nonnull chainID) {
    auto id = reinterpret_cast<const Data *>(chainID);
    return new TWEOSSigner { Signer(*id) };
}

void TWEOSSignerSign(struct TWEOSSigner *_Nonnull signer, struct TWPrivateKey *_Nonnull privateKey, enum TWEOSKeyType type, struct TWEOSTransaction *_Nonnull transaction) {
    Type eosType;
    switch (type)
    {
        case TWEOSKeyTypeLegacy:
            eosType = Type::Legacy;
            break;
    
        case TWEOSKeyTypeModernK1:
            eosType = Type::ModernK1;
            break;
    
        case TWEOSKeyTypeModernR1:
            eosType = Type::ModernR1;
            break;
    }

    signer->impl.sign(privateKey->impl, eosType, transaction->impl);
}

void TWEOSSignerDelete(TWEOSSigner *_Nonnull signer) {
    delete signer;
}
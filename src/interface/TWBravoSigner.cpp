#include <TrustWalletCore/TWBravoSigner.h>

#include "../Bravo/Signer.h"
#include "../Data.h"

#include <string>

using namespace TW;
using namespace TW::Bravo;

struct TWBravoSigner *_Nullable TWBravoSignerCreate(TWData *_Nonnull chainID) {
    auto id = reinterpret_cast<const Data *>(chainID);
    return new TWBravoSigner { Signer(*id) };
}

void TWBravoSignerSign(struct TWBravoSigner *_Nonnull signer, struct TWPrivateKey *_Nonnull privateKey, struct TWBravoTransaction *_Nonnull transaction) {
    signer->impl.sign(privateKey->impl, transaction->impl);
}

void TWBravoSignerDelete(TWBravoSigner *_Nonnull signer) {
    delete signer;
}
#include <TrustWalletCore/TWBravoTransaction.h>

#include "../Bravo/Transaction.h"

#include<string>

using namespace TW;
using namespace TW::Bravo;

struct TWBravoTransaction *_Nullable TWBravoTransactionCreate(TWString *_Nonnull referenceBlockId, uint32_t referenceBlockTime) {
    auto refBlockId = reinterpret_cast<const std::string *>(referenceBlockId);
    try {
        return new TWBravoTransaction { Transaction(*refBlockId, referenceBlockTime) };
    } catch (...) {
        return nullptr;
    }
}

void TWBravoTransactionAddOperation(struct TWBravoTransaction *_Nonnull transaction, struct TWBravoOperation *_Nonnull operation) {
    transaction->impl.addOperation(operation->implP);
}

TWString *_Nonnull TWBravoTransactionDescription(struct TWBravoTransaction *_Nonnull transaction) {
    return TWStringCreateWithUTF8Bytes(transaction->impl.serialize().dump().c_str());
}

void TWBravoTransactionDelete(struct TWBravoTransaction *_Nonnull transaction) {
    delete transaction;
}

bool TWBravoTransactionIsValid(struct TWBravoTransaction *_Nonnull transaction) {
    return transaction->impl.isValid();
}
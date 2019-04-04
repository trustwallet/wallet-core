#include <TrustWalletCore/TWEOSPackedTransaction.h>

#include "../EOS/PackedTransaction.h"

#include<string>

using namespace TW;
using namespace TW::EOS;

struct TWEOSPackedTransaction *_Nonnull TWEOSPackedTransactionCreate(struct TWEOSTransaction *_Nonnull transaction) {
    return new TWEOSPackedTransaction {PackedTransaction(transaction->impl, CompressionType::None)};
}

TWString *_Nonnull TWEOSPackedTransactionDescription(struct TWEOSPackedTransaction *_Nonnull transaction) {
    return TWStringCreateWithUTF8Bytes(transaction->impl.serialize().dump().c_str());
}

void TWEOSPackedTransactionDelete(struct TWEOSPackedTransaction *_Nonnull packedTransaction) {
    delete packedTransaction;
}
#include "Operation.h"

#include <stdexcept>

using namespace TW::Bitshares;
using json = nlohmann::json;

TransferOperation::TransferOperation(uint64_t senderId, 
                                    uint64_t recipientId,
                                    const Asset& amount,
                                    const Asset& fee,
                                    const Memo& memo) 
                                    : from({1, 2, senderId}),
                                      to({1, 2, recipientId}),
                                      amount(amount),
                                      fee(fee),
                                      memo( memo) 
{
    validate();
}

void TransferOperation::validate() {
    if (fee.amount < 0) {
        throw std::invalid_argument("Fee cannot be negative.");
    }

    if (from == to) {
        throw std::invalid_argument("Sender and recipient cannot be the same.");
    }

    if (amount.amount <= 0) {
        throw std::invalid_argument("Amount in a transfer operation must be positive.");
    }
}

void TransferOperation::serialize(Data& os) const noexcept override {

}
json TransferOperation::serialize() const noexcept override;

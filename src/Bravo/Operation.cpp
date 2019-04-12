#include "Operation.h"

#include <stdexcept>

using namespace TW::Bravo;

using json = nlohmann::json;

/// Transfer Operation
TransferOperation::TransferOperation(std::string from, std::string to, Asset asset, std::string memo)
                         : from(from), to(to), asset(asset), memo(memo) {
	validate();
}

TransferOperation::TransferOperation(std::string from, std::string to, int64_t amount, bool isTestNet, std::string memo) 
                    : TransferOperation(from, to, Asset(amount, isTestNet), memo) { }

TransferOperation::TransferOperation(std::string from, std::string to, std::string asset, std::string memo)     
                    : TransferOperation(from, to, Asset::fromString(asset), memo) { }

void TransferOperation::validate()
{
	if (from.size() > MaxAccountNameSize) {
		throw std::invalid_argument("\"from\" cannot be greater than " + std::to_string(MaxAccountNameSize));
	}

	if (to.size() > MaxAccountNameSize) {
		throw std::invalid_argument("\"to\" cannot be greater than " + std::to_string(MaxAccountNameSize));
	}

	if (memo.size() > MaxMemoSize) {
		throw std::invalid_argument("\"memo\" cannot be greater than " + std::to_string(MaxMemoSize));
	}

	if (asset.amount <= 0) {
		throw std::invalid_argument("\"amount\" Cannot transfer a negative amount (aka: stealing)");
	}
}

void TransferOperation::serialize(Data& os) const noexcept {
    encodeVarInt32(TransferOperation::OperationId, os);
    encodeString(from, os);
    encodeString(to, os);
    asset.serialize(os);
    encodeString(memo, os);
}

json TransferOperation::serialize() const noexcept {
    json data;
    data["from"] = from;
    data["to"] = to;
    data["amount"] = asset.string();
    data["memo"] = memo;

    return json::array( {"transfer", data} );
}
// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "AddressV3.h"

#include "Cbor.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <numeric>
#include <vector>

namespace TW::Cardano {

static const Data placeholderPrivateKey = parse_hex("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f");
static const auto PlaceholderFee = 170000;
static const auto ExtraInputAmount = 500000;

Proto::SigningOutput Signer::sign() {
    // plan if needed
    if (input.has_plan()) {
        _plan = TransactionPlan::fromProto(input.plan());
    } else {
        // no plan supplied, plan it
        _plan = doPlan();
    }

    return signWithPlan();
}

Common::Proto::SigningError Signer::buildTransactionAux(Transaction& tx, const Proto::SigningInput& input, const TransactionPlan& plan) {
    tx = Transaction();
    for (const auto& i : plan.utxos) {
        tx.inputs.emplace_back(i.txHash, i.outputIndex);
    }

    // Spending output
    if (!AddressV3::isValidLegacy(input.transfer_message().to_address())) {
        return Common::Proto::Error_invalid_address;
    }
    const auto toAddress = AddressV3(input.transfer_message().to_address());
    tx.outputs.emplace_back(toAddress.data(), plan.amount, plan.outputTokens);

    for (auto& output: plan.extraOutputs) {
        const auto extraToAddress = AddressV3(output.address);
        tx.outputs.emplace_back(extraToAddress.data(), output.amount, output.tokenBundle);
    }

    // Change
    bool hasChangeToken = any_of(plan.changeTokens.bundle.begin(), plan.changeTokens.bundle.end(), [](auto&& t) { return t.second.amount > 0; });
    if (plan.change > 0 || hasChangeToken) {
        if (!AddressV3::isValidLegacy(input.transfer_message().change_address())) {
            return Common::Proto::Error_invalid_address;
        }
        const auto changeAddress = AddressV3(input.transfer_message().change_address());
        tx.outputs.emplace_back(changeAddress.data(), plan.change, plan.changeTokens);
    }
    tx.fee = plan.fee;
    tx.ttl = input.ttl();

    if (input.has_register_staking_key()) {
        const auto stakingAddress = AddressV3(input.register_staking_key().staking_address());
        // here we need the bare staking key
        const auto key = stakingAddress.bytes;
        tx.certificates.emplace_back(Certificate{Certificate::SkatingKeyRegistration, {CertificateKey{CertificateKey::AddressKeyHash, key}}, Data()});
    }
    if (input.has_delegate()) {
        const auto stakingAddress = AddressV3(input.delegate().staking_address());
        // here we need the bare staking key
        const auto key = stakingAddress.bytes;
        const auto poolId = data(input.delegate().pool_id());
        tx.certificates.emplace_back(Certificate{Certificate::Delegation, {CertificateKey{CertificateKey::AddressKeyHash, key}}, poolId});
    }
    if (input.has_withdraw()) {
        const auto stakingAddress = AddressV3(input.withdraw().staking_address());
        const auto key = stakingAddress.data();
        const auto amount = input.withdraw().withdraw_amount();
        tx.withdrawals.emplace_back(Withdrawal{key, amount});
    }
    if (input.has_deregister_staking_key()) {
        const auto stakingAddress = AddressV3(input.deregister_staking_key().staking_address());
        // here we need the bare staking key
        const auto key = stakingAddress.bytes;
        tx.certificates.emplace_back(Certificate{Certificate::StakingKeyDeregistration, {CertificateKey{CertificateKey::AddressKeyHash, key}}, Data()});
    }

    return Common::Proto::OK;
}

Data deriveStakingPrivateKey(const Data& privateKeyData) {
    if (privateKeyData.size() != PrivateKey::cardanoKeySize) {
        return {};
    }
    assert(privateKeyData.size() == PrivateKey::cardanoKeySize);
    const auto halfSize = PrivateKey::cardanoKeySize / 2;
    auto stakingPrivKeyData = TW::subData(privateKeyData, halfSize);
    TW::append(stakingPrivKeyData, TW::Data(halfSize));
    return stakingPrivKeyData;
}

Common::Proto::SigningError Signer::assembleSignatures(std::vector<std::pair<Data, Data>>& signatures, const Proto::SigningInput& input, const TransactionPlan& plan, const Data& txId, bool sizeEstimationOnly) {
    signatures.clear();
    // Private keys and corresponding addresses
    std::map<std::string, Data> privateKeys;
    for (auto i = 0; i < input.private_key_size(); ++i) {
        const auto privateKeyData = data(input.private_key(i));
        if (!PrivateKey::isValid(privateKeyData)) {
            return Common::Proto::Error_invalid_private_key;
        }

        // Add this private key and associated address
        const auto privateKey = PrivateKey(privateKeyData);
        const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Cardano);
        const auto address = AddressV3(publicKey);
        privateKeys[address.string()] = privateKeyData;

        const auto legacyAddress = AddressV2(publicKey);
        privateKeys[legacyAddress.string()] = privateKeyData;

        // Also add the derived staking private key (the 2nd half) and associated address; because staking keys also need signature
        const auto stakingPrivKeyData = deriveStakingPrivateKey(privateKeyData);
        if (!stakingPrivKeyData.empty()) {
            privateKeys[address.getStakingAddress()] = stakingPrivKeyData;
        }
    }

    // collect every unique input UTXO address, preserving order
    std::vector<std::string> addresses;
    for (auto& u : plan.utxos) {
        if (!AddressV3::isValidLegacy(u.address)) {
            return Common::Proto::Error_invalid_address;
        }
        addresses.emplace_back(u.address);
    }
    // Staking key is also an address that needs signature
    if (input.has_register_staking_key()) {
        addresses.emplace_back(input.register_staking_key().staking_address());
    }
    if (input.has_deregister_staking_key()) {
        addresses.emplace_back(input.deregister_staking_key().staking_address());
    }
    if (input.has_delegate()) {
        addresses.emplace_back(input.delegate().staking_address());
    }
    if (input.has_withdraw()) {
        addresses.emplace_back(input.withdraw().staking_address());
    }
    // discard duplicates (std::set, std::copy_if, std::unique does not work well here)
    std::vector<std::string> addressesUnique;
    for (auto& a: addresses) {
        if (find(addressesUnique.begin(), addressesUnique.end(), a) == addressesUnique.end()) {
            addressesUnique.emplace_back(a);
        }
    }

    // create signature for each address
    for (auto& a : addressesUnique) {
        const auto privKeyFind = privateKeys.find(a);
        Data privateKeyData;
        if (privKeyFind != privateKeys.end()) {
            privateKeyData = privKeyFind->second;
        } else {
            // private key not found
            if (sizeEstimationOnly) {
                privateKeyData = placeholderPrivateKey;
            } else {
                return Common::Proto::Error_missing_private_key;
            }
        }
        const auto privateKey = PrivateKey(privateKeyData);
        const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519Cardano);
        const auto signature = privateKey.sign(txId, TWCurveED25519ExtendedCardano);
        signatures.emplace_back(publicKey.bytes, signature);
    }

    return Common::Proto::OK;
}

Cbor::Encode cborizeSignatures(const std::vector<std::pair<Data, Data>>& signatures, const bool addByronSignatures) {
    std::map<Cbor::Encode, Cbor::Encode> cborizeSigs;
    // signatures as Cbor
    // clang-format off
    std::vector<Cbor::Encode> sigsShelly;
    std::vector<Cbor::Encode> sigsByron;

    for (auto& s : signatures) {
        sigsShelly.emplace_back(Cbor::Encode::array({
            // public key (first 32 bytes)
            Cbor::Encode::bytes(subData(s.first, 0, 32)),
            Cbor::Encode::bytes(s.second)
        }));

        if (addByronSignatures) {
            sigsByron.emplace_back(Cbor::Encode::array({
                // skey - public key (first 32 bytes)  
                Cbor::Encode::bytes(subData(s.first, 0, 32)),
                Cbor::Encode::bytes(s.second),
                // vkey - public key (second 32 bytes started from 32)
                Cbor::Encode::bytes(subData(s.first, 32, 32)),
                // payload
                Cbor::Encode::bytes(parse_hex("A0"))
            }));
        }
    }

    cborizeSigs.emplace(
        Cbor::Encode::uint(0),
        Cbor::Encode::array(sigsShelly)
    );

    if (!sigsByron.empty()) {
        cborizeSigs.emplace(
            Cbor::Encode::uint(2),
            Cbor::Encode::array(sigsByron)
        );
    }

    // Cbor-encode txAux & signatures
    return Cbor::Encode::map(cborizeSigs);
    // clang-format on
}

Proto::SigningOutput Signer::signWithPlan() const {
    auto ret = Proto::SigningOutput();
    if (_plan.error != Common::Proto::OK) {
        // plan has error
        ret.set_error(_plan.error);
        return ret;
    }

    Data encoded;
    Data txId;
    const auto buildRet = encodeTransaction(encoded, txId, input, _plan);
    if (buildRet != Common::Proto::OK) {
        ret.set_error(buildRet);
        return ret;
    }

    ret.set_encoded(std::string(encoded.begin(), encoded.end()));
    ret.set_tx_id(std::string(txId.begin(), txId.end()));
    ret.set_error(Common::Proto::OK);

    return ret;
}

Common::Proto::SigningError Signer::encodeTransaction(Data& encoded, Data& txId, const Proto::SigningInput& input, const TransactionPlan& plan, bool sizeEstimationOnly) {
    if (plan.error != Common::Proto::OK) {
        return plan.error;
    }

    Transaction txAux;
    const auto buildRet = buildTransactionAux(txAux, input, plan);
    if (buildRet != Common::Proto::OK) {
        return buildRet;
    }
    txId = txAux.getId();

    std::vector<std::pair<Data, Data>> signatures;
    const auto sigError = assembleSignatures(signatures, input, plan, txId, sizeEstimationOnly);
    if (sigError != Common::Proto::OK) {
        return sigError;
    }

    bool hasLegacyUtxos = false;
    for (const auto& utxo : input.utxos()) {
        if (AddressV2::isValid(utxo.address())) {
            hasLegacyUtxos = true;
            break;
        }
    }

    const auto sigsCbor = cborizeSignatures(signatures, hasLegacyUtxos);

    // Cbor-encode txAux & signatures
    const auto cbor = Cbor::Encode::array({
        // txaux
        Cbor::Encode::fromRaw(txAux.encode()),
        // signatures
        sigsCbor,
        // aux data
        Cbor::Encode::null(),
    });
    encoded = cbor.encoded();
    return Common::Proto::OK;
}

// Select a subset of inputs, to cover desired coin amount. Simple algorithm: pick the largest ones.
std::vector<TxInput> selectInputsSimpleNative(const std::vector<TxInput>& inputs, Amount amount) {
    auto ii = std::vector<TxInput>(inputs);
    sort(ii.begin(), ii.end(), [](auto&& t1, auto&& t2) {
        return t1.amount > t2.amount;
    });
    auto selected = std::vector<TxInput>();
    Amount selectedAmount = 0;

    for (const auto& i : ii) {
        selected.emplace_back(i);
        selectedAmount += i.amount;
        if (selectedAmount >= amount) {
            break;
        }
    }
    return selected;
}

// Select a subset of inputs, to cover desired token amount. Simple algorithm: pick the largest ones.
void selectInputsSimpleToken(const std::vector<TxInput>& inputs, std::string key, const uint256_t& amount, std::vector<TxInput>& selectedInputs) {
    auto accumulateFunctor = [key]([[maybe_unused]] auto&& sum, auto&& si) { return si.tokenBundle.getAmount(key); };
    uint256_t selectedAmount = std::accumulate(selectedInputs.begin(), selectedInputs.end(), uint256_t(0), accumulateFunctor);
    if (selectedAmount >= amount) {
        return; // already covered
    }
    // sort inputs descending
    auto ii = std::vector<TxInput>(inputs);
    std::sort(ii.begin(), ii.end(), [key](auto&& t1, auto&& t2) { return t1.tokenBundle.getAmount(key) > t2.tokenBundle.getAmount(key); });
    for (const auto& i : ii) {
        if (static_cast<std::size_t>(distance(selectedInputs.begin(), find(selectedInputs.begin(), selectedInputs.end(), i))) < selectedInputs.size()) {
            // already selected
            continue;
        }
        selectedInputs.emplace_back(i);
        selectedAmount += i.amount;
        if (selectedAmount >= amount) {
            return;
        }
    }
    // not enough
}

// Select a subset of inputs, to cover desired amount. Simple algorithm: pick the largest ones
std::vector<TxInput> Signer::selectInputsWithTokens(const std::vector<TxInput>& inputs, Amount amount, const TokenBundle& requestedTokens) {
    auto selected = selectInputsSimpleNative(inputs, amount);
    for (auto&& [_, curAmount] : requestedTokens.bundle) {
        selectInputsSimpleToken(inputs, curAmount.key(), curAmount.amount, selected);
    }
    return selected;
}

// Create a simple plan, used for estimation
TransactionPlan simplePlan(Amount amount, const TokenBundle& requestedTokens, const std::vector<TxInput>& selectedInputs, bool maxAmount, uint64_t deposit, uint64_t undeposit, const std::vector<TxOutput>& extraOutputs) {
    TransactionPlan plan{.utxos = selectedInputs, .extraOutputs = extraOutputs, .amount = amount, .deposit = deposit, .undeposit = undeposit};
    // Sum availableAmount
    plan.availableAmount = 0;
    for (auto& u : plan.utxos) {
        plan.availableAmount += u.amount;
        for (auto && [_, curAmount] : u.tokenBundle.bundle) {
            plan.availableTokens.add(curAmount);
        }
    }
    plan.fee = PlaceholderFee; // placeholder value
    const auto availAfterDeposit = plan.availableAmount + plan.undeposit - plan.deposit;
    // adjust/compute output amount and output tokens
    if (!maxAmount) {
        // reduce amount if needed
        plan.amount = std::max(Amount(0), std::min(plan.amount, availAfterDeposit - plan.fee));
        plan.outputTokens = requestedTokens;
    } else {
        // max available amount
        plan.amount = std::max(Amount(0), availAfterDeposit - plan.fee);
        plan.outputTokens = plan.availableTokens; // use all
    }

    // compute change
    plan.change = availAfterDeposit - (plan.amount + plan.fee);
    for (auto iter = plan.availableTokens.bundle.begin(); iter != plan.availableTokens.bundle.end(); ++iter) {
        const auto key = iter->second.key();
        const auto changeAmount = iter->second.amount - plan.outputTokens.getAmount(key);
        assert(changeAmount >= 0);
        plan.changeTokens.bundle[key] = iter->second;
        plan.changeTokens.bundle[key].amount = changeAmount;
    }
    return plan;
}

uint64_t sumDeposits(const Proto::SigningInput& input) {
    uint64_t sum = 0;
    if (input.has_register_staking_key()) {
        sum += input.register_staking_key().deposit_amount();
    }
    if (input.has_delegate()) {
        sum += input.delegate().deposit_amount();
    }
    return sum;
}

uint64_t sumUndeposits(const Proto::SigningInput& input) {
    uint64_t sum = 0;
    if (input.has_deregister_staking_key()) {
        sum += input.deregister_staking_key().undeposit_amount();
    }
    if (input.has_withdraw()) {
        sum += input.withdraw().withdraw_amount();
    }
    return sum;
}

// Estimates size of transaction in bytes.
uint64_t estimateTxSize(const Proto::SigningInput& input, Amount amount, const TokenBundle& requestedTokens, const std::vector<TxInput>& selectedInputs, const std::vector<TxOutput>& extraOutputs) {
    auto inputs = std::vector<TxInput>();
    for (auto i = 0; i < input.utxos_size(); ++i) {
        inputs.emplace_back(TxInput::fromProto(input.utxos(i)));
    }
    const auto deposits = sumDeposits(input);
    const uint64_t undeposits = sumUndeposits(input);
    const auto _simplePlan = simplePlan(amount, requestedTokens, selectedInputs, input.transfer_message().use_max_amount(), deposits, undeposits, extraOutputs);

    Data encoded;
    Data txId;
    const auto encodeError = Signer::encodeTransaction(encoded, txId, input, _simplePlan, true);
    if (encodeError != Common::Proto::OK) {
        return 0;
    }

    return encoded.size();
}

// Compute fee from tx size, with some over-estimation
Amount txFeeFunction(uint64_t txSizeInBytes) {
    const double fixedTerm = 155381 + 500;
    const double linearTerm = 43.946 + 0.1;

    const auto fee = (Amount)(ceil(fixedTerm + (double)txSizeInBytes * linearTerm));
    return fee;
}

Amount Signer::estimateFee(const Proto::SigningInput& input, Amount amount, const TokenBundle& requestedTokens, const std::vector<TxInput>& selectedInputs, const std::vector<TxOutput>& extraOutputs) {
    return txFeeFunction(estimateTxSize(input, amount, requestedTokens, selectedInputs, extraOutputs));
}

TransactionPlan Signer::doPlan() const {
    auto plan = TransactionPlan();

    bool maxAmount = input.transfer_message().use_max_amount();
    if (input.transfer_message().amount() == 0 && !maxAmount && input.transfer_message().token_amount().token_size() == 0) {
        plan.error = Common::Proto::Error_zero_amount_requested;
        return plan;
    }
    // Check input UTXOs, process, sum ADA and token amounts
    auto utxos = std::vector<TxInput>();
    uint64_t inputSum = 0;
    for (auto i = 0; i < input.utxos_size(); ++i) {
        const auto& utxo = input.utxos(i);
        utxos.emplace_back(TxInput::fromProto(utxo));
        inputSum += utxo.amount();
    }
    if (inputSum == 0 || input.utxos_size() == 0) {
        plan.error = Common::Proto::Error_missing_input_utxos;
        return plan;
    }
    assert(inputSum > 0);
    // adjust inputSum with deposited/undeposited amount
    plan.deposit = sumDeposits(input);
    plan.undeposit = sumUndeposits(input);
    const auto inputSumAfterDeposit = inputSum + plan.undeposit - plan.deposit;

    // Amounts requested
    plan.amount = input.transfer_message().amount();

    uint64_t extraAmountSum = 0;
    auto extraOutputs = std::vector<TxOutput>();
    for (auto& output: input.extra_outputs()) {
        const auto extraToAddress = AddressV3(output.address());
        extraOutputs.emplace_back(extraToAddress.data(), output.amount());
        extraAmountSum = extraAmountSum + output.amount();
    }
    plan.extraOutputs = extraOutputs;

    TokenBundle requestedTokens;
    for (auto i = 0; i < input.transfer_message().token_amount().token_size(); ++i) {
        const auto token = TokenAmount::fromProto(input.transfer_message().token_amount().token(i));
        requestedTokens.add(token);
    }
    assert(plan.amount > 0 || maxAmount || input.transfer_message().token_amount().token_size() > 0);
    if (requestedTokens.size() > 1) {
        // We support transfer of only one coin (for simplicity; inputs may contain more coins which are preserved)
        plan.error = Common::Proto::Error_invalid_requested_token_amount;
        return plan;
    }

    // if amount requested is the same or more than available amount, it cannot be satisfied, but
    // treat this case as MaxAmount, and send maximum available (which will be less)
    if (!maxAmount && input.transfer_message().amount() >= inputSumAfterDeposit) {
        maxAmount = true;
    }

    // select UTXOs
    if (!maxAmount) {
        // aim for larger total input, enough for 4/3 of the target amount plus typical fee plus minimal ADA for change plus some extra
        auto targetInputAmount = (plan.amount * 4) / 3 + plan.deposit - plan.undeposit + PlaceholderFee + requestedTokens.minAdaAmount() + ExtraInputAmount;
        plan.utxos = selectInputsWithTokens(utxos, targetInputAmount, requestedTokens);
    } else {
        // maxAmount, select all
        plan.utxos = utxos;
    }
    assert(!plan.utxos.empty());

    // Sum availableAmount
    plan.availableAmount = 0;
    for (auto& u : plan.utxos) {
        plan.availableAmount += u.amount;
        for (auto && [_, curAmount] : u.tokenBundle.bundle) {
            plan.availableTokens.add(curAmount);
        }
    }
    if (plan.availableAmount == 0) {
        plan.error = Common::Proto::Error_missing_input_utxos;
        return plan;
    }
    assert(plan.availableAmount > 0);
    // adjust availableAmount with deposited/undeposited amount
    const auto availableAmountAfterDeposit = plan.availableAmount + plan.undeposit - plan.deposit;

    // check that there are enough coins in the inputs
    if (plan.amount > availableAmountAfterDeposit) {
        plan.error = Common::Proto::Error_low_balance;
        return plan;
    }
    assert(plan.amount <= availableAmountAfterDeposit);
    // check that there are enough tokens in the inputs
    for (auto && [_, curAmount] : requestedTokens.bundle) {
        if (curAmount.amount > plan.availableTokens.getAmount(curAmount.key())) {
            plan.error = Common::Proto::Error_low_balance;
            return plan;
        }
    }

    // compute fee
    if (input.transfer_message().force_fee() == 0) {
        plan.fee = estimateFee(input, plan.amount, requestedTokens, plan.utxos, plan.extraOutputs);
    } else {
        // fee provided, use it (capped)
        plan.fee = std::max(Amount(0), std::min(availableAmountAfterDeposit - plan.amount - extraAmountSum, input.transfer_message().force_fee()));
    }
    assert(plan.fee >= 0 && plan.fee < availableAmountAfterDeposit);

    // adjust/compute output amount
    if (!maxAmount) {
        // reduce amount if needed
        plan.amount = std::max(Amount(0), std::min(plan.amount, availableAmountAfterDeposit - plan.fee - extraAmountSum));
    } else {
        // max available amount
        plan.amount = std::max(Amount(0), availableAmountAfterDeposit - plan.fee - extraAmountSum);
    }
    assert(plan.amount >= 0 && plan.amount <= availableAmountAfterDeposit);

    if (plan.amount + extraAmountSum + plan.fee > availableAmountAfterDeposit) {
        plan.error = Common::Proto::Error_low_balance;
        return plan;
    }
    assert(plan.amount + extraAmountSum + plan.fee <= availableAmountAfterDeposit);

    // compute output token amounts
    if (!maxAmount) {
        plan.outputTokens = requestedTokens;
    } else {
        plan.outputTokens = plan.availableTokens; // send all
    }

    // compute change
    plan.change = availableAmountAfterDeposit - (plan.amount + extraAmountSum + plan.fee);
    for (auto iter = plan.availableTokens.bundle.begin(); iter != plan.availableTokens.bundle.end(); ++iter) {
        const auto key = iter->second.key();
        const auto changeAmount = iter->second.amount - plan.outputTokens.getAmount(key);
        if (changeAmount > 0) { // omit 0-amount tokens
            plan.changeTokens.bundle[key] = iter->second;
            plan.changeTokens.bundle[key].amount = changeAmount;
        }
    }

    assert(plan.change >= 0 && plan.change <= availableAmountAfterDeposit);
    assert(!maxAmount || plan.change == 0); // change is 0 in max amount case
    assert(plan.amount + extraAmountSum+ plan.change + plan.fee == availableAmountAfterDeposit);
    assert(plan.amount + extraAmountSum+ plan.change + plan.fee + plan.deposit == plan.availableAmount + plan.undeposit);

    return plan;
}


Data Signer::encodeTransactionWithSig(const Proto::SigningInput &input, const PublicKey &publicKey, const Data &signature) {
    Transaction txAux;
    auto buildRet = buildTx(txAux, input);
    if (buildRet != Common::Proto::OK) {
        throw Common::Proto::SigningError(buildRet);
    }

    std::vector<std::pair<Data, Data>> signatures;
    signatures.emplace_back(publicKey.bytes, signature);

    bool hasLegacyUtxos = false;
    for (const auto& utxo : input.utxos()) {
        if (AddressV2::isValid(utxo.address())) {
            hasLegacyUtxos = true;
            break;
        }
    }

    const auto sigsCbor = cborizeSignatures(signatures, hasLegacyUtxos);

    // Cbor-encode txAux & signatures
    const auto cbor = Cbor::Encode::array({
        // txaux
        Cbor::Encode::fromRaw(txAux.encode()),
        // signatures
        sigsCbor,
        // aux data
        Cbor::Encode::null(),
    });

    return cbor.encoded();
}

Common::Proto::SigningError Signer::buildTx(Transaction& tx, const Proto::SigningInput& input) {
    auto plan = Signer(input).doPlan();
    return buildTransactionAux(tx, input, plan);
}

} // namespace TW::Cardano

// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "ParamBase.h"

#include <vector>
#include <string>
#include <memory>

namespace TW::Ethereum::ABI {

/// A named parameter.
class ParamNamed final : public ParamBase
{
public:
    std::string _name;
    std::shared_ptr<ParamBase> _param;

public:
    explicit ParamNamed(const std::string& name, std::shared_ptr<ParamBase> param): _name(name), _param(param) {}

    std::string getName() const { return _name; }
    std::shared_ptr<ParamBase> getParam() const { return _param; }
    std::string getType() const override;
    size_t getSize() const override { return _param->getSize(); }
    bool isDynamic() const override { return _param->isDynamic(); }
    void encode(Data& data) const override { return _param->encode(data); }
    bool decode(const Data& encoded, size_t& offset_inout) override { return _param->decode(encoded, offset_inout); }
    bool setValueJson(const std::string& value) override { return _param->setValueJson(value); }
    Data hashStruct() const override { return _param->hashStruct(); }
    std::string getExtraTypes(std::vector<std::string>& ignoreList) const override { return _param->getExtraTypes(ignoreList); }
    std::shared_ptr<ParamBase> clone() const override { return cloneNamed(); }
    std::shared_ptr<ParamNamed> cloneNamed() const;
};

/// A collection of named parameters.  See also: ParamStruct
class ParamSetNamed {
private:
    std::vector<std::shared_ptr<ParamNamed>> _params;

public:
    ParamSetNamed() = default;
    ParamSetNamed(const std::shared_ptr<ParamNamed>& param1) { addParam(param1); }
    ParamSetNamed(const std::vector<std::shared_ptr<ParamNamed>>& params) { addParams(params); }
    virtual ~ParamSetNamed();

    /// Returns the index of the parameter
    int addParam(const std::shared_ptr<ParamNamed>& param);
    void addParams(const std::vector<std::shared_ptr<ParamNamed>>& params);
    size_t getCount() const { return _params.size(); }
    std::shared_ptr<ParamNamed> getParam(int idx) const { return _params[idx]; }
    std::string getType() const;
    Data encodeHashes() const;
    std::string getExtraTypes(std::vector<std::string>& ignoreList) const;
    std::shared_ptr<ParamNamed> findParamByName(const std::string& name) const;
    ParamSetNamed clone() const;
};

/// A named structure (set of parameters plus a type name).
class ParamStruct final : public ParamCollection
{
private:
    std::string _name;
    ParamSetNamed _params;

public:
    ParamStruct() = default;
    ParamStruct(const std::string& name, const std::vector<std::shared_ptr<ParamNamed>>& params) : ParamCollection(), _name(name), _params(ParamSetNamed(params)) {}
    ParamStruct(const std::string& name, ParamSetNamed&& params) : ParamCollection(), _name(name), _params(std::move(params)) {}

    std::string getType() const override { return _name; }
    const ParamSetNamed& getParams() const { return _params; }

    /// Compute the hash of a struct, used for signing, according to EIP712
    Data hashStruct() const override;
    /// Get full type, extended by used sub-types, of the form 'Mail(Person from,Person to,string contents)Person(string name,address wallet)'
    std::string encodeType() const {
        std::vector<std::string> ignoreList;
        return getExtraTypes(ignoreList);
    }
    /// Get the hash of the full type.
    Data hashType() const;

    size_t getSize() const override { return _params.getCount(); }
    bool isDynamic() const override { return true; }
    size_t getCount() const override { return _params.getCount(); }
    void encode([[maybe_unused]] Data& data) const override {}
    bool decode([[maybe_unused]] const Data& encoded, [[maybe_unused]] size_t& offset_inout) override { return true; }
    bool setValueJson([[maybe_unused]] const std::string& value) override { return false; } // see makeStruct
    Data encodeHashes() const;
    std::string getExtraTypes(std::vector<std::string>& ignoreList) const override;
    std::shared_ptr<ParamBase> clone() const override;
    std::shared_ptr<ParamNamed> findParamByName(const std::string& name) const { return _params.findParamByName(name); }

    /// Compute the hash of a struct, used for signing, according to EIP712 ("v4").
    /// Input is a Json object (as string), with following fields:
    /// - types: map of used struct types (see makeTypes())
    /// - primaryType: the type of the message (string)
    /// - domain: EIP712 domain specifier values
    /// - message: the message (object).
    /// Throws on error.
    /// Example input:
    ///  R"({
    ///     "types": {
    ///         "EIP712Domain": [
    ///             {"name": "name", "type": "string"},
    ///             {"name": "version", "type": "string"},
    ///             {"name": "chainId", "type": "uint256"},
    ///             {"name": "verifyingContract", "type": "address"}
    ///         ],
    ///         "Person": [
    ///             {"name": "name", "type": "string"},
    ///             {"name": "wallet", "type": "address"}
    ///         ]
    ///     },
    ///     "primaryType": "Person",
    ///     "domain": {
    ///         "name": "Ether Person",
    ///         "version": "1",
    ///         "chainId": 1,
    ///         "verifyingContract": "0xCcCCccccCCCCcCCCCCCcCcCccCcCCCcCcccccccC"
    ///     },
    ///     "message": {
    ///         "name": "Cow",
    ///         "wallet": "CD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826"
    ///     }
    ///  })");
    static Data hashStructJson(const std::string& messageJson);

    /// Make a named struct, described by a json string (with values), and its type info (may contain type info of sub-types also).
    /// Throws on error.
    static std::shared_ptr<ParamStruct> makeStruct(const std::string& structType, const std::string& valueJson, const std::string& typesJson);

    /// Parse a json with a list of types, and build a vector of named structs.  Structs params have the given name and type, and empty value.
    /// Ex. input: R"({"Person": [{"name": "name", "type": "string"}, {"name": "wallet", "type": "address"}], "Mail": [{"name": "from", "type": "Person"}, {"name": "to", "type": "Person"}, {"name": "contents", "type": "string"}]})"
    /// Order does not matter.  Note the quote delimiters.
    /// Throws on error.
    static std::vector<std::shared_ptr<ParamStruct>> makeTypes(const std::string& structTypes);

    /// Make a named struct, with the given types, with empty values.
    /// Similar to makeTypes, but works with only one type.
    /// Ex. input: "Person", R"([{"name": "name", "type": "string"}, {"name": "wallet", "type": "address"}])"
    /// Throws on error.
    static std::shared_ptr<ParamStruct> makeType(const std::string& structName, const std::string& structJson, const std::vector<std::shared_ptr<ParamStruct>>& extraTypes = {}, bool ignoreMissingType = false);
};

} // namespace TW::Ethereum::ABI

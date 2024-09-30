// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class CosmosAddressTests: XCTestCase {
    func testAddressValidation() {
        let cosmos = CoinType.cosmos
        for address in [
            "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02",
            "cosmospub1addwnpepqftjsmkr7d7nx4tmhw4qqze8w39vjq364xt8etn45xqarlu3l2wu2n7pgrq",
            "cosmosvaloper1sxx9mszve0gaedz5ld7qdkjkfv8z992ax69k08",
            "cosmosvalconspub1zcjduepqjnnwe2jsywv0kfc97pz04zkm7tc9k2437cde2my3y5js9t7cw9mstfg3sa",
        ] {
            XCTAssertTrue(cosmos.validate(address: address))
            XCTAssertEqual(cosmos.address(string: address)?.description, address)
        }
    }
}

class CosmosSignerTests: XCTestCase {

    let privateKey = PrivateKey(data: Data(hexString: "80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005")!)!

    func testSigningTransaction() {
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .cosmos)

        let sendCoinsMessage = CosmosMessage.Send.with {
            $0.fromAddress = fromAddress.description
            $0.toAddress = "cosmos1zt50azupanqlfam5afhv3hexwyutnukeh4c573"
            $0.amounts = [CosmosAmount.with {
                $0.amount = "1"
                $0.denom = "muon"
            }]
        }

        let message = CosmosMessage.with {
            $0.sendCoinsMessage = sendCoinsMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 200000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "200"
                $0.denom = "muon"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.signingMode = .protobuf;
            $0.accountNumber = 1037
            $0.chainID = "gaia-13003"
            $0.memo = ""
            $0.sequence = 8
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .cosmos)

        XCTAssertJSONEqual(output.serialized, "{\"tx_bytes\": \"CowBCokBChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEmkKLWNvc21vczFoc2s2anJ5eXFqZmhwNWRoYzU1dGM5anRja3lneDBlcGg2ZGQwMhItY29zbW9zMXp0NTBhenVwYW5xbGZhbTVhZmh2M2hleHd5dXRudWtlaDRjNTczGgkKBG11b24SATESZQpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3FEgQKAggBGAgSEQoLCgRtdW9uEgMyMDAQwJoMGkD54fQAFlekIAnE62hZYl0uQelh/HLv0oQpCciY5Dn8H1SZFuTsrGdu41PH1Uxa4woptCELi/8Ov9yzdeEFAC9H\", \"mode\": \"BROADCAST_MODE_BLOCK\"}")
        XCTAssertEqual(output.errorMessage, "")
    }
    
    func testAuthCompounding() {
        let authMessage = CosmosMessage.AuthGrant.with {
            $0.granter = "cosmos13k0q0l7lg2kr32kvt7ly236ppldy8v9dzwh3gd"
            $0.grantee = "cosmos1fs7lu28hx5m9akm7rp0c2422cn8r2f7gurujhf"
            $0.grantStake = CosmosMessage.StakeAuthorization.with {
                $0.allowList.address = ["cosmosvaloper1gjtvly9lel6zskvwtvlg5vhwpu9c9waw7sxzwx"]
                $0.authorizationType = CosmosMessage.AuthorizationType.delegate
            }
            $0.expiration = 1692309600
        }
        let message = CosmosMessage.with {
            $0.authGrant = authMessage
        }
        let fee = CosmosFee.with {
            $0.gas = 96681
            $0.amounts = [CosmosAmount.with {
                $0.amount = "2418"
                $0.denom = "uatom"
            }]
        }
        
        let input = CosmosSigningInput.with {
            $0.signingMode = .protobuf;
            $0.accountNumber = 1290826
            $0.chainID = "cosmoshub-4"
            $0.memo = ""
            $0.sequence = 5
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = PrivateKey(data: Data(hexString: "c7764249cdf77f8f1d840fa8af431579e5e41cf1af937e1e23afa22f3f4f0ccc")!)!.data
        }
        
        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .cosmos)

        XCTAssertJSONEqual(output.serialized, "{\"tx_bytes\": \"CvgBCvUBCh4vY29zbW9zLmF1dGh6LnYxYmV0YTEuTXNnR3JhbnQS0gEKLWNvc21vczEzazBxMGw3bGcya3IzMmt2dDdseTIzNnBwbGR5OHY5ZHp3aDNnZBItY29zbW9zMWZzN2x1MjhoeDVtOWFrbTdycDBjMjQyMmNuOHIyZjdndXJ1amhmGnIKaAoqL2Nvc21vcy5zdGFraW5nLnYxYmV0YTEuU3Rha2VBdXRob3JpemF0aW9uEjogARI2CjRjb3Ntb3N2YWxvcGVyMWdqdHZseTlsZWw2enNrdnd0dmxnNXZod3B1OWM5d2F3N3N4end4EgYI4LD6pgYSZwpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohA/fcQw1hCVUx904t+kCXTiiziaLIY8lyssu1ENfzaN1KEgQKAggBGAUSEwoNCgV1YXRvbRIEMjQxOBCp8wUaQEAN1nIfDawlHnep2bNEm14w+g7tYybJJT3htcGVS6s9D7va3ed1OUEIk9LZoc3G//VenJ+KLw26SRVBaRukgVI=\", \"mode\": \"BROADCAST_MODE_BLOCK\"}")
        XCTAssertEqual(output.errorMessage, "")
    }
    
    func testRevokeAuthCompounding() {
        let revokeAuthMessage = CosmosMessage.AuthRevoke.with {
            $0.granter = "cosmos13k0q0l7lg2kr32kvt7ly236ppldy8v9dzwh3gd"
            $0.grantee = "cosmos1fs7lu28hx5m9akm7rp0c2422cn8r2f7gurujhf"
            $0.msgTypeURL = "/cosmos.staking.v1beta1.MsgDelegate"
        }
        let message = CosmosMessage.with {
            $0.authRevoke = revokeAuthMessage
        }
        let fee = CosmosFee.with {
            $0.gas = 87735
            $0.amounts = [CosmosAmount.with {
                $0.amount = "2194"
                $0.denom = "uatom"
            }]
        }
        
        let input = CosmosSigningInput.with {
            $0.signingMode = .protobuf;
            $0.accountNumber = 1290826
            $0.chainID = "cosmoshub-4"
            $0.memo = ""
            $0.sequence = 4
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = PrivateKey(data: Data(hexString: "c7764249cdf77f8f1d840fa8af431579e5e41cf1af937e1e23afa22f3f4f0ccc")!)!.data
        }
        
        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .cosmos)

        XCTAssertJSONEqual(output.serialized, "{\"tx_bytes\": \"CqoBCqcBCh8vY29zbW9zLmF1dGh6LnYxYmV0YTEuTXNnUmV2b2tlEoMBCi1jb3Ntb3MxM2swcTBsN2xnMmtyMzJrdnQ3bHkyMzZwcGxkeTh2OWR6d2gzZ2QSLWNvc21vczFmczdsdTI4aHg1bTlha203cnAwYzI0MjJjbjhyMmY3Z3VydWpoZhojL2Nvc21vcy5zdGFraW5nLnYxYmV0YTEuTXNnRGVsZWdhdGUSZwpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohA/fcQw1hCVUx904t+kCXTiiziaLIY8lyssu1ENfzaN1KEgQKAggBGAQSEwoNCgV1YXRvbRIEMjE5NBC3rQUaQI7K+W7MMBoD6FbFZxRBqs9VTjErztjWTy57+fvrLaTCIZ+eBs7CuaKqfUZdSN8otjubSHVTQID3k9DpPAX0yDo=\", \"mode\": \"BROADCAST_MODE_BLOCK\"}")
        XCTAssertEqual(output.errorMessage, "")
    }

    func testStaking() {
        let stakeMessage = CosmosMessage.Delegate.with {
            $0.delegatorAddress = "cosmos1hsk6jryyqjfhp5dhc55tc9jtckygx0eph6dd02"
            $0.validatorAddress = "cosmosvaloper1zkupr83hrzkn3up5elktzcq3tuft8nxsmwdqgp"
            $0.amount = CosmosAmount.with {
                $0.amount = "10"
                $0.denom = "muon"
            }
        }

        let message = CosmosMessage.with {
            $0.stakeMessage = stakeMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 101721
            $0.amounts = [CosmosAmount.with {
                $0.amount = "1018"
                $0.denom = "muon"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.signingMode = .protobuf;
            $0.accountNumber = 1037
            $0.chainID = "gaia-13003"
            $0.memo = ""
            $0.sequence = 7
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .cosmos)

        XCTAssertJSONEqual(output.serialized, "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"CpsBCpgBCiMvY29zbW9zLnN0YWtpbmcudjFiZXRhMS5Nc2dEZWxlZ2F0ZRJxCi1jb3Ntb3MxaHNrNmpyeXlxamZocDVkaGM1NXRjOWp0Y2t5Z3gwZXBoNmRkMDISNGNvc21vc3ZhbG9wZXIxemt1cHI4M2hyemtuM3VwNWVsa3R6Y3EzdHVmdDhueHNtd2RxZ3AaCgoEbXVvbhICMTASZgpQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAlcobsPzfTNVe7uqAAsndErJAjqplnyudaGB0f+R+p3FEgQKAggBGAcSEgoMCgRtdW9uEgQxMDE4ENmaBhpA8O9Jm/kL6Za2I3poDs5vpMowYJgNvYCJBRU/vxAjs0lNZYsq40qpTbwOTbORjJA5UjQ6auc40v6uCFT4q4z+uA==\"}")
        XCTAssertEqual(output.errorMessage, "")
    }

    func testWithdraw() {
        // https://stargate.cosmos.network/txs/DE2FF0BC39E70DB576DF86C263A5BDB42FF3D5D5B914A4A30E8C13B14A950FFF
        let delegator = "cosmos100rhxclqasy6vnrcervgh99alx5xw7lkfp4u54"
        let validators = [
            "cosmosvaloper1ey69r37gfxvxg62sh4r0ktpuc46pzjrm873ae8",
            "cosmosvaloper1sjllsnramtg3ewxqwwrwjxfgc4n4ef9u2lcnj0",
            "cosmosvaloper1648ynlpdw7fqa2axt0w2yp3fk542junl7rsvq6"
        ]

        let withdrawals = validators.map { validator in
            CosmosMessage.WithdrawDelegationReward.with {
                $0.delegatorAddress = delegator
                $0.validatorAddress = validator
            }
        }.map { withdraw in
            CosmosMessage.with {
                $0.withdrawStakeRewardMessage = withdraw
            }
        }

        let fee = CosmosFee.with {
            $0.amounts = [CosmosAmount.with {
                $0.denom = "uatom"
                $0.amount = "1"
            }]
            $0.gas = 220000
        }

        let input = CosmosSigningInput.with {
            $0.signingMode = .protobuf;
            $0.fee = fee
            $0.accountNumber = 8698
            $0.chainID = "cosmoshub-2"
            $0.sequence = 318
            $0.messages = withdrawals
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .cosmos)

        XCTAssertJSONEqual(output.serialized, "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"CukDCqABCjcvY29zbW9zLmRpc3RyaWJ1dGlvbi52MWJldGExLk1zZ1dpdGhkcmF3RGVsZWdhdG9yUmV3YXJkEmUKLWNvc21vczEwMHJoeGNscWFzeTZ2bnJjZXJ2Z2g5OWFseDV4dzdsa2ZwNHU1NBI0Y29zbW9zdmFsb3BlcjFleTY5cjM3Z2Z4dnhnNjJzaDRyMGt0cHVjNDZwempybTg3M2FlOAqgAQo3L2Nvc21vcy5kaXN0cmlidXRpb24udjFiZXRhMS5Nc2dXaXRoZHJhd0RlbGVnYXRvclJld2FyZBJlCi1jb3Ntb3MxMDByaHhjbHFhc3k2dm5yY2VydmdoOTlhbHg1eHc3bGtmcDR1NTQSNGNvc21vc3ZhbG9wZXIxc2psbHNucmFtdGczZXd4cXd3cndqeGZnYzRuNGVmOXUybGNuajAKoAEKNy9jb3Ntb3MuZGlzdHJpYnV0aW9uLnYxYmV0YTEuTXNnV2l0aGRyYXdEZWxlZ2F0b3JSZXdhcmQSZQotY29zbW9zMTAwcmh4Y2xxYXN5NnZucmNlcnZnaDk5YWx4NXh3N2xrZnA0dTU0EjRjb3Ntb3N2YWxvcGVyMTY0OHlubHBkdzdmcWEyYXh0MHcyeXAzZms1NDJqdW5sN3JzdnE2EmUKUQpGCh8vY29zbW9zLmNyeXB0by5zZWNwMjU2azEuUHViS2V5EiMKIQJXKG7D830zVXu7qgALJ3RKyQI6qZZ8rnWhgdH/kfqdxRIECgIIARi+AhIQCgoKBXVhdG9tEgExEOC2DRpAXLgJ+8xEMUn7nkFj3ukg2V65Vh5ob7HKeCaNpMM6OPQrpW2r6askfssIFcOd8ThiBEz65bJz81Fmb5MtDTGv4g==\"}")
        XCTAssertEqual(output.errorMessage, "")
    }

    func testIbcTransfer() {
        let privateKey = PrivateKey(data: Data(hexString: "8bbec3772ddb4df68f3186440380c301af116d1422001c1877d6f5e4dba8c8af")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let fromAddress = AnyAddress(publicKey: publicKey, coin: .cosmos)

        let transferMessage = CosmosMessage.Transfer.with {
            $0.sourcePort = "transfer"
            $0.sourceChannel = "channel-141"
            $0.sender = fromAddress.description
            $0.receiver = "osmo18s0hdnsllgcclweu9aymw4ngktr2k0rkvn7jmn"
            $0.token = CosmosAmount.with {
                $0.amount = "100000"
                $0.denom = "uatom"
            }
            $0.timeoutHeight = CosmosHeight.with {
                $0.revisionNumber = 1
                $0.revisionHeight = 8800000
            }
        }

        let message = CosmosMessage.with {
            $0.transferTokensMessage = transferMessage
        }

        let fee = CosmosFee.with {
            $0.gas = 500000
            $0.amounts = [CosmosAmount.with {
                $0.amount = "12500"
                $0.denom = "uatom"
            }]
        }

        let input = CosmosSigningInput.with {
            $0.signingMode = .protobuf;
            $0.accountNumber = 546179
            $0.chainID = "cosmoshub-4"
            $0.sequence = 2
            $0.messages = [message]
            $0.fee = fee
            $0.privateKey = privateKey.data
        }

        let output: CosmosSigningOutput = AnySigner.sign(input: input, coin: .cosmos)

        // https://www.mintscan.io/cosmos/txs/817101F3D96314AD028733248B28BAFAD535024D7D2C8875D3FE31DC159F096B
        XCTAssertJSONEqual(output.serialized, "{\"mode\":\"BROADCAST_MODE_BLOCK\",\"tx_bytes\":\"Cr4BCrsBCikvaWJjLmFwcGxpY2F0aW9ucy50cmFuc2Zlci52MS5Nc2dUcmFuc2ZlchKNAQoIdHJhbnNmZXISC2NoYW5uZWwtMTQxGg8KBXVhdG9tEgYxMDAwMDAiLWNvc21vczFta3k2OWNuOGVrdHd5MDg0NXZlYzl1cHNkcGhrdHh0MDNna3dseCorb3NtbzE4czBoZG5zbGxnY2Nsd2V1OWF5bXc0bmdrdHIyazBya3ZuN2ptbjIHCAEQgI6ZBBJoClAKRgofL2Nvc21vcy5jcnlwdG8uc2VjcDI1NmsxLlB1YktleRIjCiEC7O9c5DejAsZ/lUaN5LMfNukR9GfX5qUrQcHhPh1WNkkSBAoCCAEYAhIUCg4KBXVhdG9tEgUxMjUwMBCgwh4aQK0HIWdFMk+C6Gi1KG/vELe1ffcc1aEWUIqz2t/ZhwqNNHxUUSp27wteiugHEMVTEIOBhs84t2gIcT/nD/1yKOU=\"}")
        XCTAssertEqual(output.errorMessage, "")
    }
}

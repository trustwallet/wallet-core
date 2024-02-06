package sample

import (
	"encoding/hex"
	"fmt"
	"math/big"
	"tw/core"
	"tw/protos/binance"
	"tw/protos/bitcoin"
	"tw/protos/ethereum"
	"tw/protos/transactioncompiler"

	"google.golang.org/protobuf/proto"
)

func ExternalSigningDemo() {
	fmt.Println("")
	SignExternalBinanceDemo()
	SignExternalEthereumDemo()
	SignExternalBitcoinDemo()
}

func SignExternalBinanceDemo() {
	fmt.Println("==> Signing with External Signature - Binance Demo")

	coin := core.CoinTypeBinance

    // bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2
    fromAddress, _ := hex.DecodeString("40c2979694bbc961023d1d27be6fc4d21a9febe6")
    // bnb1hlly02l6ahjsgxw9wlcswnlwdhg4xhx38yxpd5
    toAddress, _ := hex.DecodeString("bffe47abfaede50419c577f1074fee6dd1535cd1")

    inOutToken := binance.SendOrder_Token {
        Denom: "BNB",
        Amount: 1,
    }

    orderInput := binance.SendOrder_Input {
        Address: fromAddress,
        Coins: []*binance.SendOrder_Token{&inOutToken},
    }
    orderOutput := binance.SendOrder_Output {
        Address: toAddress,
        Coins: []*binance.SendOrder_Token{&inOutToken},
    }

	input := binance.SigningInput {
	    ChainId: "Binance-Chain-Nile",
	    OrderOneof: &binance.SigningInput_SendOrder {
	        SendOrder: &binance.SendOrder {
	            Inputs: []*binance.SendOrder_Input{&orderInput},
                Outputs: []*binance.SendOrder_Output{&orderOutput},
            },
	    },
	}

	fmt.Println("\n==> Step 1: Prepare transaction input (protobuf)")
    txInputData, _ := proto.Marshal(&input)
	fmt.Println("txInputData len: ", len(txInputData))

	fmt.Println("\n==> Step 2: Obtain preimage hash")
	hashes := core.PreImageHashes(coin, txInputData)
	fmt.Println("hash(es): ", len(hashes), hex.EncodeToString(hashes))

	var preSigningOutput transactioncompiler.PreSigningOutput
	proto.Unmarshal(hashes, &preSigningOutput)

	fmt.Println("\n==> Step 3: Compile transaction info")
	// Simulate signature, normally obtained from signature server
	signature, _ := hex.DecodeString("1b1181faec30b60a2ddaa2804c253cf264c69180ec31814929b5de62088c0c5a45e8a816d1208fc5366bb8b041781a6771248550d04094c3d7a504f9e8310679")
	publicKey, _ := hex.DecodeString("026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e502")
	txOutput := core.CompileWithSignatures(coin, txInputData, [][]byte{signature}, [][]byte{publicKey})

	var output binance.SigningOutput
	proto.Unmarshal(txOutput, &output)
	fmt.Println("final txOutput proto:  ", len(txOutput))
	fmt.Println("output.encoded:  ", len(output.Encoded), hex.EncodeToString(output.Encoded))

	fmt.Println("\n==> Double check signature validity (result should be true)")
	verifyRes := core.PublicKeyVerify(publicKey, core.PublicKeyTypeSECP256k1, signature, preSigningOutput.DataHash)
	fmt.Println(verifyRes)

	fmt.Println("")
}

func SignExternalEthereumDemo() {
	fmt.Println("==> Signing with External Signature - Ethereum Demo")

	coin := core.CoinTypeEthereum

	fmt.Println("\n==> Step 1: Prepare transaction input (protobuf)")
	var input ethereum.SigningInput
	input.Transaction = &ethereum.Transaction {
	    TransactionOneof: &ethereum.Transaction_Transfer_ {
	        Transfer: &ethereum.Transaction_Transfer{
	            Amount: big.NewInt(1000000000000000000).Bytes(),
	        },
	    },
	}
	input.ChainId = big.NewInt(1).Bytes()
	input.ToAddress = "0x3535353535353535353535353535353535353535"
	input.Nonce = big.NewInt(11).Bytes()
	input.GasPrice = big.NewInt(20000000000).Bytes()
	input.GasLimit = big.NewInt(21000).Bytes()
	input.TxMode = ethereum.TransactionMode_Legacy
	txInputData, _ := proto.Marshal(&input)
	fmt.Println("txInputData len: ", len(txInputData))

	fmt.Println("\n==> Step 2: Obtain preimage hash")
	hashes := core.PreImageHashes(coin, txInputData)
	fmt.Println("hash(es): ", len(hashes), hex.EncodeToString(hashes))

	var preSigningOutput transactioncompiler.PreSigningOutput
	proto.Unmarshal(hashes, &preSigningOutput)

	fmt.Println("\n==> Step 3: Compile transaction info")
	// Simulate signature, normally obtained from signature server
	signature, _ := hex.DecodeString("360a84fb41ad07f07c845fedc34cde728421803ebbaae392fc39c116b29fc07b53bd9d1376e15a191d844db458893b928f3efbfee90c9febf51ab84c9796677900")
	publicKey, _ := hex.DecodeString("044bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382ce28cab79ad7119ee1ad3ebcdb98a16805211530ecc6cfefa1b88e6dff99232a")
	txOutput := core.CompileWithSignatures(coin, txInputData, [][]byte{signature}, [][]byte{publicKey})

	fmt.Println("final txOutput proto:  ", len(txOutput))
	var output ethereum.SigningOutput
	_ = proto.Unmarshal(txOutput, &output)
	fmt.Println("output.encoded:  ", len(output.Encoded), hex.EncodeToString(output.Encoded))

	fmt.Println("\n==> Double check signature validity (result should be true)")
	verifyRes := core.PublicKeyVerify(publicKey, core.PublicKeyTypeSECP256k1Extended, signature, preSigningOutput.DataHash)
	fmt.Println(verifyRes)

	fmt.Println("")
}

func SignExternalBitcoinDemo() {
	fmt.Println("==> Signing with External Signature - Bitcoin Demo")

	fmt.Println("\n==> Step 1: Prepare transaction input (protobuf)")

	revUtxoHash0, _ := hex.DecodeString("07c42b969286be06fae38528c85f0a1ce508d4df837eb5ac4cf5f2a7a9d65fa8")
	revUtxoHash1, _ := hex.DecodeString("d6892a5aa54e3b8fe430efd23f49a8950733aaa9d7c915d9989179f48dd1905e")
	revUtxoHash2, _ := hex.DecodeString("6021efcf7555f90627364339fc921139dd40a06ccb2cb2a2a4f8f4ea7a2dc74d")
	inPubKey0, _ := hex.DecodeString("024bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382")
	inPubKey1, _ := hex.DecodeString("0217142f69535e4dad0dc7060df645c55a174cc1bfa5b9eb2e59aad2ae96072dfc")
	inPubKeyHash0, _ := hex.DecodeString("bd92088bb7e82d611a9b94fbb74a0908152b784f")
	inPubKeyHash1, _ := hex.DecodeString("6641abedacf9483b793afe1718689cc9420bbb1c")
	ownAddress := "bc1qhkfq3zahaqkkzx5mjnamwjsfpq2jk7z00ppggv"

	// Input UTXO infos
	type UtxoInfo struct {
		revUtxoHash []byte
		publicKey   []byte
		address     string
		amount      int
		index       int
	}

	utxoInfos := [...]UtxoInfo{
		// first
		{revUtxoHash0, inPubKey0, ownAddress, 600000, 0},
		// second UTXO, with same pubkey
		{revUtxoHash1, inPubKey0, ownAddress, 500000, 1},
		// third UTXO, with different pubkey
		{revUtxoHash2, inPubKey1, "bc1qveq6hmdvl9yrk7f6lct3s6yue9pqhwcuxedggg", 400000, 0},
	}

	// Signature infos, indexed by pubkeyhash+hash
	type SignatureInfo struct {
		signature []byte
		publicKey []byte
	}
	inSig0, _ := hex.DecodeString("304402201857bc6e6e48b46046a4bd204136fc77e24c240943fb5a1f0e86387aae59b34902200a7f31478784e51c49f46ef072745a4f263d7efdbc9c6784aa2571ff4f6f2a40")
	inSig1, _ := hex.DecodeString("3044022041294880caa09bb1b653775310fcdd1458da6b8e7d7fae34e37966414fe115820220646397c9d2513edc5974ecc336e9b287de0cdf071c366f3b3dc3ff309213e4e4")
	inSig2, _ := hex.DecodeString("30440220764e3d5b3971c4b3e70b23fb700a7462a6fe519d9830e863a1f8388c402ad0b102207e777f7972c636961f92375a2774af3b7a2a04190251bbcb31d19c70927952dc")
	signatureInfos := map[string]SignatureInfo{
		hex.EncodeToString(inPubKeyHash0) + "+" + "a296bead4172007be69b21971a790e076388666c162a9505698415f1b003ebd7": {inSig0, inPubKey0},
		hex.EncodeToString(inPubKeyHash1) + "+" + "505f527f00e15fcc5a2d2416c9970beb57dfdfaca99e572a01f143b24dd8fab6": {inSig1, inPubKey1},
		hex.EncodeToString(inPubKeyHash0) + "+" + "60ed6e9371e5ddc72fd88e46a12cb2f68516ebd307c0fd31b1b55cf767272101": {inSig2, inPubKey0},
	}

	coin := core.CoinTypeBitcoin

	// Setup input for Plan
	input := bitcoin.SigningInput{
		HashType:      uint32(core.BitcoinSigHashTypeAll),
		Amount:        1200000,
		ByteFee:       1,
		ToAddress:     "bc1q2dsdlq3343vk29runkgv4yc292hmq53jedfjmp",
		ChangeAddress: ownAddress,
		Utxo:          []*bitcoin.UnspentTransaction{},
		CoinType:      uint32(coin),
		Scripts:       map[string][]byte{},
	}

	// process UTXOs
	for i := 0; i < len(utxoInfos); i++ {
		address := utxoInfos[i].address
		fmt.Println("utxo", i, ":     ", address, utxoInfos[i].amount)

		lockScript := core.BitcoinScriptLockScriptForAddress(address, coin)
		fmt.Println("  lockScript:   ", hex.EncodeToString(lockScript))
		keyHash := core.BitcoinScriptMatchPayToWitnessPublicKeyHash(lockScript)
		fmt.Println("  keyHash:      ", hex.EncodeToString(keyHash))
		redeemScript := core.BitcoinScriptBuildPayToPublicKeyHash(keyHash)
		fmt.Println("  redeemScript: ", hex.EncodeToString(redeemScript))
		input.Scripts[hex.EncodeToString(keyHash)] = redeemScript

		utxo := bitcoin.UnspentTransaction{
			OutPoint: &bitcoin.OutPoint{
				Hash:     utxoInfos[i].revUtxoHash,
				Index:    uint32(utxoInfos[i].index),
				Sequence: 4294967295,
			},
			Amount: int64(utxoInfos[i].amount),
			Script: lockScript,
		}
		input.Utxo = append(input.Utxo, &utxo)
	}

	txInputData, _ := proto.Marshal(&input)
	fmt.Println("txInputData len: ", len(txInputData))

	fmt.Println("\n==> Step 2: Obtain preimage hashes")
	hashes := core.PreImageHashes(coin, txInputData)

	var preSigningOutput bitcoin.PreSigningOutput
	proto.Unmarshal(hashes, &preSigningOutput)
	fmt.Println("hashes+pubkeyhashes: ", len(preSigningOutput.HashPublicKeys))
	for _, h := range preSigningOutput.HashPublicKeys {
		fmt.Println("    ", hex.EncodeToString(h.DataHash), hex.EncodeToString(h.PublicKeyHash))
	}

	fmt.Println("\n==> Step 3: Compile transaction info")
	// Simulate signature, normally obtained from signature server
	signatureVec := [][]byte{}
	pubkeyVec := [][]byte{}
	for _, h := range preSigningOutput.HashPublicKeys {
		preImageHash := h.DataHash
		pubkeyHash := h.PublicKeyHash
		key := hex.EncodeToString(pubkeyHash) + "+" + hex.EncodeToString(preImageHash)
		sigInfo := signatureInfos[key]

		signatureVec = append(signatureVec, sigInfo.signature)
		pubkeyVec = append(pubkeyVec, sigInfo.publicKey)

		// Verify signature (pubkey & hash & signature)
		verifyRes := core.PublicKeyVerifyAsDER(sigInfo.publicKey, core.PublicKeyTypeSECP256k1, sigInfo.signature, preImageHash)
		fmt.Println("signature verification:", verifyRes)
	}
	txOutput := core.CompileWithSignatures(coin, txInputData, signatureVec, pubkeyVec)

	fmt.Println("final txOutput proto:  ", len(txOutput))
	var output bitcoin.SigningOutput
	_ = proto.Unmarshal(txOutput, &output)
	fmt.Println("output.encoded:  ", len(output.Encoded), hex.EncodeToString(output.Encoded))

	fmt.Println("")
}

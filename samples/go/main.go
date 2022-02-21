package main

import (
	"encoding/hex"
	"fmt"
	"math/big"
	"tw/core"
	"tw/protos/bitcoin"
	"tw/protos/ethereum"

	"google.golang.org/protobuf/proto"
)

func signExternalBinanceDemo() {
	fmt.Println("==> Signing with External Signature - Binance Demo")

	coin := core.CoinTypeBinance

	fmt.Println("\n==> Step 1: Prepare transaction input (protobuf)")
	txInputData := core.BuildInput(
		coin,
		"bnb1grpf0955h0ykzq3ar5nmum7y6gdfl6lxfn46h2", // from
		"bnb1hlly02l6ahjsgxw9wlcswnlwdhg4xhx38yxpd5", // to
		"1",   // amount
		"BNB", // asset
		"",    // memo
		"",    // chainId
	)
	fmt.Println("txInputData len: ", len(txInputData))

	fmt.Println("\n==> Step 2: Obtain preimage hash")
	hashes := core.PreImageHashes(coin, txInputData)
	fmt.Println("hash(es): ", len(hashes), hex.EncodeToString(hashes[0]))

	fmt.Println("\n==> Step 3: Compile transaction info")
	// Simulate signature, normally obtained from signature server
	signature, _ := hex.DecodeString("1b1181faec30b60a2ddaa2804c253cf264c69180ec31814929b5de62088c0c5a45e8a816d1208fc5366bb8b041781a6771248550d04094c3d7a504f9e8310679")
	publicKey, _ := hex.DecodeString("026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e502")
	txOutput := core.CompileWithSignatures(coin, txInputData, [][]byte{signature}, [][]byte{publicKey})

	fmt.Println("final txOutput proto:  ", len(txOutput))
	fmt.Println(hex.EncodeToString(txOutput))

	fmt.Println("\n==> Double check signature validity (result should be true)")
	verifyRes := core.PublicKeyVerify(publicKey, core.PublicKeyTypeSECP256k1, signature, hashes[0])
	fmt.Println(verifyRes)

	fmt.Println("")
}

func signExternalEthereumDemo() {
	fmt.Println("==> Signing with External Signature - Ethereum Demo")

	coin := core.CoinTypeEthereum

	fmt.Println("\n==> Step 1: Prepare transaction input (protobuf)")
	txInputData := core.BuildInput(
		coin,
		"0x9d8A62f656a8d1615C1294fd71e9CFb3E4855A4F", // from
		"0x3535353535353535353535353535353535353535", // to
		"1000000000000000000",                        // amount
		"ETH",                                        // asset
		"",                                           // memo
		"",                                           // chainId
	)
	fmt.Println("txInputData len: ", len(txInputData))

	// Set a few other values
	var input ethereum.SigningInput
	proto.Unmarshal(txInputData, &input)
	input.Nonce = big.NewInt(11).Bytes()
	input.GasPrice = big.NewInt(20000000000).Bytes()
	input.GasLimit = big.NewInt(21000).Bytes()
	input.TxMode = ethereum.TransactionMode_Legacy
	txInputData2, _ := proto.Marshal(&input)
	fmt.Println("txInputData len: ", len(txInputData2))

	fmt.Println("\n==> Step 2: Obtain preimage hash")
	hashes := core.PreImageHashes(coin, txInputData2)
	fmt.Println("hash(es): ", len(hashes), hex.EncodeToString(hashes[0]))

	fmt.Println("\n==> Step 3: Compile transaction info")
	// Simulate signature, normally obtained from signature server
	signature, _ := hex.DecodeString("360a84fb41ad07f07c845fedc34cde728421803ebbaae392fc39c116b29fc07b53bd9d1376e15a191d844db458893b928f3efbfee90c9febf51ab84c9796677900")
	publicKey, _ := hex.DecodeString("044bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382ce28cab79ad7119ee1ad3ebcdb98a16805211530ecc6cfefa1b88e6dff99232a")
	txOutput := core.CompileWithSignatures(coin, txInputData2, [][]byte{signature}, [][]byte{publicKey})

	fmt.Println("final txOutput proto:  ", len(txOutput))
	var output ethereum.SigningOutput
	_ = proto.Unmarshal(txOutput, &output)
	fmt.Println("output.encoded:  ", len(output.Encoded), hex.EncodeToString(output.Encoded))

	fmt.Println("\n==> Double check signature validity (result should be true)")
	verifyRes := core.PublicKeyVerify(publicKey, core.PublicKeyTypeSECP256k1Extended, signature, hashes[0])
	fmt.Println(verifyRes)

	fmt.Println("")
}

func signExternalBitcoinDemo() {
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
		UtxoInfo{revUtxoHash0, inPubKey0, ownAddress, 600000, 0},
		// second UTXO, with same pubkey
		UtxoInfo{revUtxoHash1, inPubKey0, ownAddress, 500000, 1},
		// third UTXO, with different pubkey
		UtxoInfo{revUtxoHash2, inPubKey1, "bc1qveq6hmdvl9yrk7f6lct3s6yue9pqhwcuxedggg", 400000, 0},
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
		hex.EncodeToString(inPubKeyHash0) + "+" + "a296bead4172007be69b21971a790e076388666c162a9505698415f1b003ebd7": SignatureInfo{inSig0, inPubKey0},
		hex.EncodeToString(inPubKeyHash1) + "+" + "505f527f00e15fcc5a2d2416c9970beb57dfdfaca99e572a01f143b24dd8fab6": SignatureInfo{inSig1, inPubKey1},
		hex.EncodeToString(inPubKeyHash0) + "+" + "60ed6e9371e5ddc72fd88e46a12cb2f68516ebd307c0fd31b1b55cf767272101": SignatureInfo{inSig2, inPubKey0},
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
	lenHashes := len(hashes)
	fmt.Println("hashes+pubkeyhashes: ", lenHashes)
	for i := 0; i < lenHashes; i += 2 {
		fmt.Println("    ", hex.EncodeToString(hashes[i]), hex.EncodeToString(hashes[i+1]))
	}

	fmt.Println("\n==> Step 3: Compile transaction info")
	// Simulate signature, normally obtained from signature server
	signatureVec := [][]byte{}
	pubkeyVec := [][]byte{}
	for i := 0; i < lenHashes; i += 2 {
		preImageHash := hashes[i]
		pubkeyHash := hashes[i+1]
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

func main() {
	signExternalBinanceDemo()
	signExternalEthereumDemo()
	signExternalBitcoinDemo()

	/*
		fmt.Println("==> calling wallet core from go")

		mn := "confirm bleak useless tail chalk destroy horn step bulb genuine attract split"

		fmt.Println("==> mnemonic is valid: ", core.IsMnemonicValid(mn))

		// bitcoin wallet
		bw, err := core.CreateWalletWithMnemonic(mn, core.CoinTypeBitcoin)
		if err != nil {
			panic(err)
		}
		printWallet(bw)

		// ethereum wallet
		ew, err := core.CreateWalletWithMnemonic(mn, core.CoinTypeEthereum)
		if err != nil {
			panic(err)
		}
		printWallet(ew)

		// tron wallet
		tw, err := core.CreateWalletWithMnemonic(mn, core.CoinTypeTron)
		if err != nil {
			panic(err)
		}
		printWallet(tw)

		// Ethereum transaction
		ethTxn := createEthTransaction(ew)
		fmt.Println("Ethereum signed tx:")
		fmt.Println("\t", ethTxn)

		// Bitcion transaction
		btcTxn := createBtcTransaction(bw)
		fmt.Println("\nBitcoin signed tx:")
		fmt.Println("\t", btcTxn)
	*/
}

/*
func createEthTransaction(ew *core.Wallet) string {
	priKeyByte, _ := hex.DecodeString(ew.PriKey)

	input := ethereum.SigningInput{
		ChainId:    big.NewInt(4).Bytes(), // mainnet: 1, rinkeby: 4 https://chainlist.org/
		Nonce:      big.NewInt(0).Bytes(), // get nonce from network
		TxMode:     ethereum.TransactionMode_Legacy,
		GasPrice:   big.NewInt(100000000000).Bytes(), // 100 gwei
		GasLimit:   big.NewInt(21000).Bytes(),
		ToAddress:  "0xE9B511C0753649E5F3E78Ed8AdBEE92d0d2Db384",
		PrivateKey: priKeyByte,
		Transaction: &ethereum.Transaction{
			TransactionOneof: &ethereum.Transaction_Transfer_{
				Transfer: &ethereum.Transaction_Transfer{
					// amount should be in wei unit, eth * (10^decimals) = wei
					Amount: big.NewInt(int64(
						0.01 * math.Pow10(ew.CoinType.Decimals()),
					)).Bytes(),
					Data: []byte{},
				},
			},
		},
	}

	var output ethereum.SigningOutput
	err := core.CreateSignedTx(&input, ew.CoinType, &output)
	if err != nil {
		panic(err)
	}
	return hex.EncodeToString(output.GetEncoded())
}

func createBtcTransaction(bw *core.Wallet) string {
	lockScript := core.BitcoinScriptLockScriptForAddress(bw.Address, bw.CoinType)
	fmt.Println("\nBitcoin address lock script:")
	fmt.Println("\t", hex.EncodeToString(lockScript))

	utxoHash, _ := hex.DecodeString("fff7f7881a8099afa6940d42d1e7f6362bec38171ea3edf433541db4e4ad969f")

	utxo := bitcoin.UnspentTransaction{
		OutPoint: &bitcoin.OutPoint{
			Hash:     utxoHash,
			Index:    0,
			Sequence: 4294967295,
		},
		Amount: 625000000,
		Script: lockScript,
	}

	priKeyByte, _ := hex.DecodeString(bw.PriKey)

	input := bitcoin.SigningInput{
		HashType:      uint32(core.BitcoinSigHashTypeAll),
		Amount:        1000000,
		ByteFee:       1,
		ToAddress:     "1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx",
		ChangeAddress: "1FQc5LdgGHMHEN9nwkjmz6tWkxhPpxBvBU",
		PrivateKey:    [][]byte{priKeyByte},
		Utxo:          []*bitcoin.UnspentTransaction{&utxo},
		CoinType:      uint32(core.CoinTypeBitcoin),
	}

	var output bitcoin.SigningOutput
	err := core.CreateSignedTx(&input, bw.CoinType, &output)
	if err != nil {
		panic(err)
	}
	if output.GetError() != common.SigningError_OK {
		panic(output.GetError().String())
	}
	return hex.EncodeToString(output.GetEncoded())
}

func printWallet(w *core.Wallet) {
	fmt.Printf("%s wallet: \n", w.CoinType.GetName())
	fmt.Printf("\t address: %s \n", w.Address)
	fmt.Printf("\t pri key: %s \n", w.PriKey)
	fmt.Printf("\t pub key: %s \n", w.PubKey)
	fmt.Println("")
}
*/

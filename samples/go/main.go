package main

import (
	"encoding/hex"
	"fmt"
	"math"
	"math/big"
	"tw/core"
	"tw/protos/bitcoin"
	"tw/protos/common"
	"tw/protos/ethereum"

	"google.golang.org/protobuf/proto"
)

func tssSignBinanceDemo() {
	fmt.Println("==> TSS Signing Binance Demo")

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
	hash := core.PreImageHash(coin, txInputData)
	fmt.Println("hash: ", hex.EncodeToString(hash))

	fmt.Println("\n==> Step 3: Compile transaction info")
	// Simulate signature, normally obtained from signature server
	signature, _ := hex.DecodeString("1b1181faec30b60a2ddaa2804c253cf264c69180ec31814929b5de62088c0c5a45e8a816d1208fc5366bb8b041781a6771248550d04094c3d7a504f9e8310679")
	publicKey, _ := hex.DecodeString("026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e502")
	txOutput := core.CompileWithSignature(coin, txInputData, signature, publicKey)

	fmt.Println("final txOutput proto:  ", len(txOutput))
	fmt.Println(hex.EncodeToString(txOutput))
	//var output binance.SigningOutput
	//_ = proto.Unmarshal(txOutput, &output)
	//fmt.Println("output.encoded:  ", len(output.Encoded), hex.EncodeToString(output.Encoded))

	fmt.Println("\n==> Double check signature validity (result should be true)")
	verifyRes := core.PublicKeyVerify(publicKey, core.PublicKeyTypeSECP256k1, signature, hash)
	fmt.Println(verifyRes)

	fmt.Println("")
}

func tssSignEthereumDemo() {
	fmt.Println("==> TSS Signing Ethereum Demo")

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
	hash := core.PreImageHash(coin, txInputData2)
	fmt.Println("hash: ", hex.EncodeToString(hash))

	fmt.Println("\n==> Step 3: Compile transaction info")
	// Simulate signature, normally obtained from signature server
	signature, _ := hex.DecodeString("360a84fb41ad07f07c845fedc34cde728421803ebbaae392fc39c116b29fc07b53bd9d1376e15a191d844db458893b928f3efbfee90c9febf51ab84c9796677900")
	publicKey, _ := hex.DecodeString("044bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382ce28cab79ad7119ee1ad3ebcdb98a16805211530ecc6cfefa1b88e6dff99232a")
	txOutput := core.CompileWithSignature(coin, txInputData2, signature, publicKey)

	fmt.Println("final txOutput proto:  ", len(txOutput))
	//fmt.Println(hex.EncodeToString(txOutput))
	var output ethereum.SigningOutput
	_ = proto.Unmarshal(txOutput, &output)
	fmt.Println("output.encoded:  ", len(output.Encoded), hex.EncodeToString(output.Encoded))

	fmt.Println("\n==> Double check signature validity (result should be true)")
	verifyRes := core.PublicKeyVerify(publicKey, core.PublicKeyTypeSECP256k1Extended, signature, hash)
	fmt.Println(verifyRes)

	fmt.Println("")
}

func tssSignBitcoinDemo() {
	fmt.Println("==> TSS Signing Bitcoin Demo")

	coin := core.CoinTypeBitcoin

	fmt.Println("\n==> Step 1: Prepare transaction input (protobuf)")
	ownAddress := "bc1qhkfq3zahaqkkzx5mjnamwjsfpq2jk7z00ppggv"

	lockScript := core.BitcoinScriptLockScriptForAddress(ownAddress, coin)
	fmt.Println("lockScript: ", hex.EncodeToString(lockScript))
	utxoHash, _ := hex.DecodeString("07c42b969286be06fae38528c85f0a1ce508d4df837eb5ac4cf5f2a7a9d65fa8") // reversed
	utxo := bitcoin.UnspentTransaction{
		OutPoint: &bitcoin.OutPoint{
			Hash:     utxoHash,
			Index:    0,
			Sequence: 4294967295,
		},
		Amount: 3899774,
		Script: lockScript,
	}

	input := bitcoin.SigningInput{
		HashType:      uint32(core.BitcoinSigHashTypeAll),
		Amount:        1200000,
		ByteFee:       1,
		ToAddress:     "bc1q2dsdlq3343vk29runkgv4yc292hmq53jedfjmp",
		ChangeAddress: ownAddress,
		Utxo:          []*bitcoin.UnspentTransaction{&utxo},
		CoinType:      uint32(coin),
		Scripts:       map[string][]byte{},
	}
	keyHash0 := core.BitcoinScriptMatchPayToWitnessPublicKeyHash(lockScript)
	fmt.Println("keyHash0: ", hex.EncodeToString(keyHash0))
	redeemScript := core.BitcoinScriptBuildPayToPublicKeyHash(keyHash0)
	fmt.Println("redeemScript: ", hex.EncodeToString(redeemScript))
	input.Scripts[hex.EncodeToString(keyHash0)] = redeemScript

	txInputData, _ := proto.Marshal(&input)
	fmt.Println("txInputData len: ", len(txInputData))

	fmt.Println("\n==> Step 2: Obtain preimage hash")
	hash := core.PreImageHash(coin, txInputData)
	fmt.Println("hash: ", hex.EncodeToString(hash))

	fmt.Println("\n==> Step 3: Compile transaction info")
	// Simulate signature, normally obtained from signature server
	signature, _ := hex.DecodeString("30450221009f61304eb182a4862825f27f1d93406d1e8b25d2cb889ffb7d191d8c8f6de1ea0220205e43a91abb8b7e4e4f40f54cbb3c7ebfcc725c3a97c0c42c77acebf17a7d00")
	publicKey, _ := hex.DecodeString("024bc2a31265153f07e70e0bab08724e6b85e217f8cd628ceb62974247bb493382")
	txOutput := core.CompileWithSignature(coin, txInputData, signature, publicKey)

	fmt.Println("final txOutput proto:  ", len(txOutput))
	//fmt.Println(hex.EncodeToString(txOutput))
	var output bitcoin.SigningOutput
	_ = proto.Unmarshal(txOutput, &output)
	fmt.Println("output.encoded:  ", len(output.Encoded), hex.EncodeToString(output.Encoded))

	fmt.Println("\n==> Double check signature validity (result should be true)")
	verifyRes := core.PublicKeyVerifyDer(publicKey, core.PublicKeyTypeSECP256k1, signature, hash)
	fmt.Println(verifyRes)

	fmt.Println("")
}

func main() {
	tssSignBinanceDemo()
	tssSignEthereumDemo()
	tssSignBitcoinDemo()

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

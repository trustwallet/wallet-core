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
	"tw/sample"
)

func main() {
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

	// Bitcoin transaction
	btcTxn := createBtcTransaction(bw)
	fmt.Println("\nBitcoin signed tx:")
	fmt.Println("\t", btcTxn)

	sample.ExternalSigningDemo()
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

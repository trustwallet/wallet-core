<!--- Thank you for requesting new blockchain support -->

<!--- Before submitting please check to see if this coin was already requested -->

<!--- Provide as many relevant details about the coin -->
## Description

<!-- Coin Name and official website e.g. [Bitcoin](https://bitcoin.org) -->
Name:
<!-- HD Derivation Scheme e.g. BIP44 / 0 -->
HD Derivation Scheme:
<!-- Symbol e.g. BTC -->
Symbol:
<!-- Documentation (Address / Transaction signing etc) and reference implementation links -->
Documentation:
<!-- Why we should support it? -->
Reason:

## Checklist

- [ ] Add the coin definition to `coins.json` and `TWCoinType`.
- [ ] Implement functionality in C++. Put it in a subfolder of `src/`.
    - [ ] Address (if necessary)
    - [ ] Transaction (if necessary)
    - [ ] Signer
- [ ] Write unit tests. Put them in a subfolder of `tests/`.
    - [ ] `Mnemonic phrase - > Address` derivation test. Put this test in the `CoinTests.cpp` file
- [ ] Add relevant constants in `TWP2SHPrefix`, `TWEthereymChainID`, `TWHRP`, etc., as necessary.
- [ ] Implement address validation and derivation in `src/Coin.cpp`.
- [ ] Write interface header in `include/TrustWalletCore` and implement the interface in `src/interface`.
    - [ ] Address interface (if necessary).
    - [ ] Signing interface.
- [ ] Validate generated code in Android an iOS projects. Write integration tests for each.
- [ ] Add a coin icon to "Supported Blockchains" section in `README.md`
    - [ ] Upload coin icon to [TrustWallet/tokens](https://github.com/TrustWallet/tokens) if necessary

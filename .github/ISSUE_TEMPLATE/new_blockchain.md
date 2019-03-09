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

<!-- Sample Checklist -->

- [ ] Address
- - [ ] Return correct curve and purpose in `src/Coin.cpp`.
- - [ ] Derivation from private key in `src/Coin.cpp`.
- - [ ] Validation in `src/Coin.cpp`.
- - [ ] TW[Blockchain]Address
- - [ ] Unit test
- [ ] Transaction
- - [ ] serialization / encoding
- - [ ] Unit test
- [ ] Signer
- - [ ] signing input / output protobuf messages
- - [ ] signature
- - [ ] TW[Blockchain]Signer
- - [ ] Unit test
- - [ ] C Interface test ( Java / Swift)
- [ ] RPC documentation
- - [ ] query balance
- - [ ] query fee
- - [ ] query transaction list
- - [ ] query transaction detail
- - [ ] submit rawtx
- - [ ] blockchain info (block height)

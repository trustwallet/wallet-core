# WalletConsole utility

The *Wallet Core* library comes with an interactive command-line utility, for accessing key- and address management functionality of the library. 

## Quick Start

    $ ./build/walletconsole/walletconsole 
    Wallet-Core Console                          (c) TrustWallet
    Type 'help' for list of commands.
    > help
    Commands:
      . . .
      newKey                  Create new pseudo-random 32-byte key (secret!)
      . . .
    > coin btc
    Set active coin to: bitcoin
    > addrDefault
    Result:  bc1q2kecrqfvzj7l6phet956whxkvathsvsgn7twav

## Starting

The utility builds together with the library and can be started from: *build/walletconsole/walletconsole* .

## Commands

Here is a snapshot of the commands:

    Commands:
    exit                    Exit
    quit                    Exit
    help                    This help
    Inputs, buffer:
    #                       Take last result
    #<n>                    Take nth previous result
    buffer                  Take buffer values
    Coins:
    coins                   List known coins
    coin <coin>             Set active coin, selected by its ID or symbol or name
    Keys:
    newKey                  Create new pseudo-random 32-byte key (secret!)
    pubPri <priKey>         Derive public key from a secret private key (type is coin-dependent)
    priPub <pubKey>         Derive private key from public key :)
    setMnemonic <word1> ... Set current mnemonic, several words (secret!)
    newMnemonic <strength>  Create and store a new mnemonic, of strength (128 -- 256) (secret!)
    dumpSeed                Dump the seed of the current mnemonic (secret!)
    dumpMnemonic            Dump the current mnemonic (secret!)
    dumpDP                  Dump the default derivation path of the current coin (ex.: m/84'/0'/0'/0/0)
    priDP [<derivPath>]     Derive a new private key for the coin, from the current mnemonic and given derivation path.
                            If derivation path is missing, the default one is used (see dumpDP).
    Addresses:
    addrPub <pubKey>        Create <coin> address from public key
    addrPri <priKey>        Create <coin> address from private key
    addr <addr>             Check string <coin> address
    addrDefault             Derive default address, for current coin, fom current mnemonic; see dumpDP
    addrDP <derivPath>      Derive a new address with the given derivation path (using current coin and mnemonic)
    Coin-specific methods:
    tonInitMsg <priKey>     Build TON account initialization message.
    Transformations:
    hex <inp>               Encode given string to hex
    base64Encode <inp>      Encode given hex data to Base64
    base64Decode <inp>      Decode given Base64 string to hex data
    File methods:
    fileW <fileName> <data> Write data to a (new) binary file.
    fileR <fileName>        Read data from a binary file.

## Examples

Here are various sample usages.

Create a new private key, derive a BTC address from it, write it to a file:

```
> coin bitcoin
Set active coin to: bitcoin    Use 'coin' to change.  (name: 'bitcoin'  symbol: btc  numericalid: 0)
> newKey
Result:  4e8c1773ce1ca447594fa23a445d9952236c7a15e96802b880aab4d918bdcfd9
> addrPri #
Result:  bc1qvjf93nc80f3fu7j2ehqv6xw6zqa5cny32hl90y
> fileW btcaddr.txt #
Written to file 'btcaddr.txt', 21 bytes.
```

Create a new private key, derive the public key from it, derive a BTC address from it:

```
> newKey
Result:  ef8f76035c4d4dd29ed4bbe3fc7c0db45d81cd616f2ac8b038cb982bec2a63ad
> pubPri #
Result:  0381277ec943a6cd4033171da547bbe93585a8905fb3dad108e8e51e88a4e136ea
> addrPub #
Result:  bc1qvf6gzfhcelpugw84ks677x5zuke46jm946dtpx
```

Check an ALGO address for validity:

```
> coin algo
Set active coin to: algorand    Use 'coin' to change.  (name: 'algorand'  symbol: algo  numericalid: 283)
> addr LCSUSBOLNVT6BND6DWWGM4DLVUYJN3PGBT4T7LTCMDMKS7TR7FZAOHOVPE
Address is a valid algorand address:  LCSUSBOLNVT6BND6DWWGM4DLVUYJN3PGBT4T7LTCMDMKS7TR7FZAOHOVPE
```

Derive private keys using different BIP39 derivation paths, and create address from it:

```
> coin btc
Set active coin to: bitcoin    Use 'coin' to change.  (name: 'bitcoin'  symbol: btc  numericalid: 0)
> dumpDP
Result:  m/84'/0'/0'/0/0
> priDP
Using derivation path "m/84'/0'/0'/0/0" for coin bitcoin
Result:  df6a78d65fe3c82f020c25be12debd46e58f5f4366381102485ce26c52416a96
> priDP m/84'/0'/0'/0/1
Using derivation path "m/84'/0'/0'/0/1" for coin bitcoin
Result:  19c44359b4953bb3084dd73ba2854f6822de3b6f8f358055a16a1b19e533e37c
> pubPri #
Result:  02222d3889870d36e0c9e788c69a6509eddfb2105309e7733cfa669bb52ba996d1
> addrPub #
Result:  bc1q7ghe6xyx38vdkzs9ryv3gz47mgvpa747z2mcvy
> addr #
Address is a valid bitcoin address:  bc1q7ghe6xyx38vdkzs9ryv3gz47mgvpa747z2mcvy
```

Derive several NANO addresses (BIP39):

```
> coin nano
Set active coin to: nano    Use 'coin' to change.  (name: 'nano'  symbol: nano  numericalid: 165)
> dumpDP
Result:  m/44'/165'/0'
> setMnemonic edge defense waste choose enrich upon flee junk siren film clown finish luggage leader kid quick brick print evidence swap drill paddle truly occur
Mnemonic set (24 words).
> addrDefault
Result:  nano_3yyipbgtnd7183k61nkh5mxnt9wpsfhto95mksdqj6s7p45mwj9osai7asad
> addrDP m/44'/165'/0'
Result:  nano_3yyipbgtnd7183k61nkh5mxnt9wpsfhto95mksdqj6s7p45mwj9osai7asad
> addrDP m/44'/165'/1'
Result:  nano_3zni7o8m4dq1aphffnaq5pwseh7rrzhkrz5mpr8bcm68n6u775pbk7csedam
```

Hex and Base64 encoding example:

```
> hex Hello
Result:  48656c6c6f
> base64Encode #
Result:  SGVsbG8=
> base64Decode #
Result:  48656c6c6f
> buffer
Last value:  48656c6c6f
3 previous values:
    #1  48656c6c6f
    #2  SGVsbG8=
    #3  48656c6c6f
```

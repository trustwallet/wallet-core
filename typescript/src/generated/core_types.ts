// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

export enum CoinType {
  bitcoin = 0,
  litecoin = 2,
  doge = 3,
  dash = 5,
  viacoin = 14,
  groestlcoin = 17,
  digibyte = 20,
  monacoin = 22,
  decred = 42,
  ethereum = 60,
  classic = 61,
  icon = 74,
  cosmos = 118,
  zcash = 133,
  zcoin = 136,
  ripple = 144,
  bitcoincash = 145,
  stellar = 148,
  bitcoingold = 156,
  nano = 165,
  ravencoin = 175,
  poa = 178,
  eos = 194,
  tron = 195,
  fio = 235,
  nimiq = 242,
  algorand = 283,
  iotex = 304,
  zilliqa = 313,
  terra = 330,
  polkadot = 354,
  ton = 396,
  near = 397,
  aion = 425,
  kusama = 434,
  aeternity = 457,
  kava = 459,
  filecoin = 461,
  band = 494,
  theta = 500,
  solana = 501,
  elrond = 508,
  binance = 714,
  vechain = 818,
  callisto = 820,
  neo = 888,
  tomochain = 889,
  thundertoken = 1001,
  harmony = 1023,
  oasis = 474,
  ontology = 1024,
  tezos = 1729,
  cardano = 1815,
  kin = 2017,
  qtum = 2301,
  nebulas = 2718,
  gochain = 6060,
  nuls = 8964,
  zelcash = 19167,
  wanchain = 5718350,
  waves = 5741564,
  bsc = 10000714,
  smartchain = 20000714,
  polygon = 966,
  thorchain = 931,
}

export namespace CoinType {
  export function id(coin: CoinType): string {
    switch (coin) {
      case CoinType.bitcoin:
        return 'bitcoin';
      case CoinType.litecoin:
        return 'litecoin';
      case CoinType.doge:
        return 'doge';
      case CoinType.dash:
        return 'dash';
      case CoinType.viacoin:
        return 'viacoin';
      case CoinType.groestlcoin:
        return 'groestlcoin';
      case CoinType.digibyte:
        return 'digibyte';
      case CoinType.monacoin:
        return 'monacoin';
      case CoinType.decred:
        return 'decred';
      case CoinType.ethereum:
        return 'ethereum';
      case CoinType.classic:
        return 'classic';
      case CoinType.icon:
        return 'icon';
      case CoinType.cosmos:
        return 'cosmos';
      case CoinType.zcash:
        return 'zcash';
      case CoinType.zcoin:
        return 'zcoin';
      case CoinType.ripple:
        return 'ripple';
      case CoinType.bitcoincash:
        return 'bitcoincash';
      case CoinType.stellar:
        return 'stellar';
      case CoinType.bitcoingold:
        return 'bitcoingold';
      case CoinType.nano:
        return 'nano';
      case CoinType.ravencoin:
        return 'ravencoin';
      case CoinType.poa:
        return 'poa';
      case CoinType.eos:
        return 'eos';
      case CoinType.tron:
        return 'tron';
      case CoinType.fio:
        return 'fio';
      case CoinType.nimiq:
        return 'nimiq';
      case CoinType.algorand:
        return 'algorand';
      case CoinType.iotex:
        return 'iotex';
      case CoinType.zilliqa:
        return 'zilliqa';
      case CoinType.terra:
        return 'terra';
      case CoinType.polkadot:
        return 'polkadot';
      case CoinType.ton:
        return 'ton';
      case CoinType.near:
        return 'near';
      case CoinType.aion:
        return 'aion';
      case CoinType.kusama:
        return 'kusama';
      case CoinType.aeternity:
        return 'aeternity';
      case CoinType.kava:
        return 'kava';
      case CoinType.filecoin:
        return 'filecoin';
      case CoinType.band:
        return 'band';
      case CoinType.theta:
        return 'theta';
      case CoinType.solana:
        return 'solana';
      case CoinType.elrond:
        return 'elrond';
      case CoinType.binance:
        return 'binance';
      case CoinType.vechain:
        return 'vechain';
      case CoinType.callisto:
        return 'callisto';
      case CoinType.neo:
        return 'neo';
      case CoinType.tomochain:
        return 'tomochain';
      case CoinType.thundertoken:
        return 'thundertoken';
      case CoinType.harmony:
        return 'harmony';
      case CoinType.oasis:
        return 'oasis';
      case CoinType.ontology:
        return 'ontology';
      case CoinType.tezos:
        return 'tezos';
      case CoinType.cardano:
        return 'cardano';
      case CoinType.kin:
        return 'kin';
      case CoinType.qtum:
        return 'qtum';
      case CoinType.nebulas:
        return 'nebulas';
      case CoinType.gochain:
        return 'gochain';
      case CoinType.nuls:
        return 'nuls';
      case CoinType.zelcash:
        return 'zelcash';
      case CoinType.wanchain:
        return 'wanchain';
      case CoinType.waves:
        return 'waves';
      case CoinType.bsc:
        return 'bsc';
      case CoinType.smartchain:
        return 'smartchain';
      case CoinType.polygon:
        return 'polygon';
      case CoinType.thorchain:
        return 'thorchain';
    }
  }
  export function decimals(coin: CoinType): number {
    switch (coin) {
      case CoinType.bitcoin:
        return 8;
      case CoinType.litecoin:
        return 8;
      case CoinType.doge:
        return 8;
      case CoinType.dash:
        return 8;
      case CoinType.viacoin:
        return 8;
      case CoinType.groestlcoin:
        return 8;
      case CoinType.digibyte:
        return 8;
      case CoinType.monacoin:
        return 8;
      case CoinType.decred:
        return 8;
      case CoinType.ethereum:
        return 18;
      case CoinType.classic:
        return 18;
      case CoinType.icon:
        return 18;
      case CoinType.cosmos:
        return 6;
      case CoinType.zcash:
        return 8;
      case CoinType.zcoin:
        return 8;
      case CoinType.ripple:
        return 6;
      case CoinType.bitcoincash:
        return 8;
      case CoinType.stellar:
        return 7;
      case CoinType.bitcoingold:
        return 8;
      case CoinType.nano:
        return 30;
      case CoinType.ravencoin:
        return 8;
      case CoinType.poa:
        return 18;
      case CoinType.eos:
        return 4;
      case CoinType.tron:
        return 6;
      case CoinType.fio:
        return 9;
      case CoinType.nimiq:
        return 5;
      case CoinType.algorand:
        return 6;
      case CoinType.iotex:
        return 18;
      case CoinType.zilliqa:
        return 12;
      case CoinType.terra:
        return 6;
      case CoinType.polkadot:
        return 10;
      case CoinType.ton:
        return 9;
      case CoinType.near:
        return 24;
      case CoinType.aion:
        return 18;
      case CoinType.kusama:
        return 12;
      case CoinType.aeternity:
        return 18;
      case CoinType.kava:
        return 6;
      case CoinType.filecoin:
        return 18;
      case CoinType.band:
        return 6;
      case CoinType.theta:
        return 18;
      case CoinType.solana:
        return 9;
      case CoinType.elrond:
        return 18;
      case CoinType.binance:
        return 8;
      case CoinType.vechain:
        return 18;
      case CoinType.callisto:
        return 18;
      case CoinType.neo:
        return 8;
      case CoinType.tomochain:
        return 18;
      case CoinType.thundertoken:
        return 18;
      case CoinType.harmony:
        return 18;
      case CoinType.oasis:
        return 9;
      case CoinType.ontology:
        return 0;
      case CoinType.tezos:
        return 6;
      case CoinType.cardano:
        return 6;
      case CoinType.kin:
        return 5;
      case CoinType.qtum:
        return 8;
      case CoinType.nebulas:
        return 18;
      case CoinType.gochain:
        return 18;
      case CoinType.nuls:
        return 8;
      case CoinType.zelcash:
        return 8;
      case CoinType.wanchain:
        return 18;
      case CoinType.waves:
        return 8;
      case CoinType.bsc:
        return 18;
      case CoinType.smartchain:
        return 18;
      case CoinType.polygon:
        return 18;
      case CoinType.thorchain:
        return 18;
    }
  }
  export function name(coin: CoinType): string {
    switch (coin) {
      case CoinType.bitcoin:
        return 'Bitcoin';
      case CoinType.litecoin:
        return 'Litecoin';
      case CoinType.doge:
        return 'Dogecoin';
      case CoinType.dash:
        return 'Dash';
      case CoinType.viacoin:
        return 'Viacoin';
      case CoinType.groestlcoin:
        return 'Groestlcoin';
      case CoinType.digibyte:
        return 'DigiByte';
      case CoinType.monacoin:
        return 'Monacoin';
      case CoinType.decred:
        return 'Decred';
      case CoinType.ethereum:
        return 'Ethereum';
      case CoinType.classic:
        return 'Ethereum Classic';
      case CoinType.icon:
        return 'ICON';
      case CoinType.cosmos:
        return 'Cosmos';
      case CoinType.zcash:
        return 'Zcash';
      case CoinType.zcoin:
        return 'Zcoin';
      case CoinType.ripple:
        return 'XRP';
      case CoinType.bitcoincash:
        return 'Bitcoin Cash';
      case CoinType.stellar:
        return 'Stellar';
      case CoinType.bitcoingold:
        return 'Bitcoin Gold';
      case CoinType.nano:
        return 'Nano';
      case CoinType.ravencoin:
        return 'Ravencoin';
      case CoinType.poa:
        return 'POA Network';
      case CoinType.eos:
        return 'EOS';
      case CoinType.tron:
        return 'Tron';
      case CoinType.fio:
        return 'FIO';
      case CoinType.nimiq:
        return 'Nimiq';
      case CoinType.algorand:
        return 'Algorand';
      case CoinType.iotex:
        return 'IoTeX';
      case CoinType.zilliqa:
        return 'Zilliqa';
      case CoinType.terra:
        return 'Terra';
      case CoinType.polkadot:
        return 'Polkadot';
      case CoinType.ton:
        return 'TON';
      case CoinType.near:
        return 'NEAR';
      case CoinType.aion:
        return 'Aion';
      case CoinType.kusama:
        return 'Kusama';
      case CoinType.aeternity:
        return 'Aeternity';
      case CoinType.kava:
        return 'Kava';
      case CoinType.filecoin:
        return 'Filecoin';
      case CoinType.band:
        return 'BandChain';
      case CoinType.theta:
        return 'Theta';
      case CoinType.solana:
        return 'Solana';
      case CoinType.elrond:
        return 'Elrond';
      case CoinType.binance:
        return 'Binance';
      case CoinType.vechain:
        return 'VeChain';
      case CoinType.callisto:
        return 'Callisto';
      case CoinType.neo:
        return 'NEO';
      case CoinType.tomochain:
        return 'TomoChain';
      case CoinType.thundertoken:
        return 'Thunder Token';
      case CoinType.harmony:
        return 'Harmony';
      case CoinType.oasis:
        return 'Oasis';
      case CoinType.ontology:
        return 'Ontology';
      case CoinType.tezos:
        return 'Tezos';
      case CoinType.cardano:
        return 'Cardano';
      case CoinType.kin:
        return 'Kin';
      case CoinType.qtum:
        return 'Qtum';
      case CoinType.nebulas:
        return 'Nebulas';
      case CoinType.gochain:
        return 'GoChain';
      case CoinType.nuls:
        return 'NULS';
      case CoinType.zelcash:
        return 'Zelcash';
      case CoinType.wanchain:
        return 'Wanchain';
      case CoinType.waves:
        return 'Waves';
      case CoinType.bsc:
        return 'Smart Chain Legacy';
      case CoinType.smartchain:
        return 'Smart Chain';
      case CoinType.polygon:
        return 'Polygon';
      case CoinType.thorchain:
        return 'THORChain';
    }
  }
  export function derivationPath(coin: CoinType): string {
    switch (coin) {
      case CoinType.bitcoin:
        return "m/84'/0'/0'/0/0";
      case CoinType.litecoin:
        return "m/84'/2'/0'/0/0";
      case CoinType.doge:
        return "m/44'/3'/0'/0/0";
      case CoinType.dash:
        return "m/44'/5'/0'/0/0";
      case CoinType.viacoin:
        return "m/84'/14'/0'/0/0";
      case CoinType.groestlcoin:
        return "m/84'/17'/0'/0/0";
      case CoinType.digibyte:
        return "m/84'/20'/0'/0/0";
      case CoinType.monacoin:
        return "m/44'/22'/0'/0/0";
      case CoinType.decred:
        return "m/44'/42'/0'/0/0";
      case CoinType.ethereum:
        return "m/44'/60'/0'/0/0";
      case CoinType.classic:
        return "m/44'/61'/0'/0/0";
      case CoinType.icon:
        return "m/44'/74'/0'/0/0";
      case CoinType.cosmos:
        return "m/44'/118'/0'/0/0";
      case CoinType.zcash:
        return "m/44'/133'/0'/0/0";
      case CoinType.zcoin:
        return "m/44'/136'/0'/0/0";
      case CoinType.ripple:
        return "m/44'/144'/0'/0/0";
      case CoinType.bitcoincash:
        return "m/44'/145'/0'/0/0";
      case CoinType.stellar:
        return "m/44'/148'/0'";
      case CoinType.bitcoingold:
        return "m/84'/156'/0'/0/0";
      case CoinType.nano:
        return "m/44'/165'/0'";
      case CoinType.ravencoin:
        return "m/44'/175'/0'/0/0";
      case CoinType.poa:
        return "m/44'/178'/0'/0/0";
      case CoinType.eos:
        return "m/44'/194'/0'/0/0";
      case CoinType.tron:
        return "m/44'/195'/0'/0/0";
      case CoinType.fio:
        return "m/44'/235'/0'/0/0";
      case CoinType.nimiq:
        return "m/44'/242'/0'/0'";
      case CoinType.algorand:
        return "m/44'/283'/0'/0'/0'";
      case CoinType.iotex:
        return "m/44'/304'/0'/0/0";
      case CoinType.zilliqa:
        return "m/44'/313'/0'/0/0";
      case CoinType.terra:
        return "m/44'/330'/0'/0/0";
      case CoinType.polkadot:
        return "m/44'/354'/0'/0'/0'";
      case CoinType.ton:
        return "m/44'/396'/0'";
      case CoinType.near:
        return "m/44'/397'/0'";
      case CoinType.aion:
        return "m/44'/425'/0'/0'/0'";
      case CoinType.kusama:
        return "m/44'/434'/0'/0'/0'";
      case CoinType.aeternity:
        return "m/44'/457'/0'/0'/0'";
      case CoinType.kava:
        return "m/44'/459'/0'/0/0";
      case CoinType.filecoin:
        return "m/44'/461'/0'/0/0";
      case CoinType.band:
        return "m/44'/494'/0'/0/0";
      case CoinType.theta:
        return "m/44'/500'/0'/0/0";
      case CoinType.solana:
        return "m/44'/501'/0'";
      case CoinType.elrond:
        return "m/44'/508'/0'/0'/0'";
      case CoinType.binance:
        return "m/44'/714'/0'/0/0";
      case CoinType.vechain:
        return "m/44'/818'/0'/0/0";
      case CoinType.callisto:
        return "m/44'/820'/0'/0/0";
      case CoinType.neo:
        return "m/44'/888'/0'/0/0";
      case CoinType.tomochain:
        return "m/44'/889'/0'/0/0";
      case CoinType.thundertoken:
        return "m/44'/1001'/0'/0/0";
      case CoinType.harmony:
        return "m/44'/1023'/0'/0/0";
      case CoinType.oasis:
        return "m/44'/474'/0'/0'/0'";
      case CoinType.ontology:
        return "m/44'/1024'/0'/0/0";
      case CoinType.tezos:
        return "m/44'/1729'/0'/0'";
      case CoinType.cardano:
        return "m/1852'/1815'/0'/0/0";
      case CoinType.kin:
        return "m/44'/2017'/0'";
      case CoinType.qtum:
        return "m/44'/2301'/0'/0/0";
      case CoinType.nebulas:
        return "m/44'/2718'/0'/0/0";
      case CoinType.gochain:
        return "m/44'/6060'/0'/0/0";
      case CoinType.nuls:
        return "m/44'/8964'/0'/0/0";
      case CoinType.zelcash:
        return "m/44'/19167'/0'/0/0";
      case CoinType.wanchain:
        return "m/44'/5718350'/0'/0/0";
      case CoinType.waves:
        return "m/44'/5741564'/0'/0'/0'";
      case CoinType.bsc:
        return "m/44'/714'/0'/0/0";
      case CoinType.smartchain:
        return "m/44'/60'/0'/0/0";
      case CoinType.polygon:
        return "m/44'/60'/0'/0/0";
      case CoinType.thorchain:
        return "m/44'/931'/0'/0/0";
    }
  }
  export function symbol(coin: CoinType): string {
    switch (coin) {
      case CoinType.bitcoin:
        return 'BTC';
      case CoinType.litecoin:
        return 'LTC';
      case CoinType.doge:
        return 'DOGE';
      case CoinType.dash:
        return 'DASH';
      case CoinType.viacoin:
        return 'VIA';
      case CoinType.groestlcoin:
        return 'GRS';
      case CoinType.digibyte:
        return 'DGB';
      case CoinType.monacoin:
        return 'MONA';
      case CoinType.decred:
        return 'DCR';
      case CoinType.ethereum:
        return 'ETH';
      case CoinType.classic:
        return 'ETC';
      case CoinType.icon:
        return 'ICX';
      case CoinType.cosmos:
        return 'ATOM';
      case CoinType.zcash:
        return 'ZEC';
      case CoinType.zcoin:
        return 'FIRO';
      case CoinType.ripple:
        return 'XRP';
      case CoinType.bitcoincash:
        return 'BCH';
      case CoinType.stellar:
        return 'XLM';
      case CoinType.bitcoingold:
        return 'BTG';
      case CoinType.nano:
        return 'NANO';
      case CoinType.ravencoin:
        return 'RVN';
      case CoinType.poa:
        return 'POA';
      case CoinType.eos:
        return 'EOS';
      case CoinType.tron:
        return 'TRX';
      case CoinType.fio:
        return 'FIO';
      case CoinType.nimiq:
        return 'NIM';
      case CoinType.algorand:
        return 'ALGO';
      case CoinType.iotex:
        return 'IOTX';
      case CoinType.zilliqa:
        return 'ZIL';
      case CoinType.terra:
        return 'LUNA';
      case CoinType.polkadot:
        return 'DOT';
      case CoinType.ton:
        return 'GRAM';
      case CoinType.near:
        return 'NEAR';
      case CoinType.aion:
        return 'AION';
      case CoinType.kusama:
        return 'KSM';
      case CoinType.aeternity:
        return 'AE';
      case CoinType.kava:
        return 'KAVA';
      case CoinType.filecoin:
        return 'FIL';
      case CoinType.band:
        return 'BAND';
      case CoinType.theta:
        return 'THETA';
      case CoinType.solana:
        return 'SOL';
      case CoinType.elrond:
        return 'eGLD';
      case CoinType.binance:
        return 'BNB';
      case CoinType.vechain:
        return 'VET';
      case CoinType.callisto:
        return 'CLO';
      case CoinType.neo:
        return 'NEO';
      case CoinType.tomochain:
        return 'TOMO';
      case CoinType.thundertoken:
        return 'TT';
      case CoinType.harmony:
        return 'ONE';
      case CoinType.oasis:
        return 'ROSE';
      case CoinType.ontology:
        return 'ONT';
      case CoinType.tezos:
        return 'XTZ';
      case CoinType.cardano:
        return 'ADA';
      case CoinType.kin:
        return 'KIN';
      case CoinType.qtum:
        return 'QTUM';
      case CoinType.nebulas:
        return 'NAS';
      case CoinType.gochain:
        return 'GO';
      case CoinType.nuls:
        return 'NULS';
      case CoinType.zelcash:
        return 'FLUX';
      case CoinType.wanchain:
        return 'WAN';
      case CoinType.waves:
        return 'WAVES';
      case CoinType.bsc:
        return 'BNB';
      case CoinType.smartchain:
        return 'BNB';
      case CoinType.polygon:
        return 'MATIC';
      case CoinType.thorchain:
        return 'RUNE';
    }
  }
  export function slip44(coin: CoinType): number {
    switch (coin) {
      case CoinType.bitcoin:
        return 0;
      case CoinType.litecoin:
        return 2;
      case CoinType.doge:
        return 3;
      case CoinType.dash:
        return 5;
      case CoinType.viacoin:
        return 14;
      case CoinType.groestlcoin:
        return 17;
      case CoinType.digibyte:
        return 20;
      case CoinType.monacoin:
        return 22;
      case CoinType.decred:
        return 42;
      case CoinType.ethereum:
        return 60;
      case CoinType.classic:
        return 61;
      case CoinType.icon:
        return 74;
      case CoinType.cosmos:
        return 118;
      case CoinType.zcash:
        return 133;
      case CoinType.zcoin:
        return 136;
      case CoinType.ripple:
        return 144;
      case CoinType.bitcoincash:
        return 145;
      case CoinType.stellar:
        return 148;
      case CoinType.bitcoingold:
        return 156;
      case CoinType.nano:
        return 165;
      case CoinType.ravencoin:
        return 175;
      case CoinType.poa:
        return 178;
      case CoinType.eos:
        return 194;
      case CoinType.tron:
        return 195;
      case CoinType.fio:
        return 235;
      case CoinType.nimiq:
        return 242;
      case CoinType.algorand:
        return 283;
      case CoinType.iotex:
        return 304;
      case CoinType.zilliqa:
        return 313;
      case CoinType.terra:
        return 330;
      case CoinType.polkadot:
        return 354;
      case CoinType.ton:
        return 396;
      case CoinType.near:
        return 397;
      case CoinType.aion:
        return 425;
      case CoinType.kusama:
        return 434;
      case CoinType.aeternity:
        return 457;
      case CoinType.kava:
        return 459;
      case CoinType.filecoin:
        return 461;
      case CoinType.band:
        return 494;
      case CoinType.theta:
        return 500;
      case CoinType.solana:
        return 501;
      case CoinType.elrond:
        return 508;
      case CoinType.binance:
        return 714;
      case CoinType.vechain:
        return 818;
      case CoinType.callisto:
        return 820;
      case CoinType.neo:
        return 888;
      case CoinType.tomochain:
        return 889;
      case CoinType.thundertoken:
        return 1001;
      case CoinType.harmony:
        return 1023;
      case CoinType.oasis:
        return 474;
      case CoinType.ontology:
        return 1024;
      case CoinType.tezos:
        return 1729;
      case CoinType.cardano:
        return 1815;
      case CoinType.kin:
        return 2017;
      case CoinType.qtum:
        return 2301;
      case CoinType.nebulas:
        return 2718;
      case CoinType.gochain:
        return 6060;
      case CoinType.nuls:
        return 8964;
      case CoinType.zelcash:
        return 19167;
      case CoinType.wanchain:
        return 5718350;
      case CoinType.waves:
        return 5741564;
      case CoinType.bsc:
        return 714;
      case CoinType.smartchain:
        return 714;
      case CoinType.polygon:
        return 60;
      case CoinType.thorchain:
        return 931;
    }
  }
}

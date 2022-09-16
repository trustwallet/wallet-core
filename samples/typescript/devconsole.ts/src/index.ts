#!/usr/bin/env node

const chalk = require('chalk');
const clear = require('clear');
const figlet = require('figlet');
const repl = require('node:repl');
//const inquirer = require('inquirer');
const { initWasm, TW } = require("@trustwallet/wallet-core");

function enumerateNamespaces(topLevelNamespace: any) {
    var ns : string[] = [];
    for (var member in topLevelNamespace) {
        if (typeof topLevelNamespace[member] == 'function') {
            var firstLetter = member[0];
            if (firstLetter >= 'A' && firstLetter <= 'Z') {
                ns.push(member);
            }
        }
    }
    return ns;
}

(async function () {
    class AddressCoin {
        coin: any;
        address: string;
        constructor(coin: string, address: string) {
            this.coin = coin;
            this.address = address;
        }
    };

    class TestWallet {
        name: string;
        wallet: any;
        addresses: AddressCoin[];

        constructor(name: string = '', wallet: any) {
            this.name = name;
            this.wallet = wallet;
            this.addresses = [];
            this.addDefaultCoins();
        }
        public static createRandom(name: string = '', strength: number = 256): TestWallet {
            let wallet = WC.HDWallet.create(strength, '');
            return new TestWallet(name, wallet);
        }
        public static createWithMnemonic(name: string = '', mnemonic: string): TestWallet {
            let wallet = WC.HDWallet.createWithMnemonic(mnemonic, '');
            return new TestWallet(name, wallet);
        }
        addCoin(coin: string): void {
            const address = this.wallet.getAddressForCoin(coin);
            this.addresses.push(new AddressCoin(coin, address));
        }
        addDefaultCoins(): void {
            this.addCoin(WC.CoinType.bitcoin);
            this.addCoin(WC.CoinType.ethereum);
            this.addCoin(WC.CoinType.binance);
        }
        status(): string {
            return `Wallet '${this.name}', with ${this.addresses.length} addresses`;
        }
        dump(): void {
            console.error(`Wallet ${this.name}:`);

            if (this.addresses.length == 0) {
                console.log('No addresses');
            } else {
                console.log('Addresses:');
                this.addresses.forEach(element => {
                    console.log(`  ${WC.CoinTypeConfiguration.getSymbol(element.coin)}:  ${element.address}`);
                });
            }
        }
    };

    function help(): void {
        console.log('This is an interactive typescript shell, to work with wallet-core (wasm)');
        console.log('You can use:');
        console.log();
        console.log("- Any typescript commands                        EXAMPLES");
        console.log("                                                 'wallet-core'.length");
        console.log("                                                 x = 5; ++x");
        console.log();
        console.log('- Any method from Wallet-Core, through various namespaces, see full list below.  Some examples:');
        console.log("    CoinType                                     CoinType.bitcoin.value");
        console.log("    HDWallet                                     wallet = HDWallet.create(256, ''); wallet.mnemonic()");
        console.log("    PrivateKey                                   pk = PrivateKey.createWithData(HexCoding.decode('afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5'))");
        console.log("    PublicKey                                    pubkey = PublicKey.createWithData(HexCoding.decode('0399c6f51ad6f98c9c583f8e92bb7758ab2ca9a04110c0a1126ec43e5453d196c1'), PublicKeyType.secp256k1)");
        console.log("                                                 pubkey.verify(HexCoding.decode('0x673b54a91d87cfb9389e54cc55b1a9343a6eb9f2ea1f449cb19a248a86bb904c1efb109f8cf655c4f510211053c1696e52c75843c5c803fa1b78fe0c263f468201'), HexCoding.decode('0001020304050607080910111213141519171819202122232425262728293031'))");
        console.log("    AnyAddress                                   AnyAddress.createWithString('EQCTVra3xPXenA1wNFMba2taTsc9XMdfCWC7FJpGXjk7', CoinType.solana).description()");
        console.log("    CoinTypeConfiguration                        CoinTypeConfiguration.getSymbol(CoinType.bitcoin)");
        console.log("    Mnemonmic                                    Mnemonic.isValidWord('acid')");
        console.log("    HexCoding                                    HexCoding.encode(HexCoding.decode('01ab02'))");
        console.log();
        console.log("- Convenience methods:");
        console.log("    walletCreate([strength], [name])             walletCreate(128)");
        console.log("    walletImport(mnemonic, [name])               walletImport('ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal', 'Test1')");
        console.log("    walletDump()                                 walletDump()");
        console.log();
        console.log("A longer example:");
        console.log("    coin = CoinType.solana");
        console.log("    wallet = HDWallet.createWithMnemonic('ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal', '')");
        console.log("    wallet.getAddressForCoin(coin)");
        console.log("    pk = wallet.getKeyForCoin(coin)");
        console.log("    HexCoding.encode(pk.data())");
        console.log("    pubkey = pk.getPublicKeyEd25519()");
        console.log("    HexCoding.encode(pubkey.data())");
        console.log("    a1 = AnyAddress.createWithPublicKey(pubkey, coin)");
        console.log("    a1.description()");
        console.log();
        console.log("Full list of exposed Wallet-core namespaces:");
        enumerateNamespaces(WC).sort().forEach(ns => process.stdout.write(ns + ', '));
        console.log();
        console.log();
    }

    function exit(code: number): void {
        process.exit(code);
    }

    function walletCreate(strength: number = 256, name: string = ''): any {
        wallet = TestWallet.createRandom(name, strength);
        console.log(`Wallet ${wallet.name} created, mnemonic: ${wallet.wallet.mnemonic()}`);
        walletStatus();
        return wallet;
    }

    function walletImport(mnemonic: string, name: string = ''): any {
        if (!WC.Mnemonic.isValid(mnemonic)) {
            console.error(`Mnemonic is not valid ${mnemonic}`);
            return null;
        }
        wallet = TestWallet.createWithMnemonic(name, mnemonic);
        console.log(`Wallet ${wallet.name} imported, mnemonic: ${wallet.wallet.mnemonic()}`);
        walletStatus();
        return wallet;
    }

    function walletStatus(): void {
        if (wallet === null) {
            console.error('No wallet, see createWallet()');
        } else {
            console.error(`Wallet loaded: ${wallet.status()}`);
        }
    }

    function walletDump(): void {
        walletStatus();
        if (wallet) {
            wallet.dump();
        }
    }

    // single global TestWallet instance
    let wallet: TestWallet | null = null;

    clear();
    console.log(
        chalk.blue(
            figlet.textSync('wallet-core', { horizontalLayout: 'full' })
        )
    );
    console.log(chalk.blue('  devconsole.ts, wasm lib'));
    console.log();

    process.stdout.write("Initializing WalletCore library ...");
    const WC = await initWasm();
    console.log(` done.`);
    console.log();
    console.log(`Type 'help()' for help`);
    console.log(`Press Ctrl-C twice to exit`);
    console.log(chalk.red(`This is a test tool, DO NOT USE WITH REAL FUNDS!`));
    console.log();

    const local = repl.start('> ');

    // Expose WC namespaces
    var nss = enumerateNamespaces(WC);
    nss.forEach(ns => local.context[ns] = WC[ns]);
    local.context.WC = WC;

    // Expose more stuff; utilities
    local.context.help = help;
    local.context.exit = exit;
    local.context.walletCreate = walletCreate;
    local.context.walletImport = walletImport;
    local.context.walletDump = walletDump;
    local.context.wallet = wallet;
    local.context.coin = WC.CoinType.bitcoin;

    local.on('exit', () => {
        console.log('Bye!');
        process.exit();
    });
})();

#!/usr/bin/env node

const chalk = require('chalk');
const clear = require('clear');
const figlet = require('figlet');
const repl = require('node:repl');
//const inquirer = require('inquirer');
const { initWasm, TW } = require("@trustwallet/wallet-core");

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
            let wallet = HDWallet.create(strength, '');
            return new TestWallet(name, wallet);
        }
        public static createWithMnemonic(name: string = '', mnemonic: string): TestWallet {
            let wallet = HDWallet.createWithMnemonic(mnemonic, '');
            return new TestWallet(name, wallet);
        }
        addCoin(coin: string): void {
            const address = this.wallet.getAddressForCoin(coin);
            this.addresses.push(new AddressCoin(coin, address));
        }
        addDefaultCoins(): void {
            this.addCoin(CoinType.bitcoin);
            this.addCoin(CoinType.ethereum);
            this.addCoin(CoinType.binance);
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
                    console.log(`  ${CoinTypeConfiguration.getSymbol(element.coin)}:  ${element.address}`);
                });
            }
        }
    };

    function help(): void {
        console.log('This is an interactive node typescript shell, you can use:');
        console.log();
        console.log("- Any typescript commands                        EXAMPLES");
        console.log("                                                 'wallet-core'.length");
        console.log("                                                 x = 5; ++x");
        console.log();
        console.log('- Any method from Wallet-Core, through following namespaces:');
        console.log("    CoinType                                     CoinType.bitcoin.value");
        console.log("    HDWallet                                     wallet = HDWallet.create(256, ''); wallet.mnemonic()");
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
    }

    function walletCreate(strength: number = 256, name: string = ''): any {
        /*
        if (name === '') {
            let input = await inquirer.prompt([{
                type: 'input',
                name: 'name',
                message: "Wallet name:",
            }]);
            name = input.name;
        }
        */

        wallet = TestWallet.createRandom(name, strength);
        console.log(`Wallet ${wallet.name} created, mnemonic: ${wallet.wallet.mnemonic()}`);
        walletStatus();
        return wallet;
    }

    function walletImport(mnemonic: string, name: string = ''): any {
        if (!Mnemonic.isValid(mnemonic)) {
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
    const { CoinType, HDWallet, AnyAddress, CoinTypeConfiguration, Mnemonic, HexCoding } = await initWasm();
    console.log(` done.`);
    console.log();
    console.log(`Type 'help()' for help`);
    console.log(`Press Ctrl-C twice to exit`);
    console.log();

    const local = repl.start('> ');

    local.context.CoinType = CoinType;
    local.context.HDWallet = HDWallet;
    local.context.AnyAddress = AnyAddress;
    local.context.CoinTypeConfiguration = CoinTypeConfiguration;
    local.context.Mnemonic = Mnemonic;
    local.context.HexCoding = HexCoding;

    local.context.help = help;
    local.context.walletCreate = walletCreate;
    local.context.walletImport = walletImport;
    local.context.walletDump = walletDump;
    local.context.wallet = wallet;
    local.context.coin = CoinType.bitcoin;

    local.on('exit', () => {
        console.log('Bye!');
        process.exit();
    });
})();

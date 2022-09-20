#!/usr/bin/env ts-node

const chalk = require('chalk');
const clear = require('clear');
const figlet = require('figlet');
const repl = require('node:repl');
const { initWasm, TW, KeyStore } = require("@trustwallet/wallet-core");
const fs = require("fs");

function enumerateNamespaces(topLevelNamespace: any) {
    const exceptions: string[] = ['AsciiToString', 'ExitStatus', 'ENV', 'ERRNO_CODES', 'ERRNO_MESSAGES', 'DNS', 'Protocols', 'Sockets', 'UNWIND_CACHE', 'PATH', 'PATH_FS', 'SYSCALLS', 'JSEvents', 'JSEvents_requestFullscreen', 'JSEvents_resizeCanvasForFullscreen', 'ExceptionInfo', 'Browser', 'FS', 'MEMFS', 'TTY', 'PIPEFS', 'SOCKFS', 'RegisteredClass', 'Emval'];
    var ns : string[] = [];
    for (var member in topLevelNamespace) {
        if (typeof topLevelNamespace[member] == 'function' || typeof topLevelNamespace[member] == 'object') {
            ns.push(member);
        }
    }
    return ns
        .filter(n => { var firstLetter = n[0]; return (firstLetter >= 'A' && firstLetter <= 'Z'); })
        .filter(n => !n.includes('Error'))
        .filter(n => !n.startsWith('HEAP'))
        .filter(n => !n.startsWith('UTF'))
        .filter(n => !n.startsWith('FS_'))
        .filter(n => !n.startsWith('ClassHandle'))
        .filter(n => !n.startsWith('RegisteredPointer'))
        .filter(n => !exceptions.includes(n));
}

async function main() {
    // Wrapper for a wallet stored in keystore
    class KeystoreWallet {
        wallet: any;
        constructor(w: any) {
            this.wallet = w;
        }
        status(): string {
            return `Wallet '${this.wallet.name}', with ${this.wallet.activeAccounts.length} addresses`;
        }
        dump(): void {
            console.error(`Wallet '${this.wallet.name}', type: ${this.wallet.type}:`);

            if (this.wallet.activeAccounts.length == 0) {
                console.log('No addresses');
            } else {
                console.log('Addresses:');
                this.wallet.activeAccounts.forEach((element: any) => {
                    // TODO: print symbol instead of CoinId
                    console.log(`  ${element.coin}:  ${element.address}`);
                });
            }
        }
    }

    // Handles several wallets, with keystore
    class TestWalletManager {
        // single KeystoreWallet instance
        wallet: KeystoreWallet | null = null;
        StoreFolderPath = "/tmp/devconsole.ts";
        StoreFixedPassword = "devconsole.ts";
        HDWalletPassord = '';
        DefaultCoins: any[] = [];
        keystore: any;
        // wallets available in storage
        storeWallets: any[] = [];
        
        init(): void {
            this.DefaultCoins = [WC.CoinType.bitcoin, WC.CoinType.ethereum, WC.CoinType.binance];
            fs.mkdir(this.StoreFolderPath, () => {});
            const storage = new KeyStore.FileSystemStorage(this.StoreFolderPath);
            this.keystore = new KeyStore.Default(WC, storage);
        }
        async refreshStoreWallets(): Promise<void> {
            process.stdout.write(`Refreshing wallets list ... `);
            this.storeWallets = await this.keystore.loadAll();
            console.log(`found ${this.storeWallets.length} wallets`);
        }
        async list(): Promise<void> {
            await this.refreshStoreWallets();
            if (!this.storeWallets || this.storeWallets.length == 0) {
                console.log("No wallets found in storage.");
            } else {
                let idx = 0;
                console.log(`Found ${this.storeWallets.length} wallets in storage, use walletLoad(id) to load:`);
                this.storeWallets.forEach(w => console.log(`${idx++}:  ${w.name} \t${w.type} \t${w.id}`));
            }
        }
        async load(index: number) {
            await this.refreshStoreWallets();
            if (index >= this.storeWallets.length) {
                console.log(`Index out of range, max ${this.storeWallets.length}`);
                await walletsList();
                return;
            }
            const id = this.storeWallets[index].id;
            process.stdout.write(`Loading wallet ${index} ${id} ... `);
            const w = await this.keystore.load(id);
            console.log("done");
            this.wallet = new KeystoreWallet(w);
        }
        async loadAWallet() {
            if (this.storeWallets.length > 0) {
                await this.load(0);
                this.status();
            }
        }
        // Delete loaded wallet
        async delete(param: string): Promise<void> {
            if (!this.wallet) {
                console.log(`No wallet loaded`);
                return;
            }
            if (param !== 'delete') {
                console.log(`Are you sure? Invoke with 'delete' parameter!`);
                return;
            }
            const id = this.wallet.wallet.id;
            await this.keystore.delete(id, this.StoreFixedPassword);
            this.wallet = null;
            console.log(`Wallet ${id} deleted.`)
            await this.refreshStoreWallets();
        }
        async deleteAll(param: string): Promise<void> {
            await this.refreshStoreWallets();
            if (this.storeWallets.length == 0) {
                console.log(`No wallets found`);
                return;
            }
            if (param !== 'deleteall') {
                console.log(`Are you sure? Invoke with 'deleteall' parameter!`);
                return;
            }
            while (this.storeWallets.length > 0) {
                const id = this.storeWallets[0].id;
                await this.keystore.delete(id, this.StoreFixedPassword);
                await this.refreshStoreWallets();
            }
        }
        autoFillName(name: string): string {
            return (name === '') ? `Wallet${(this.storeWallets.length + 1).toString()}` : name;
        }
        async create(strength: number, name: string): Promise<any> {
            if (name === '') { name = this.autoFillName(name); }
            const hdWallet = WC.HDWallet.create(strength, this.HDWalletPassord);
            const storedKey = WC.StoredKey.importHDWallet(hdWallet.mnemonic(), name, Buffer.from(this.StoreFixedPassword), this.DefaultCoins[0]);
            this.DefaultCoins.forEach((coin) => {
                storedKey.accountForCoin(coin, hdWallet);
            });
            let wallet = this.keystore.mapWallet(storedKey);
            await this.keystore.importWallet(wallet);
            this.wallet = new KeystoreWallet(wallet);
            console.log(`Wallet ${this.wallet?.wallet?.name} created, mnemonic: ${hdWallet.mnemonic()}`);
            walletStatus();
            return this.wallet;
        }
        async import(mnemonic: string, name: string): Promise<any> {
            if (!WC.Mnemonic.isValid(mnemonic)) {
                console.error(`Mnemonic is not valid ${mnemonic}`);
                return null;
            }
            if (name === '') { name = this.autoFillName(name); }
            /*
            const hdWallet = WC.HDWallet.createWithMnemonic(mnemonic, this.HDWalletPassord);
            const storedKey = WC.StoredKey.importHDWallet(hdWallet.mnemonic(), name, Buffer.from(this.StoreFixedPassword), this.DefaultCoins[0]);
            this.DefaultCoins.forEach((coin) => {
                storedKey.accountForCoin(coin, hdWallet);
            });
            let wallet = this.keystore.mapWallet(storedKey);
            await this.keystore.importWallet(wallet);
            */
            let wallet = await this.keystore.import(mnemonic, name, this.StoreFixedPassword, this.DefaultCoins);
            this.wallet = new KeystoreWallet(wallet);
            console.log(`Wallet ${this.wallet?.wallet?.name} imported, mnemonic: ${mnemonic}`);
            walletStatus();
            return this.wallet;
        }
        async addCoin(coin: string): Promise<void> {
            if (this.wallet == null) {
                console.error('No wallet open, see walletCreate() / walletLoad() / walletsList()');
                return;
            }
            const wallet = await this.keystore.addAccounts(this.wallet?.wallet.id, this.StoreFixedPassword, [coin]);
            this.wallet = new KeystoreWallet(wallet);
        }
        status(): void {
            if (this.wallet === null) {
                console.error('No wallet open, see walletCreate() / walletLoad() / walletsList()');
            } else {
                console.error(`Wallet open: ${this.wallet.status()}`);
            }
        }
        dump(): void {
            this.status();
            if (this.wallet) {
                this.wallet.dump();
            }
        }
    };

    // single global TestWalletManager instance
    let wallets: TestWalletManager = new TestWalletManager();

    async function walletsList() { await wallets.list(); }
    async function walletLoad(index: number) { await wallets.load(index); }
    async function walletsDeleteAll(param: string) { await wallets.deleteAll(param); }
    async function walletCreate(strength: number = 256, name: string = ''): Promise<any> { return await wallets.create(strength, name); }
    async function walletImport(mnemonic: string, name: string = ''): Promise<any> { return wallets.import(mnemonic, name); }
    async function walletAddCoin(coin: string): Promise<void> { return wallets.addCoin(coin); }
    function walletStatus(): void { wallets.status(); }
    function walletDump(): void { wallets.dump(); }
    async function walletDelete(param: string) { await wallets.delete(param); }

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
        console.log("See examples in samplescripts folder, e.g.:  cat samplescripts/protoeth.sampleinput.txt | npm run start");
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
        console.log(enumerateNamespaces(WC).sort().join(', '));
        console.log(enumerateNamespaces(TW).sort().map(n => 'TW.' + n).join(', '));
        console.log();
    }

    function exit(code: number): void {
        process.exit(code);
    }

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

    wallets.init();
    await wallets.refreshStoreWallets();
    await wallets.loadAWallet();

    const local = repl.start('> ');

    // Expose WC namespaces, as top-level
    var nss = enumerateNamespaces(WC);
    nss.forEach(ns => local.context[ns] = WC[ns]);
    // also all under WC
    local.context.WC = WC;
    // Expose TW namespaces (under TW)
    local.context.TW = TW;
    // Expose KeyStore namespace (under KeyStore)
    local.context.KeyStore = KeyStore;

    // Expose more stuff; utilities
    local.context.help = help;
    local.context.exit = exit;
    local.context.walletsList = walletsList;
    local.context.walletLoad = walletLoad;
    local.context.walletsDeleteAll = walletsDeleteAll;
    local.context.walletCreate = walletCreate;
    local.context.walletImport = walletImport;
    local.context.walletAddCoin = walletAddCoin;
    local.context.walletDump = walletDump;
    local.context.walletDelete = walletDelete;
    local.context.wallets = wallets;
    local.context.wallet = wallets.wallet;
    local.context.coin = WC.CoinType.bitcoin;

    local.on('exit', () => {
        console.log('Bye!');
        process.exit();
    });
}

main();

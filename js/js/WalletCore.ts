import * as binary from "node-pre-gyp";
import * as path from "path";

const bindingPath = binary.find(path.resolve(path.join(__dirname,"./../package.json")));
export const WalletCore = require(bindingPath);

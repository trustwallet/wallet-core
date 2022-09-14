// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import { KeyStore } from "./keystore";
import * as fs from "fs/promises";

export class FileSystemStorage implements KeyStore.IKeyStoreStorage {
  private readonly directory: string;

  constructor(directory: string) {
    this.directory = directory.endsWith("/") ? directory : directory + "/";
  }

  getFilename(id): string {
    return this.directory + id + ".json";
  }

  get(id: string): Promise<KeyStore.Wallet> {
    return fs.readFile(this.getFilename(id)).then((data) => {
      let wallet = JSON.parse(data.toString()) as KeyStore.Wallet;
      return wallet;
    });
  }

  set(id: string, wallet: KeyStore.Wallet): Promise<void> {
    return fs.writeFile(this.getFilename(id), JSON.stringify(wallet));
  }

  loadAll(): Promise<KeyStore.Wallet[]> {
    return fs.readdir(this.directory).then((files) => {
      return Promise.all(
        files
          .filter((file) => file.endsWith(".json"))
          .map((file) => {
            return this.get(file.replace(".json", ""));
          })
      );
    });
  }

  delete(id: string, password: string): Promise<void> {
    return fs.unlink(this.getFilename(id));
  }
}

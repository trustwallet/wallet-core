#include <stdio.h>
#include <time.h>
#include <string.h>
#include <TrezorCrypto/bip39.h>
#include <TrezorCrypto/bip32.h>
#include <TrezorCrypto/ecdsa.h>
#include <TrezorCrypto/curves.h>

char iter[256];
uint8_t seed[512 / 8];
uint8_t addr[21], pubkeyhash[20];
int count = 0, found = 0;
HDNode node;
clock_t start;

// around 280 tries per second

// testing data:
//
// mnemonic:   "all all all all all all all all all all all all"
// address:    "1JAd7XCBzGudGpJQSDSfpmJhiygtLQWaGL"
// passphrase: ""
//
// mnemonic:   "all all all all all all all all all all all all"
// address:    "1N3uJ5AU3FTYQ1ZQgTMtYmgSvMBmQiGVBS"
// passphrase: "testing"

int main(int argc, char **argv)
{
	if (argc != 2 && argc != 3) {
		fprintf(stderr, "Usage: bip39bruteforce address [mnemonic]\n");
		return 1;
	}
	const char *address = argv[1];
	const char *mnemonic, *item;
	if (argc == 3) {
		mnemonic = argv[2];
		item = "passphrase";
	} else {
		mnemonic = NULL;
		item = "mnemonic";
	}
	if (mnemonic && !mnemonic_check(mnemonic)) {
		fprintf(stderr, "\"%s\" is not a valid mnemonic\n", mnemonic);
		return 2;
	}
	if (!ecdsa_address_decode(address, 0, HASHER_SHA2, addr)) {
		fprintf(stderr, "\"%s\" is not a valid address\n", address);
		return 3;
	}
	printf("Reading %ss from stdin ...\n", item);
	start = clock();
	for (;;) {
		if (fgets(iter, 256, stdin) == NULL) break;
		int len = strlen(iter);
		if (len <= 0) {
			continue;
		}
		count++;
		iter[len - 1] = 0;
		if (mnemonic) {
			mnemonic_to_seed(mnemonic, iter, seed, NULL);
		} else {
			mnemonic_to_seed(iter, "", seed, NULL);
		}
		hdnode_from_seed(seed, 512 / 8, SECP256K1_NAME, &node);
		hdnode_private_ckd_prime(&node, 44);
		hdnode_private_ckd_prime(&node, 0);
		hdnode_private_ckd_prime(&node, 0);
		hdnode_private_ckd(&node, 0);
		hdnode_private_ckd(&node, 0);
		hdnode_fill_public_key(&node);
		ecdsa_get_pubkeyhash(node.public_key, HASHER_SHA2, pubkeyhash);
		if (memcmp(addr + 1, pubkeyhash, 20) == 0) {
			found = 1;
			break;
		}
	}
	float dur = (float)(clock() - start) / CLOCKS_PER_SEC;
	printf("Tried %d %ss in %f seconds = %f tries/second\n", count, item, dur, (float)count/dur);
	if (found) {
		printf("Correct %s found! :-)\n\"%s\"\n", item, iter);
		return 0;
	}
	printf("Correct %s not found. :-(\n", item);
	return 4;
}

# Releasing

Before releasing please update the version numbers in `android/gradle.properties` and `TrustWalletCore.podspec` and commit this change. Then create a new git tag for that number and push \(replace 0.0.0 with the actual version number\):

```bash
git tag 0.0.0
git push
git push origin 0.0.0
```

## iOS

Run `bootstrap.sh` then `tools/ios-release`. This will build, archive and upload a Cocoapod. You need to be registered as an owner for the pod.

## JavaScript

### First Time

You need to first log into your npm account that has permissions to publish.

```bash
npm login
```

### Every Release

1. Run `bootstrap.sh`.
2. Update the version number in `js/package.json` and the `npm` badge in `js/README.md`
3. Run `tools/js-release` to build, test and publish the updated version to the `npm` registry.

## Android

### First Time

1. See [https://docs.gradle.org/current/userguide/signing\_plugin.html](https://docs.gradle.org/current/userguide/signing_plugin.html) to set up GPG signing.
2. Get a OSSRH username and password, get added to the group ID.
3. Install PGP Tools : `brew install gnupg`  -  [https://gpgtools.org/](https://gpgtools.org/)
4. Create `~/.gradle/gradle.properties` and add your information from the previous steps:
5. Generate GPG Key
6. Send Key to the server using the installed tool GPG Keychain
7. Generate the signing.keyId value:

   ```bash
   gpg --list-keys --keyid-format short
   ```

8. Get the `.gpg` secret key that will be assigned to signing.secretKeyRingFile parameter:

   ```bash
   gpg --export-secret-keys -o secring.gpg
   ```

```bash
# Note: both spellings of username and password are needed
NEXUS_USERNAME=user
NEXUS_PASSWORD=pass
nexusUsername=user
nexusPassword=pass

signing.keyId=00000000
signing.password=password
signing.secretKeyRingFile=/Users/me/.gnupg/secring.gpg

org.gradle.daemon=true
```

### Every Release

1. Update the `VERSION_NAME` in `android/gradle.properties`.
2. In the `android` folder run `./gradlew uploadArchive`.
3. In the `android` folder run `./gradlew closeAndReleaseRepository`.

In case of problems in the last step go to the [Sonatype](https://oss.sonatype.org/#stagingRepositories) webpage. Look for the staging library. It should be at the end of the list. Select it, and press "Close" button. Once it's closed select it an press "Release". Make sure to drop stale repositories so that the automatic release works in subsequent tries.

## Docker Hub

We need to release a new prebuilt Docker image every time our dependencies change. To build the docker image run `docker build .` in the root folder. Use `docker push` to upload to Docker Hub.


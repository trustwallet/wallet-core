plugins {
    `maven-publish`
}

group = "com.trustwallet"
if (version == Project.DEFAULT_VERSION) {
    version = "0.0.0-alpha"
}

publishing {
    repositories {
        maven {
            name = "GitHubPackages"
            url = uri("https://maven.pkg.github.com/trustwallet/wallet-core")
            credentials {
                username = System.getenv("GITHUB_USER")
                password = System.getenv("GITHUB_TOKEN")
            }
        }
    }
}

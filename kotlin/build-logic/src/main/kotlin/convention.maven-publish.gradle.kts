plugins {
    `maven-publish`
}

group = "com.trustwallet"
if (version == Project.DEFAULT_VERSION) {
    version = "0.0.0-alpha"
}

val publishUrl: String = findProperty("publishUrl") as String?
    ?: "https://maven.pkg.github.com/trustwallet/wallet-core"

publishing {
    repositories {
        maven {
            name = "GitHubPackages"
            url = uri(publishUrl)
            credentials {
                username = System.getenv("GITHUB_USER")
                password = System.getenv("GITHUB_TOKEN")
            }
        }
    }
}

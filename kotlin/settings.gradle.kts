@file:Suppress("UnstableApiUsage")

rootProject.name = "WalletCoreKotlin"

pluginManagement {
    repositories {
        google()
        mavenCentral()
    }
}

dependencyResolutionManagement {
    // Uncomment after https://youtrack.jetbrains.com/issue/KT-55620/
    // repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
    }
}

include(
    ":TrustWalletCore",
)

pluginManagement {
    repositories {
        google()
        gradlePluginPortal()
        mavenCentral()
    }
}

val properties = java.util.Properties()
val localProps = file("local.properties")
if (localProps.exists()) {
    properties.load(localProps.inputStream())
    println("Authenticating user: " + properties.getProperty("gpr.user"))
} else {
    println(
        "local.properties not found, please create it next to settings.gradle.kts and set gpr.user and gpr.key (Create a GitHub package read only and non expiration token at https://github.com/settings/tokens)\n" +
                "Or set GITHUB_USER and GITHUB_TOKEN environment variables"
    )
}

dependencyResolutionManagement {
    repositories {
        google()
        mavenCentral()
        maven {
            url = uri("https://maven.pkg.github.com/trustwallet/wallet-core")
            credentials {
                username = properties["gpr.user"] as? String ?: System.getenv("GITHUB_USER")
                password = properties["gpr.key"] as? String ?: System.getenv("GITHUB_TOKEN")
            }
        }
    }
}

rootProject.name = "kmpSample"
include(":androidApp")
include(":shared")
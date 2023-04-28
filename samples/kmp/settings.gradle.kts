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
    val user = properties.getProperty("gpr.user")
    if (user != null) {
        println("Authenticating user: $user")
    } else {
        println("local.properties found, but gpr.user property not set. ")
    }
} else {
    val user = System.getenv("GITHUB_USER")
    if (user != null) {
        println("Authenticating user: $user")
    } else {
        println("local.properties not found, and GITHUB_USER environment variable not set. ")
    }
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
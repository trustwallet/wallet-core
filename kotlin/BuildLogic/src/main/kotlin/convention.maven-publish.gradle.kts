import java.io.ByteArrayOutputStream

plugins {
    `maven-publish`
}

group = "com.trustwallet"
val moduleName = "wallet-core-kotlin"

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

// Use afterEvaluate because of Android: org.jetbrains.kotlin.gradle.plugin.mpp.createTargetPublications
project.afterEvaluate {
    publishing.publications.withType<MavenPublication> {
        artifactId = artifactId.replaceFirst(project.name, moduleName)

        // Only for local builds
        if (version.isNullOrBlank() || version == Project.DEFAULT_VERSION) {
            val output = ByteArrayOutputStream()
            exec {
                standardOutput = output
                workingDir(rootDir.parentFile)
                commandLine("git", "describe", "--long", "--tags")
            }
            val gitTagVersion = output.toString().split('-').first()
            val nextVersion =
                gitTagVersion
                    .splitToSequence('.')
                    .map(String::toInt)
                    .mapIndexed { i, s -> if (i == 2) s + 1 else s }
                    .joinToString(".")

            version = "$nextVersion-alpha"
        }
    }
}

plugins {
    id("com.android.application").version("8.8.0").apply(false)
    id("com.android.library").version("8.8.0").apply(false)
    kotlin("android").version("2.1.0").apply(false)
    kotlin("plugin.compose").version("2.1.0").apply(false)
    kotlin("multiplatform").version("2.1.0").apply(false)
}

allprojects {
    configurations.configureEach {
        resolutionStrategy.eachDependency {
            if (requested.group == "com.google.protobuf" &&
                (requested.name == "protobuf-java" || requested.name == "protobuf-java-util")) {
                useVersion("3.25.5")
            }
        }
    }
    dependencyLocking {
        lockAllConfigurations()
    }
}

tasks.register("clean", Delete::class) {
    delete(rootProject.layout.buildDirectory)
}

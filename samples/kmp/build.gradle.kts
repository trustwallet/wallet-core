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
            } else if (requested.group == "com.squareup.okio") {
                useVersion("3.4.0")
            } else if (requested.group == "commons-io") {
                useVersion("2.14.0")
            } else if (requested.group == "io.netty") {
                useVersion("4.1.138.Final")
            } else if (requested.group == "com.google.guava" && requested.name == "guava") {
                useVersion("32.0.1-jre")
            } else if (requested.group == "com.squareup.wire") {
                useVersion("5.3.3")
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

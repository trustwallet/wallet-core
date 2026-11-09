plugins {
    id("com.android.application").version("8.8.0").apply(false)
    id("com.android.library").version("8.8.0").apply(false)
    kotlin("android").version("2.4.21").apply(false)
    kotlin("plugin.compose").version("2.4.21").apply(false)
    kotlin("multiplatform").version("2.4.21").apply(false)
}

// Kotlin 2.4's Swift export worker (swift-export-embeddable) pulls in opentelemetry 1.41.0,
// which has known DoS advisories fixed in 1.67.0. Drop once Kotlin ships a fixed version.
subprojects {
    configurations.configureEach {
        resolutionStrategy.eachDependency {
            if (requested.group == "io.opentelemetry") {
                useVersion("1.67.0")
                because("opentelemetry-java < 1.67.0 has baggage and tracestate parsing DoS advisories")
            }
        }
    }
}

tasks.register("clean", Delete::class) {
    delete(rootProject.layout.buildDirectory)
}

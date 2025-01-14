plugins {
    id("com.android.application").version("8.8.0").apply(false)
    id("com.android.library").version("8.8.0").apply(false)
    kotlin("android").version("2.1.0").apply(false)
    kotlin("plugin.compose").version("2.1.0").apply(false)
    kotlin("multiplatform").version("2.1.0").apply(false)
}

tasks.register("clean", Delete::class) {
    delete(rootProject.layout.buildDirectory)
}

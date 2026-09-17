// Workaround https://github.com/gradle/gradle/issues/22797
@file:Suppress("DSL_SCOPE_VIOLATION")

import org.jetbrains.kotlin.gradle.dsl.JvmTarget
import org.jetbrains.kotlin.gradle.tasks.KotlinCompile

plugins {
    id("com.android.application") version libs.versions.agp.get() apply false
    id("com.android.library") version libs.versions.agp.get() apply false
    kotlin("android") version libs.versions.kotlin.get() apply false
    kotlin("multiplatform") version libs.versions.kotlin.get() apply false
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
            }
        }
    }
    tasks.withType<KotlinCompile> {
        compilerOptions {
            allWarningsAsErrors.set(true)
            jvmTarget.set(JvmTarget.JVM_17)
            freeCompilerArgs.add("-Xexpect-actual-classes")
        }
    }
    dependencyLocking {
        lockAllConfigurations()
    }
}

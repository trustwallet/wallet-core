@file:Suppress("UnstableApiUsage")

import org.jetbrains.kotlin.gradle.internal.ensureParentDirsCreated
import org.jetbrains.kotlin.gradle.targets.js.webpack.KotlinWebpackOutput
import org.jetbrains.kotlin.incremental.createDirectory
import org.jetbrains.kotlin.incremental.deleteDirectoryContents

plugins {
    kotlin("multiplatform")
    id("com.android.library")
}

kotlin {
    android()

    val nativeTargets =
        listOf(
            iosArm64(),
            iosSimulatorArm64(),
            iosX64(),
        )

    js {
        browser {
            webpackTask {
                output.libraryTarget = KotlinWebpackOutput.Target.COMMONJS2
            }
        }
        useCommonJs()
    }

    sourceSets {
        all {
            languageSettings {
                optIn("kotlin.js.ExperimentalJsExport")
            }
        }

        val androidMain by getting {
            kotlin.srcDir(projectDir.resolve("src/androidMain/generated"))
        }
        val commonMain by getting {
            kotlin.srcDirs(
                projectDir.resolve("src/commonMain/generated"),
                projectDir.resolve("src/commonMain/proto"),
            )

            dependencies {
                api(libs.wire.runtime)
            }
        }
        val iosArm64Main by getting
        val iosSimulatorArm64Main by getting
        val iosX64Main by getting
        val iosMain by creating {
            kotlin.srcDir(projectDir.resolve("src/iosMain/generated"))

            dependsOn(commonMain)
            iosArm64Main.dependsOn(this)
            iosSimulatorArm64Main.dependsOn(this)
            iosX64Main.dependsOn(this)
        }
        val jsMain by getting {
            kotlin.srcDir(projectDir.resolve("src/jsMain/generated"))

            dependencies {
                // TODO: Replace with local build
                implementation(npm(name = "@trustwallet/wallet-core", version = "3.1.10", generateExternals = false))
            }
        }
    }

    nativeTargets.forEach { nativeTarget ->
        nativeTarget.apply {
            val main by compilations.getting
            val walletCore by main.cinterops.creating {
                includeDirs.allHeaders(rootDir.parentFile.resolve("include/TrustWalletCore"))
            }
        }
    }
}

android {
    namespace = "com.trustwallet.core"
    compileSdk = libs.versions.android.sdk.compile.get().toInt()
    buildToolsVersion = libs.versions.android.sdk.tools.get()

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }

    defaultConfig {
        minSdk = libs.versions.android.sdk.min.get().toInt()
        ndkVersion = libs.versions.android.ndk.get()

        externalNativeBuild {
            cmake {
                arguments += listOf("-DCMAKE_BUILD_TYPE=Release", "-DKOTLIN=True")
            }
        }
    }

    buildFeatures {
        aidl = false
        compose = false
        buildConfig = false
        prefab = false
        renderScript = false
        resValues = false
        shaders = false
        viewBinding = false
    }

    androidComponents {
        beforeVariants(selector().withBuildType("debug")) {
            it.enable = false
        }
    }

    externalNativeBuild {
        cmake {
            version = libs.versions.android.cmake.get()
            path = rootDir.parentFile.resolve("CMakeLists.txt")
        }
    }
}

// Use Composite builds
val generateCinteropTask = task("generateCinterop") {
    doFirst {
        val headersDir = rootDir.parentFile.resolve("include/TrustWalletCore")
        val headers = headersDir
            .listFiles { file -> file.extension == "h" }
            .orEmpty()
            .sortedBy { it.name }
            .joinToString(separator = " ") { it.name }

        val defFile = projectDir.resolve("src/nativeInterop/cinterop/walletCore.def")
        defFile.ensureParentDirsCreated()
        defFile.writeText(
            text =
            """
                headers = $headers
                package = com.trustwallet.core

            """.trimIndent(),
        )
    }
}

val copyProtoTask = task<Copy>("copyProtos") {
    val sourceDir = rootDir.parentFile.resolve("src/proto")
    val destinationDir = projectDir.resolve("build/tmp/proto")

    doFirst {
        destinationDir.deleteDirectoryContents()
    }

    from(sourceDir) {
        include("*.proto")
    }
    into(destinationDir)

    doLast {
        destinationDir
            .listFiles { file -> file.extension == "proto" }
            .orEmpty()
            .forEach { file ->
                val packageName = file.nameWithoutExtension.toLowerCase()
                file
                    .readText()
                    .replaceFirst(
                        oldValue = """option java_package = "wallet.core.jni.proto";""",
                        newValue = """option java_package = "com.trustwallet.core.$packageName";""",
                    )
                    .let { file.writeText(it) }
            }
    }
}

val wire: Configuration by configurations.creating

val generateProtosTask = task<JavaExec>("generateProtos") {
    dependsOn(copyProtoTask)

    val sourceDir = projectDir.resolve("build/tmp/proto")
    val destinationDir = projectDir.resolve("src/commonMain/proto")

    doFirst {
        destinationDir.createDirectory()
        destinationDir.deleteDirectoryContents()
    }

    mainClass.set("com.squareup.wire.WireCompiler")
    classpath = wire

    args(
        "--proto_path=$sourceDir",
        "--kotlin_out=$destinationDir",
    )
}

dependencies {
    wire(libs.wire.compiler)
}

task<Exec>("generateFiles") {
    dependsOn(generateCinteropTask)
    dependsOn(generateProtosTask)

    workingDir(rootDir.parentFile)
    commandLine("./codegen/bin/codegen")
}

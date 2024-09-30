@file:Suppress("UnstableApiUsage", "OPT_IN_USAGE")

import org.jetbrains.kotlin.gradle.targets.js.webpack.KotlinWebpackOutput

plugins {
    kotlin("multiplatform")
    id("com.android.library")
    id("convention.maven-publish")
    id("convention.proto-generation")
}

kotlin {
    targetHierarchy.default()

    android {
        publishLibraryVariants = listOf("release")
    }

    jvm {
        testRuns.named("test") {
            executionTask.configure {
                useJUnitPlatform()
            }
        }
    }

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

        val commonMain by getting {
            kotlin.srcDirs(
                projectDir.resolve("src/commonMain/generated"),
                projectDir.resolve("src/commonMain/proto"),
            )

            dependencies {
                api(libs.wire.runtime)
            }
        }

        getByName("commonTest") {
            dependencies {
                implementation(kotlin("test"))
            }
        }

        val androidMain by getting
        val jvmMain by getting
        create("commonAndroidJvmMain") {
            kotlin.srcDir(projectDir.resolve("src/commonAndroidJvmMain/generated"))

            dependsOn(commonMain)
            androidMain.dependsOn(this)
            jvmMain.dependsOn(this)
        }

        getByName("iosMain") {
            kotlin.srcDir(projectDir.resolve("src/iosMain/generated"))
        }

        getByName("jsMain") {
            kotlin.srcDir(projectDir.resolve("src/jsMain/generated"))

            dependencies {
                implementation(npm(name = "webpack", version = "5.89.0"))
            }
        }
    }

    nativeTargets.forEach { nativeTarget ->
        nativeTarget.apply {
            val main by compilations.getting
            main.cinterops.create("WalletCore") {
                packageName = "com.trustwallet.core"
                headers(rootDir.parentFile.resolve("include/TrustWalletCore").listFiles()!!)
            }
        }
    }
}

android {
    namespace = "com.trustwallet.core"
    compileSdk = libs.versions.android.sdk.compile.get().toInt()
    buildToolsVersion = libs.versions.android.sdk.tools.get()

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }

    defaultConfig {
        minSdk = libs.versions.android.sdk.min.get().toInt()
        ndkVersion = libs.versions.android.ndk.get()

        consumerProguardFiles += projectDir.resolve("consumer-rules.pro")

        externalNativeBuild {
            cmake {
                arguments += listOf("-DCMAKE_BUILD_TYPE=Release", "-DKOTLIN=True", "-DTW_UNITY_BUILD=ON")
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
        beforeVariants {
            it.enable = it.name == "release"
        }
    }

    externalNativeBuild {
        cmake {
            version = libs.versions.android.cmake.get()
            path = rootDir.parentFile.resolve("CMakeLists.txt")
        }
    }
}

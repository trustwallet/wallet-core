import org.gradle.nativeplatform.platform.internal.DefaultNativePlatform

plugins {
    kotlin("multiplatform")
    kotlin("native.cocoapods")
    id("com.android.library")
}

kotlin {
    android {
        compilations.all {
            kotlinOptions {
                jvmTarget = "1.8"
            }
        }
    }

    iosX64()
    iosArm64()

    cocoapods {
        summary = "Some description for the Shared Module"
        homepage = "Link to the Shared Module homepage"
        version = "1.0"
        ios.deploymentTarget = "14.1"
        dependencies {
            pod("TrustWalletCore", moduleName = "WalletCore")
        }
        podfile = project.file("../iosApp/Podfile")
        framework {
            baseName = "shared"

            listOf(
                iosX64(),
                iosArm64(),
                iosSimulatorArm64()
            ).forEach {
                it.binaries.all {
                    linkerOpts += "-ld64"
                }
            }
        }
    }

    sourceSets {
        val commonMain by getting {
            dependencies {
                implementation("com.trustwallet:wallet-core-kotlin:+")
            }
        }
        val commonTest by getting {
            dependencies {
                implementation(kotlin("test"))
            }
        }
        val androidMain by getting
        val androidUnitTest by getting
        val iosX64Main by getting
        val iosArm64Main by getting

        val iosMain by creating {
            dependsOn(commonMain)
            iosX64Main.dependsOn(this)
            iosArm64Main.dependsOn(this)
        }
        val iosX64Test by getting
        val iosArm64Test by getting

        val iosTest by creating {
            dependsOn(commonTest)
            iosX64Test.dependsOn(this)
            iosArm64Test.dependsOn(this)
        }

        val isArmMac = DefaultNativePlatform("test").architecture.isArm
        if (isArmMac) {
            iosSimulatorArm64()

            val iosSimulatorArm64Main by getting
            val iosSimulatorArm64Test by getting

            iosSimulatorArm64Main.dependsOn(iosMain)
            iosSimulatorArm64Test.dependsOn(iosTest)
        }
    }
}

android {
    namespace = "com.example.kmpsample"
    compileSdk = 33
    defaultConfig {
        minSdk = 24
        targetSdk = 33
    }
}

// Workaround https://github.com/gradle/gradle/issues/22797
@file:Suppress("DSL_SCOPE_VIOLATION")

plugins {
    id("com.android.library") version libs.versions.agp.get() apply false
    kotlin("android") version libs.versions.kotlin.get() apply false
    kotlin("multiplatform") version libs.versions.kotlin.get() apply false
}

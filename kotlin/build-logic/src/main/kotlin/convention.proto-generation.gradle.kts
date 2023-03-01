val libs = extensions.getByType<VersionCatalogsExtension>().named("libs")

val copyProtoTask = task<Copy>("copyProtos") {
    val sourceDir = rootDir.parentFile.resolve("src/proto")
    val destinationDir = projectDir.resolve("build/tmp/proto")

    doFirst {
        destinationDir.deleteRecursively()
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
                val packageName = file.nameWithoutExtension.lowercase()
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
dependencies {
    wire(libs.findLibrary("wire.compiler").get().get())
}

val generateProtosTask = task<JavaExec>("generateProtos") {
    dependsOn(copyProtoTask)

    val sourceDir = projectDir.resolve("build/tmp/proto")
    val destinationDir = projectDir.resolve("src/commonMain/proto")

    doFirst {
        destinationDir.deleteRecursively()
        destinationDir.mkdirs()
    }

    mainClass.set("com.squareup.wire.WireCompiler")
    classpath = wire

    args(
        "--proto_path=$sourceDir",
        "--kotlin_out=$destinationDir",
    )
}

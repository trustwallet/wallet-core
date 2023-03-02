package com.example.kmpsample

interface Platform {
    val name: String
}

expect fun getPlatform(): Platform
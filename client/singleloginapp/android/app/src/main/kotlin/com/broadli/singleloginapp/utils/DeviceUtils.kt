package com.broadli.singleloginapp.utils

import java.util.*

object DeviceUtils {
    val deviceId: String
        get() {
            val uuid = UUID.randomUUID()
            return uuid.toString()
        }
}
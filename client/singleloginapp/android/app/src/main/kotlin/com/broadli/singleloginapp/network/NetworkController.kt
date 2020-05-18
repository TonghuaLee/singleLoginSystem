package com.broadli.singleloginapp.network

import android.app.Application
import com.broadli.singleloginapp.AppApplication
import java.io.IOException
import java.io.InputStream

class NetworkController private constructor() {
    val networkMethod: NetworkMethod

    companion object {
        const val SERVER_HOST = "118.126.89.19" //腾讯云-Ubuntu
        const val SERVER_PORT = 50051
        private var mNetworkController: NetworkController? = null
        private var reqISPem: InputStream? = null
        val instance: NetworkController?
            get() {
                if (mNetworkController == null) {
                    synchronized(NetworkController::class.java) {
                        if (mNetworkController == null) {
                            mNetworkController = NetworkController()
                        }
                    }
                }
                return mNetworkController
            }
    }

    init {
        try {
            reqISPem = AppApplication.getContext().assets?.open("server.pem")
        } catch (e: IOException) {
            e.printStackTrace()
        }
        networkMethod = reqISPem?.let { NetworkMethod(it) }!!
    }
}
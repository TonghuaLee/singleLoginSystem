package com.broadli.singleloginapp.network

import android.app.Application
import java.io.IOException
import java.io.InputStream

class NetworkController private constructor() {
    val networkMethod: NetworkMethod

    companion object {
        const val SERVER_HOST = "98.142.128.182" //国外-Ubuntu
        //    public static final String SERVER_HOST = "47.92.196.45"; //阿里云-Ubuntu
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
            reqISPem = Application.().getResources().getAssets().open("server.pem")
        } catch (e: IOException) {
            e.printStackTrace()
        }
        networkMethod = NetworkMethod(reqISPem)
    }
}
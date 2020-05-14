package com.broadli.singleloginapp


import android.util.Log
import com.broadli.clibs.LoginCore

class LoginControler {
    val TAG = "LoginControl"

    var mLoginCore: LoginCore? = null

    constructor() {
        mLoginCore = LoginCore.create()
    }

    public fun addTest() {
        Log.d(TAG, "add Test: mLoginCore is null ? :" + (mLoginCore == null))
        val result = mLoginCore?.nativeAdd(10, 20)
        Log.d(TAG, "add Test: 10+20= " + result)
    }
}
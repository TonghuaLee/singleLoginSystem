package com.broadli.singleloginapp.controller

interface LoginUIController {
    fun performLoginSuccess()
    fun performSignSuccess()
    fun performLoginFail()
    fun performSignFail()
    fun performLogout()
    fun performUserOnline(account: String?)
    fun toastMsg(content: String?)
    fun disconnect()
}
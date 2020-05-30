package com.broadli.singleloginapp.controller

interface LoginUIController {
    fun performLoginSuccess()
    fun performSignSuccess()
    fun performLoginFail(msg: String?)
    fun performSignFail(msg: String?)
    fun performLogout()
    fun performUserOnline(account: String?)
    fun performAddCategorySuccess(data: String)
    fun performAddCategoryFail(data: String)
    fun toastMsg(content: String?)
    fun disconnect()
}
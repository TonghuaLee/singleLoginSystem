package com.broadli.singleloginapp.controller

internal interface ILoginController {
    fun actionLoginIn(userAccount: String?, userPassword: String?)
    fun actionSignIn(userAccount: String?, userPassword: String?)
    fun actionLogout()
    fun actionCheckLoginStatus()
}
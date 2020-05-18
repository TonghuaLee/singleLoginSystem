package com.broadli.singleloginapp

import android.app.Application
import android.content.Context
import io.flutter.app.FlutterApplication

class AppApplication : FlutterApplication() {
    companion object {
        var  _context:Application? = null
        fun getContext():Context{
            return _context!!
        }

    }

    override fun onCreate() {
        super.onCreate()
        _context = this
    }
}
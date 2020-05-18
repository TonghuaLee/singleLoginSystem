package com.broadli.singleloginapp.utils

import android.content.Context
import android.widget.Toast

object CommonUtils {
    fun showToast(context: Context, resId: Int, type: Int) {
        showToast(context, context.getString(resId), type)
    }

    fun showToast(context: Context?, content: String?, type: Int) {
        Toast.makeText(context, content, type).show()
    }
}
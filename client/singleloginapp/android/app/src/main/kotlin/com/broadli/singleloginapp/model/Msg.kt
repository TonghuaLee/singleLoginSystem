package com.broadli.singleloginapp.model

import android.os.Parcel
import android.os.Parcelable
import com.broadli.singleloginapp.config.MsgType
import org.json.JSONException
import org.json.JSONObject

data class Msg(var mainCmd: Int, var subCmd: Int, var code: Int, var message: String) : Parcelable {
    var data: Map<String, Any>? = null

    public fun fromJson(json: String) {
        var jsonObj: JSONObject? = null
        try { //解析Flutter 传递的参数
            jsonObj = JSONObject(json.toString())
            //方法名标识
            mainCmd = jsonObj?.optInt(MsgType.PARAM_MAIN_CMD)
            subCmd = jsonObj?.optInt(MsgType.PARAM_SUB_CMD)
            code = jsonObj?.optInt(MsgType.PARAM_CODE)
            message = jsonObj?.optString(MsgType.PARAM_MSG)
            data = jsonObj?.opt(MsgType.PARAM_DATA) as Map<String, Any>?
        } catch (e: JSONException) {
            e.printStackTrace()
        }
    }

    override fun writeToParcel(parcel: Parcel, flags: Int) {
        parcel.writeInt(mainCmd)
        parcel.writeInt(subCmd)
        parcel.writeInt(code)
        parcel.writeString(message)
    }

    override fun describeContents(): Int {
        return 0
    }

    companion object CREATOR : Parcelable.Creator<Msg> {
        override fun createFromParcel(parcel: Parcel): Msg {
            return Msg(parcel.readInt(), parcel.readInt(), parcel.readInt(), parcel.readString())
        }

        override fun newArray(size: Int): Array<Msg?> {
            return arrayOfNulls(size)
        }
    }

}
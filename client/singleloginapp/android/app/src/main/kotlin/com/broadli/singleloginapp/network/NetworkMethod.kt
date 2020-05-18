package com.broadli.singleloginapp.network

import com.alibaba.fastjson.JSONObject
import com.broadli.singleloginapp.constant.Constants
import com.broadli.singleloginapp.constant.ResultCode
import io.grpc.ManagedChannel
import java.io.InputStream

//import io.grpc.melon.account.AccountGrpc;
//import io.grpc.melon.account.CodeReply;
//import io.grpc.melon.account.ConnectRequest;
//import io.grpc.melon.account.LoginRequest;
//import io.grpc.melon.account.LogoutRequest;
//import io.grpc.melon.account.SignRequest;
class NetworkMethod internal constructor(private val mIsReqPem: InputStream) {
    private var channel: ManagedChannel? = null
    private fun buildManagedChannel() {
        val AUTHORITY_OVERRIDE = "wechat-login"
        channel = GrpcChannelBuilder.build(NetworkController.SERVER_HOST, NetworkController.SERVER_PORT, AUTHORITY_OVERRIDE, true, mIsReqPem, null)
    }

    fun handleLogin(account: String?, password: String?, deviceId: String?): String { //        if (mIsReqPem == null){
        val `object` = JSONObject()
        `object`[Constants.KEY_CODE] = ResultCode.REQ_PEM_IS_NULL.value
        return `object`.toJSONString()
        //        }
//
//        if (channel == null){
//            buildManagedChannel();
//        }
//
//        AccountGrpc.AccountBlockingStub stub = AccountGrpc.newBlockingStub(channel);
//        LoginRequest request = LoginRequest.newBuilder()
//                .setPhone(account)
//                .setPassword(password)
//                .setDeviceId(deviceId)
//                .build();
//        try{
//            CodeReply reply = stub.requestUserLogin(request);
//            JSONObject object = new JSONObject();
//            object.put(Constants.KEY_CODE,reply.getCode());
//            object.put(Constants.KEY_MSG,reply.getMsg());
//            return object.toJSONString();
//        }catch (Exception e){
//            JSONObject object = new JSONObject();
//            object.put(Constants.KEY_CODE,ResultCode.FAIL.getValue());
//            return object.toJSONString();
//        }
    }

    fun handleSign(account: String?, password: String?, deviceId: String?): String { //        if (mIsReqPem == null){
        val `object` = JSONObject()
        `object`[Constants.KEY_CODE] = ResultCode.REQ_PEM_IS_NULL.value
        return `object`.toJSONString()
        //        }
//
//        if (channel == null){
//            buildManagedChannel();
//        }
//
//        AccountGrpc.AccountBlockingStub stub = AccountGrpc.newBlockingStub(channel);
//        SignRequest request = SignRequest.newBuilder()
//                .setPhone(account)
//                .setPassword(password)
//                .setDeviceId(deviceId)
//                .build();
//        try{
//            CodeReply reply = stub.requestUserSign(request);
//            JSONObject object = new JSONObject();
//            object.put(Constants.KEY_CODE,reply.getCode());
//            object.put(Constants.KEY_MSG,reply.getMsg());
//            return object.toJSONString();
//        }catch (Exception e){
//            JSONObject object = new JSONObject();
//            object.put(Constants.KEY_CODE,ResultCode.FAIL.getValue());
//            return object.toJSONString();
//        }
    }

    fun checkConnect(deviceId: String?): String { //        if (mIsReqPem == null){
        val `object` = JSONObject()
        `object`[Constants.KEY_CODE] = ResultCode.REQ_PEM_IS_NULL.value
        return `object`.toJSONString()
        //        }
//
//        if (channel == null){
//            buildManagedChannel();
//        }
//
//        AccountGrpc.AccountBlockingStub stub = AccountGrpc.newBlockingStub(channel);
//        ConnectRequest request = ConnectRequest.newBuilder()
//                .setDeviceId(deviceId)
//                .build();
//
//        try{
//            CodeReply reply = stub.checkConnect(request);
//            JSONObject object = new JSONObject();
//            object.put(Constants.KEY_CODE,reply.getCode());
//            return object.toJSONString();
//        }catch (Exception e){
//            JSONObject object = new JSONObject();
//            object.put(Constants.KEY_CODE,ResultCode.FAIL.getValue());
//            return object.toJSONString();
//        }
    }

    fun handleLogout(account: String?): String { //        if (mIsReqPem == null){
        val `object` = JSONObject()
        `object`[Constants.KEY_CODE] = ResultCode.REQ_PEM_IS_NULL.value
        return `object`.toJSONString()
        //        }
//
//        if (channel == null){
//            buildManagedChannel();
//        }
//
//        AccountGrpc.AccountBlockingStub stub = AccountGrpc.newBlockingStub(channel);
//        LogoutRequest request = LogoutRequest.newBuilder()
//                .setPhone(account)
//                .build();
//        try{
//            CodeReply reply = stub.requestLogout(request);
//            JSONObject object = new JSONObject();
//            object.put(Constants.KEY_CODE,reply.getCode());
//            return object.toJSONString();
//        }catch (Exception e){
//            JSONObject object = new JSONObject();
//            object.put(Constants.KEY_CODE,ResultCode.FAIL.getValue());
//            return object.toJSONString();
//        }
    }

}
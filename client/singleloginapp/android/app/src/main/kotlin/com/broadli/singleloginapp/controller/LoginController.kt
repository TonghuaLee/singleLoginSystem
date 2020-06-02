package com.broadli.singleloginapp.controller

import android.app.Activity
import android.os.Handler
import android.os.Looper
import android.text.TextUtils
import android.util.Log
import com.broadli.clibs.ActionResult
import com.broadli.clibs.LoginCore
import com.broadli.clibs.LoginListener
import com.broadli.singleloginapp.constant.ResultCode
import java.lang.ref.WeakReference
import java.util.concurrent.Executors

class LoginController constructor(activity: Activity?, controller: LoginUIController?) : ILoginController {
    val TAG = "LoginController"
    private val pageActivity: WeakReference<Activity?> = WeakReference<Activity?>(activity)
    private val mUiController: WeakReference<LoginUIController?> = WeakReference(controller)
    private lateinit var mLoginCore: LoginCore
    private lateinit var mLoginListener: LoginListener
    private val executorCache = Executors.newCachedThreadPool()
    private val executorSingle = Executors.newSingleThreadExecutor()
    private val mMainHandler = Handler(Looper.getMainLooper())

    /**
     * 发起登录操作
     */
    override fun actionLoginIn(userAccount: String?, userPassword: String?) {
        executorCache.execute { mLoginCore.userLogin(userAccount, userPassword) }
    }

    /**
     * 发起注册操作
     */
    override fun actionSignIn(userAccount: String?, userPassword: String?) {
        executorCache.execute { mLoginCore.userSign(userAccount, userPassword) }
    }

    /**
     * 发起退出登录操作
     */
    override fun actionLogout() {
        executorCache.execute { mLoginCore.userLogout() }
    }

    /**
     * 发起检查登录状态操作
     */
    override fun actionCheckLoginStatus() {
        executorCache.execute { mLoginCore.checkLoginStatus() }
    }

    /**
    `     * 发起检查连接状态操作
     */
    private fun checkConnect() {
        executorSingle.execute { mLoginCore.checkConnection() }
    }

    /**
     * 添加todo分类
     */
    override fun actionAddCategory(title: String) {
        executorCache.execute { mLoginCore.addCategory(title) }
    }

    /**
     * 添加todo
     */
    override fun actionAddTodo(content: String, cid: Int) {
        executorCache.execute { mLoginCore.addTodo(content, cid) }
    }

    override fun actionGetCategoryList() {
        executorCache.execute { mLoginCore.getCategoryList() }
    }

    override fun actionGetTodoList(cid: Int) {
        executorCache.execute { mLoginCore.getTodoList(cid) }
    }

    override fun actionUpdateTodoStatus(cid: Int, status: Int){
        executorCache.execute { mLoginCore.updateTodo(cid, status) }
    }

    /**
     * 处理登录完成后的UI方法调用
     */
    private fun handleLoginResult(result: ActionResult) {
        if (isPageRecycle) {
            return
        }
        if (result.getCode() === ResultCode.SUCCESS.getValue()) {
            mUiController.get()!!.performLoginSuccess()
            checkConnect()
        } else {
            mUiController.get()!!.performLoginFail(result.getMsg())
        }
    }

    private fun handOnAddCategory(result: ActionResult) {
        if (isPageRecycle) {
            return
        }
        if (result.getCode() === ResultCode.SUCCESS.getValue()) {
            mUiController.get()!!.performAddCategorySuccess(result.data)
            checkConnect()
        } else {
            mUiController.get()!!.performAddCategoryFail(result.msg)
        }
    }

    private fun handOnAddTodo(result: ActionResult) {
        if (isPageRecycle) {
            return
        }
        if (result.getCode() === ResultCode.SUCCESS.getValue()) {
            mUiController.get()!!.performAddTodoSuccess(result.data)
            checkConnect()
        } else {
            mUiController.get()!!.performAddTodoFail(result.msg)
        }
    }

    /**
     * 处理注册完成后的UI方法调用
     * @param result
     */
    private fun handleSignResult(result: ActionResult) {
        if (isPageRecycle) {
            return
        }
        if (result.getCode() === ResultCode.SUCCESS.getValue()) {
            Log.d(TAG, "handleSignResult success")
            mUiController.get()!!.performSignSuccess()
            checkConnect()
        } else {
            Log.d(TAG, "handleSignResult fail")
            mUiController.get()!!.performSignFail(result.getMsg())
        }
    }

    private fun handleGetCategoryResult(result: ActionResult) {
        if (isPageRecycle) {
            return
        }
        if (result.getCode() === ResultCode.SUCCESS.getValue()) {
            Log.d(TAG, "handleGetCategoryResult success")
            mUiController.get()!!.performGetCategoryListSuccess(result.data)
        } else {
            Log.d(TAG, "handleGetCategoryResult fail")
            mUiController.get()!!.performGetCategoryListFail(result.getMsg())
        }
    }

    private fun handleGetTodoResult(result: ActionResult) {
        if (isPageRecycle) {
            return
        }
        if (result.getCode() === ResultCode.SUCCESS.getValue()) {
            Log.d(TAG, "handleGetCategoryResult success")
            mUiController.get()!!.performGetTodoListSuccess(result.data)
        } else {
            Log.d(TAG, "handleGetCategoryResult fail")
            mUiController.get()!!.performGetTodoListFail(result.getMsg())
        }
    }

    /**
     * 处理断开登录状态后的UI方法调用
     */
    private fun handleDeviceDisconnect(result: ActionResult) {
        if (isPageRecycle) {
            return
        }
        mUiController.get()!!.disconnect()
        if (!TextUtils.isEmpty(result.getMsg())) {
            handleToast(result.getMsg())
        }
    }

    /**
     * 处理退出登录状态后的UI方法调用
     */
    private fun handleLogout(result: ActionResult) {
        if (isPageRecycle) {
            return
        }
        mUiController.get()!!.performLogout()
        if (!TextUtils.isEmpty(result.getMsg())) {
            handleToast(result.getMsg())
        }
    }

    /**
     * 检查用户登录状态
     */
    private fun handleCheckUserStatus(result: ActionResult) {
        if (isPageRecycle) {
            return
        }
        if (result.getCode() === ResultCode.SUCCESS.getValue()) {
            mUiController.get()!!.performUserOnline(result.getData())
            checkConnect()
        } else {
            mUiController.get()!!.performUserOnline("")
        }
        if (!TextUtils.isEmpty(result.getMsg())) {
            handleToast(result.getMsg())
        }
    }

    private fun handleUpdateTodoStatus(result: ActionResult) {
        if (isPageRecycle) {
            return
        }
        if (result.getCode() === ResultCode.SUCCESS.getValue()) {
            mUiController.get()!!.performUpdateTodoSuccess(result.getData())
        } else {
            mUiController.get()!!.performUpdateTodoFail("")
        }
    }

    /**
     * 处理消息提示
     */
    private fun handleToast(content: String) {
        if (isPageRecycle) {
            return
        }
        mUiController.get()!!.toastMsg(content)
    }

    /**
     * 检查页面是否存在
     * @return
     */
    private val isPageRecycle: Boolean
        private get() = pageActivity.get() == null || mUiController.get() == null

    /**
     * 回调抽象类的实现
     */
    private inner class LoginListener : com.broadli.clibs.LoginListener() {
        override fun onLoginFinish(result: ActionResult) {
            mMainHandler.post { handleLoginResult(result) }
        }

        override fun onAddCategory(result: ActionResult) {
            mMainHandler.post { handOnAddCategory(result) }
        }

        override fun onSignFinish(result: ActionResult) {
            mMainHandler.post { handleSignResult(result) }
        }

        override fun onGetCategoryList(result: ActionResult) {
            mMainHandler.post { handleGetCategoryResult(result) }
        }

        override fun onLogoutFinish(result: ActionResult) {
            mMainHandler.post { handleLogout(result) }
        }

        override fun onCheckStatusFinish(result: ActionResult) {
            mMainHandler.post { handleCheckUserStatus(result) }
        }

        override fun onUpdateTodo(result: ActionResult) {
            mMainHandler.post { handleUpdateTodoStatus(result) }
        }

        override fun onDisconnect(result: ActionResult) {
            mMainHandler.post { handleDeviceDisconnect(result) }
        }

        override fun onAddTodo(result: ActionResult) {
            mMainHandler.post { handOnAddTodo(result) }
        }

        override fun onGetTodoList(result: ActionResult) {
            mMainHandler.post { handleGetTodoResult(result) }
        }
    }

    init {
        mLoginListener = LoginListener()
        mLoginCore = LoginCore.create(mLoginListener)
    }

    public fun addTest() {
        Log.d(TAG, "add Test: mLoginCore is null ? :" + (mLoginCore == null))
        val result = mLoginCore?.nativeAdd(10, 20)
        Log.d(TAG, "add Test: 10+20= " + result)
    }
}
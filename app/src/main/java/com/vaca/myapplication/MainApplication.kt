package com.vaca.myapplication

import android.app.Application

import kotlinx.coroutines.launch

class MainApplication:Application() {
    companion object{
        lateinit var myApplication: Application
    }
    override fun onCreate() {
        super.onCreate()
        myApplication=this

        BleServer.dataScope.launch {
            JWebSocketClient.initWebSocket();
        }
    }


}
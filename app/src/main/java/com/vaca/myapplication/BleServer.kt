package com.vaca.myapplication

import androidx.lifecycle.MutableLiveData
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers

object BleServer {
    val dataScope = CoroutineScope(Dispatchers.IO)
    val msg=MutableLiveData<String>()
    val phone="15769415445"
}
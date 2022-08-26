package com.vaca.myapplication

import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import com.vaca.myapplication.databinding.ActivityMainBinding
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers

class MainActivity : AppCompatActivity() {
    val dataScope = CoroutineScope(Dispatchers.IO)
    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        initUdp(byteArrayOf(192.toByte(),168.toByte(),0,1))
//        native_say_hello(null)
        native_say_hello(object:GreetListener{
            override fun onGreetReceive(greet: GreetCallback?) {
                Log.e("gaga",greet!!.greetMsg)
            }

        })
/*       initKcp()


        dataScope.launch {
            while (true){
                updateKcp(System.currentTimeMillis())
                delay(20)
            }
        }*/
    }

    interface GreetListener {
        fun onGreetReceive(greet: GreetCallback?)
    }


    external fun initUdp(ip: ByteArray)
    external fun sendUdp(data: ByteArray,len:Int)
    external fun initKcp()
    external fun updateKcp(t:Long)
    external fun native_say_hello(listener: GreetListener?)

    companion object {
        init {
            System.loadLibrary("myapplication")
        }
    }
}
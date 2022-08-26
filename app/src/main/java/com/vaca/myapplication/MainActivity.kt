package com.vaca.myapplication

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import com.vaca.myapplication.databinding.ActivityMainBinding
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch

class MainActivity : AppCompatActivity() {
    val dataScope = CoroutineScope(Dispatchers.IO)
    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        initUdp(byteArrayOf(192.toByte(),168.toByte(),0,1))

/*       initKcp()


        dataScope.launch {
            while (true){
                updateKcp(System.currentTimeMillis())
                delay(20)
            }
        }*/
    }

    external fun initUdp(ip: ByteArray)

    external fun initKcp()
    external fun updateKcp(t:Long)

    companion object {
        init {
            System.loadLibrary("myapplication")
        }
    }
}
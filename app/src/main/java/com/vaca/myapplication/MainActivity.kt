package com.vaca.myapplication

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.vaca.myapplication.databinding.ActivityMainBinding
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.MainScope
import kotlinx.coroutines.launch
import org.json.JSONObject

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    var ip=""
    var port=0

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.sendPing.setOnClickListener {
            sendUdp()
        }

        binding.send2.setOnClickListener {
            if(ip.isNotEmpty()){
                beginStun(ip,port)
            }
        }

        initUdp()
    }


    external fun initUdp()
    external fun sendUdp()

    external fun beginStun(ip:String,port:Int)

    companion object {
        init {
            System.loadLibrary("myapplication")
        }
    }
    val dataScope = CoroutineScope(Dispatchers.IO)
    fun recvData(s: String) {
        MainScope().launch {
            try {
                val json=JSONObject(s)
                ip=json.getString("ip")
                port=json.getInt("port")
                binding.info.text=s
            }catch (e:Exception){
                binding.info.text=s
            }

        }
    }
}
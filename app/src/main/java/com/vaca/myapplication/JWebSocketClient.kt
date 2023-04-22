package com.vaca.myapplication

import android.util.Log


import org.greenrobot.eventbus.EventBus

import org.java_websocket.client.WebSocketClient
import org.java_websocket.drafts.Draft_6455
import org.java_websocket.handshake.ServerHandshake
import kotlin.Throws
import org.java_websocket.handshake.ClientHandshake
import org.java_websocket.WebSocket
import org.java_websocket.exceptions.InvalidDataException
import org.json.JSONObject
import java.lang.Exception
import java.net.URI

class JWebSocketClient(serverUri: URI?) : WebSocketClient(serverUri, Draft_6455()) {
    override fun onOpen(handshakeData: ServerHandshake) {}
    override fun onMessage(message2: String) {
        val da=JSONObject(message2)
        val message=da.getString("content")
        val action=da.getString("action")
        val turnMsg=TurnMsg(action,message)
        EventBus.getDefault().post(message2)
        Log.e("receiveMsg", message2)


        Log.e("JWebSocketClient", "onMessage()")
    }

    override fun onClose(code: Int, reason: String, remote: Boolean) {
        reconnectSocket()
        Log.e("JWebSocketClient", "onClose()$reason")
    }

    @Throws(InvalidDataException::class)
    override fun onWebsocketHandshakeSentAsClient(conn: WebSocket, request: ClientHandshake) {
        super.onWebsocketHandshakeSentAsClient(conn, request)
    }

    @Throws(InvalidDataException::class)
    override fun onWebsocketHandshakeReceivedAsClient(
        conn: WebSocket,
        request: ClientHandshake,
        response: ServerHandshake
    ) {
        super.onWebsocketHandshakeReceivedAsClient(conn, request, response)
    }

    override fun onError(ex: Exception) {
        ex.printStackTrace()
        Log.e("JWebSocketClient", "onError()" + ex.localizedMessage)
        ex.printStackTrace()
    }

    companion object {
        private var jWebSocketClient: JWebSocketClient? = null
        fun sendSafe(msg: String?) {
            try {
                jWebSocketClient!!.send(msg)
            } catch (e: Exception) {
            }
        }

        fun initWebSocket() {
            val netAddress = "ws://galic.fit:3003/ws?phone=15769415445";
            jWebSocketClient = JWebSocketClient(URI.create(netAddress))

        }

        var isReconnecting = false
        fun reconnectSocket() {
            if (isReconnecting) {
                return
            }
            val restartThread: Thread = object : Thread() {
                override fun run() {
                    try {
                        isReconnecting = true
                        while (true) {
                            sleep(1000)
                            jWebSocketClient = null
                            jWebSocketClient =
                                JWebSocketClient(URI.create("ws://galic.fit:3003/ws?phone=15769415445"))
                            try {
                                if (jWebSocketClient!!.isOpen) {
                                    break
                                }
                            } catch (e: Exception) {
                            }
                        }
                        sleep(1000)
                        isReconnecting = false
                    } catch (ignored: Exception) {
                    }
                }
            }
            restartThread.start()
        }
    }

    init {
        connectionLostTimeout = 5
        try {
            connectBlocking()
        } catch (e: Exception) {
        }
    }
}
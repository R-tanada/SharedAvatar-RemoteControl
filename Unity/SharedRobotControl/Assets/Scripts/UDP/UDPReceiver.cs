using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine;

public class UDPReceiver
{
    static UdpClient udpClient;
    private Thread receiveThread;

    bool isQuit = false;

    public void Connect(string ipAddr, int port, bool isMulti = false)
    {
        isQuit = false;
        udpClient = new UdpClient(port);

        if (isMulti)
        {
            IPAddress grpAddr = IPAddress.Parse(ipAddr);
            udpClient.JoinMulticastGroup(grpAddr);
        }

        receiveThread = new Thread(ReceiveData);
        receiveThread.Start();
    }

    public void Disconnect()
    {
        isQuit = true;

        if (receiveThread != null)
            receiveThread.Abort();

        if (udpClient != null)
            udpClient.Close();

        /*
        #if UNITY_EDITOR
            UnityEditor.EditorApplication.isPlaying = false;
        #elif UNITY_STANDALONE
            UnityEngine.Application.Quit();
        #endif
        */
    }

    public void ReceiveData()
    {
        while(true)
        {
            if (isQuit)
                break;

            IPEndPoint endPoint = null;
            byte[] data = udpClient.Receive(ref endPoint);
            string text = Encoding.UTF8.GetString(data);

            Debug.Log(text);

            if (text == "quit")
                Disconnect();
        } 
    }
}

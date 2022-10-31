using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class UDPSender
{
    private UdpClient udpClient;

    public void Connect(string ipAddr, int port, bool isMulti = false)
    {
        udpClient = new UdpClient();

        if(isMulti)
        {
            IPAddress grpAddr = IPAddress.Parse(ipAddr);
            IPEndPoint remoteEp = new IPEndPoint(grpAddr, port);
            udpClient.JoinMulticastGroup(grpAddr);
            udpClient.Connect(remoteEp);
        }
        else
            udpClient.Connect(ipAddr, port);
    }

    public void Disconnect()
    {
        if (udpClient != null)
            udpClient.Close();
    }

    public void SendText(string text)
    {
        byte[] msg = Encoding.UTF8.GetBytes(text);
        udpClient.Send(msg, msg.Length);

        StringBuilder sb = new StringBuilder();
        Debug.Log(sb.Append("Send: ").Append(Encoding.UTF8.GetString(msg)));
    }

    public void SendObjectsTransform(List<GameObject> objects)
    {
        StringBuilder sbTransform = new StringBuilder();
        for(int i = 0; i < objects.Count; i++)
        {
            sbTransform.Append("obj").Append(i).Append(",").Append("pos").Append(",");
            sbTransform.Append(objects[i].transform.localPosition.x.ToString("F4")).Append(",")
                       .Append(objects[i].transform.localPosition.y.ToString("F4")).Append(",")
                       .Append(objects[i].transform.localPosition.z.ToString("F4")).Append(",");

            sbTransform.Append("rot").Append(",");
            sbTransform.Append(objects[i].transform.localRotation.x.ToString("F4")).Append(",")
                       .Append(objects[i].transform.localRotation.y.ToString("F4")).Append(",")
                       .Append(objects[i].transform.localRotation.z.ToString("F4")).Append(",")
                       .AppendLine(objects[i].transform.localRotation.w.ToString("F4"));
        }

        byte[] byteTransform = Encoding.UTF8.GetBytes(sbTransform.ToString());
        udpClient.Send(byteTransform, byteTransform.Length);

        //StringBuilder sb = new StringBuilder();
        //Debug.Log(sb.Append("Send: ").Append(System.Text.Encoding.ASCII.GetString(byteTransform)));
    }

    /// <summary>
    /// Send to UDP stream: Shared objects and controller trigger value
    /// </summary>
    /// <param name="header">Data header</param>
    /// <param name="objects">Shared objects</param>
    /// <param name="triggerValue">Trigger value</param>
    /// <param name="useEuler">Send rotation as euler angles. If False, use quaternion</param>
    /// <param name="isTriggerOnly">Send only trigger data</param>
    /// <param name="isTransformOnly">Send only transform</param>
    /// <param name="positionUnit">Send unit</param>
    /// <param name="round">Number of digits after the decimal point</param>
    public void SendMultipleData(string header, List<GameObject> objects, float triggerValue, bool useEuler = false, bool isTriggerOnly = false, bool isTransformOnly = false, string positionUnit = "m", string round = "F4")
    {
        StringBuilder sbMultipleData = new StringBuilder();
        sbMultipleData.Append(header).Append(",");

        int posUnit = 1;
        if (positionUnit.Equals("mm"))
            posUnit = 1000;

        for (int i = 0; i < objects.Count; i++)
        {
            Vector3 sendPosition;
            Quaternion sendQuaternion;

            if (isTriggerOnly)
            {
                sendPosition    = Vector3.zero;
                sendQuaternion  = Quaternion.identity;
            }
            else
            {
                sendPosition    = objects[i].transform.localPosition * posUnit;
                sendQuaternion  = objects[i].transform.localRotation;
            }

            if (i != 0)
                sbMultipleData.Append(",");

            sbMultipleData.Append("obj").Append(i).Append(",").Append("pos").Append(",");
            sbMultipleData.Append(sendPosition.x.ToString(round)).Append(",")
                          .Append(sendPosition.y.ToString(round)).Append(",")
                          .Append(sendPosition.z.ToString(round)).Append(",");

            if(useEuler)
            {
                sbMultipleData.Append("rotEuler").Append(",");
                sbMultipleData.Append(sendQuaternion.eulerAngles.x.ToString(round)).Append(",")
                              .Append(sendQuaternion.eulerAngles.y.ToString(round)).Append(",")
                              .Append(sendQuaternion.eulerAngles.z.ToString(round)).Append(",");
            }
            else
            {
                sbMultipleData.Append("rotQuaternion").Append(",");
                sbMultipleData.Append(sendQuaternion.x.ToString(round)).Append(",")
                              .Append(sendQuaternion.y.ToString(round)).Append(",")
                              .Append(sendQuaternion.z.ToString(round)).Append(",")
                              .Append(sendQuaternion.w.ToString(round)).Append(",");
            }

            sbMultipleData.Append("trigger").Append(",");
            if (isTransformOnly)
                sbMultipleData.Append(0.ToString());
            else
                sbMultipleData.Append(triggerValue.ToString(round));
        }

        byte[] byteTransform = Encoding.UTF8.GetBytes(sbMultipleData.ToString());
        udpClient.Send(byteTransform, byteTransform.Length);
    }

    public void SendRigidBody(List<GameObject> objects)
    {
        float posX = -objects[0].transform.localPosition.x;
        StringBuilder sbRigidTransform = new StringBuilder();

        //sbRigidTransform.Append("position:").Append(",");
        sbRigidTransform.Append(objects[0].transform.localPosition.z.ToString("F6")).Append(",")
                        .Append(posX.ToString("F6")).Append(",")
                        .Append(objects[0].transform.localPosition.y.ToString("F6")).Append(",");

        //sbRigidTransform.Append("rotation:").Append(",");
        sbRigidTransform.Append(objects[0].transform.localRotation.z.ToString("F6")).Append(",")
                        .Append(objects[0].transform.localRotation.x.ToString("F6")).Append(",")
                        .Append(objects[0].transform.localRotation.y.ToString("F6")).Append(",")
                        .Append(objects[0].transform.localRotation.w.ToString("F6"));

        byte[] byteTransform = Encoding.UTF8.GetBytes(sbRigidTransform.ToString());
        udpClient.Send(byteTransform, byteTransform.Length);
    }
}

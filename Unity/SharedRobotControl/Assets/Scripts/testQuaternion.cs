using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class testQuaternion : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        Quaternion q = this.transform.rotation;

        //Debug.Log(q);
        Debug.Log("w > " + q.w + "   x > " + q.x + "   y > " + q.y + "   z > " + q.z);
    }
}

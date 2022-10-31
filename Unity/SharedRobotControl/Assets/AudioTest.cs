using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AudioTest : MonoBehaviour
{
    [SerializeField] private AudioSource a1;//AudioSource�^�̕ϐ�a1��錾 �g�p����AudioSource�R���|�[�l���g���A�^�b�`�K�v
    [SerializeField] private AudioSource a2;//AudioSource�^�̕ϐ�a2��錾 �g�p����AudioSource�R���|�[�l���g���A�^�b�`�K�v
    [SerializeField] private AudioSource a3;//AudioSource�^�̕ϐ�a3��錾 �g�p����AudioSource�R���|�[�l���g���A�^�b�`�K�v

    [SerializeField] private AudioClip b1;//AudioClip�^�̕ϐ�b1��錾 �g�p����AudioClip���A�^�b�`�K�v
    [SerializeField] private AudioClip b2;//AudioClip�^�̕ϐ�b2��錾 �g�p����AudioClip���A�^�b�`�K�v 
    [SerializeField] private AudioClip b3;//AudioClip�^�̕ϐ�b3��錾 �g�p����AudioClip���A�^�b�`�K�v 

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Alpha1))
            a1.PlayOneShot(b1);

        if (Input.GetKeyDown(KeyCode.Alpha2))
            a2.PlayOneShot(b2);
    }
}

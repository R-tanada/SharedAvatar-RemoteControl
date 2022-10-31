/// <summary>
/// The model to which this script is attached is no longer affected by the camera's Culling.
/// </summary>
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnableOffScreenRendering : MonoBehaviour {
	void Start () {
        foreach(var renderer in GetComponentsInChildren<SkinnedMeshRenderer>(true))
        {
            renderer.updateWhenOffscreen = true;
        }
	}
}
using UnityEngine.Audio;
using UnityEngine;

[CreateAssetMenu(menuName = "Game Asset/Sound")]
public class Sound:ScriptableObject
{

    public string name;
    public AudioClip clip;

    [Range(0f, 1f)]
    public float volume;

    [Range(.1f, 3f)]
    public float pitch;

    [Range(0f, 1f)]
    public float spatialBlend;

    public bool loop;

    public bool isBgm;

    public bool fadeIn;

    public bool fadeOut;

    public float fadeDuration;

    public bool randomInterval;

    public int intervalMin;

    public int intervalMax;

}

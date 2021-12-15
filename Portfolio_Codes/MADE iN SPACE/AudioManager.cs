using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine.Audio;
using UnityEngine;

[System.Serializable]
public class SoundSource
{
    public string name;
    public float volume;
    public float spatialBlend;
    public bool fadeIn;
    public bool fadeOut;
    public float fadeDuration;
    public bool randomInterval;
    public int intervalMin;
    public int intervalMax;
    public AudioClip clip;
    public AudioSource source;
}

public class AudioManager : MonoBehaviour
{

    public List<SoundSource> m_sfxSources;
    public List<SoundSource> m_bgmSources;

    public static AudioManager instance;

    private void Awake()
    {
        if (instance == null)
            instance = this;
        else
        {
            Destroy(this);
            return;
        }
    }
    // Use this for initialization
    void Start()
    {
        
    }

    public void SetVolumeSFX(int vol)
    {
        float val = Mathf.Clamp((float)vol, 0.0f, 100.0f);
        val = val / 100.0f;
        PlayerPrefs.SetFloat("volume_sfx", val);

        foreach (var src in m_sfxSources)
        {
            src.source.volume = val * src.volume;
        }
    }

    public void SetVolumeBGM(int vol)
    {
        float val = Mathf.Clamp((float)vol, 0.0f, 100.0f);
        val = val / 100.0f;
        PlayerPrefs.SetFloat("volume_bgm", val);
        
        foreach (var src in m_bgmSources)
        {
            src.source.volume = val * src.volume;
        }
    }

    // public void Play(Sound sound)
    // {
    //     if (sound == null)
    //         return;
    //
    //     Play(sound, sound.pitch);
    // }

    public void Play(Sound sound, bool waitFinish = false, GameObject go = null, bool separate = false,float pitch=-100.0f)
    {
        if (sound == null || sound.clip == null)
            return;
        
        SoundSource soundSource;// = soundSources.Find(source => source.name == sound.name);
        float volume = sound.volume;
        
        if (sound.isBgm)
        {
            soundSource = m_bgmSources.Find(source => source.name == sound.name);
            volume *= PlayerPrefs.GetFloat("volume_bgm");
        }
        else
        {
            soundSource = m_sfxSources.Find(source => source.name == sound.name);
            volume *= PlayerPrefs.GetFloat("volume_sfx");
        }
        
        if (soundSource == null)
        {
            soundSource = new SoundSource();
            soundSource.name = sound.name;
            soundSource.volume = volume;
            soundSource.spatialBlend = sound.spatialBlend;
            soundSource.fadeIn = sound.fadeIn;
            soundSource.fadeOut = sound.fadeOut;
            soundSource.fadeDuration = sound.fadeDuration;
            soundSource.randomInterval = sound.randomInterval;
            soundSource.intervalMin = sound.intervalMin;
            soundSource.intervalMax = sound.intervalMax;
            if (separate)
            {
                soundSource.source = go.AddComponent<AudioSource>();
            }
            else
            {
                soundSource.source = gameObject.AddComponent<AudioSource>();
            }

            soundSource.clip = sound.clip;

            if (sound.loop)
            {
                m_bgmSources.Add(soundSource);
                
            }
            else
            {
                m_sfxSources.Add(soundSource);
                
            }
        }
        
        float audioPitch = pitch > 0.0f ? pitch : sound.pitch;

        // if (sound.loop)
        // {
        //     //var src = m_bgmSources.Find(source => source.name == sound.name);
        //     //if (src == null)
        // }
        // else
        // {
        //     //var src = m_sfxSources.Find(source => source.name == sound.name);
        //     //if (src == null) 
        //     
        //     volume = sound.volume * PlayerPrefs.GetFloat("volume_sfx");
        // }
        
        instance.PlayAudioSource(soundSource, volume, audioPitch, sound.loop, waitFinish);




        // AudioSource s = soundSource.source;
        // s.volume = sound.volume;
        // s.pitch = pitch > 0.0f ? pitch : sound.pitch;
        // s.loop = sound.loop;
        // if (!s.isPlaying)
        // {
        //     s.Play();
        // }
    }

    public void PlayAtRandom(List<Sound> soundList)
    {
        if (soundList.Count == 0)
        {
            Debug.Log("Empty sound list");
            return;
        }
            
        int randomIndex = UnityEngine.Random.Range(0, soundList.Count);
        Sound sound = soundList[randomIndex];
        
        this.Play(sound, false);
        // if (sound == null)
        //     return;
        //
        // SoundSource soundSource = soundSources.Find(source => source.name == sound.name);
        //
        // if (soundSource == null)
        // {
        //     soundSource = new SoundSource();
        //     soundSource.name = sound.name;
        //     soundSource.source = gameObject.AddComponent<AudioSource>();
        //     soundSource.source.clip = sound.clip;
        //     soundSources.Add(soundSource);
        // }
        //
        // AudioSource s = soundSource.source;
        // s.volume = sound.volume;
        // s.pitch = sound.pitch;
        // s.loop = sound.loop;
        // s.Play();
    }

    public void PlayAudioSource(SoundSource soundSource,float volume, float pitch, bool loop, bool waitFinish)
    {
        AudioSource s = soundSource.source;
        s.clip = soundSource.clip;
        if (soundSource.fadeIn)
        {
            s.volume = 0;
        }
        else
        {
            s.volume = volume;
        }
        s.spatialBlend = soundSource.spatialBlend;
        s.pitch = pitch;
        s.loop = loop;

        if (waitFinish)
        {
            if(!s.isPlaying && soundSource.randomInterval)
            {
                StartCoroutine(PlayOnRandomInterval(s, soundSource));

            }
            else if (!s.isPlaying && !soundSource.randomInterval)
            {
                s.Play();
            }

        }
        else
        {
            s.Play();
        }

        StartCoroutine(FadeSound(s, soundSource, volume));


    }

    public static IEnumerator FadeSound(AudioSource player, SoundSource soundSource, float volume)
    {

        if (soundSource.fadeIn)
        {

            while (player.volume < volume)
            {
                player.volume += volume * Time.deltaTime / soundSource.fadeDuration;
                yield return null;
            }

            player.volume = volume;
        }

        if (soundSource.fadeOut)
        {
            while (player.volume > 0)
            {
                player.volume -= volume * Time.deltaTime / soundSource.fadeDuration;
                yield return null;
            }

            player.volume = 0;
        }
    }

    public static IEnumerator PlayOnRandomInterval(AudioSource player, SoundSource soundSource)
    {
        while (soundSource.randomInterval)
        {
            int timer = UnityEngine.Random.Range(soundSource.intervalMin, soundSource.intervalMax + 1);
            player.Play();
            yield return new WaitForSeconds(timer);

        }


    }

    public void ClearSound()
    {
        m_sfxSources.Clear();
        m_bgmSources.Clear();
    }
    

    //
    public void Stop(string name)
    {
        
        SoundSource m_bgm = m_bgmSources.Find(source => source.name == name);
        SoundSource m_sfx = m_sfxSources.Find(source => source.name == name);
        
        if (m_bgm == null && m_sfx == null)
        {
            Debug.LogWarning("Cannot find sound: " + name);
            return;
        }
        
        if(m_bgm != null){
            m_bgm.source.Stop();
        }

        if(m_sfx != null)
        {
            m_sfx.source.Stop();
        }
    }

    public void StopAll()
    {
        for(int i = 0; i < m_bgmSources.Count; i++)
        {
            SoundSource m_bgm = m_bgmSources[i];
            m_bgm.source.Stop();

        }

        for (int i = 0; i < m_sfxSources.Count; i++)
        {
            SoundSource m_sfx = m_sfxSources[i];
            m_sfx.source.Stop();

        }
    }
}

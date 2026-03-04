#include <alsa/asoundlib.h>
#include <stdio.h>

#define CARD "default"
#define SELEM_NAME "Master"

typedef struct
{
    float percentage;
    int muted;
} audio;

extern inline void GetAlsaMasterVolume(audio *result)
{
    long volume, min, max;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;

    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, CARD);
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, SELEM_NAME);
    snd_mixer_elem_t *elem = snd_mixer_find_selem(handle, sid);

    snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &volume);
    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);

    snd_mixer_selem_get_playback_switch(elem, SND_MIXER_SCHN_FRONT_LEFT, &(result->muted));

    snd_mixer_close(handle);

    result->percentage = (float)(volume - min) / (float)(max - min) * 100.0f;
}

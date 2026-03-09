#include <alsa/asoundlib.h>

#define CARD "default"
#define SELEM_NAME "Master"

extern inline void get_alsa_master_volume(void)
{
    long volume, min, max;
    int muted;

    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    snd_mixer_elem_t *elem;

    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, CARD);
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, SELEM_NAME);
    elem = snd_mixer_find_selem(handle, sid);

    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_MONO, &volume);
    snd_mixer_selem_get_playback_switch(elem, SND_MIXER_SCHN_MONO, &muted);

    snd_mixer_close(handle);

    printf(" | %.0f%% ", (double)(volume - min) / (double)(max - min) * 100.0);

    if (!muted)
    {
        fputs("MUTED ", stdout);
    }
}

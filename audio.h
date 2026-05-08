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

    if (!elem)
    {
        fprintf(stderr, "Unable to find mixer element '%s'\n", SELEM_NAME);

        if (sid)
        {
            snd_mixer_selem_id_free(sid);
        }

        if (handle)
        {
            snd_mixer_close(handle);
        }

        exit(EXIT_FAILURE);
    }

    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);

    if (snd_mixer_selem_is_playback_mono(elem))
    {
        snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_MONO, &volume);
        snd_mixer_selem_get_playback_switch(elem, SND_MIXER_SCHN_MONO, &muted);
    }
    else
    {
        snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &volume);
        snd_mixer_selem_get_playback_switch(elem, SND_MIXER_SCHN_FRONT_LEFT, &muted);
    }

    snd_mixer_close(handle);

    printf(" | %.0f%% ", (double)(volume - min) / (double)(max - min) * 100.0);

    if (!muted)
    {
        if (fputs("MUTED ", stdout) == EOF)
        {
            perror("fputs failed");
            exit(EXIT_FAILURE);
        }
    }
}

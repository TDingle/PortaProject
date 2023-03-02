#include "audio.h"
#include <stdio.h>
#include "SDL/SDL.h"

#define MUSIC_FILE "BeepBox-Song.wav"

static Uint8* audio_buffer_og; // pointer to our audio buffer (wav data)
static Uint32 audio_len_og; // length of audio buffer

static Uint8* audio_pos; // offset into audio buffer we are currently playing



// AudioData: a pointer to the audio data buffer
// Length: the length of that buffer in bytes
void SDLAudioCallback(void* UserData, Uint8* AudioData, int Length) {
    Uint32 remaining_len = (audio_buffer_og + audio_len_og) - audio_pos;
    if (remaining_len == 0) {
        // when we have nothing left in the buffer, we reset the pointer to loop the audio
        audio_pos = audio_buffer_og;
    }

    // when we reach end of buffer, don't copy over the bounds
    Length = (Length > remaining_len ? remaining_len : Length);
    SDL_memcpy(AudioData, audio_pos, Length); // simply copy from one buffer into the other

    audio_pos += Length;
}


void InitMusic() {
    /* Load the WAV */
    Uint32 wav_length; // length of our sample
    Uint8* wav_buffer; // buffer containing our audio file
    SDL_AudioSpec wav_spec; // the specs of our piece of music
    if (SDL_LoadWAV(MUSIC_FILE, &wav_spec, &wav_buffer, &wav_length) == NULL) {
        printf("Failed to load wav\n");
        return;
    }

    wav_spec.callback = &SDLAudioCallback;
    audio_pos = wav_buffer;
    audio_buffer_og = wav_buffer;
    audio_len_og = wav_length;


    SDL_OpenAudio(&wav_spec, nullptr);
    SDL_PauseAudio(0);
}
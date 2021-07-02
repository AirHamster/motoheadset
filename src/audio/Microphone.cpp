//#include <SPIFFS.h>
//#include <FS.h>
#include "Microphone.h"
#include "files.h"
#include "i2s_microphone.h"


#pragma pack(push, 1)
typedef struct
{
    // RIFF Header
    char riff_header[4]; // Contains "RIFF"
    int wav_size;        // Size of the wav portion of the file, which follows the first 8 bytes. File size - 8
    char wave_header[4]; // Contains "WAVE"

    // Format Header
    char fmt_header[4]; // Contains "fmt " (includes trailing space)
    int fmt_chunk_size; // Should be 16 for PCM
    short audio_format; // Should be 1 for PCM. 3 for IEEE Float
    short num_channels;
    int sample_rate;
    int byte_rate;          // Number of bytes per second. sample_rate * num_channels * Bytes Per Sample
    short sample_alignment; // num_channels * Bytes Per Sample
    short bit_depth;        // Number of bits per sample

    // Data
    char data_header[4]; // Contains "data"
    int data_bytes;      // Number of bytes in data. Number of samples * num_channels * sample byte size
    // uint8_t bytes[]; // Remainder of wave file is bytes
} wav_header_t;
#pragma pack(pop)

Microphone::Microphone()
{
}

Microphone::~Microphone()
{
}

void Microphone::getFrames(Frame_t *frames, int number_frames)
{

    for (int i = 0; i < number_frames; i++)
    {
        micropone_read((&frames[i].left), 1);
        //frames[i].left = 0;
        //printf("%d\n", frames[i].left);
    }
    return;
    // fill the buffer with data from the file wrapping around if necessary
    for (int i = 0; i < number_frames; i++)
    {
        // if we've reached the end of the file then seek back to the beginning (after the header)
        if (feof(m_file) != 0)
        {
            //m_file.seek(44);
            fseek(m_file, 44, SEEK_SET);
        }
        // read in the next sample to the left channel
        fread((uint8_t *)(&frames[i].left), sizeof(int16_t), 1, m_file);
        // if we only have one channel duplicate the sample for the right channel
        if (m_num_channels == 1)
        {
            frames[i].right = frames[i].left;
        }
        else
        {
            // otherwise read in the right channel sample
            fread((uint8_t *)(&frames[i].right), sizeof(int16_t), 1, m_file);
        }
    }
}
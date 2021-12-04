#ifndef __wav_file_reader_h__
#define __wav_file_reader_h__

//#include <SPIFFS.h>
//#include <FS.h>
#include "SampleSource.h"
#include "files.h"

class WAVFileReader : public SampleSource
{
private:
    int m_num_channels;
    int m_sample_rate;
    FILE *m_file;
    
    FilesStorage &files = FilesStorage::get_instance();

public:
    WAVFileReader(const char *file_name);
    ~WAVFileReader();
    int sampleRate() { return m_sample_rate; }
    void getFrames(Frame_t *frames, int number_frames);
    char f_name[128] = {0};
};

#endif
#ifndef __microphone_file_reader_h__
#define __microphone_file_reader_h__

//#include <SPIFFS.h>
//#include <FS.h>
#include "SampleSource.h"
#include "files.h"

class Microphone : public SampleSource
{
private:
    int m_num_channels;
    int m_sample_rate = 16000;
    FILE *m_file;
    
    FilesStorage &files = FilesStorage::get_instance();

public:
    Microphone();
    ~Microphone();
    int sampleRate() { return m_sample_rate; }
    void getFrames(Frame_t *frames, int number_frames);
    //char f_name[128] = {0};
};

#endif
idf_component_register(SRCS "Microphone.cpp" "files.cpp" "WAVFileReader.cpp" "i2s_microphone.cpp" "i2s_driver.cpp"
                            "SinWaveGenerator.cpp"
                            "I2SOutput.cpp"
                            "i2s_microphone.cpp"
                            "WAVFileReader.cpp"
                            "files.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES spiffs)
spiffs_create_partition_image(files ./files)

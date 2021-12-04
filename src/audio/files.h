#pragma once
#include <string>
#include "esp_spiffs.h"

class FilesStorage
{
public:
    static FilesStorage &get_instance()
    {
        static FilesStorage instance;
        return instance;
    }
    esp_err_t init();
    esp_err_t deinit();
    void ls();
    void info();

private:
    FilesStorage();
    FilesStorage(const FilesStorage &) = delete;
    FilesStorage &operator=(const FilesStorage &) = delete;
    static esp_vfs_spiffs_conf_t m_conf;
};

class File
{
public:
    File(std::string path, std::string mode);
    ~File();
    esp_err_t read(size_t offset, size_t len, void *out_data, size_t *out_len);
    size_t write(size_t offset, const void *data, size_t len);
    size_t size();
    bool exist();

private:
    const std::string m_path{};
    const std::string m_mode{};
    FILE *m_file{nullptr};
};

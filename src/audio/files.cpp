#include <dirent.h>
#include <filesystem>
#include "esp_log.h"
#include "files.h"

static constexpr auto TAG = "SPIFFS";
static constexpr auto BASE_PATH = "/spiffs";
esp_vfs_spiffs_conf_t FilesStorage::m_conf;

FilesStorage::FilesStorage()
{
    m_conf = {.base_path = BASE_PATH,
              .partition_label = nullptr,
              .max_files = 1, // Maximum files that could be open at the same time.
              .format_if_mount_failed = false};
    init();
}

esp_err_t FilesStorage::init()
{
    if (esp_spiffs_mounted(m_conf.partition_label)) {
        ESP_LOGD(TAG, "SPIFFS already mounted");
        return ESP_OK;
    }

    esp_err_t ret = esp_vfs_spiffs_register(&m_conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
    }

    return ret;
}

esp_err_t FilesStorage::deinit()
{
    return esp_vfs_spiffs_unregister(m_conf.partition_label);
}

void FilesStorage::info()
{
    size_t total = 0, used = 0;
    esp_err_t ret = esp_spiffs_info(m_conf.partition_label, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)",
                 esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }
}

void FilesStorage::ls()
{
    DIR *dir;
    struct dirent *ent;
    int i{};
    if ((dir = opendir(BASE_PATH)) != nullptr) {
        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != nullptr) {
            i++;
            File file{
                std::string{BASE_PATH} + std::string{"/"} + std::string{ent->d_name},
                "rb"};
            printf("%d. %s\t\t%d bytes\r\n", i, ent->d_name, file.size());
        }
        closedir(dir);
    } else {
        /* could not open directory */
        perror("");
        return;
    }
}

File::File(std::string path, std::string mode) : m_path(path), m_mode(mode)
{
    m_file = fopen(m_path.c_str(), m_mode.c_str());
    if (m_file == nullptr) {
        ESP_LOGE(TAG, "File %s open error", m_path.c_str());
        perror("");
    }
}

File::~File()
{
    fclose(m_file);
}

bool File::exist()
{
    return access(m_path.c_str(), F_OK);
}

esp_err_t File::read(size_t offset, size_t len, void *out_data, size_t *out_len)
{
    if (m_file != nullptr) {
        fseek(m_file, offset, SEEK_SET);
        *out_len = fread(out_data, sizeof(uint8_t), len, m_file);
        return ESP_OK;
    }
    return ESP_ERR_NOT_FOUND;
}

size_t File::write(size_t offset, const void *data, size_t len)
{
    size_t written{};
    if (m_file != nullptr) {
        fseek(m_file, offset, SEEK_SET);
        written = fwrite(data, sizeof(uint8_t), len, m_file);
        return written;
    }
    return 0;
}

size_t File::size()
{
    if (m_file != nullptr) {
        fseek(m_file, 0, SEEK_END);
        size_t size = ftell(m_file);
        return size;
    }
    return 0;
}

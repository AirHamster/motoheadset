idf_component_register(SRCS "console.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES console spi_flash fatfs nvs_flash)

idf_component_register(SRCS "cmd_system.cpp"
                            "console.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES console spi_flash fatfs nvs_flash)

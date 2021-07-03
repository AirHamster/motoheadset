#set(COMPONENT_SRCS "main.cpp")
#register_component(SRCS "main.cpp"
#                    INCLUDE_DIRS ""
#                    REQUIRES cli bt audio audio_pipeline audio_stream esp_peripherals bluetooth_service nvs_flash)

set(COMPONENT_SRCS "main.cpp")
set(COMPONENT_ADD_INCLUDEDIRS ".")
set(COMPONENT_REQUIRES cli bt audio audio_pipeline audio_stream esp_peripherals bluetooth_service nvs_flash)
register_component()
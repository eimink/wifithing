idf_component_register(SRCS "main.c" "webserver.c"
                    PRIV_REQUIRES esp_wifi nvs_flash
                    INCLUDE_DIRS ".")

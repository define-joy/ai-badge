# IDF 强制要求名为 main 的组件。此处只做源文件清单，业务在 boot/ kit/ core/ adapters/ apps/。

set(BADGE_APP "playground" CACHE STRING "User app path under apps/, e.g. playground or examples/buttons")

set(APP_SRC "${CMAKE_SOURCE_DIR}/apps/${BADGE_APP}/app.cpp")
if(NOT EXISTS "${APP_SRC}")
    message(FATAL_ERROR "BADGE_APP='${BADGE_APP}' not found: ${APP_SRC}")
endif()

idf_component_register(
    SRCS
        "${CMAKE_SOURCE_DIR}/boot/main/app_main.cpp"
        "${APP_SRC}"
        "${CMAKE_SOURCE_DIR}/kit/src/AppRegistry.cpp"
        "${CMAKE_SOURCE_DIR}/kit/src/Badge.cpp"
        "${CMAKE_SOURCE_DIR}/core/PortHub.cpp"
        "${CMAKE_SOURCE_DIR}/core/Time.cpp"
        "${CMAKE_SOURCE_DIR}/core/EventPump.cpp"
        "${CMAKE_SOURCE_DIR}/core/PowerPolicy.cpp"
        "${CMAKE_SOURCE_DIR}/core/JobQueue.cpp"
        "${CMAKE_SOURCE_DIR}/core/Runtime.cpp"
        "${CMAKE_SOURCE_DIR}/adapters/board/bringup.cpp"
    INCLUDE_DIRS
        "${CMAKE_SOURCE_DIR}/kit/include"
        "${CMAKE_SOURCE_DIR}"
    REQUIRES
        pthread
        esp_timer
        freertos
)

target_compile_features(${COMPONENT_LIB} PUBLIC cxx_std_17)
target_compile_options(${COMPONENT_LIB} PRIVATE -fno-exceptions -fno-rtti)

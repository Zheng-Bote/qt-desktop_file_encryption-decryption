cmake_minimum_required(VERSION 3.23)

project(file_encryption-decryption
  VERSION 2.1.0
  DESCRIPTION "Desktop app to encrypt and decrypt files"
  HOMEPAGE_URL "https://github.com/Zheng-Bote/qt-desktop_file_encryption-decryption"
  LANGUAGES CXX
)

set(PROG_EXEC_NAME "${PROJECT_NAME}")
set(PROG_NAME "File encryption/decryption")
set(PROG_VERSION "${PROJECT_VERSION}")
set(PROG_HOMEPAGE "${PROJECT_HOMEPAGE_URL}")
set(PROG_DESCRIPTION "${PROJECT_DESCRIPTION}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(configure)
include_directories(include)

# WICHTIG: Header-Dateien (.h) MÜSSEN hier gelistet sein, damit MOC funktioniert!
set(PROJECT_SOURCES
    # Sources
    src/main.cpp
    src/encryption_manager.cpp
    src/encrypt_file_dialog.cpp
    src/decrypt_file_dialog.cpp
    src/menu_page.cpp
    src/template.cpp
    src/qaesencryption.cpp
    
    # Headers (Kritisch für vtable Fehler)
    include/encryption_manager.h
    include/encrypt_file_dialog.h
    include/decrypt_file_dialog.h
    include/menu_page.h
    include/template.h
    include/qaesencryption.h
    include/rz_config.h
    
    # Resources
    resources.qrc
)

if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE file_encryption-decryption.icns)
    set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/resources/file_encryption-decryption.icns")
    set_source_files_properties(${app_icon_macos} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    qt_add_executable(${PROJECT_NAME} MACOSX_BUNDLE MANUAL_FINALIZATION ${PROJECT_SOURCES} ${app_icon_macos})
else()
    qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION WIN32 ${PROJECT_SOURCES})
endif()

if(UNIX AND NOT APPLE)
    # Warnungen etwas strikter, aber Switch-Warnungen ignorieren wir ggf. im Code
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -O3)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(${PROJECT_NAME})

# --- 5. Custom Generators (Original Code) ---

# CMake File API Query (Datei erzeugen)
set(QUERY_DIR "${CMAKE_BINARY_DIR}/.cmake/api/v1/query/client-project")
file(MAKE_DIRECTORY "${QUERY_DIR}")
file(WRITE "${QUERY_DIR}/codemodel-v2" "") 

# libs.txt Generierung (Timing-sicher via GENERATE)
file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/libs.txt" 
     CONTENT "$<TARGET_PROPERTY:${PROJECT_NAME},LINK_LIBRARIES>")

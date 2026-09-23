cmake_minimum_required(VERSION 3.20)
project(PasswordManager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON) 
#set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(SOURCES 
    src/main.cpp
    src/MainWindow.cpp
    src/LoginDialog.cpp
    src/SetupDialog.cpp
    src/Entry.cpp
    src/PasswordModel.cpp
    src/CopyDelegate.cpp
    src/DatabaseManager.cpp
    src/SelectUserDialog.cpp
    resources/resources.qrc
    Qt-AES/qaesencryption.cpp
    
)
set(HEADERS
    include/MainWindow.hpp
    include/LoginDialog.hpp
    include/SetupDialog.hpp
    include/Entry.hpp
    include/PasswordModel.hpp
    include/CopyDelegate.hpp
    include/Utils.hpp
    include/DatabaseManager.hpp
    include/SelectUserDialog.hpp
    Qt-AES/qaesencryption.h
)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Svg
    Sql
)

add_executable(PasswordManager
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(PasswordManager PRIVATE
    include
    Qt-AES
)

target_link_libraries(PasswordManager PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Svg
    Qt6::Sql
)
cmake_minimum_required(VERSION 3.5)

project(MAZE_CPP VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Задаем флаги компиляции
if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
add_compile_options(
    # -Werror
    # -Wall
    # -Wextra
    # -Wpedantic
    # -std=c++17
    )
endif()
    
#Задааем переменную для тестирования и выключаем тестирование по дефолту
option(MAZE_TESTING "Module testing ON" OFF)
    
#Задааем переменную для покрытия тестами и выключаем тестирование по дефолту
option(MAZE_COVERAGE "Test coverage ON" OFF)

include_directories(view)

    #Находим необходимые кьютишные пакеты
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS PrintSupport)

    #Задаем исходники модели
    set(MODEL_SRC
        model/model.cpp
        model/model.hpp
        model/maze/maze.cpp
        model/maze/maze.hpp
        model/cave/cave.cpp
        model/cave/cave.hpp
        myenum.h
    )

    #Создаем либу модели
    add_library(
        CPP_MAZE_lib_back
        STATIC
        ${MODEL_SRC}
    )

if(MAZE_TESTING) ### проверяем включено ли модульное тестирование 
    add_subdirectory(tests) ### если да, то собираем тесты и покрытие при необходимости
else() ### если нет, то собираем проект
    message(STATUS "CPP_MAZE project testing OFF")
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    #Задаем исходники контроллера
    set(CONTR_SRC
        controller/controller.hpp
        controller/controller.cpp
    )

    #Создаем либу контроллера
    add_library(
        CPP_MAZE_lib_contr
        STATIC
        ${CONTR_SRC}
    )

    #Устанавливаем путь до либы scene.hpp
    set(CMAKE_PREFIX_PATH "../../../src/view")

    #Задаем исходники интерфейса
    set(VIEW_SRC
        view/main_view.cpp
        view/main_view.h
        view/main_view.ui
        view/scene.cpp
        view/scene.hpp
        myenum.h
    )

    #Создаем либу интерфейса
    add_library(
        CPP_MAZE_lib_view
        STATIC
        ${VIEW_SRC}
    )

    #Создаем кьютишный таргет сборки
    qt_add_executable(CPP_MAZE
        MANUAL_FINALIZATION
        main.cpp
    )

    #Говорим, что для либы вью нужна обязательна либа модели и контроллера, и что снаружи вью недоступны все методы бэка
    target_link_libraries(CPP_MAZE_lib_contr PRIVATE CPP_MAZE_lib_back)
    target_link_libraries(CPP_MAZE_lib_view PRIVATE CPP_MAZE_lib_contr)

    #Говорим, что для либы вью нужны обязательна либы кьюта, и что снаружи вью они недоступны
    target_link_libraries(CPP_MAZE_lib_view PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
    target_link_libraries(CPP_MAZE_lib_view PRIVATE Qt${QT_VERSION_MAJOR}::PrintSupport)

    #Говорим, что для таргета билда обязательна либа вью, и что снаружи ее методы недоступны
    target_link_libraries(CPP_MAZE PRIVATE CPP_MAZE_lib_view)

    set_target_properties(CPP_MAZE PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
    )

    install(TARGETS CPP_MAZE
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

    if(QT_VERSION_MAJOR EQUAL 6)
        qt_finalize_executable(CPP_MAZE)
    endif()

endif()

# to generate makefile on mac use : qmake -spec macx-g++
HEADERS     = gui.h 
SOURCES     = gui.cpp main.cpp ../mdsrc/mdsys.cc mdthread.cpp
FORMS       = gui.ui

mac {
  CONFIG -= app_bundle
}


# install
target.path = gui
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS *.pro
sources.path = .
INSTALLS += target sources


TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    mbr_table.c \
    other.c

HEADERS += \
    mbr_table.h \
    other.h

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    other.c \
    mbr_table.c

HEADERS += \
    other.h \
    mbr_table.h

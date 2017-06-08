TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    mbr_table.c \
    other.c \
    gpt_table.c

HEADERS += \
    other.h \
    mbr_table.h \
    gpt_table.h

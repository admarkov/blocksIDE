#-------------------------------------------------
#
# Project created by QtCreator 2017-09-06T18:46:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = blocksIDE
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    diagramscene.cpp \
    diagramitem.cpp \
    arrow.cpp

HEADERS  += mainwindow.h \
    diagramscene.h \
    diagramitem.h \
    arrow.h

SOURCES += 	muParser.cpp\
                muParserBase.cpp \
                muParserBytecode.cpp\
                muParserCallback.cpp\
                muParserDLL.cpp\
                muParserError.cpp\
                muParserInt.cpp\
                muParserTest.cpp\
                muParserTokenReader.cpp\

HEADERS  +=	muParser.h\
                muParserBase.h \
                muParserBytecode.h\
                muParserCallback.h\
                muParserDef.h\
                muParserDLL.h\
                muParserError.h\
                muParserFixes.h\
                muParserInt.h\
                muParserStack.h\
                muParserTemplateMagic.h\
                muParserTest.h\
                muParserToken.h\
                muParserTokenReader.h\

FORMS    +=

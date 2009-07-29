# File generated by kdevelop's qmake manager.
# -------------------------------------------
# Subdir relative project main directory: ./src
# Target is an application: ../bin/architecture/glnemo2
# include global configuration
include(../config.arch)

DEFINES += GLEW_STATIC
FORMS += formobjectcontrol.ui \
    formabout.ui \
    formscreenshot.ui \
    formselectpart.ui \
    formoptions.ui
HEADERS += mainwindow.h \
    glwindow.h \
    globject.h \
    glgridobject.h \
    globaloptions.h \
    snapshotinterface.h \
    pluginsmanage.h \
    particlesobject.h \
    particlesselectrange.h \
    particlesdata.h \
    globjectparticles.h \
    loadingthread.h \
    formobjectcontrol.h \
    formabout.h \
    gltexture.h \
    componentrange.h \
    userselection.h \
    glselection.h \
    frustumculling.h \
    tools3d.h \
    vec3d.h \
    gltextobject.h \
    globjectosd.h \
    formscreenshot.h \
    formselectpart.h \
    formoptions.h \
    orbits.h \
    gloctree.h \
    densityhisto.h \
    colormap.h \
    densitycolorbar.h \
    camera.h \
    catmull_rom_spline.h
SOURCES += glnemo.cc \
    mainwindow.cc \
    glwindow.cc \
    globject.cc \
    glgridobject.cc \
    globaloptions.cc \
    pluginsmanage.cc \
    particlesobject.cc \
    particlesselectrange.cc \
    particlesdata.cc \
    globjectparticles.cc \
    loadingthread.cc \
    formobjectcontrol.cc \
    formabout.cc \
    gltexture.cc \
    componentrange.cc \
    userselection.cc \
    glselection.cc \
    frustumculling.cc \
    tools3d.cc \
    vec3d.cc \
    gltextobject.cc \
    globjectosd.cc \
    formscreenshot.cc \
    formselectpart.cc \
    formoptions.cc \
    orbits.cc \
    glew/glew.c \
    gloctree.cc \
    densityhisto.cc \
    colormap.cc \
    densitycolorbar.cc \
    camera.cc \
    catmull_rom_spline.cc
RESOURCES = glnemo.qrc
CONFIG += $$GLOBAL \
    warn_on \
    opengl \
    thread


CONFIG(debug, debug|release) { 
    TARGET = ../bin/$$ARCH/glnemo2.debug
    win32 { 
        DESTDIR = ../bin/$$ARCH
        TARGET = glnemo2_debug
    }
    unix {
        #TARGET = ../bin/$$ARCH/glnemo2
        #INSTALLS += target
        #NEMOBIN = $(NEMOBIN)
        #target.path += $$NEMOBIN
    } 
    
}
else { 
    TARGET = ../bin/$$ARCH/glnemo2.release
    win32 { 
        DESTDIR = ../bin/$$ARCH
        TARGET = glnemo2
    }
}
TEMPLATE = app
QT += opengl
MOC_DIR = .moc/$$ARCH
UI_DIR = ._ui/$$ARCH
OBJECTS_DIR = .obj/$$ARCH/$$COMPILEMODE
RCC_DIR = .res/$$ARCH/$$COMPILEMODE
QMAKE_LIBDIR = ../plugins/lib/$$ARCH/$$COMPILEMODE \
    $$NEMOLIB \
    ../plugins/ftm/lib/$$ARCH/$$COMPILEMODE \
    ../plugins/gadget/lib/$$ARCH/$$COMPILEMODE \
    ../plugins/zlib/lib/$${ARCH}/$$COMPILEMODE
# !!!!!!
# DESTDIR = .
# !!!!!!
# TARGET = glnemo2
# DESTDIR = ../bin/$$ARCH
INCLUDEPATH += ../plugins \
    ../plugins/ftm \
    ../plugins/nemolight \
    ../plugins/gadget \
    ../plugins/zlib \
    ../src \
    $$NEMOLIB \
    $$NEMOINC \
    glew
LIBS += -lsnapshot \
    -lftm \
    -lnemo \
    -lgadget \
    -lzlib
TARGETDEPS += ../plugins/nemolight/lib/$${ARCH}/$$COMPILEMODE/libnemo.a \
    ../plugins/lib/$${ARCH}/$$COMPILEMODE/libsnapshot.a \
    ../plugins/ftm/lib/$${ARCH}/$$COMPILEMODE/libftm.a \
    ../plugins/gadget/lib/$${ARCH}/$$COMPILEMODE/libgadget.a \
    ../plugins/zlib/lib/$${ARCH}/$$COMPILEMODE/libzlib.a
DISTFILES += ../ChangeLog
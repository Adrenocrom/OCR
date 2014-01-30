CC            = gcc
CXX           = g++
LINK          = g++
DEL_FILE      = rm -f

# for Linux (remove "-m64" for 32bit arch)
CFLAGS        = -m64 -pipe -O2 -Wall -W $(DEFINES)
CXXFLAGS      = -m64 -pipe -O2 -Wall -W $(DEFINES)
LFLAGS        = -m64 -Wl,-O3

# for MAC (remove "-m64" for 32bit arch)
# LFLAGS        = -m64

# For Linux
#---------------------------------------------------
# For OpenCV-2.3.1 (local installation), put correct pathes here.
# DEFINES       = -DOPENCV231
# INCPATH       = -I../../../local_installs/opencv/OpenCV-2.3.1-install/include
# LIBS          = -L../../../local_installs/opencv/OpenCV-2.3.1-install/lib -lopencv_core -lopencv_highgui -lopencv_imgproc

# For OpenCV-2.3.1 (standard for Ubuntu 12.10)
DEFINES       = -DOPENCV231
INCPATH       = -I/usr/include/opencv
LIBS          = -L/usr/lib -lopencv_core -lopencv_highgui -lopencv_imgproc

# For OpenCV-2.1 (standard for Ubuntu 11.10)
# DEFINES       = -DOPENCV210
# INCPATH       = -I/usr/include/opencv
# LIBS          = -L/usr/lib -lcv -lcxcore -lhighgui
#--------------------------------------------------

# For OpenCV-2.3.1 under MAC
# DEFINES       = -DOPENCV231
# INCPATH       = -I/opt/local/include
# LIBS          = -L/opt/local/lib -lopencv_core -lopencv_highgui

SOURCES       = main.cpp image.cpp feedforward.cpp ocr.cpp
OBJECTS       = main.o image.o feedforward.o ocr.o
TARGET        = OCRConverter

all: $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

clean:
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


distclean: clean
	-$(DEL_FILE) $(TARGET) 

ocr.o: ocr.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ocr.o ocr.cpp

feedforward.o: feedforward.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o feedforward.o feedforward.cpp

image.o : image.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o image.o image.cpp

main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

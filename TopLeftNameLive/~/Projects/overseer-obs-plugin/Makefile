# Overseer Plugin Makefile
CXX = g++
CXXFLAGS = -std=c++17 -fPIC -shared -Wall -Wextra
INCLUDES = -I/usr/include/obs -I/usr/include/qt5 -I/usr/include/qt5/QtCore -I/usr/include/qt5/QtWidgets
LIBS = -lobs-frontend-api -lobs -lQt5Core -lQt5Widgets

TARGET = overseer-plugin.so
SOURCES = src/overseer-plugin.cpp
OBJECTS = src/overseer-plugin.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

install: $(TARGET)
	cp $(TARGET) ~/.config/obs-studio/plugins/overseer-plugin/bin/64bit/

.PHONY: all clean install

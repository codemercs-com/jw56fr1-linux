CXX = g++
CSTD = -std=c++0x
CFLAGS  += -Wall
LDFLAGS = -I/usr/include/libusb-1.0/ -lusb-1.0 -pthread

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

TARGET_DIR = ../bin
TARGET = read_axis

all: $(TARGET)
	@rm -f $(OBJS)

$(TARGET): $(OBJS)
	$(CXX) $(CSTD)  $(CFLAGS) $(OBJS) -o $(TARGET_DIR)/$(TARGET) $(LDFLAGS)
	
%.o: %.cpp
	$(CXX) $(CSTD) $(LDFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET_DIR)/$(TARGET)
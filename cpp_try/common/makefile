

TARGET = libcommon.a
TARGET_PATH = ../bin

INCLUDE = Net
INCLUDE_PATH = $(addprefix -I,$(INCLUDE))

SRC = Net
SRC_PATH = $(wildcard *.cpp)
SRC_PATH += $(foreach dir, $(subst :, ,$(SRC)), $(wildcard $(dir)/*.cpp))

OBJ_DIR = obj
OBJ_PATH = $(addprefix $(OBJ_DIR)/, $(subst :, , $(SRC)))
OBJS = $(addprefix $(OBJ_DIR)/,$(subst .cpp,.o,$(SRC_PATH)))
MAKE_OBJECT_DIR := $(shell mkdir -p $(OBJ_DIR) $(OBJ_PATH))

$(TARGET) : $(OBJS)
	@echo "Linking... "
	ar -r $@ $(OBJS)

$(OBJS) : $(OBJ_DIR)/%.o : %.cpp $(OBJ_DIR)/%.cpp.d
	g++ -o $@ -c $< $(INCLUDE_PATH)

DEPS = $(OBJS:.o=.cpp.d)
$(DEPS) : $(OBJ_DIR)/%.cpp.d : %.cpp
	g++ $< -MM $(INCLUDE_PATH) > $(OBJ_DIR)/$<.d

clean:
	@rm -rf ./obj
	@rm -rf $(TARGET)
	@echo "clean over"

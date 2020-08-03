

TARGET = app1
TARGET_PATH = ../../bin
MACROS = -D NDEBUG

INCLUDE = ../common
INCLUDE_PATH = $(addprefix -I,$(INCLUDE))
LIB_NAME = -lcommon
LIB_PATH = -L../common
LIB_DEP = ../common/libcommon.a

SRC = Module:Module/ModuleB:Util
SRC_PATH = $(wildcard *.cpp)
SRC_PATH += $(foreach dir, $(subst :, ,$(SRC)), $(wildcard $(dir)/*.cpp))

OBJ_DIR = obj
OBJ_PATH = $(addprefix $(OBJ_DIR)/, $(subst :, , $(SRC)))
OBJS = $(addprefix $(OBJ_DIR)/,$(subst .cpp,.o,$(SRC_PATH)))
MAKE_OBJECT_DIR := $(shell mkdir -p $(OBJ_DIR) $(OBJ_PATH))

$(TARGET_PATH)/$(TARGET) : $(OBJS) $(LIB_DEP) 
	@echo "Linking... "
	g++ -o $@ $(OBJS) $(INCLUDE_PATH) $(LIB_NAME) $(LIB_PATH)

$(OBJS) : $(OBJ_DIR)/%.o : %.cpp $(OBJ_DIR)/%.cpp.d
	g++ -o $@ -c $< $(INCLUDE_PATH) $(MACROS)

DEPS = $(OBJS:.o=.cpp.d)
$(DEPS) : $(OBJ_DIR)/%.cpp.d : %.cpp
	g++ $< -MM $(INCLUDE_PATH) > $(OBJ_DIR)/$<.d

test:
	@echo $(SRC_PATH)
	@echo $(OBJ_PATH)
	@echo $(OBJS)
	@echo $(DEPS)


clean:
	@rm -rf ./obj
	@rm -rf $(TARGET_PATH)/$(TARGET)
	@echo "clean over"

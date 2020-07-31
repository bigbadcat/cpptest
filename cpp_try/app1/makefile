

TARGET = app1
TARGET_PATH = ../bin

INCLUDE = Util
INCLUDE_PATH = $(addprefix -I,$(INCLUDE))

SRC = Module:Module/ModuleB:Util
SRC_PATH = $(wildcard *.cpp)
SRC_PATH += $(foreach dir, $(subst :, ,$(SRC)), $(wildcard $(dir)/*.cpp))

OBJ_DIR = obj
OBJ_PATH = $(addprefix $(OBJ_DIR)/, $(subst :, , $(SRC)))
OBJS = $(addprefix $(OBJ_DIR)/,$(subst .cpp,.o,$(SRC_PATH)))
MAKE_OBJECT_DIR := $(shell mkdir -p $(OBJ_DIR) $(OBJ_PATH))

$(TARGET) : $(OBJS)
	@echo "Linking "$(TARGET)
	g++ -o $@ $(OBJS) $(INCLUDE_PATH)

#obj/main.o : main.cpp obj/main.cpp.d
#	@echo "Compling "$^
#	g++ -o $@ -c $< $(INCLUDE_PATH)

$(OBJS) : $(OBJ_DIR)/%.o : %.cpp $(OBJ_DIR)/%.cpp.d
	@echo "Compling "$^
	g++ -o $@ -c $< $(INCLUDE_PATH)i

#%.o: %.cpp %.cpp.d
#	@echo "Compling "$^
#	g++ -o $@ -c $< $(INCLUDE_PATH)

DEPS = $(OBJS:.o=.cpp.d)
$(DEPS) : $(OBJ_DIR)/%.cpp.d : %.cpp
	@echo "Build depend "$<
	g++ $< -MM $(INCLUDE_PATH) > $(OBJ_DIR)/$<.d

test:
	@echo $(SRC_PATH)
	@echo $(OBJ_PATH)
	@echo $(OBJS)
	@echo $(DEPS)


clean:
	@rm -rf ./obj
	@rm -rf $(TARGET)

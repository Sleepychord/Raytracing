BUILD_DIR ?= build
CXXSOURCES += $(shell find . -name "*.cpp")
CPPFLAGS += -Isrc/include -Ilib/include \
			-Igeometry/include -Ibasic/include \
			-lopencv_core -lopencv_highgui \
			-g -std=c++11
OBJS = $(addprefix $(BUILD_DIR)/,$(CXXSOURCES:.cpp=.o))
DEPFILES = $(OBJS:.o=.d)

all: main

$(BUILD_DIR)/%.d: %.cpp
	@mkdir -pv $(dir $@)
	@echo "[dep] $< ..."
	@g++ $(CPPFLAGS) -MM -MT "$@ $(@:.d=.o)" "$<"  > "$@"

$(BUILD_DIR)/%.o: %.cpp
	@echo "[cxx] $< ..."
	@g++ -c $< -o $@ $(CPPFLAGS)

sinclude $(DEPFILES)

main: $(OBJS)
	@echo "Linking ..."
	g++ $(OBJS) $(CPPFLAGS) -o main

clean:
	rm -rf $(BUILD_DIR)/*

.PHONY: all clean
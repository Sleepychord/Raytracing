BUILD_DIR ?= build
CXXSOURCES += $(shell find . -name "*.cpp")
CPPFLAGS += -Isrc/include -Ilib/include \
			-Igeometry/include -Ibasic/include \
		    -lopencv_core -lopencv_highgui -lopencv_imgcodecs \
			-g -std=c++11 -O2
OBJS = $(addprefix $(BUILD_DIR)/,$(CXXSOURCES:.cpp=.o))
DEPFILES = $(OBJS:.o=.d)
$(CXX) = g++-5
all: main

$(BUILD_DIR)/%.d: %.cpp
	@mkdir -pv $(dir $@)
	@echo "[dep] $< ..."
	@$(CXX) $(CPPFLAGS) -MM -MT "$@ $(@:.d=.o)" "$<"  > "$@"

$(BUILD_DIR)/%.o: %.cpp
	@echo "[cxx] $< ..."
	$(CXX) -c $< -o $@ $(CPPFLAGS)

sinclude $(DEPFILES)

main: $(OBJS)
	@echo "Linking ..."
	$(CXX) $(OBJS) $(CPPFLAGS) -o main

clean:
	rm -rf $(BUILD_DIR)/*

.PHONY: all clean
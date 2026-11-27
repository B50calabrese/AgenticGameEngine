# Makefile for Agentic Game Engine
# Wraps modern CMake targets for ergonomic developer workflows.

BUILD_DIR ?= build
BUILD_TYPE ?= Release
NUM_JOBS ?= $(shell nproc 2>/dev/null || echo 4)

.PHONY: all build configure test demo format clean help

all: build

help:
	@echo "Agentic Game Engine Build System"
	@echo "--------------------------------"
	@echo "make configure   - Generate CMake configuration files"
	@echo "make build       - Build the engine and targets"
	@echo "make test        - Build and run unit tests"
	@echo "make demo        - Build and run sandbox demo"
	@echo "make format      - Run clang-format on all C++ source and header files"
	@echo "make clean       - Remove the build directory"

configure:
	cmake -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

build: configure
	cmake --build $(BUILD_DIR) --parallel $(NUM_JOBS)

test: build
	ctest --test-dir $(BUILD_DIR) --output-on-failure

demo: build
	./$(BUILD_DIR)/demos/sandbox_demo

window-demo: build
	./$(BUILD_DIR)/demos/window_demo

primitives-demo: build
	./$(BUILD_DIR)/demos/primitives_demo

breakout: build
	./$(BUILD_DIR)/demos/breakout_demo

platformer: build
	./$(BUILD_DIR)/demos/platformer_demo

textures-demo: build
	./$(BUILD_DIR)/demos/textures_demo

font-demo: build
	./$(BUILD_DIR)/demos/font_demo

ui-demo: build
	./$(BUILD_DIR)/demos/ui_demo

format:
	@if command -v clang-format >/dev/null 2>&1; then \
		find include src tests demos -name "*.h" -o -name "*.cpp" | xargs clang-format -i; \
		echo "Formatting complete."; \
	else \
		echo "clang-format not found. Install with: sudo apt install clang-format"; \
	fi

clean:
	rm -rf $(BUILD_DIR)

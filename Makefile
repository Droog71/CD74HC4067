SOURCES = src/main.c
TARGET  = dist/chip.wasm

$(TARGET): $(SOURCES) src/wokwi-api.h
		docker run -v $(shell pwd):/src wokwi/builder-clang-wasm \
	  	clang --target=wasm32-unknown-wasi --sysroot /opt/wasi-libc -nostartfiles -Wl,--import-memory -Wl,--export-table -Wl,--no-entry -Werror -o $(TARGET) $(SOURCES)

.PHONY: clean
clean:
		rm -f $(TARGET)

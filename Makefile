COMPILE_FILES=main.cpp lexing.cpp ast.cpp nodes.cpp codegen.cpp
DEPEND_FILES=$(COMPILE_FILES) Makefile lexing.hpp ast.hpp nodes.hpp codegen.hpp

OUT_FILE=pebkacc
DBG_FILE=$(OUT_FILE)_dbg

COMPILER=clang++ --std=c++17 $(COMPILE_FILES)

$(OUT_FILE): $(DEPEND_FILES)
	$(COMPILER) -o $(OUT_FILE) -march=native -O2
	strip -s $(OUT_FILE)

$(DBG_FILE): $(DEPEND_FILES)
	$(COMPILER) -o $(DBG_FILE) -g -fsanitize=address

.PHONY: run debug clean all

run: $(OUT_FILE)
	./$(OUT_FILE) test.pebkac

debug: $(DBG_FILE)
	./$(DBG_FILE) test.pebkac

clean:
	rm -rf $(OUT_FILE) $(DBG_FILE)

all: $(OUT_FILE) $(DBG_FILE)

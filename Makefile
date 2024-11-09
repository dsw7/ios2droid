.PHONY = help compile clean lint format
.DEFAULT_GOAL = help

define HELP_LIST_TARGETS
To compile binary:
    $$ make compile
To remove build directory:
    $$ make clean
To run cppcheck linter:
    $$ make lint
To format code:
    $$ make format
endef

export HELP_LIST_TARGETS

help:
	@echo "$$HELP_LIST_TARGETS"

compile:
	@cmake -S ios2droid -B build
	@make --jobs=12 --directory=build install

clean:
	@rm -rfv build

lint:
	@cppcheck ios2droid --enable=all

format:
	@clang-format -i --verbose --style=file ios2droid/src/*.cpp ios2droid/include/*.hpp

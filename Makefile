.PHONY = help format compile clean lint test py
.DEFAULT_GOAL = help

define HELP_LIST_TARGETS
To format code:
    $$ make format
To compile binary:
    $$ make compile
To remove build directory:
    $$ make clean
To run cppcheck linter:
    $$ make lint
To run unit tests:
    $$ make test
To lint, analyze and format python code:
    $$ make py
endef

export HELP_LIST_TARGETS

help:
	@echo "$$HELP_LIST_TARGETS"

format:
	@clang-format -i --verbose --style=file ios2droid/*.cpp ios2droid/*.hpp

compile: format
	@cmake -S ios2droid -B build
	@make --jobs=12 --directory=build install

clean:
	@rm -rfv build

lint:
	@cppcheck ios2droid --enable=all

test: export PATH_BIN = $(CURDIR)/build/ios2droid
test: compile
	@python3 -m unittest -v tests/test_basic.py

py:
	@python3 -m black tests
	@python3 -m pylint --exit-zero tests
	@python3 -m mypy --strict tests

.PHONY = help compile clean lint format test py
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
To run unit tests:
    $$ make test
To lint, analyze and format python code:
    $$ make py
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
	@clang-format -i --verbose --style=file ios2droid/*.cpp ios2droid/*.hpp

test: export PATH_BIN = $(CURDIR)/build/ios2droid
test: compile
	@python3 -m unittest -v tests/*.py

py:
	@python3 -m black tests
	@python3 -m pylint --exit-zero tests
	@python3 -m mypy --strict tests

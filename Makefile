.PHONY = format compile tidy clean lint test py
.DEFAULT_GOAL = compile

format:
	@clang-format -i --verbose --style=file src/*.cpp src/*.hpp

compile: format
	@cmake -S src -B build
	@make --jobs=12 --directory=build install

tidy:
	@cmake -S src -B build
	@clang-tidy -p build src/*.cpp src/*.hpp

clean:
	@rm -rfv build

lint:
	@cppcheck src --enable=all

test: export PATH_BIN = $(CURDIR)/build/ios2droid
test: compile
	@python3 -m unittest -v tests/*.py

py:
	@python3 -m black tests
	@python3 -m pylint --exit-zero tests
	@python3 -m mypy --strict tests

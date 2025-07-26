.PHONY = format compile tidy clean lint test py
.DEFAULT_GOAL = compile

DIR_PROD_BUILD = build/prod
DIR_TEST_BUILD = build/test

format:
	@clang-format -i --verbose --style=file src/*.cpp src/*.hpp

compile: format
	@cmake -S src -B $(DIR_PROD_BUILD)
	@make --jobs=12 --directory=$(DIR_PROD_BUILD) install

tidy:
	@cmake -S src -B $(DIR_PROD_BUILD)
	@clang-tidy -p $(DIR_PROD_BUILD) src/*.cpp src/*.hpp

clean:
	@rm -rfv build

lint:
	@cppcheck src --enable=all

test: export PATH_BIN = $(CURDIR)/$(DIR_TEST_BUILD)/ios2droid
test: format
	@cmake -S src -B $(DIR_TEST_BUILD) -DENABLE_COVERAGE=ON
	@make --jobs=12 --directory=$(DIR_TEST_BUILD)
	@python3 -m unittest -v tests/*.py
	@lcov --capture --directory=$(DIR_TEST_BUILD) --output-file $(DIR_TEST_BUILD)/coverage.info
	@lcov --remove $(DIR_TEST_BUILD)/coverage.info "/usr/*" "*/external/*" --output-file $(DIR_TEST_BUILD)/coverage.info
	@genhtml $(DIR_TEST_BUILD)/coverage.info --output-directory $(DIR_TEST_BUILD)/coverageResults
	@echo "See coverage report at: $(DIR_TEST_BUILD)/coverageResults/index.html"

py:
	@python3 -m black tests
	@python3 -m pylint --exit-zero tests
	@python3 -m mypy --strict tests

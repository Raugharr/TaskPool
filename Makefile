SRC=src/
TEST=test/

.PHONY: all clean

all:
	$(MAKE) -C $(SRC) all
	$(MAKE) -C $(TEST) all

clean:
	$(MAKE) -C $(SRC) clean
	$(MAKE) -C $(TEST) clean

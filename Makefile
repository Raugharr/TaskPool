SRC=src/

.PHONY: all clean

all:
	$(MAKE) -C $(SRC) all

clean:
	$(MAKE) -C $(SRC) clean

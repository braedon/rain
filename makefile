all:: rain

rain: rain.c
	gcc -o $@ $? -lncurses -lm

.PHONY: clean
clean::
	rm -f rain

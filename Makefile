.PHONY: test $(MAKECMDGOALS)

$(MAKECMDGOALS): test

test:
	make -C ./test $(MAKECMDGOALS)

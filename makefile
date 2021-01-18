.PHONY: clean

make:
	$(MAKE) -C src/make/

clean:
	cd src/make/ && $(MAKE) clean

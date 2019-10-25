ft_handle: ft_handle.c spi.c lib/libftd2xx.dylib D2XX/ftd2xx.h
	gcc -ID2XX -Llib -lftd2xx -O2 -o ft_handle ft_handle.c

clean:
	rm -f ft_handle

run: ft_handle
	DYLD_LIBRARY_PATH=$(CURDIR)/lib/:$(DYLD_LIBRARY_PATH) ./ft_handle

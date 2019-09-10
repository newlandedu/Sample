#cc = mipsel-openwrt-linux-gcc
cc = gcc
prom = main
deps = $(shell find -name "*.h")
src = $(shell find -name "*.c")
dir=$(dir $(deps))
obj = $(src:%.c=%.o)
inc = $(dir:%=-I %)
OPTIONS:=-O3  -g -lm -ldl -pthread 
SUBOPTS:=

$(prom): $(obj)
	$(cc) $(OPTIONS) -o $(prom) $(obj) -lm
	

%.o: %.c $(deps)
	@$(cc) $(inc) $(SUBOPTS) -c $< -o $@
	@echo "$(cc) $(SUBOPTS) -c $< -o $@"
	
.PHONY: clean
clean:
	rm -rf $(obj) $(prom)

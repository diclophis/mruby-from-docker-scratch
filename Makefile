# Makefile

product=mirbrc
build=build/$(product)-build
target=$(build)/$(product)
mruby_static_lib=mruby/build/host/lib/libmruby.a
mrbc=mruby/bin/mrbc

objects = $(patsubst %,$(build)/%, $(patsubst %.c,%.o, $(wildcard *.c)))
static_ruby_headers = $(patsubst %,$(build)/%, $(patsubst lib/%.rb,%.h, $(wildcard lib/*.rb)))
.SECONDARY: $(static_ruby_headers) $(objects)
objects += $(mruby_static_lib)

LDFLAGS=-lm $(shell (uname | grep -q Darwin || echo -static) )

CFLAGS=-Imruby/include -I$(build)

$(shell mkdir -p $(build))

docker-build: $(target)
	docker build .

$(target): $(build) $(objects)
	$(CC) -o $@ $(objects) $(LDFLAGS)

$(build)/test.yml: $(target) .mirbrc
	$(target) > $@

clean:
	cd mruby && make clean
	rm -R $(build)

$(build):
	mkdir -p $(build)

$(build)/%.o: %.c $(static_ruby_headers)
	$(CC) $(CFLAGS) -c $< -o $@

$(mruby_static_lib): config/mruby.rb
	cd mruby && MRUBY_CONFIG=../config/mruby.rb make

$(mrbc): $(mruby_static_lib)

$(build)/%.h: lib/%.rb $(mrbc)
	mruby/bin/mrbc -g -B $(patsubst $(build)/%.h,%, $@) -o $@ $<

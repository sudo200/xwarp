include config.mk
include lib/libs.mk
all: out/xwarp

out/xwarp: obj/main.c.o out
	if [ -n '$(wildcard obj/*.cpp.o)' ]; then $(CXX) $(LDFLAGS) -o'out/xwarp' $(wildcard obj/*.o) $(wildcard lib/bin/*.a); else $(CC) $(LDFLAGS) -o'out/xwarp' $(wildcard obj/*.o) $(wildcard lib/bin/*.a); fi
	$(OBJCOPY) --only-keep-debug 'out/xwarp' 'out/xwarp.dbg'
	chmod -x 'out/xwarp.dbg'
	$(OBJCOPY) --strip-unneeded 'out/xwarp'
	$(OBJCOPY) --add-gnu-debuglink='out/xwarp.dbg' 'out/xwarp'

obj/%.cpp.o: obj src/%.cpp
	$(CXX) -c -o'$@' 'src/$(patsubst obj/%.cpp.o,%,$@).cpp' $(CXXFLAGS)

obj/%.c.o: obj src/%.c
	$(CC) -c -o'$@' 'src/$(patsubst obj/%.c.o,%,$@).c' $(CFLAGS)

clean:
	$(RM) -r out
	$(RM) -r obj

out:
	$(MKDIR) out

obj:
	$(MKDIR) obj

.PHONY: clean all

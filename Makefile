##############################################################
#               CMake Project Wrapper Makefile               #
############################################################## 

SHELL := /bin/bash
RM    := rm -rf

all: ./build/Makefile
	@ $(MAKE) -C build

./build/Makefile:
	@ mkdir -p build
	@ (cd build >/dev/null 2>&1 && cmake ..)

distclean:
	@- (cd build >/dev/null 2>&1 && cmake .. >/dev/null 2>&1)
	@- $(MAKE) --silent -C build clean || true
	@- $(RM) -rf ./build
	@- $(RM) -rf ./bin
	@- $(RM) -rf ./lib





ifeq ($(findstring distclean,$(MAKECMDGOALS)),)

    $(MAKECMDGOALS): ./build/Makefile
	@ $(MAKE) -C build $(MAKECMDGOALS)

endif

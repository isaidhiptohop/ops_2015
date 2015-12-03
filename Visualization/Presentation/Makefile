# Copyright (c) 2013 The Native Client Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

#
# GNU Make based build file.  For details on GNU Make see:
# http://www.gnu.org/software/make/manual/make.html
#

# Project Build flags
WARNINGS := -Wno-long-long -Wall -Wswitch-enum -pedantic -Werror
CXXFLAGS := -pthread -std=gnu++11 $(WARNINGS)

#
# Compute tool paths
#

NACL_SDK_ROOT := ../nacl_sdk/pepper_45
OSHELPERS = python $(NACL_SDK_ROOT)/tools/oshelpers.py
RM := $(OSHELPERS) rm

PNACL_TC_PATH := $(abspath $(NACL_SDK_ROOT)/toolchain/mac_pnacl)
PNACL_CXX := $(PNACL_TC_PATH)/bin/pnacl-clang++
PNACL_FINALIZE := $(PNACL_TC_PATH)/bin/pnacl-finalize
CXXFLAGS := -I$(NACL_SDK_ROOT)/include $(CXXFLAGS) 
#LDFLAGS := -L$(NACL_SDK_ROOT)/lib/pnacl/Release -lppapi_cpp -lppapi
LDFLAGS :=

PNACL_LDFLAGS := -L$(NACL_SDK_ROOT)/lib/pnacl/Release -lppapi_cpp -lppapi -lpthread $(LDFLAGS)
PNACL_CXXFLAGS := -I$(NACL_SDK_ROOT)/include $(CXXFLAGS)

HEADERS := ant.h antcolony.h ants.h exceptions.h interfaces.h landscape.h path.h pathiterator.h
CFILES := ant.cpp antcolony.cpp exceptions.cpp interfaces.cpp landscape.cpp path.cpp pathiterator.cpp 

# Declare the ALL target first, to make the 'all' target the default build
all: antColony.pexe

clean:
	$(RM) antColony.pexe antColony.bc

#antColony.bc: $(HEADERS) $(CFILES) program.cpp 
#antColony.bc: $(HEADERS) $(CFILES) program.cpp
antColony.bc: program.cpp $(HEADERS) $(CFILES)
	$(PNACL_CXX) -o $@ $< $(CFILES) program.cpp -O2 $(PNACL_CXXFLAGS) $(PNACL_LDFLAGS)

antColony.pexe: antColony.bc
	$(PNACL_FINALIZE) -o $@ $<

serve:
	python -m SimpleHTTPServer 8000

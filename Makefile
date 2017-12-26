#
# Copyright (c) 2018 [n/a] info@embeddora.com All rights reserved
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#        * Redistributions of source code must retain the above copyright
#          notice, this list of conditions and the following disclaimer.
#        * Redistributions in binary form must reproduce the above copyright
#          notice, this list of conditions and the following disclaimer in the
#          documentation and/or other materials provided with the distribution.
#        * Neither the name of The Linux Foundation nor
#          the names of its contributors may be used to endorse or promote
#          products derived from this software without specific prior written
#          permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NON-INFRINGEMENT ARE DISCLAIMED.    IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#

# Prefix for 'cross'es' (purposed for future) 
PREFIX= 

# A suffix to tell 
SUFFIX=mri

CFLAGS=  -g -DWIDE_CONSOLE -DRESPONCE_LATENCY_GAUGING

LDFLAGS=

# -g debug symbols (to explore dumped core, for instance)

# -fprofile-arcs -ftest-coverage  (to collect code coverage statistics)

# -DEXTRA_INFORMATIVITY (to mix packet data and packet details in curl's output)

# -DEXTRA_PURITY (to clean intermediate files an completion; as the files contain useful site-crack hints they're valuable while debug/evaluation)

# -DEXTRA_PRECISION (to simulate conversation between browser and webpage _exactly by running not necessary HTTP-requests)

# -DRESPONCE_LATENCY_GAUGING (time computation)


OBJS= site_crawler.$(SUFFIX).o

GRBG=*.o *~ site_crawler.$(SUFFIX)



CC=$(PREFIX)gcc

LD=$(PREFIX)ld

all:	site_crawler.$(SUFFIX)

site_crawler.$(SUFFIX): $(OBJS)
	$(CC) $(CFLAGS)   $(OBJS)  -lcurl   -o $@


clean:
	rm $(GRBG) _delme.* 

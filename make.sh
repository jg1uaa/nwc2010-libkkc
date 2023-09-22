#!/usr/bin/env bash

set -x

src/add_yomi -n 3 -i 3gm -o 3gm.out -l ${1:-0}

# convert to format ARPA with IRSTLM
# reference: A tutorial on the IRSTLM library (Nicola Bertoldi, 2008)
# https://citeseerx.ist.psu.edu/document?repid=rep1&type=pdf&doi=2ef9472ecfc0c3d48a0f9b2a031916c703fb1140
#
# for Debian, apt-get install irstlm

IRSTLM=/usr/lib/irstlm
IRSTLM_BIN=${IRSTLM}/bin

${IRSTLM_BIN}/build-sublm.pl --size 3 --ngrams 3gm.out --sublm LM.000
${IRSTLM_BIN}/merge-sublm.pl --size 3 --sublm LM -lm iARPA_LM.gz
${IRSTLM_BIN}/compile-lm --text iARPA_LM.gz data.arpa

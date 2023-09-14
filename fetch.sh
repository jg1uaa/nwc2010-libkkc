#!/usr/bin/env bash

# 日本語ウェブコーパス2010 (https://www.s-yata.jp/corpus/nwc2010/ngrams/) より、
# N-gramコーパス/形態素N-gram/頻度1000以上 3-gramを使用

rm -rf 3gm-0000{,.xz}

wget https://s3-ap-northeast-1.amazonaws.com/nwc2010-ngrams/word/over999/3gms/3gm-0000.xz

xz -d 3gm-0000.xz

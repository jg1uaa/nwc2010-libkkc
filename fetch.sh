#!/usr/bin/env bash

set -x

# 日本語ウェブコーパス2010 (https://www.s-yata.jp/corpus/nwc2010/ngrams/) より、
# N-gramコーパス/形態素N-gram 3-gramを使用

rm -rf 3gm-*.xz 3gm

case $1 in
    -l)
	# 出現頻度100以上
	wget https://s3-ap-northeast-1.amazonaws.com/nwc2010-ngrams/word/over99/3gms/3gm-0000.xz
	wget https://s3-ap-northeast-1.amazonaws.com/nwc2010-ngrams/word/over99/3gms/3gm-0001.xz
	wget https://s3-ap-northeast-1.amazonaws.com/nwc2010-ngrams/word/over99/3gms/3gm-0002.xz
	wget https://s3-ap-northeast-1.amazonaws.com/nwc2010-ngrams/word/over99/3gms/3gm-0003.xz
	wget https://s3-ap-northeast-1.amazonaws.com/nwc2010-ngrams/word/over99/3gms/3gm-0004.xz
	wget https://s3-ap-northeast-1.amazonaws.com/nwc2010-ngrams/word/over99/3gms/3gm-0005.xz
	;;
    *)
	# 出現頻度1000以上（デフォルト）
	wget https://s3-ap-northeast-1.amazonaws.com/nwc2010-ngrams/word/over999/3gms/3gm-0000.xz
	;;
esac

xz -d -c 3gm-*.xz > 3gm

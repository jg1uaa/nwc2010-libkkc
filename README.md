nwc2010-libkkc
====

libkkc-dataのdata.arpaを[日本語ウェブコーパス2010](https://www.s-yata.jp/corpus/nwc2010/)で作成する試み

## はじめに

2023年において個人が自由に利用可能な言語資源は、[日本語ウェブコーパス2010](https://www.s-yata.jp/corpus/nwc2010/)のみである。[Web日本語Nグラム](https://www.gsk.or.jp/catalog/gsk2007-c/)(Google)や[Yahoo!知恵袋データ](https://www.nii.ac.jp/dsc/idr/yahoo/chiebkr3/Y_chiebukuro.html)等、多くの言語資源においては利用に際し多額の支払いを要したり、研究目的に限定されている（学術機関への所属が必要）という制約があるため、OSS開発目的への考慮が何一つされていないという非常に由々しき問題が存在していることを記す。

## 目的

[libkkc](https://github.com/ueno/libkkc/)の辞書データを生成する際に使用するdata.arpaは、[ngram-format(5)](http://www.speech.sri.com/projects/srilm/manpages/ngram.1.html)に従ったN-gram言語モデルのデータである。[Next Generation Input Methods](https://deisui.org/~ueno/junk/guadec-2014.pdf)(PDF)のスライドにはWikipedia日本語版から得た十万文とYahoo!知恵袋から得た二万文を用いたとあるものの、どのような手法でdata.arpaを作成したかについては記載が無いどころか、検索しても見当たらない。

何の後ろ盾もない一個人がこのようなデータを作成することができるかどうかを試すことがこのプロジェクトを立ち上げた理由であり、最後に目指すところは日本語ウェブコーパス2010で作成したdata.arpaを用いた辞書でlibkkcを利用することである。

## 手法

言語資源は目的の項に記した通り、日本語ウェブコーパス2010を使用した。これはWeb日本語Nグラムのデータ形式（[README](https://www.gsk.or.jp/files/catalog/GSK2007-C/GSK2007C_README.utf8.txt)に記載されている、これ以降はGoogle Web N-gram形式と記す）に準拠しているとあるが、完全に準拠している訳ではない。語彙一覧(1gms/vocab)に関してはファイル名を変更し、インデックス(Ngms/Ngm.idx)を別途作成する必要がある。またこの修正と[いくつかのファイルを用意する](http://www.speech.sri.com/projects/srilm/manpages/srilm-faq.7.html)ことで[SRILM](http://www.speech.sri.com/projects/srilm/)から言語モデルを利用することが可能になるものの、SRILM自体はGoogle Web N-gram形式からARPA形式への変換には[対応しない](https://mailman.speech.sri.com/pipermail/srilm-user/2009q2/000751.html)。そのため、ARPA形式への変換は[IRSTLM](https://github.com/irstlm-team/irstlm)を使用する。これは[A tutorial on the IRSTLM library](https://citeseerx.ist.psu.edu/document?repid=rep1&type=pdf&doi=2ef9472ecfc0c3d48a0f9b2a031916c703fb1140)にあるように、Google Web N-gram形式の3-gramデータから2-gram, 1-gramを生成し、かつARPA形式にまとめることができるもので、今回の目的に適っている。

libkkcで使用するdata.arpaは単なる言語モデルではなくかな漢字変換辞書として用いられるため、単語一覧には読み仮名を付加する必要がある。日本語ウェブコーパス2010においては形態素解析に[MeCab](https://taku910.github.io/mecab/)を使用しているためこれによる読み仮名の付加を行うのが適当ではあるが、今回は手軽に使用できるという理由により[kakasi](http://kakasi.namazu.org/index.html.ja)を用いている。kakasiの辞書はEUCで記述されているため、変換候補となる単語がEUCで表現できない場合は単語一覧からこれを取り除いている。また、変換を要しない英単語等も同様に除いている。

## 作成物

### fetch.sh

日本語ウェブコーパス2010から、data.arpaの作成に必要となるN-gramコーパス/形態素N-gram/頻度1000以上(3-gram)をダウンロードし、圧縮を解く処理を行う。
実行にはbash, wget, xzが必要。

### src/calc

展開された3-gramデータから不要なものを取り除き、読み仮名を付加するツール。
makeでビルドできるが、Debianであればlibkakasi2-devが必要。

### make.sh

src/calcおよびIRSTLMを用い、無圧縮の日本語ウェブコーパス2010からlibkkc用のdata.arpaを作成するための一連の手順を記している。
Debianであればapt-get install irstlmでIRSTLMをインストールできる。その後、fetch.sh→src/calcのmake→make.shの順に実行する。

## 結果

現時点において、得られたdata.arpaを使用してもlibkkc-dataによる辞書の作成は行えない。sortlm.pyの修正が必要なことが分かっているが、これに加えてlibkkc本体の修正を行わないとsegmentation faultを起こすことも分かっている。また、頻度1000以上の3-gramデータを使用しているが、これでもデータ数が多いのか環境次第ではlibkkc-dataによる辞書データの作成においてメモリ不足が発生してしまう。これを回避するため、出現頻度1800以上の3-gramを使用するよう制限している（生成されたdata.arpaの1-gramのエントリ数とのバランスを考え、この出現頻度とした）。


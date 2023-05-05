# haco3_JS_X


「ハコさん」というキャラが大活躍する（はずの）esp32で動くミニゲーム開発プラットフォームです。今のところ開発者の方向けコードの共有用ベータ版です。

ina_aniさんのo-bakoをlovyanGFXに移植し、タッチパネル対応にするため自作ライブラリDentaroUI（同梱のカスタム仕様）を追加したのがhaco3です。

スプライトも自作です。LovyanGFXもo-bakoも素晴らしいプロジェクトで、この２つをベースにhaco3はできています。

https://inajob.github.io/o-bako-simulator/index.html

o-bakoのプログラムに互換性があります。（つまり、こちらのサイトで開発したゲームは動かせます）

JavaScript言語(Duktape)とLua5.3が動いていて、

JavaScriptを充実させていくつもりですが、JavaScript & Luaのどちらでも開発ができます。（data内のサンプルゲーム.jsと.luaを参照してください。）

下のサイトで紹介されている、2432S028Rというタッチパネルでうごく設定を公開しています。

https://lang-ship.com/blog/work/esp32-2432s028r-1/

platformIOプロジェクトのフォルダをコピーした「コード」だけをあげています。

ビルドオプションの設定などが必要なため、インストールはplatformIOでしかできません。（Arduino不可）

使い方は以下の通りです。

ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー

＜使い方＞

まず、基板に対応させる

/lib/LovyanGFX_DentaroUI_haco3/src/TouchBtn.hppの15行目にある

LGFX_ESP32_custom_2432S028R.hppにしてください。（違うものになっている場合があります。）

カスタム設定で、自作基板のピンアサインなどにカスタムもできます。

１，platformIOでesp32のプロジェクトを作る

２，ダウンロードした「platformio.ini」「src」「lib」「data」をプロジェクトフォルダに置き換えコピー

３，platformio.iniのCOMポートを書き込み可能なポートに書き換える（Device機能を使って調べられる）

４，ゲームに必要なデータは「data」にはいっているので、SPIFFS領域にアップする（Upload File System Image機能を使ってアップできる）

５、プロジェクトをコンパイルする

 ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー

platformIOのインストールができたら、

dataフォルダ内のinitの中身がゲームの本体になります。それぞれを編集してゲーム制作ができます。

　　・/data/init/initspr.pngが、すべてのゲームが参照しているゲームスプライトです。ドット絵を描いて編集できます。

　　・/data/init/main.luaはホーム画面そのものプログラムです。lua言語で開発ができます。

　　・/data/init/game.jsonは通常編集しません。
  
 ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー

タッチボタンについて

定数CTRLBTNNUMで定義していて、デフォルトでは5つのタッチボタンが右側に配置されています。

ゲーム選択画面（＝ホーム）では下のような機能を割り付けてあります。

[0]ホーム（ハコさんロゴ部分）

[1]決定ボタン

[2]タブ移動

[3]↑

[4]↓

１～４はゲームによって役割が違います。

fileのうち、拡張子がjsまたは、luaのファイルを選ぶと、ゲームがスタートします。

一番上のハコさんのロゴを押すとゲームが終了し、リセットされます。

 ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
 
タッチキャリブレーションについて
 
/data/init/param/caldata.txtというファイルにタッチパネルのキャリブレーションデータが書き込まれますが、

デフォルトではデータがないので、立ち上げ時に表示される4角をタッチしてキャリブレーションをしてください。

もう一度キャリブレーションをやり直したい場合は、

/data/init/param/caldata.txtを開くと、txtファイルを開く前に、4角をタッチしてキャリブレーションする画面が出ます。

この名前以外のtxtファイルは普通に開きます。

ロゴ（の周辺）をクリックすると、ホームに戻ります。

 ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
 
 個別のゲームについて
 
 /dataの中に入っています。
 
 /data/asteroid/main.js
 
 小惑星のスピードを変えて、衝突を避けるゲームです。
 
 jsを使って開発したい場合は参考にしてみてください。
 
 スプライトはメモリ節約のため、init/initspr.pngを参照しています。
 
 /data/drawing/main.js
  
 タッチパネルをつかって、ドローイングをするサンプルゲームです。
  
 オセロなどのパズルに応用できると思います。
  
 /data/init/param/caldata.txt
  
 タッチキャリブレーションを行います
  
 /data/init/txt/main.js
  
 txtビュワです。いまは1行だけ読めます。長くても1行にすれば読めます。
  
 /data/init/png/main.js
  
 pngビュワです。128×120ピクセルが表示限界です。

 ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
 
初回に立ち上げるゲームの設定

/data/init/param/firstAppName.txtに立ち上げたいゲームのパスを一行で書いておくと、それが優先して立ち上がります。

デフォルトでは、

/init/main.lua

というゲーム選択のための「ホーム」になっていますが、デバッグの際に設定すると、効率的に作業ができます。


 ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
 
スプライトについて

![initspr](https://user-images.githubusercontent.com/77978725/215836664-73583fb5-a69d-4c90-aea8-afb5fa87b1df.png)

64×128のpng画像から任意の大きさをspr()で切り出して使います。

/init/initspi.pngという基本スプライトが入っており、メモリ節約のために、デフォルトではすべてのゲームがそちらを参照しています。

サンプルのキャラスプライトは8×8で切り出してから、2倍表示モードでは16×16で表示しています。


 ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
 
 色について
  
  基本色は、color(0~15)までの16色です。pico-8の配色と一緒なので、pico-8のスプライトリソースがあれば使えます。

![colorNo](https://user-images.githubusercontent.com/77978725/215329561-b157ce6d-9d23-4f74-b0eb-3451bcc89fd8.png)

ホントはcolor(r,g,b)でフルカラーが使えるのはヒミツです。
  
  
 ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
 
 （2023.5.3)

ゲーム選択時に、「.lua」「.js」の拡張子と「caldata.txt」のみを表示しメモリ使用量を抑えることで、落ちなくなりました。

min,max関数を追加しました。

flippy birdと3Dのサンプルを追加しました。(ほぼchatGPTで生成）

（2023.3.7)

music(0)で、下のmymelo2で書き出したmymelo.hのBGMが流せるようになりました。（いまのところ1曲のみですが、「テキスト音楽サクラ」で作曲ができます）sfx(soundNo)で効果音用の波形を選べるようにしました。/data/init/sound/wave_sfx.txtに波形データが256*8個入っています。

https://koyama.verse.jp/elecraft/mymelo/
「テキスト音楽サクラ」(http://oto.chu.jp)

（2023.3.1)

haco8にmget()実装。fset()とfget()も実装。デフォルトのスプライト属性を(/init/param/sprbits.txt)から8bitで設定できるようにしました。

スプライトにアニメーション領域64*64を追加しました。

（2023.2.6)

wifiモードでの起動を外部設定ファイル(/init/param/modeset.txt)からできるようにしました。

webブラウザからの書き込みができるようになりました。

（2023.2.5)

mget関数を追加しました。

特定の色でpngファイルで絵を描くとそのままマップになるようにしました。

pngマップの色に対応するスプライトの設定を外部ファイルにしました。

（2023.2.4)
LUAでpico8に近い文法でかけるようになりました。

spr関数を追加しました。

map関数を追加しました。

（2023.2.1)

タッチしやすい2倍表示(20FPS程度)から、高速モード(30〜60FPS)に1行で切り替えられるようにしました。（src/main.cppの以下の部分）

int outputMode = FAST_MODE;//50FPS程度128*128 速いけど小さい画面　速度が必要なモード

int outputMode = WIDE_MODE;//20FPS程度240*240 遅いけれどタッチしやすい画面　パズルなど

o-bako のシーケンサー「sound」lua版をjsに移植しました。

JS,LUAともにボタンの挙動を修正しました。

初回に立ち上げるゲームを外部ファイル(init/firstAppName.txt)に書いておけるようになりました。

pngビュワ（.png)実装しました。

一行ビュワ（.txt)実装しました。

タッチキャリブレーションを実装しました。(caldata.txtを開いてください)

タッチキャリブレーションデータのSPIFFSへの保存ができるようになりました。

caldata.txtにデータがないときのみタッチキャリブレーションに自動で切り替えられるようになりました。

（2023.1.29)

タッチパネルに対応しました。

/data/drawing/main.jsにサンプルがあります。

tp(0)でx座標tp(1)でy座標を取得できます。

tbtns()で、タッチボタンを生成、グリッド配置、ボタンIDの取得ができます。

rwtb(）でタッチボタンが保持しているbool値を操作したのち、取得できます。

円形描画に対応しました。

fillCircle()で円が描けます。

タッチボタンの経過フレーム数をリターンするようにしました。

（2022.9.4)

色番号に対応しました。color(n)で色指定ができます。

（2022.9.1)

３つの物理ボタンに対応しました。

ディレクトリ名から日本語を削除しました。ディレクトリ構造を修正しました。

（2022.8.31)

Wifi機能を切ってメモリ確保しました。

RAM:   [=         ]   9.4% (used 30756 bytes from 327680 bytes)

Flash: [=====     ]  53.2% (used 697929 bytes from 1310720 bytes)

（2022.8.29)

サンプルスプライトを追加しました。


  ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー

ゲームエンジンではなく、JSでのゲームサンプルを作っていただける方は、luaを使ったWebブラウザでゲーム開発ができるhaco3simによる開発も楽しいかもしれません。

https://github.com/dentaro/haco3sim/

ゲームエンジン、ゲームサンプル両方から「ハコさん」を大活躍させていただける方は、ぜひ一緒に開発していただけませんか？

プログラミングできないよという方でも使えそうなドット絵作ってみたなど、ご協力の形は楽しんでいただける方なら、なんでもOKです！

ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー

2432S028R以外のオプション情報（難しい）

一応、物理ボタンにも対応しています。

ui.setupPhBtns( screen, 33, 39, 36);
のように設定できます。

ボタンの数が増えるので、定数CTRLBTNNUMを8にする

バグありありです。ご了承の上、遊んでみてください。

（少しずつ改善していきます）

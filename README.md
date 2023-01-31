# haco3_JS_X


「ハコさん」というキャラが大活躍する（はずの）esp32で動くミニゲーム開発プラットフォームです。今のところ開発者の方向けコードの共有用ベータ版です。

ina_aniさんのo-bakoをlovyanGFXに移植し、タッチパネル対応にするため自作ライブラリDentaroUI（同梱のカスタム仕様）を追加したのがhaco3です。

スプライトも自作です。LovyanGFXもo-bakoも素晴らしいプロジェクトで、この２つをベースにhaco3はできています。

https://inajob.github.io/o-bako-simulator/index.html

JavaScript言語(Duktape)とLua5.3が動いていて、それぞれの言語で開発できます。

JavaScript & Lua版です。どちらでも開発できるようになりました。（data内のサンプルゲーム.jsと.luaを参照してください。）

下のサイトで紹介されている、2432S028Rというタッチパネルでうごく設定を公開しています。

https://lang-ship.com/blog/work/esp32-2432s028r-1/

platformIOプロジェクトのフォルダをコピーした「コード」だけをあげています。

ビルドオプションの設定などが必要なため、インストールはplatformIOでしかできません。（Arduino不可）

使い方は以下の通りです。

ーーーーーーーーーーーーーーーーーーー＜使い方＞ーーーーーーーーーーーーーーーーーーー

１，platformIOでesp32のプロジェクトを作る

２，ダウンロードした「platformio.ini」「src」「lib」「data」をプロジェクトフォルダに置き換えコピー

３，platformio.iniのCOMポートを書き込み可能なポートに書き換える（Device機能を使って調べられる）

４，ゲームに必要なデータは「data」にはいっているので、SPIFFS領域にアップする（Upload File System Image機能を使ってアップできる）

５、プロジェクトをコンパイルする

 ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー

platformIOのインストールができたら、

dataフォルダ内のinitの中身がゲームの本体になります。それぞれを編集してゲーム制作ができます。

　　・init/initspr.pngが、すべてのゲームが参照しているゲームスプライトです。ドット絵を描いて編集できます。

　　・init/main.luaはホーム画面そのものプログラムです。lua言語で開発ができます。

　　・init/game.jsonは通常編集しません。
  
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
 
/init/caldata.txtというファイルにタッチパネルのキャリブレーションデータが書き込まれますが、

デフォルトではデータがないので、立ち上げ時に表示される4角をタッチしてキャリブレーションをしてください。

もう一度キャリブレーションをやり直したい場合は、

/init/caldata.txtを開くと、txtファイルを開く前に、4角をタッチしてキャリブレーションする画面が出ます。

この名前以外のtxtファイルは普通に開きます。

ロゴ（の周辺）をクリックすると、ホームに戻ります。

 ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
 
 個別のゲームについて
 
 data/asteroid
 
 小惑星のスピードを変えて、衝突を避けるゲームです。
 
 jsを使って開発したい場合は参考にしてみてください。
 
 スプライトはメモリ節約のため、init/initspr.pngを参照しています。
 
  data/drawing
  
  タッチパネルをつかって、ドローイングをするサンプルゲームです。
  
  オセロなどのパズルに応用できると思います。
  
 
 ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
 
 色について
  
  基本色は、color(0~15)までの16色です。pico-8の配色と一緒なので、pico-8のスプライトリソースがあれば使えます。

![colorNo](https://user-images.githubusercontent.com/77978725/215329561-b157ce6d-9d23-4f74-b0eb-3451bcc89fd8.png)

ホントはcolor(r,g,b)でフルカラーが使えるのはヒミツです。
  
  
 ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
  
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

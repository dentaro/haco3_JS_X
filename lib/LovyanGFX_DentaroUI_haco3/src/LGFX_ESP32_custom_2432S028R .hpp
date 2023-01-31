// #pragma once

// // #define LGFX_USE_V1

// // #include <LovyanGFX.hpp>


// // ESP32でLovyanGFXを独自設定で利用する場合の設定例

// /*
// このファイルを複製し、新しい名前を付けて、環境に合わせて設定内容を変更してください。
// 作成したファイルをユーザープログラムからincludeすることで利用可能になります。
// 複製したファイルはライブラリのlgfx_userフォルダに置いて利用しても構いませんが、
// その場合はライブラリのアップデート時に削除される可能性があるのでご注意ください。
// 安全に運用したい場合はバックアップを作成しておくか、ユーザープロジェクトのフォルダに置いてください。
// //*/


// /// 独自の設定を行うクラスを、LGFX_Deviceから派生して作成します。
// class LGFX : public lgfx::LGFX_Device
// {
//     lgfx::Panel_ILI9341 _panel_instance;
//     lgfx::Bus_SPI       _bus_instance;
//     lgfx::Light_PWM     _light_instance;
//     lgfx::Touch_XPT2046 _touch_instance;
//   public:
//     LGFX(void)
//     {
//       {
//         auto cfg = _bus_instance.config();

//         // SPIバスの設定
//         cfg.spi_host         = VSPI_HOST;
//         cfg.spi_mode         = 0;
//         cfg.freq_write       = 40000000;
//         cfg.freq_read        = 16000000;
//         cfg.spi_3wire        = true;
//         cfg.use_lock         = true;
//         cfg.dma_channel      = SPI_DMA_CH_AUTO;
//         cfg.pin_sclk         = 14; // 変更
//         cfg.pin_mosi         = 13; // 変更
//         cfg.pin_miso         = 12; // 変更
//         cfg.pin_dc           =  2; // 変更

//         _bus_instance.config(cfg);
//         _panel_instance.setBus(&_bus_instance);
//       }

//      {
//         auto cfg = _panel_instance.config();

//         cfg.pin_cs           =    15; // 変更
//         cfg.pin_rst          =    -1; // 変更
//         cfg.pin_busy         =    -1; // 変更

//         cfg.panel_width      =   240;
//         cfg.panel_height     =   320;
//         cfg.offset_x         =     0;
//         cfg.offset_y         =     0;
//         cfg.offset_rotation  =     0;
//         cfg.dummy_read_pixel =     8;
//         cfg.dummy_read_bits  =     1;
//         cfg.readable         =  true;
//         cfg.invert           = false;
//         cfg.rgb_order        = false;
//         cfg.dlen_16bit       = false;
//         cfg.bus_shared       = false; // 変更

//         _panel_instance.config(cfg);
//       }
//      {
//         auto cfg = _light_instance.config();

//         cfg.pin_bl = 21;              // 変更
//         cfg.invert = false;
//         cfg.freq   = 44100;           
//         cfg.pwm_channel = 7;

//         _light_instance.config(cfg);
//         _panel_instance.setLight(&_light_instance);
//       }

//       {
//         auto cfg = _touch_instance.config();

//         cfg.x_min      =  300;  // 変更
//         cfg.x_max      = 3900;  // 変更
//         cfg.y_min      = 3700;  // 変更
//         cfg.y_max      =  200;  // 変更
//         cfg.pin_int    = -1;    // 変更
//         cfg.bus_shared = false; // 変更
//         cfg.offset_rotation = 0;

//         cfg.spi_host = HSPI_HOST; // 変更
//         cfg.freq = 1000000;
//         cfg.pin_sclk = 25;        // 変更
//         cfg.pin_mosi = 32;        // 変更
//         cfg.pin_miso = 39;        // 変更
//         cfg.pin_cs   = 33;        // 変更

//         _touch_instance.config(cfg);
//         _panel_instance.setTouch(&_touch_instance);
//       }
//     setPanel(&_panel_instance); // 使用するパネルをセットします。
//   }
// };
// // class LGFX : public lgfx::LGFX_Device
// // {
// //     lgfx::Panel_ILI9341 _panel_instance;
// //     lgfx::Bus_SPI       _bus_instance;
// //     lgfx::Light_PWM     _light_instance;
// //     lgfx::Touch_XPT2046 _touch_instance;
// //   public:
// //     LGFX(void)
// //     {
// //       {
// //         auto cfg = _bus_instance.config();

// //         // SPIバスの設定
// //         cfg.spi_host         = VSPI_HOST;
// //         cfg.spi_mode         = 0;
// //         cfg.freq_write       = 40000000;
// //         cfg.freq_read        = 16000000;
// //         cfg.spi_3wire        = true;
// //         cfg.use_lock         = true;
// //         cfg.dma_channel      = SPI_DMA_CH_AUTO;
// //         cfg.pin_sclk         = 14; // 変更
// //         cfg.pin_mosi         = 13; // 変更
// //         cfg.pin_miso         = 12; // 変更
// //         cfg.pin_dc           =  2; // 変更

// //         _bus_instance.config(cfg);
// //         _panel_instance.setBus(&_bus_instance);
// //       }

// //       {
// //         auto cfg = _panel_instance.config();

// //         cfg.pin_cs           =    15; // 変更
// //         cfg.pin_rst          =    -1; // 変更
// //         cfg.pin_busy         =    -1; // 変更

// //         cfg.panel_width      =   240;
// //         cfg.panel_height     =   320;
// //         cfg.offset_x         =     0;
// //         cfg.offset_y         =     0;
// //         cfg.offset_rotation  =     0;
// //         cfg.dummy_read_pixel =     8;
// //         cfg.dummy_read_bits  =     1;
// //         cfg.readable         =  true;
// //         cfg.invert           = false;
// //         cfg.rgb_order        = false;
// //         cfg.dlen_16bit       = false;
// //         cfg.bus_shared       = false; // 変更

// //         _panel_instance.config(cfg);
// //       }

// //       {
// //         auto cfg = _light_instance.config();

// //         cfg.pin_bl = 21;              // 変更
// //         cfg.invert = false;
// //         cfg.freq   = 44100;           
// //         cfg.pwm_channel = 7;

// //         _light_instance.config(cfg);
// //         _panel_instance.setLight(&_light_instance);
// //       }

// //       {
// //         auto cfg = _touch_instance.config();

// //         cfg.x_min      =  300;  // 変更
// //         cfg.x_max      = 3900;  // 変更
// //         cfg.y_min      = 3700;  // 変更
// //         cfg.y_max      =  200;  // 変更
// //         cfg.pin_int    = -1;    // 変更
// //         cfg.bus_shared = false; // 変更
// //         cfg.offset_rotation = 0;

// //         cfg.spi_host = HSPI_HOST; // 変更
// //         cfg.freq = 1000000;
// //         cfg.pin_sclk = 25;        // 変更
// //         cfg.pin_mosi = 32;        // 変更
// //         cfg.pin_miso = 39;        // 変更
// //         cfg.pin_cs   = 33;        // 変更

// //         _touch_instance.config(cfg);
// //         _panel_instance.setTouch(&_touch_instance);
// //       }

// //       setPanel(&_panel_instance);
// //     }
// // };


// // #pragma once

// // // #define LGFX_USE_V1

// // // #include <LovyanGFX.hpp>

// // class LGFX : public lgfx::LGFX_Device
// // {
// //   // lgfx::Panel_ILI9341 _panel_instance;
// // lgfx::Panel_GC9A01      _panel_instance;
// // lgfx::Bus_SPI       _bus_instance;   // SPIバスのインスタンス
// // // lgfx::Bus_I2C       _bus_instance;   // I2Cバスのインスタンス (ESP32のみ)
// // //lgfx::Bus_Parallel8 _bus_instance;   // 8ビットパラレルバスのインスタンス (ESP32のみ)

// // // バックライト制御が可能な場合はインスタンスを用意します。(必要なければ削除)
// //   // lgfx::Light_PWM     _light_instance;
// // // タッチスクリーンの型にあったインスタンスを用意します。(必要なければ削除)
// //   // lgfx::Touch_XPT2046     _touch_instance;

// // public:

// //   // コンストラクタを作成し、ここで各種設定を行います。
// //   // クラス名を変更した場合はコンストラクタも同じ名前を指定してください。
// //   LGFX(void)
// //   {
// //     { // バス制御の設定を行います。
// //       auto cfg = _bus_instance.config();    // バス設定用の構造体を取得します。

// // // SPIバスの設定
// //       cfg.spi_host = HSPI_HOST;     // 使用するSPIを選択  (VSPI_HOST or HSPI_HOST)
// //       cfg.spi_mode = 3;             // SPI通信モードを設定 (0 ~ 3)
// //       cfg.freq_write = 40000000;//40000000;    // 送信時のSPIクロック (最大80MHz, 80MHzを整数で割った値に丸められます)
// //       cfg.freq_read  = 16000000;    // 受信時のSPIクロック
// //       cfg.spi_3wire  = true;        // 受信をMOSIピンで行う場合はtrueを設定
// //       cfg.use_lock   = true;        // トランザクションロックを使用する場合はtrueを設定
// //       cfg.dma_channel = SPI_DMA_CH_AUTO;          // Set the DMA channel (1 or 2. 0=disable)   使用するDMAチャンネルを設定 (0=DMA不使用)
// //       cfg.pin_sclk = 14;            // SPIのSCLKピン番号を設定
// //       cfg.pin_mosi = 13;            // SPIのMOSIピン番号を設定
// //       cfg.pin_miso = -1;            // SPIのMISOピン番号を設定 (-1 = disable)
// //       cfg.pin_dc   = 4;            // SPIのD/Cピン番号を設定  (-1 = disable)
// //      // SDカードと共通のSPIバスを使う場合、MISOは省略せず必ず設定してください。
// // //*/
// // /*
// // // I2Cバスの設定//すごい電卓
// //       cfg.i2c_port    = 0;          // 使用するI2Cポートを選択 (0 or 1)
// //       cfg.freq_write  = 400000;     // 送信時のクロック
// //       cfg.freq_read   = 400000;     // 受信時のクロック
// //       cfg.pin_sda     = 21;         // SDAを接続しているピン番号
// //       cfg.pin_scl     = 22;         // SCLを接続しているピン番号
// //       cfg.i2c_addr    = 0x3C;       // I2Cデバイスのアドレス
// // //*/

// // // I2Cバスの設定//white2 2インチ改造版
// //       // cfg.i2c_port    = 0;          // 使用するI2Cポートを選択 (0 or 1)
// //       // cfg.freq_write  = 400000;     // 送信時のクロック
// //       // cfg.freq_read   = 400000;     // 受信時のクロック
// //       // cfg.pin_sda     = 13;         // SDAを接続しているピン番号
// //       // cfg.pin_scl     = 14;         // SCLを接続しているピン番号
// //       // cfg.i2c_addr    = 0x3C;       // I2Cデバイスのアドレス
// // //

// //       _bus_instance.config(cfg);    // 設定値をバスに反映します。
// //       _panel_instance.setBus(&_bus_instance);      // バスをパネルにセットします。
// //     }

// //     { // 表示パネル制御の設定を行います。
// //       auto cfg = _panel_instance.config();    // 表示パネル設定用の構造体を取得します。

// //       cfg.pin_cs           =    15;  // CSが接続されているピン番号   (-1 = disable)
// //       cfg.pin_rst          =    -1;  // RSTが接続されているピン番号  (-1 = disable)
// //       cfg.pin_busy         =    -1;  // BUSYが接続されているピン番号 (-1 = disable)

// //       // ※ 以下の設定値はパネル毎に一般的な初期値が設定されていますので、不明な項目はコメントアウトして試してみてください。

// //       cfg.memory_width     =   240;  // ドライバICがサポートしている最大の幅
// //       cfg.memory_height    =   240;  // ドライバICがサポートしている最大の高さ
// //       cfg.panel_width      =   240;  // 実際に表示可能な幅
// //       cfg.panel_height     =   240;  // 実際に表示可能な高さ
// //       cfg.offset_x         =     0;  // パネルのX方向オフセット量
// //       cfg.offset_y         =     0;  // パネルのY方向オフセット量
// //       cfg.offset_rotation  =     0;  // 回転方向の値のオフセット 0~7 (4~7は上下反転)
// //       cfg.dummy_read_pixel =     8;  // ピクセル読出し前のダミーリードのビット数
// //       cfg.dummy_read_bits  =     1;  // ピクセル以外のデータ読出し前のダミーリードのビット数
// //       cfg.readable         =  false;  // データ読出しが可能な場合 trueに設定
// //       cfg.invert           = false;  // パネルの明暗が反転してしまう場合 trueに設定
// //       cfg.rgb_order        = false;  // パネルの赤と青が入れ替わってしまう場合 trueに設定
// //       cfg.dlen_16bit       = false;  // データ長を16bit単位で送信するパネルの場合 trueに設定
// //       cfg.bus_shared       = false;  // SDカードとバスを共有している場合 trueに設定(drawJpgFile等でバス制御を行います)

// //       _panel_instance.config(cfg);
// //     }

// //     setPanel(&_panel_instance); // 使用するパネルをセットします。
// //   }
// // };
# bve-plugin-bridge

Bve 5 用のプラグインを BVE 6 で使うための変換プラグインです。

## 使い方

Bve 5 用のプラグインが含まれている既存の車両データに bve-plugin-bridge を導入する方法を説明します。

なお、DetailManager.dll で複数のプラグインを同時に使用している場合は、DetailManager.dll に対してだけ下記の手順を行ってください。全部の dll ファイルにそれぞれ bve-plugin-bridge を導入する必要はありません。

1. [リリースページ](https://github.com/magicant/bve-plugin-bridge/releases)から bve-plugin-bridge-v2.zip をダウンロードしてください。ZIP ファイルの中に backend.exe と frontend.dll が入っています。
1. 車両の既存のプラグインがあるフォルダーに backend.exe と frontend.dll を入れます。
1. 以下のようにファイル名を変更します。
   - `backend.exe` のファイル名の拡張子以外の部分を、既存のプラグインに合わせます。
      - 例えば既存のプラグイン名が `Ats.dll` ならば、 `backend.exe` を `Ats.exe` に変えます。
   - `frontend.dll` のファイル名を、既存のプラグインの拡張子の直前に `_` を付けたものに変えます。
      - 例えば既存のプラグイン名が `Ats.dll` ならば、 `frontend.dll` を `Ats_.dll` に変えます。
   - 既存のプラグインのファイル名は変えません。
1. [車両ファイル](http://bvets.net/jp/edit/formats/vehicle/vehicle.html) (普通は Vehicle.txt のような名前が付けられているファイル) を開いて、以下のように変更し、保存します。
   - ファイル先頭行のバージョン番号が `2.00` になっていない場合は、 `2.00` にします。最初から `2.00` になっているならばそのままで構いません。
   - `Ats = ...` と書いてあるところを `Ats32 = ...` に変更します。最初から `Ats32` になっているならばそのままで構いません。
   - `Ats64 = ...` の行を追加します。 `=` の右側には先ほど用意した 名前に `_` が付いている方のファイルのパスを記入します。
   - まとめると、以下のようになるはずです。(ファイル名は実際のものに合わせて変えてください。)
     ```
     BveTs Vehicle 2.00
     (中略)
     Ats32 = plugin/Ats.dll
     Ats64 = plugin/Ats_.dll
     (後略)
     ```

**注意!** ファイル名が間違っていると正常に動作しません。追加する二つのファイルは既存のプラグインと同じフォルダーに置く必要があります。よく確かめてください。

## 解説

`frontend.dll` は BVE 6 から呼び出される 64 ビットのプラグインであり、内部で `backend.exe` を起動し、無名パイプを使って相互に通信します。
`backend.exe` は Bve 5 の代わりに既存のプラグインを呼び出す 32 ビットアプリケーションです。

主要なソースコードファイルは以下の三つです。

- [`frontend/dllmain.cpp`](frontend/dllmain.cpp): `frontend.dll` の実装。
- [`backend/exemain.cpp`](backend/exemain.cpp): `backend.exe` の実装。
- [`encoder.h`](encoder.h): `ATS_VEHICLESPEC` などのデータ構造と文字列の間を相互に変換する処理。 `frontend/dllmain.cpp` と `backend/exemain.cpp` の両方で使います。

## 既知の課題

解説に書いた通り、bve-plugin-bridge を使用するとプラグインが BVE 本体とは別のアプリケーションプロセスで動作するようになります。したがって、BVE 本体と同じプロセスで動作する前提の作りになっているプラグインは正常に動作しない可能性があります。

現在、正常に動作しないことが分かっているものは

- FWCH さんの MotorMaster.dll

(その他、情報がありましたらご連絡ください)

## ライセンス

The Unlicense (パブリックドメイン)

## 質問・問い合わせ

- [ツイッター](https://twitter.com/tnacigam)で
- [メール](mailto:magicant@wonderwand.net)で
- [Issues](https://github.com/magicant/bve-plugin-bridge/issues/new) で

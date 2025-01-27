# TripleBufferSystem
## 概要
RingBufferを使いやすくし，クリティカルセクションを短くする

## 注意事項
バッファがあふれたらもう使い物にならん．バッファを作り直してくれ

## License
MIT License

Copyright © 2025 ygdstmidn

see [LICENSE](./LICENSE)

## 使い方

基本的にRingBufferと同じ
[RingBuffer](./RingBuffer/README.md)

### 初期化
```c
TripleBufferSystem tbs;
```
```c
TripleBufferSystem_Init(&tbs, 256);
```
Initしたときにmallocしてメモリ確保に失敗すると1が返ってくる．
```c
TripleBufferSystem_setFunc(&tbs, beforeSwap, afterSwap);
TripleBufferSystem_setFunc(&tbs, disable_irq, enable_irq);
```
Swapの前後に実行する関数を登録する\
例として，割り込みを有効/無効にする関数を登録する\
HALで扱う場合，割り込みを有効/無効にするだけでなく，すでに存在する割り込みを中止して新しく割り込みを発生させる必要がある\
※これをInitの後に呼び出してからReadを使うこと

### Put
```c
TripleBufferSystem_Put(&tbs, data);
```
Putします．はい

### NextWriteBuffer及びHeadMove
```c
*TripleBufferSystem_NextWriteBuffer(tbs) = data;
TripleBufferSystem_HeadMove(&tbs);
```
```c
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  TripleBufferSystem_HeadMove(&tbs);
  HAL_UART_Receive_IT(&huart2, TripleBufferSystem_NextWriteBuffer(tbs), 1);
}
```
HAL_UART_RxCpltCallbackのような関数で使う

### Get
```c
#define BufferSize 256
unsigned char dataBuffer[BufferSize];
const size_t dataSize = TripleBufferSystem_Get(&tbs, dataBuffer, BufferSize);
```
Getします\
`dataBuffer`にデータが入ります\
`dataSize`に読み取ったサイズが入ります

### Swap
```c
TripleBufferSystem_Swap(&tbs);
```
書き込みバッファを入れ替えます(高速です)\
実際には，Swapしている最中にPutされるといけないので，割り込み禁止にしてから使います

### Transfer
```c
const int transferState = TripleBufferSystem_Transfer(&tbs);
```
書き込みバッファから読み取りバッファにデータを移します\
`transferState`が
- 0: 成功
- 1: 書き込みバッファがいっぱい(激ヤバ)
- 2: 読み取りバッファがいっぱいで書き込みバッファにデータが残っている

1のときはバッファを作り直してくれ\
2のときはGetした後，Transferする．GetしてTransferする前にSwapしてはならない(データの順番が壊れる)

### Read
```c
#define BufferSize 256
unsigned char dataBuffer[BufferSize];
int readError;
const size_t dataSize = TripleBufferSystem_Read(&tbs, dataBuffer, BufferSize, &readError);
```
SwapとTransferとGetをまとめた関数\
setFuncで必要な関数を登録していることが前提\
`dataBuffer`にデータが入ります\
`dataSize`に読み取ったサイズが入ります\
`readError`が
- 0: 成功
- 1: 書き込みバッファがいっぱい(激ヤバ)
- 2: 読み取りバッファがいっぱいで書き込みバッファにデータが残っている

readErrorが1のときは返り値も0になります．\
Readしか使わない場合，2はあまり考慮しなくてもいいかも

### メモリ開放
```c
TripleBufferSystem_Destroy(&tbs);
```
メモリは開放すること

## TripleBufferSystemClass
```c++
TripleBufferSystemClass tbs(256);
tbs.init();
tbs.setFunc(beforeSwap, afterSwap);
tbs.put(data);
*TripleBufferSystem_NextWriteBuffer(tbs) = data;
tbs.headMove();
const size_t dataSize = tbs.get(dataBuffer, BufferSize);
tbs.swap();
const int transferState = tbs.transfer();
const size_t dataSize = tbs.read(dataBuffer, BufferSize, &readError);
tbs.destroy();
```
クラス\
使い方は同じ\
こっちのほうが記述量が減っていいかんじ

## memo
```bash
gcc a.c TripleBufferSystem.c RingBuffer/RingBuffer.c
```
同時にコンパイルしないとエラーになるよ

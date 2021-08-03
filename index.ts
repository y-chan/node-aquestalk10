// eslint-disable-next-line @typescript-eslint/no-unsafe-assignment, @typescript-eslint/no-unsafe-call
const addon = require('bindings')('aquestalk10')

export enum VoiceBase {
  F1E = 0,
  F2E,
  M1E,
}

export interface VoiceParams {
  base: VoiceBase
  speed: number
  volume: number
  pitch: number
  accent: number
  lmd: number
  fsc: number
}

export const gVoice_F1: VoiceParams = {
  base: VoiceBase.F1E,
  speed: 100,
  volume: 100,
  pitch: 100,
  accent: 100,
  lmd: 100,
  fsc: 100,
}

export const gVoice_F2: VoiceParams = {
  base: VoiceBase.F2E,
  speed: 100,
  volume: 100,
  pitch: 77,
  accent: 150,
  lmd: 100,
  fsc: 100,
}

export const gVoice_F3: VoiceParams = {
  base: VoiceBase.F1E,
  speed: 80,
  volume: 100,
  pitch: 100,
  accent: 100,
  lmd: 61,
  fsc: 148,
}

export const gVoice_M1: VoiceParams = {
  base: VoiceBase.M1E,
  speed: 100,
  volume: 100,
  pitch: 30,
  accent: 100,
  lmd: 100,
  fsc: 100,
}

export const gVoice_M2: VoiceParams = {
  base: VoiceBase.M1E,
  speed: 105,
  volume: 100,
  pitch: 45,
  accent: 130,
  lmd: 120,
  fsc: 100,
}

export const gVoice_R1: VoiceParams = {
  base: VoiceBase.M1E,
  speed: 100,
  volume: 100,
  pitch: 30,
  accent: 20,
  lmd: 190,
  fsc: 100,
}

export const gVoice_R2: VoiceParams = {
  base: VoiceBase.F2E,
  speed: 70,
  volume: 100,
  pitch: 50,
  accent: 50,
  lmd: 50,
  fsc: 180,
}

export interface IAquesTalk10 {
  AquesTalkSyntheSjis(vParams: VoiceParams, koe: string): Buffer
  AquesTalkSyntheUtf8(vParams: VoiceParams, koe: string): Buffer
  AquesTalkSyntheUtf16(vParams: VoiceParams, koe: string): Buffer
  AquesTalkSetDevKey(key: string): boolean
  AquesTalkSetUsrKey(key: string): boolean
}

class AquesTalk10 implements IAquesTalk10 {
  private readonly addon: IAquesTalk10
  constructor(libraryPath: string) {
    // eslint-disable-next-line @typescript-eslint/no-unsafe-assignment, @typescript-eslint/no-unsafe-call
    this.addon = new addon(libraryPath)
  }

  /**
   * Shift-JIS形式の音声記号列からWave形式の音声波形データを生成する関数。
   * また、Wave形式の音声データの先頭アドレス(ポインタ)も同時に返す。
   * エラー発生時はエラーを投げるので、try-catchでのエラーハンドリングを推奨。
   * @param {VoiceParams} vParams - 声質パラメータ
   * @param {string} koe - 音声記号列(Shift-JIS)
   * @return {Buffer} - Waveのバイナリ(Buffer)を返す。
   */
  AquesTalkSyntheSjis(vParams: VoiceParams, koe: string): Buffer {
    return this.addon.AquesTalkSyntheSjis(vParams, koe)
  }

  /**
   * UTF-8形式の音声記号列からWave形式の音声波形データを生成する関数。
   * また、Wave形式の音声データの先頭アドレス(ポインタ)も同時に返す。
   * エラー発生時はエラーを投げるので、try-catchでのエラーハンドリングを推奨。
   * @param {VoiceParams} vParams - 声質パラメータ
   * @param {string} koe - 音声記号列(UTF-8 BOM無し)
   * @return {Buffer} - Waveのバイナリ(Buffer)を返す。
   */
  AquesTalkSyntheUtf8(vParams: VoiceParams, koe: string): Buffer {
    return this.addon.AquesTalkSyntheUtf8(vParams, koe)
  }

  /**
   * UTF-8形式の音声記号列からWave形式の音声波形データを生成する関数。
   * また、Wave形式の音声データの先頭アドレス(ポインタ)も同時に返す。
   * エラー発生時はエラーを投げるので、try-catchでのエラーハンドリングを推奨。
   * @param {VoiceParams} vParams - 声質パラメータ
   * @param {string} koe - 音声記号列(UTF-16 BOMの有無は問わない エンディアンは実行環境に従う)
   * @return {Buffer} - Waveのバイナリ(Buffer)を返す。
   */
  AquesTalkSyntheUtf16(vParams: VoiceParams, koe: string): Buffer {
    return this.addon.AquesTalkSyntheUtf16(vParams, koe)
  }
  /**
   * 開発ライセンスキーの設定用関数。音声波形を生成する前に一度呼び出す。
   * これにより評価版の制限がなくなる。
   * キーの解析を防ぐため不正なキーでもtrueを返す場合がある。このとき制限は解除されない。
   * なお、本来のAquesTalk10の仕様と違って結果が反転しているので注意(本来は正しければ0、正しくなければ1を返す)
   * @param {string} key - ライセンスキーを指定
   * @return {boolean} - ライセンスキーが正しければtrue、正しくなければfalseが返る
   */
  AquesTalkSetDevKey(key: string): boolean {
    return this.addon.AquesTalkSetDevKey(key)
  }

  /**
   * 使用ライセンスキーの設定用関数。音声波形を生成する前に一度呼び出す。
   * 以降、合成音声データに含まれる透かしが、使用ライセンス無しから取得済みに変化する。
   * キーの解析を防ぐため不正なキーでもtrueを返す場合がある。このとき制限は解除されない。
   * なお、本来のAquesTalk10の仕様と違って結果が反転しているので注意(本来は正しければ0、正しくなければ1を返す)
   * @param {string} key - ライセンスキーを指定
   * @return {boolean} - ライセンスキーが正しければtrue、正しくなければfalseが返る
   */
  AquesTalkSetUsrKey(key: string): boolean {
    return this.addon.AquesTalkSetUsrKey(key)
  }
}

export default AquesTalk10

import * as fs from 'fs'

import AquesTalk10, { gVoice_F1 } from '@/index'

const aquestalk = new AquesTalk10('AquesTalk.dll')
const result = aquestalk.AquesTalkSyntheUtf8(gVoice_F1, 'こんにちは。')
fs.writeFileSync('file1.wav', result)

// GenerateKifu.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include "ai_core.h"
#include "move.h"
#include "board.h"
#include "eval.h"
#include "cpu.h"
#include "mt.h"
#include "rev.h"

#define MAX_GAME_NUM 500000
#define FIRST_BK 34628173824
#define FIRST_WH 68853694464


#define EXACT_SEARCH_DEPTH 22
#define MIDDLE_SEARCH_DEPTH 12

//#define RANDOM10_ENABLE
#define DATABASE_ENABLE

#ifdef DATABASE_ENABLE

char ggg[10528056];

UINT64 g_count_sum;
INT32  g_done_moves;

typedef struct
{
	char *line;
	char *name;
} st_db;

// 定石データベース
st_db database_ascii[] =
{
	//// 兔 ////
	{ "F5D6C5F4E3C6D3F6E6D7G3C4",                 "兔基本形"},
    { "F5D6C5F4E3C6D3F6E6D7G3C4B4B3B5",           "爲則ローズ" },
	{ "F5D6C5F4E3C6D3F6E6D7G3C4B4B3B6",           "爲則ローズ2" },
	{ "F5D6C5F4E3C6D3F6E6D7G3C4B4B3G5",           "爲則ローズ3" },
	{ "F5D6C5F4E3C6D3F6E6D7G3C4E7",               "Sローズ・13-e7　兜割り型" },
	{ "F5D6C5F4E3C6D3F6E6D7G3C4G6",               "Sローズ・13-g6　ろーてーしょん型" },
	{ "F5D6C5F4E3C6D3F6E6D7G3C4G5",               "Sローズ・13-g5　ろーてーしょん型" },
	{ "F5D6C5F4E3C6D3F6E6D7G4C4",                 "Fローズ・基本形" },
	{ "F5D6C5F4E3C6D3F6E6D7G4C4G5C3F7D2E7F2C8",   "Fローズ・13-g5　ろーてーしょん型" },
	{ "F5D6C5F4E3C6D3F6E6D7G4C4G5C3F7D2E7F2E2",   "Fローズ・13-g5　ろーてーしょん型　19-e2変化" },
	{ "F5D6C5F4E3C6D3F6E6D7G4C4G5C3B4",           "Fローズ・13-g5 → 横中割り型" },
	{ "F5D6C5F4E3C6D3F6E6D7G4C4E7C3G5",           "Fローズ・13-e7　兜割り型" },
	{ "F5D6C5F4E3C6D3F6E6D7G4C4G6",               "Fローズ・13-g6　（村上流）" },
	{ "F5D6C5F4E3C6D3F6E6D7E7C7C4F3D8",           "手塚システム･基本形" },
	{ "F5D6C5F4E3C6D3F6E6D7E7C7C4F3D8C8B8",       "手塚システム･黒C打ち型" },
	{ "F5D6C5F4E3C6D3F6E6D7E7C7C4F3D8C8F7",       "手塚システム･3：1片方C打ち型" },
	{ "F5D6C5F4E3C6D3F6E6D7E7C7C4F3D8C8G4",       "手塚システム･17-g4型" },
	{ "F5D6C5F4E3C6D3F6E6D7E7C7C4F8",             "手塚システム･14-f8型" },
	{ "F5D6C5F4D3",                               "馬定石・基本形" },
	{ "F5D6C5F4D3E3G4G5E6C4G3C6",                 "馬定石・最善型" },
	{ "F5D6C5F4D3E3G4G5E6C4G3F6",                 "馬定石・12-f6変化" },
	{ "F5D6C5F4D3E3G4G5E6C4E2",                   "馬定石・11-e2変化" },
	{ "F5D6C5F4D3E3G4G5E6F6",                     "馬定石・10-f6変化" },
	{ "F5D6C5F4D3C4",                             "馬定石・白次善手対応" },
	{ "F5D6C5F4D7",                               "鶴定石" },
	{ "F5D6C5F4E3C6F3",                           "シャーマン定石" },
	{ "F5D6C5F4E3C6E6F6",                         "雪うさぎ/井上流･基本形" },
	{ "F5D6C5F4E3C6E6F6G4C4G5C3D7",               "雪うさぎ　9-g4→13-d7型" },
	{ "F5D6C5F4E3C6E6F6G4C4G5C3F7",               "雪うさぎ　9-g4→13-f7型" },
	{ "F5D6C5F4E3C6E6F6D7C7E7",                   "雪うさぎ　9-d7→11-e7型" },
	{ "F5D6C5F4E3C6E6F6D7C7F7",                   "雪うさぎ　9-d7→11-f7型" },
	{ "F5D6C5F4E3C6E6F7D7E8",                     "井上流・最善型" },
	{ "F5D6C5F4E3C6E6F7D7F6",                     "井上流・10-f6変化" },
	{ "F5D6C5F4E3C6E6F7D7C7",                     "井上流・10-c7変化" },
	{ "F5D6C5F4E3C6D3G5",                         "横うさぎ・基本形" },
	{ "F5D6C5F4E3C6D3G5E6",                       "横うさぎ・最善型" },
	{ "F5D6C5F4E3C6D3G5F6F3G4",                   "横うさぎ・9-f6→11-g4変化型" },
	{ "F5D6C5F4E3C6D3G5F6F3G6",                   "横うさぎ・9-f6変化型" },
	{ "F5D6C5F4E3C6D3F3E6F6",                     "Ralle定石" },
	{ "F5D6C5F4E3F6F3C4D3",                       "大和久流" },
	{ "F5D6C5F4E3C6D3E2F3",                       "たまうさぎ9-f3型" },
	{ "F5D6C5F4E3C6D3E2G4",                       "たまうさぎ9-g4型" },
	{ "F5D6C5F4E3D3E6",                           "三村流7-e6型" },
	{ "F5D6C5F4E3D3F3",                           "三村流7-f3型" },
	{ "F5D6C6F4E6",                               "野兎定石・基本形" },
	{ "F5D6C6F4E6G5D3",                           "野兎定石・7-d3型" },
	{ "F5D6C6F4E6G5E3",                           "野兎定石・7-e3型" },
	{ "F5D6C6F4E6G6",                             "野兎定石・6-g6型" },

	//// 虎 ////

	{ "F5D6C3D3C4F4C5B3C2",                       "花形（ローズビル）基本形" },
	{ "F5D6C3D3C4F4C5B3C2E3D2C6B4A3",             "酉フック　14-a3型" },
	{ "F5D6C3D3C4F4C5B3C2E3D2C6B4A4",             "酉ストレート　14-a4型" },
	{ "F5D6C3D3C4F4C5B3C2E3D2C6B4B5",             "酉アッパー　14-b5型" },
	{ "F5D6C3D3C4F4C5B3C2E6C6",                   "金魚定石" },
	{ "F5D6C3D3C4F4C5B3C2E6B4",                   "金魚回避　11-b4変化型" },
	{ "F5D6C3D3C4F4C5B3C2B4E3E6C6F6",             "FJT" },
	{ "F5D6C3D3C4F4C5B3C2B4E3E6C6G4",             "FJT　14-g4型" },
	{ "F5D6C3D3C4F4C5B3C2B4C6",                   "FJT　11-c6変化型" },
	{ "F5D6C3D3C4F4C5B4B3",                       "バナナ定石" },
	{ "F5D6C3D3C4F4C5B4B5",                       "バナナ定石　9-b5変化型" },
	{ "F5D6C3D3C4F4C5B5",                         "まこ虎" },
	{ "F5D6C3D3C4F4F6F3E6E7D7",                   "コンポス基本形" },
	{ "F5D6C3D3C4F4F6F3E6E7D7G6F8F7",             "コンポス･F.A.T型" },
	{ "F5D6C3D3C4F4F6F3E6E7D7G6F8C5",             "コンポス　14-c5変化型" },
	{ "F5D6C3D3C4F4F6F3E6E7D7G6D8",               "D8コンポス　13-d8変化型" },
	{ "F5D6C3D3C4F4F6F3E6E7D7G6G5C5B6",           "シャープコンポス" },
	{ "F5D6C3D3C4F4F6F3E6E7D7G6G5C5C6",           "フラットコンポス" },
	{ "F5D6C3D3C4F4F6F3E6E7F7",                   "たまぷらーざ基本型" },
	{ "F5D6C3D3C4F4F6F3E6E7F7C5B6G6G5",           "たまぷらーざ最善型" },
	{ "F5D6C3D3C4F4F6F3E6E7F7C5B6G6E3",           "たまぷらーざ　15-e3変化型" },
	{ "F5D6C3D3C4F4F6F3E6E7F7C5E3",               "たまぷら　13-d3変化型" },
	{ "F5D6C3D3C4F4F6F3E6E7C6",                   "ロジステロ流基本形" },
	{ "F5D6C3D3C4F4F6F3E6E7C6G6",                 "ロジステロ最善型" },
	{ "F5D6C3D3C4F4F6F3E6E7C6G5",                 "ロジステロ　12-g5型" },
	{ "F5D6C3D3C4F4F6F3E3",                       "ライトニングボルト・ピカチュウ" },
	{ "F5D6C3D3C4F4F6F3E3B4",                     "ライトニングボルト・白最善対応型" },
	{ "F5D6C3D3C4F4F6F3E3G5",                     "ライトニングボルト・白次善対応型" },
	{ "F5D6C3D3C4F4F6G5",                         "ノーカン基本形" },
	{ "F5D6C3D3C4F4F6G5E6D7E3C5F3E7H5",           "ノーカン最善1　15-h5型" },
	{ "F5D6C3D3C4F4F6G5E6D7E3C5F3E7B6",           "ノーカン最善2　15-b6型" },
	{ "F5D6C3D3C4F4F6G5E6D7E3C5F7E7",             "ノーカン　13-f7変化型" },
	{ "F5D6C3D3C4F4F6G5E6D7E3C5E7F3",             "ノーカン　13-e7変化型" },
	{ "F5D6C3D3C4F4F6G5E6D7F7C5B6",               "ノーカン　11-f7変化型（北島流）" },
	{ "F5D6C3D3C4F4F6G5E6D7E7C5F3",               "ノーカン　11-e7変化型" },
	{ "F5D6C3D3C4F4F6G5E6C5",                     "ノーカン　10-c5型（縦取り殺され）" },
	{ "F5D6C3D3C4F4F6G5E3",                       "ノーカン　9-e3変化型" },
	{ "F5D6C3D3C4F4F6B4F3",                       "Kung" },
	{ "F5D6C3D3C4F4F6B4C2",                       "Kung　黒9-c2変化型" },
	{ "F5D6C3G5C6C5C4",                           "ミサイル定石" },
	{ "F5D6C3F4F6",                               "龍定石" },
	{ "F5D6C3F4F6D3F3B3C5",                       "風神定石　9-c5型" },
	{ "F5D6C3F4F6D3F3B3C7",                       "竜巻定石　9-c7型" },
	{ "F5D6C3D3C5",                               "ザリガニ定石" },
	{ "F5D6C3D3C6",                               "海老定石" },
	{ "F5D6C3D3C4B3C6F4",                         "イエス流 8-f4型" },
	{ "F5D6C3D3C4B3C6B6",                         "イエス流 8-b6型" },
	{ "F5D6C3D3C4B5",                             "Berg Tiger" },
	{ "F5D6C3D3C4F4E6B3E2C5C6E3",                 "Leader's Tiger" },
	{ "F5D6C3D3C4F4E6F6E3C5C6B5",                 "Leader's Tiger　8-f6変化（ビン定石）" },
	{ "F5D6C3D3C4F4E6F6E3C5C6B6",                 "Leader's Tiger　8-f6変化型（坂口流）" },
	{ "F5D6C3D3C4F4E3",                           "ブライトウェル" },
	//// 牛 ////
	{ "F5F6E6F4E3C5",                             "牛基本定石・基本形" },
	{ "F5F6E6F4E3C5C4E7C6E2F3F2",                 "快速船・基礎形" },
	{ "F5F6E6F4E3C5C4E7C6E2F3F2G5",               "快速船・最善形" },
	{ "F5F6E6F4E3C5C4E7C6E2F3F2G4",               "快速船・13-g4変化" },
	{ "F5F6E6F4E3C5C4E7C6E2F3F2G6",               "快速船・13-g6変化" },
	{ "F5F6E6F4E3C5C4E7C6E2F3D3",                 "快速船外し　12-d3" },
	{ "F5F6E6F4E3C5C4D3F3E2G4G5F2",               "8-d3変化　黒最善対応型" },
	{ "F5F6E6F4E3C5C4D3C3F2E2D7E7D6",             "8-d3変化　黒次善対応型" },
	{ "F5F6E6F4E3C5C4E7B5E2F3",                   "ヨット定石" },
	{ "F5F6E6F4E3C5C4E7B5E2F2",                   "ヨット定石　11-f2変化型" },
	{ "F5F6E6F4E3C5C4D7C6",                       "沈没船" },
	{ "F5F6E6F4E3C5C4D6B5",                       "最善進行　9-b5型" },
	{ "F5F6E6F4E3C5C4D6B6",                       "最善進行　9-b6型" },
	{ "F5F6E6F4E3C5C4D6C6",                       "最善進行　9-c6型" },
	{ "F5F6E6F4E3C5G5G3",                         "こうもり定石・基本形" },
	{ "F5F6E6F4E3C5G5G3G4",                       "こうもり定石　9-g4型" },
	{ "F5F6E6F4E3C5G5G3C6",                       "こうもり定石　9-c6型" },
	{ "F5F6E6F4E3C5G5F3",                         "こうもり定石　8-f3型" },
	{ "F5F6E6F4E3C5C6",                           "戦車定石" },
	{ "F5F6E6F4E3D6",                             "白大量取り" },
	{ "F5F6E6F4E3D6G4D3C3H3C4G3C6",               "白大量・最善進行　13-c6型" },
	{ "F5F6E6F4E3D6G4D3C3H3C4G3G5",               "最善進行　13-g5型" },
	{ "F5F6E6F4E3D6G4D3C3H3C4G3E7",               "最善進行　13-e7型" },
	{ "F5F6E6F4E3D6C4",                           "7-c4型" },
	{ "F5F6E6F4G5",                               "飛出し牛・基本形" },
	{ "F5F6E6F4G5E7F7H5",                         "裏快速船・基本形" },
	{ "F5F6E6F4G5E7F7H5G4G6E3C5",                 "裏快速船　9-g4型ver1" },
	{ "F5F6E6F4G5E7F7H5G4G6E3H4",                 "裏快速船　9-g4型ver2" },
	{ "F5F6E6F4G5E7F7H5G4G6E3F3",                 "裏快速船　9-g4型ver3" },
	{ "F5F6E6F4G5E7F7H5E3D3G3G4",                 "裏快速船　9-e3型ver1" },
	{ "F5F6E6F4G5E7F7H5E3D3G3F3",                 "裏快速船　9-e3型ver2" },
	{ "F5F6E6F4G5E7F7H5E3D3G4",                   "裏快速船　9-e3変化型　（村上流）" },
	{ "F5F6E6F4G5E7F7C5",                         "裏快速船　8-c5変化型" },
	{ "F5F6E6F4G5E7F7D6",                         "野いちご定石" },
	{ "F5F6E6F4G5E7F7H5",                         "裏ヨット定石" },
	{ "F5F6E6F4G5E7D7",                           "7-d7変化型　（岩崎流）" },
	{ "F5F6E6F4G5E7E3",                           "裏こうもり・基本形" },
	{ "F5F6E6F4G5E7E3G6D3",                       "裏こうもり 8-g6型→9-d3" },
	{ "F5F6E6F4G5E7E3G6D6",                       "裏こうもり 8-g6型→9-d6" },
	{ "F5F6E6F4G5E7E3F3",                         "裏こうもり 8-f3型" },
	{ "F5F6E6F4G5E7E3G4",                         "裏こうもり 8-g4変化型" },
	{ "F5F6E6F4G5D6E7G6",                         "リンゴ定石" },
	{ "F5F6E6F4G5D6E7G4",                         "リンゴ定石 8-g4型" },
	{ "F5F6E6F4G6",                               "蛇定石・基本形" },
	{ "F5F6E6F4G6C5F3",                           "砂蛇・基本形" },
	{ "F5F6E6F4G6C5F3E7C3",                       "砂蛇　9-c3型" },
	{ "F5F6E6F4G6C5F3E7D6",                       "砂蛇　9-d6型" },
	{ "F5F6E6F4G6C5F3G4",                         "砂蛇　8-g4変化型" },
	{ "F5F6E6F4G6C5F3G5",                         "砂蛇　8-g5変化型" },
	{ "F5F6E6F4G6C5G4G5H4",                       "マムシ定石・基本形" },
	{ "F5F6E6F4G6C5G4G5H4H3H2",                   "マムシ定石　11-h2型" },
	{ "F5F6E6F4G6C5G4G5H4H3F3",                   "マムシ定石　11-f3型" },
	{ "F5F6E6F4G6C5G4G5H4H6",                     "マムシ定石　10-h6型" },
	{ "F5F6E6F4G6C5G4G5F3E3",                     "バシリスク定石・基本形" },
	{ "F5F6E6F4G6C5G4G5F3E3C4",                   "バシリスク定石　11-c4型" },
	{ "F5F6E6F4G6C5G4G5F3E3H5",                   "バシリスク定石　11-h5型" },
	{ "F5F6E6F4G6C6",                             "コブラ定石・基本形（Lysons）" },
	{ "F5F6E6F4G6C6G5",                           "コブラ定石　7-g5型" },
	{ "F5F6E6F4G6C6F3G5",                         "コブラ定石　7-f3型" },
	{ "F5F6E6F4G6C6F3E7",                         "コブラ定石　7-f3型　8-e7変化" },
	{ "F5F6E6F4G6D6",                             "つちのこ定石・基本形" },
	{ "F5F6E6F4G6D6E3",                           "つちのこ定石　7-e3型" },
	{ "F5F6E6F4G6D6G4",                           "つちのこ定石　7-g4型" },
	{ "F5F6E6F4G6D6C6",                           "つちのこ定石　7-c6型" },
	{ "F5F6E6F4C3",                               "バッファロー定石・基本形" },
	{ "F5F6E6F4C3D7",                             "丸岡バッファロー・基本形" },
	{ "F5F6E6F4C3D7F3D6G5",                       "丸岡バッファロー　7-f3→9-g5型" },
	{ "F5F6E6F4C3D7F3D6E7",                       "丸岡バッファロー　7-f3→9-e7型" },
	{ "F5F6E6F4C3D7E3",                           "丸岡バッファロー　7-e3型" },
	{ "F5F6E6F4C3D6F3",                           "谷田バッファロー・基本形" },
	{ "F5F6E6F4C3D6F3C4",                         "谷田バッファロー・最善型" },
	{ "F5F6E6F4C3D6F3E3",                         "谷田バッファロー　8-e3変化型" },
	{ "F5F6E6F4C3D6F3C5",                         "谷田バッファロー　8-c5変化型" },
	{ "F5F6E6F4C3C4D3",                           "北陸バッファロー・基本形" },
	{ "F5F6E6F4C3C4D3D6",                         "北陸バッファロー 8-d6型" },
	{ "F5F6E6F4C3C4D3C6",                         "北陸バッファロー 8-c6型" },
	{ "F5F6E6F4C3E7",                             "e7バッファロー・基本形" },
	{ "F5F6E6F4C3E7F3F2",                         "e7バッファロー　8-f2型" },
    { "F5F6E6F4C3E7F3E3",                         "e7バッファロー　8-e3型" },
	{ "F5F6E6F4G3",                               "ロケット定石・基本形" },
	{ "F5F6E6F4G3D6",                             "ロケット定石・最善型" },
	{ "F5F6E6F4G3F3",                             "ロケット定石　6-f3変化型" },
	{ "F5F6E6F4D3",                               "裏蛇定石・基本形" },
	{ "F5F6E6F4D3D6F3",                           "裏蛇定石・最善形" },
	{ "F5F6E6F4D3D6G5",                           "裏蛇定石　7-g5型" },
	{ "F5F6E6F4D3E7",                             "裏蛇定石　6-e7変化型" },
	{ "F5F6E6F4G4",                               "狸定石・基本形" },
	{ "F5F6E6F4G4E7",                             "狸定石・最善型" },
	{ "F5F6E6F4F3",                               "裏狸定石・基本形" },
	{ "F5F6E6F4F3C5",                             "裏狸定石・最善型" },
	{ "F5F6E6F4G7",                               "暴走牛定石・基本形" },

	//// 鼠 ////
	{ "F5F4E3",                                   "ネズミ定石・基本形" },
	{ "F5F4E3F6",                                 "ネズミ定石・最善型" },
	{ "F5F4E3F2",                                 "ネズミ定石　4-f2変化型（旅チュー）" },
	{ "F5F4E3D2",                                 "ネズミ定石　4-f2変化型（T字）" },
	{ "F5F4E3D6",                                 "ネズミ定石　4-f2変化型（ネズミ裏）" },
	{ "F5F4F3",                                   "荷車定石・基本形" },
	{ "F5F4F3D6",                                 "荷車定石・荷車裏" },
	{ "F5F4F3F6",                                 "荷車定石・ゴリラ裏" },
	{ "F5F4F3G4",                                 "荷車定石・椅子裏" },

	//// その他 ////
	{ "F5D6C4D3C5F4",                             "猫定石・基本形" },
	{ "F5D6C4D3C5F4E3F3E2",                       "猫定石・最善型" },
	{ "F5D6C4D3C5F4E3F3C2",                       "猫定石・坂口流" },
	{ "F5D6C4D3C5F4E3F3G4",                       "猫定石　9-g4型" },
	{ "F5D6C4G5",                                 "No Cat・基本形" },
	{ "F5D6C4G5C6C5D7D3B4C3E3B5",                 "燕定石" },
	{ "F5D6C4G5C6C5D7D3B4C3E3F3",                 "燕定石　12-f3型" },
	{ "F5D6C4G5F6",                               "ラッコ定石" },
	{ "F5D6C4D3E6F4E3F3",                         "羊定石・基本形" },
	{ "F5D6C4D3E6F4E3F3C6F6G5G6",                 "羊定石・最善型" },
	{ "F5D6C4D3E6F4E3F3C6F6G5E7",                 "羊定石　12-e7変化型" }
};
#endif


BOOL setMovesToBoard(UINT64 *bk, UINT64 *wh, UINT64 move, INT32 pos, UINT32 color)
{
	UINT64 l_bk, l_wh;
	UINT64 rev;

	l_bk = *bk;
	l_wh = *wh;

	if (color == WHITE) swap(&l_bk, &l_wh);

	if ((l_bk & move) || (l_wh & move))
	{
		return FALSE;
	}

	rev = KZ_GetBoardChangeInfo(l_bk, l_wh, pos);
	if (rev == 0) return FALSE;

	l_bk = l_bk ^ (1ULL << pos) | rev;
	l_wh = l_wh ^ rev;

	if (color == BLACK)
	{
		*bk = l_bk;
		*wh = l_wh;
	}
	else
	{
		*bk = l_wh;
		*wh = l_bk;
	}

	return TRUE;
}


UINT64 RandomMove(UINT64 bk, UINT64 wh, INT32 empty, UINT32 color)
{
	UINT32 count;
	UINT64 moves;
	UINT32 rnd;
	INT32  pos;
	UINT64 l_bk, l_wh;

	l_bk = bk;
	l_wh = wh;
	if (color == WHITE) swap(&l_bk, &l_wh);

	moves = CreateMoves(l_bk, l_wh, &count);

	//g_count_sum += count;
	//g_done_moves++;
	//printf("Num of random moves average = %lf\n", g_count_sum / (double)g_done_moves);

	if (count != 0)
	{
		rnd = genrand_int32() % count;
		do
		{
			pos = KZ_CountBit((~moves) & (moves - 1));
			moves ^= (1ULL << pos);
		} while (rnd--);
	}
	else return MOVE_PASS;

	return 1ULL << pos;
}


INT32 checkMove(UINT64 *bk, UINT64 *wh, int color, INT32 move)
{
	INT32  ret;
	UINT64 rev, l_bk, l_wh;

	l_bk = *bk;
	l_wh = *wh;

	ret = 0;

	if (color == WHITE) swap(&l_bk, &l_wh);

	rev = GetRev[move](l_bk, l_wh);
	/* 黒パス？ */
	if (rev == 0)
	{
		ret = -1;
	}
	else
	{
		l_bk ^= ((1ULL << move) | rev);
		l_wh ^= rev;
	}

	if (ret == 0)
	{
		if (color == BLACK)
		{
			*bk = l_bk;
			*wh = l_wh;
		}
		else
		{
			*bk = l_wh;
			*wh = l_bk;
		}
	}


	return ret;
}


#ifdef DATABASE_ENABLE
INT32 doDatabaseMoves(st_db* db, UINT64 *bk, UINT64 *wh)
{
	INT32  move, color;
	INT32  i;
	UINT64 l_bk, l_wh;

	color = BLACK;
	l_bk = *bk;
	l_wh = *wh;

	for (i = 0; i < strlen(db->line); i += 2)
	{
		move = (db->line[i] - 'A') * 8;
		move += db->line[i + 1] - '1';
		if (checkMove(&l_bk, &l_wh, color, move) == -1)
		{
			printf("databese failed!!![%s : %s]\n", db->name, db->line);
			break;
		}
		color ^= 1;
	}

	*bk = l_bk;
	*wh = l_wh;

	return i / 2;
}
#endif

int main(int argc, char* argv[])
{
	FILE      *wfp;
	UINT32     addr, pass, move_cnt;
	UINT32     retry_count;
	UINT64     bk, wh, move;
	CPUCONFIG  cpuConfig = {0};
	INT32      score, pos, empty, modval, eval, rnd, solveColor;
	INT32      rnd_b, rnd_w;
	char       asciiLine[256];
	BOOL       boolret;
	UINT32     error1cnt = 0;
	UINT32     error2cnt = 0;
	UINT32     error3cnt = 0;
	INT32      b_eval;
#ifdef DATABASE_ENABLE
	INT32      turn;
	st_db     *db_ptr;
	INT32      db_len;
#endif

	BOOL ret = KZ_LibInit();
	if (ret == FALSE)
	{
		return -1;
	}

	bk = FIRST_BK;
	wh = FIRST_WH;
	cpuConfig.exactDepth = EXACT_SEARCH_DEPTH;
	cpuConfig.winLossDepth = 0;
	cpuConfig.searchDepth = MIDDLE_SEARCH_DEPTH;
	cpuConfig.casheSize = 128 * 1024 * 16;
	cpuConfig.tableFlag = TRUE;
	g_tableFlag = TRUE;
	cpuConfig.mpcFlag = TRUE;
	g_mpcFlag = TRUE;

	init_genrand((UINT32)time(NULL));

#if 0
	// 自己対局シミュ
	int n = 0;
	int c = 0;
	int end = 0;
	while (end < 500000)
	{
		n++;
		end++;
		rnd = genrand_int32() % 10528056;
		if (ggg[rnd] == 0)
		{
			ggg[rnd] = 1;
		}
		else
		{
			c++;
			if ((c % 100) == 0)
			{
				printf("At %d, Collision rate = %lf%%, end = %d\n", n, (c / (double)n) * 100, end);
			}
			end--;
		}
	}
	printf("At %d, Collision rate = %lf%%, end = %d\n", n, (c / (double)n) * 100, end);
	printf("200万局の収集までにかかったループ数 : %d\n", n);

	exit(1);
#endif

#ifdef DATABASE_ENABLE
	rnd = 0;
	// データベースチェック
	printf("------------------------------定石一覧------------------------------\n");
	while (rnd < (sizeof(database_ascii) / sizeof(st_db)) && (strlen(database_ascii[rnd].line) % 2) == 0)
	{
		printf("rnd = %d, line = %s, name = %s\n", rnd, database_ascii[rnd].line, database_ascii[rnd].name);
		rnd++;
	}
	if (rnd < (sizeof(database_ascii) / sizeof(st_db)))
	{
		printf("rnd = %d, line = %s, name = %s\n", rnd, database_ascii[rnd].line, database_ascii[rnd].name);
		return;
	}
	printf("--------------------------------------------------------------------\n");
#endif

	// 自己対戦メインループ
	for (int i = 0; i < MAX_GAME_NUM; i++)
	{
		HashClear(g_hash);
		HashClear(g_pvHash);

		bk = FIRST_BK;
		wh = FIRST_WH;
		empty = 60;
		addr = 0;
		pass = 0;
		cpuConfig.color = BLACK;

#ifdef DATABASE_ENABLE
		rnd = genrand_int32() % (sizeof(database_ascii) / sizeof(st_db));
	    db_ptr = &database_ascii[rnd];
		db_len = strlen(db_ptr->line);
		printf("game = %d  (%s) --> ", i, db_ptr->name);
#else
		rnd = 10; // 開始ランダム
		printf("game = %d ", i);
#endif

		memset(asciiLine, 0x00, sizeof(asciiLine));
		// 1ゲームループ
		while (TRUE)
		{
#ifdef RANDOM10_ENABLE
			if (empty == 60)
			{
				move = f5;
				pos = 44;
			}
			else if((60 - empty) < rnd)
			{
				// ランダム着手
				move = RandomMove(bk, wh, empty, cpuConfig.color);
				if (move == MOVE_PASS)
				{
					printf("Random move fault...\n");
					move = 0xFFFF;
					break;
				}
			}
#else
			if (db_ptr != NULL)
			{
				// 定石データから着手
				turn = doDatabaseMoves(db_ptr, &bk, &wh);
				empty -= turn;
				strcpy_s(asciiLine, sizeof(asciiLine), db_ptr->line);
				addr += 2 * turn;
				pass = 0;
				cpuConfig.color ^= turn % 2;
				db_ptr = NULL;
				// 黒と白のランダム着手ターン数の決定
				rnd_b = (genrand_int32() % (empty - EXACT_SEARCH_DEPTH)) + turn;
				while((rnd_w = (genrand_int32() % (empty - EXACT_SEARCH_DEPTH)) + turn) == rnd_b);
				continue;
			}
			else if (60 - empty == rnd_b || 60 - empty == rnd_w)
			{
				// ランダム着手
				move = RandomMove(bk, wh, empty, cpuConfig.color);
				if (move == MOVE_PASS)
				{
					printf("Random move fault...\n");
					move = 0xFFFF;
					break;
				}
			}
#endif
			else
			{
#if 0
				if (abs(rnd_b - rnd_w) == 1)
				{
					if ((60 - empty == rnd_b + 1 || 60 - empty == rnd_w + 1))
					{
						b_eval = KZ_GetLastEvaluation() * (1 - 2 * cpuConfig.color);
						move = GetMoveFromAI(bk, wh, empty, &cpuConfig);
						solveColor = cpuConfig.color;
						eval = KZ_GetLastEvaluation() * (1 - 2 * cpuConfig.color);
						printf("(bk&wh rnd) eval = %.2lf --> %.2lf ", b_eval / (double)EVAL_ONE_STONE, eval / (double)EVAL_ONE_STONE);
					}
					else
					{
						move = GetMoveFromAI(bk, wh, empty, &cpuConfig);
						solveColor = cpuConfig.color;
					}
				}
				else if (60 - empty == rnd_b + 1)
				{
					b_eval = KZ_GetLastEvaluation() * (1 - 2 * cpuConfig.color);
					move = GetMoveFromAI(bk, wh, empty, &cpuConfig);
					solveColor = cpuConfig.color;
					eval = KZ_GetLastEvaluation() * (1 - 2 * cpuConfig.color);
					printf("(bk rnd) eval = %.2lf --> %.2lf ", b_eval / (double)EVAL_ONE_STONE, eval / (double)EVAL_ONE_STONE);
				}
				else if(60 - empty == rnd_w + 1)
				{
					b_eval = KZ_GetLastEvaluation() * (1 - 2 * cpuConfig.color);
					move = GetMoveFromAI(bk, wh, empty, &cpuConfig);
					solveColor = cpuConfig.color;
					eval = KZ_GetLastEvaluation() * (1 - 2 * cpuConfig.color);
					printf("(wh rnd) eval = %.2lf --> %.2lf ", b_eval / (double)EVAL_ONE_STONE, eval / (double)EVAL_ONE_STONE);
				}
				else
				{
					move = GetMoveFromAI(bk, wh, empty, &cpuConfig);
					solveColor = cpuConfig.color;
				}
#else
				move = GetMoveFromAI(bk, wh, empty, &cpuConfig);
				solveColor = cpuConfig.color;
#endif
			}

			if (move == MOVE_PASS)
			{
				pass++;
				cpuConfig.color ^= 1;
				if (pass == 2)
				{
					// 終局
					break;
				}
			}
			else
			{
				pos = KZ_CountBit((~move) & (move - 1));
				boolret = setMovesToBoard(&bk, &wh, move, pos, cpuConfig.color);
				if (!boolret)
				{
					printf("BUG!!!\n");
					printf("pos = %d, move = %llx, bk = %llx, wh = %llx\n", pos, move, bk, wh);
					printf("%s\n", asciiLine);
					fopen_s(&wfp, "errolog.txt", "a");
					fprintf(wfp, "%llx, %llx\n", bk, wh);
					fprintf(wfp, "%s\n", asciiLine);
					fclose(wfp);
					break;
				}
				asciiLine[addr] = (pos / 8) + 'A';
				asciiLine[addr + 1] = (pos % 8) + '1';
				empty--;
				addr += 2;
				pass = 0;
				cpuConfig.color ^= 1;
			}
		}

		// ランダム着手のみでゲーム終了したものは除外
		if (move == 0xFFFF) 
		{
			i--;
			continue;
		}

		// 万が一のAI不正をトラップ
		if (pass != 2)
		{
			printf("GAMES ERROR\n");
			fopen_s(&wfp, "errolog.txt", "a");
			fprintf(wfp, "%llx, %llx\n", bk, wh);
			fprintf(wfp, "%s\n", asciiLine);
			fclose(wfp);
			continue;
		}

		// まだ打ててたらエラー
		if (CreateMoves(bk, wh, &move_cnt) != 0 || CreateMoves(wh, bk, &move_cnt) != 0)
		{
			// 間違っている
			error2cnt++;
			printf("2 : Solve routine error[%d]\n", error2cnt);
			printf("%llx, %llx\n", bk, wh);
			i--;
			continue;
		}

		// スコアを取得
		score = KZ_CountBit(bk) - KZ_CountBit(wh);

		// 評価値を取得
		eval = KZ_GetLastEvaluation() * (1 - 2 * solveColor);
		// 完全読み切りの前に終了していた場合
		if (eval > 1280000)
		{
			modval = 0;
			eval -= 1280000;
		}
		else if (eval < -1280000)
		{
			modval = 0;
			eval += 1280000;
		}
		else
		{
			// 完全読み切りの評価値は偶数に補正されているため修正
			empty = KZ_CountBit(~(bk | wh));
			if (score > 0) modval = empty;
			else if (score < 0)  modval = -empty;
			else modval = 0;
		}

		if (abs(eval) != 64 && score + modval != eval)
		{
			// 間違っている
			error3cnt++;
			printf("3 : Solve routine error[%d]\n", error3cnt);
			printf("%llx, %llx\n", bk, wh);
			printf("score = %d, g_eval = %d\n", score + modval, eval);
			i--;
			continue;
		}

		asciiLine[addr++] = ' ';
		addr += sprintf_s(&asciiLine[addr], sizeof(asciiLine) - addr, "%d", score);
		if (rnd_b > rnd_w) rnd = rnd_w;
		else rnd = rnd_b;
		addr += sprintf_s(&asciiLine[addr], sizeof(asciiLine) - addr, " %d", rnd); // rnd + 1 --> end random moves
		asciiLine[addr] = '\0';
		printf("score = %d\n", score);

		// asciiLineをファイルに書き込み
#if 1
		retry_count = 5;
		wfp = NULL;
		while (retry_count--)
		{
			if (fopen_s(&wfp, "kifu.dat", "a") != 0)
			{
				printf("file cannot open!\n");
				Sleep(1000);
			}
			else
			{
				break;
			}
		}
		if (wfp != NULL)
		{
			fprintf(wfp, "%s\n", asciiLine);
			fclose(wfp);
			wfp = NULL;
		}
#else
		printf("%d : %s\n", i + 1, asciiLine);
#endif
	}

	return 0;
}


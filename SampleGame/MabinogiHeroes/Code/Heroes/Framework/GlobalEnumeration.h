#pragma once

enum CurrentWindow { WINDOW_MAIN/*== WINDOW_GAME*/, WINDOW_MAP_TOOL, WINDOW_OPTION, WINDOW_COUNT };

enum Mouse_State
{
	Mouse_Nomal,
	Mouse_Ldown,
	Mouse_Lup,
	Mouse_Lover,
	Mouse_Ldrag,
	Mouse_Rdown,
	Mouse_Rover,
	Mouse_Rup,
	Mouse_Rdrag,
	Mouse_End
};

enum CH_INFOMATION
{
	CH_GOLD, //소비 골드량 ,골드~LEVEL unEqip만 사용
	CH_EXP, //경험치
	CH_LEVEL, //레벨
	CH_HP, //체력
	CH_MP, //엠피
	CH_SP, //스테미나
	CH_ATK, //공격력
	CH_DEF, // 방어력
	CH_END
};
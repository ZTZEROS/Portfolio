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
	CH_GOLD, //�Һ� ��差 ,���~LEVEL unEqip�� ���
	CH_EXP, //����ġ
	CH_LEVEL, //����
	CH_HP, //ü��
	CH_MP, //����
	CH_SP, //���׹̳�
	CH_ATK, //���ݷ�
	CH_DEF, // ����
	CH_END
};
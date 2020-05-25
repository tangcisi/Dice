/*
 * ������￨
 * Copyright (C) 2019-2020 String.Empty
 */
#include "CharacterCard.h"

map<string, CardTemp>& getmCardTemplet()
{
	static map<string, CardTemp>mCardTemplet = {
	{"COC7",{"COC7",SkillNameReplace,BasicCOC7,InfoCOC7,AutoFillCOC7,mVariableCOC7,ExpressionCOC7,SkillDefaultVal,{
		{"",CardBuild({BuildCOC7},CardDeck::mPublicDeck["�������"],{})},
		{"bg",CardBuild({
			{"�Ա�","{�Ա�}"},{"����","7D6+8"},{"ְҵ","{����Աְҵ}"},{"��������","{��������}"},{"��Ҫ֮��","{��Ҫ֮��}"},{"˼������","{˼������}"},{"����Ƿ�֮��","{����Ƿ�֮��}"},{"����֮��","{����֮��}"},{"����","{����Ա�ص�}"}
		},CardDeck::mPublicDeck["�������"],{})}
	}}},
	{"BRP",{}}
	};
	return mCardTemplet;
}

Player& getPlayer(long long qq) {
	if (!PList.count(qq))PList[qq] = {};
	return PList[qq];
}

string getPCName(long long qq, long long group) {
	if (PList.count(qq) && PList[qq][group].Name != "��ɫ��")return PList[qq][group].Name;
	return getName(qq, group);
}
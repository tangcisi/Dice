/*
 * ������￨
 * Copyright (C) 2019 String.Empty
 */
#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <mutex>
#include "CQTools.h"
#include "Unpack.h"
#include "RD.h"
#include "DiceXMLTree.h"
#include "DiceFile.hpp"
#include "ManagerSystem.h"
#include "MsgFormat.h"
#include "CardDeck.h"
using std::string;
using std::to_string;
using std::vector;
using std::map;

#define NOT_FOUND -32767

inline map<string, short>mTempletTag = {
	{"name",1},
	{"type",2},
	{"alias",20},
	{"basic",31},
	{"info",102},
	{"autofill",22},
	{"variable",23},
	{"diceexp",21},
	{"default",12},
	{"build",41},
	{"generate",24 },
	{"note",101},
};
inline map<string, short>mCardTag = {
	{"Name",1},
	{"Type",2},
	{"Attr",11},
	{"DiceExp",21},
	{"Note",101},
	{"Info",102},
	{"End",255}
};
//����ģ��
class CardBuild {
public:
	CardBuild() = default;
	CardBuild(vector<std::pair<string, string>>attr,vector<string>name, vector<string>note):vBuildList(attr), vNameList(name), vNoteList(note) {}
	//��������
	vector<std::pair<string, string>>vBuildList = {};
	//�������
	vector<string>vNameList = {};
	//Note����
	vector<string>vNoteList = {};
	CardBuild(DDOM d) {
		for (auto sub : d.vChild) {
			switch (mTempletTag[sub.tag]) {
			case 1:
				vNameList = getLines(sub.strValue);
				break;
			case 24:
				readini(sub.strValue, vBuildList);
				break;
			case 101:
				vNoteList = getLines(sub.strValue);
				break;
			default:break;
			}
		}
	}
};
class CardTemp {
public:
	string type;
	map<string, string>replaceName = {};
	//����ʱ����
	vector<vector<string>>vBasicList = {};
	set<string>sInfoList = {};
	//����ʱ����
	map<string, string>mAutoFill = {};
	//��̬����
	map<string, string>mVariable = {};
	//���ʽ
	map<string, string>mExpression = {};
	//Ĭ��ֵ
	map<string, short>defaultSkill = {};
	//���ɲ���
	map<string, CardBuild>mBuildOption = {};
	CardTemp() = default;
	CardTemp(string type, map<string, string>replace, vector<vector<string>>basic, set<string>info, map<string, string>autofill, map<string, string>dynamic, map<string, string>exp, map<string, short>skill, map<string, CardBuild> option) :type(type),replaceName(replace), vBasicList(basic), sInfoList(info),mAutoFill(autofill), mVariable(dynamic), mExpression(exp), defaultSkill(skill), mBuildOption(option) {}
	CardTemp(DDOM d) {
		readt(d);
	}
	void readt(DDOM d) {
		for (auto node : d.vChild) {
			switch (mTempletTag[node.tag]) {
			case 2:
				type = node.strValue;
				break;
			case 20:
				readini(node.strValue, replaceName);
				break;
			case 31:
				vBasicList.clear();
				for (auto sub : node.vChild) {
					vBasicList.push_back(getLines(sub.strValue));
				}
				break;
			case 102:
				for (auto sub : getLines(node.strValue)) {
					sInfoList.insert(sub);
				}
			case 22:
				readini(node.strValue, mAutoFill);
				break;
			case 23:
				readini(node.strValue, mVariable);
				break;
			case 21:
				readini(node.strValue, mExpression);
				break;
			case 12:
				readini(node.strValue, defaultSkill);
				break;
			case 41:
				for (auto sub : node.vChild) {
					mBuildOption[sub.strValue] = CardBuild(sub);
				}
				break;
			default:
				break;
			}
		}
	}
	string getName() {
		return type;
	}
	string showItem() {
		string strItem = listKey(mBuildOption);
		if (strItem.empty())return type;
		else return type + "[" + strItem + "]";
	}
};


map<string, CardTemp>& getmCardTemplet();



class CharaCard {
private:
public:
	string Name = "��ɫ��";
	string Type = "COC7";
	map<string, short>Attr{};
	map<string, string>Info{};
	map<string, string>DiceExp{};
	string Note;
	const CardTemp* pTemplet = &getmCardTemplet()[Type];
	CharaCard(){}
	CharaCard(string name, string type = "COC7") :Name(name), Type(type){}
	short call(string &key){
		if (Attr.count(key))return Attr.find(key)->second;
		key = standard(key);
		if (Attr.count(key))return Attr.find(key)->second;
		else if (pTemplet->mAutoFill.count(key)){
			Attr[key] = cal(pTemplet->mAutoFill.find(key)->second);
			return Attr[key];
		}
		else if (pTemplet->mVariable.count(key)) {
			return cal(pTemplet->mVariable.find(key)->second);
		}
		else if (pTemplet->defaultSkill.count(key))return pTemplet->defaultSkill.find(key)->second;
		else return 0;
	}
	//���ʽת��
	string escape(string exp, set<string>sRef){
		if (exp[0] == '&') {
			string key = exp.substr(1);
			if (sRef.count(key))return "";
			return getExp(key);
		}
		int intCnt = 0, lp = 0, rp = 0;
		while ((lp = exp.find('[', intCnt)) != std::string::npos && (rp = exp.find(']', lp)) != std::string::npos) {
			string strProp = exp.substr(lp + 1, rp - lp - 1);
			if (sRef.count(strProp))return "";
			string val = getExp(strProp);
			exp.replace(exp.begin() + lp, exp.begin() + rp + 1, val);
			intCnt = lp + val.length();
		}
		return exp;
	}
	//��key��Ӧ�������ʽ
	string getExp(string& key, set<string>sRef = {}) {
		sRef.insert(key);
		std::map<string, string>::const_iterator exp = DiceExp.find(key);
		if (exp != DiceExp.end()) return escape(exp->second, sRef);
		exp = pTemplet->mExpression.find(key);
		if (exp != pTemplet->mExpression.end()) return escape(exp->second, sRef);
		key = standard(key);
		std::map<string, short>::const_iterator val = Attr.find(key);
		if (val != Attr.end())return to_string(val->second);
		exp = pTemplet->mVariable.find(key);
		if (exp != pTemplet->mVariable.end())return to_string(cal(exp->second));
		val = pTemplet->defaultSkill.find(key);
		if (val != pTemplet->defaultSkill.end())return to_string(val->second);
		return "0";
	}
	bool countExp(string key) {
		return DiceExp.count(key) || pTemplet->mExpression.count(key);
	}
	//������ʽ
	short cal(string exp){
		if (exp[0] == '&') {
			string key = exp.substr(1);
			return call(key);
		}
		int intCnt = 0, lp = 0, rp = 0;
		while ((lp = exp.find('[', intCnt)) != std::string::npos && (rp = exp.find(']', lp)) != std::string::npos) {
			string strProp = exp.substr(lp + 1, rp - lp - 1);
			short val = call(strProp);
			exp.replace(exp.begin() + lp, exp.begin() + rp + 1, std::to_string(val));
			intCnt = lp + std::to_string(val).length();
		}
		RD Res(exp);
		Res.Roll();
		return Res.intTotal;
	}
	void build(string para = "") {
		auto it = pTemplet->mBuildOption.find(para);
		if (it == pTemplet->mBuildOption.end())return;
		CardBuild build = it->second;
		for (auto it : build.vBuildList) {
			//exp
			if (it.first[0] == '&') {
				it.first.erase(it.first.begin());
				if (DiceExp.count(it.first))continue;
				DiceExp[it.first] = it.second;
			}
			//info
			else if (pTemplet->sInfoList.count(it.first)) {
				if (Info.count(it.first))continue;
				Info[it.first] = CardDeck::draw(it.second);
			}
			//attr
			else {
				if (Attr.count(it.first))continue;
				Attr[it.first] = cal(it.second);
			}
		}
		while (Note.empty() && !build.vNoteList.empty()) {
			Note = CardDeck::drawCard(build.vNoteList);
		}
	}
	//�������ɲ���
	void buildv(string para = "") {
		std::stack<string> vOption;
		int Cnt;
		vOption.push("");
		while ((Cnt = para.rfind(':')) != string::npos) {
			vOption.push(para.substr(Cnt + 1));
			para.erase(para.begin() + Cnt, para.end());
		}
		if (!para.empty())vOption.push(para);
		while (!vOption.empty()) {
			string para = vOption.top();
			vOption.pop();
			build(para);
		}
	}
	string standard(string key)const {
		if (pTemplet->replaceName.count(key))return pTemplet->replaceName.find(key)->second;
		return key;
	}
	int set(string key, short val) {
		key = standard(key);
		if (pTemplet->defaultSkill.count(key) && val == pTemplet->defaultSkill.find(key)->second) {
			if (Attr.count(key)) Attr.erase(key);
			return -1;
		}
		Attr[key] = val;
		return 0;
	}
	int setInfo(string key, string s) {
		if (s.length() > 48)return -1;
		Info[key] = s;
		return 0;
	}
	int setExp(string key, string exp) {
		if (exp.length() > 48)return -1;
		DiceExp[key] = exp;
		return 0;
	}
	int setNote(string note) {
		if (note.length() > 255)return -11;
		Note = note;
		scanImage(note, sReferencedImage);
		return 0;
	}
	bool erase(string& key, bool isExp = false) {
		string strKey = standard(key);
		if (pTemplet->sInfoList.count(key)) {
			if (Info.count(key)) {
				Info.erase(key);
				return true;
			}
			else return false;
		}
		else if (!isExp && Attr.count(key)) {
			Attr.erase(strKey);
			key = strKey;
			return true;
		}
		else if (!isExp && Attr.count(strKey)) {
			Attr.erase(strKey);
			key = strKey;
			return true;
		}
		else if (DiceExp.count(key)) {
			DiceExp.erase(key);
			return true;
		}
		return false;
	}
	void clear() {
		Attr.clear();
		Info.clear();
		DiceExp.clear();
		Note.clear();
	}
	int show(string key, string& val)const {
		if (pTemplet->sInfoList.count(key)) {
			if (Info.count(key)) {
				val = Info.find(key)->second;
				return 3;
			}
			else return -1;
		}
		if (key == "note") {
			val = Note;
			return 2;
		}
		if (DiceExp.count(key)) {
			val = DiceExp.find(key)->second;
			return 1;
		}
		key = standard(key);
		if (Attr.count(key)) {
			val = to_string(Attr.find(key)->second);
			return 0;
		}
		else {
			return -1;
		}
	}
	string show(bool isWhole)const {
		std::set<string>sDefault;
		ResList Res;
		for (auto list : pTemplet->vBasicList) {
			ResList subList;
			string strVal;
			for (auto it : list) {
				switch (show(it, strVal)) {
				case 0:
					sDefault.insert(it);
					subList << it + ":" + strVal;
					break;
				case 1:
					sDefault.insert(it);
					subList << "&" + it + "=" + strVal;
					break;
				case 3:
					sDefault.insert(it);
					subList.dot("\t");
					subList << it + ":" + strVal;
					break;
				default:
					continue;
				}
			}
			Res << subList.show();
		}
		string strAttrRest;
		for (auto it : Attr) {
			if (sDefault.count(it.first))continue;
			strAttrRest += it.first + ":" + to_string(it.second) + " ";
		}
		Res << strAttrRest;
		if (isWhole && !Info.empty())for (auto it : Info) {
			if (sDefault.count(it.first))continue;
			Res << it.first + ":" + it.second;
		}
		if (isWhole && !DiceExp.empty())for (auto it : DiceExp) {
			if (sDefault.count(it.first))continue;
			Res << "&" + it.first + "=" + it.second;
		}
		if (isWhole && !Note.empty())Res << "====================\n" + Note;
		return Res.show();
	}
	bool count(string& key)const {
		if (Attr.count(key))return true;
		key = standard(key);
		return Attr.count(key) || DiceExp.count(key) || pTemplet->mAutoFill.count(key) || pTemplet->mVariable.count(key) || pTemplet->defaultSkill.count(key);
	}
	bool stored(string& key)const {
		key = standard(key);
		return Attr.count(key) || pTemplet->mAutoFill.count(key) || pTemplet->defaultSkill.count(key);
	}
	short& operator[](string& key){
		key = standard(key);
		if (!Attr.count(key)) {
			if (pTemplet->mAutoFill.count(key))Attr[key] = cal(pTemplet->mAutoFill.find(key)->second);
			if (pTemplet->defaultSkill.count(key))Attr[key] = pTemplet->defaultSkill.find(key)->second;
		}
		return Attr[key];
	}
	void operator<<(const CharaCard& card) {
		string name = Name;
		(*this) = card;
		Name = name;
	}
	void writeb(std::ofstream& fout) {
		fwrite(fout, (string)"Name");
		fwrite(fout, Name);
		fwrite(fout, (string)"Type");
		fwrite(fout, Type);
		if (!Attr.empty()) {
			fwrite(fout, (string)"Attr");
			fwrite(fout, Attr);
		}
		if (!Info.empty()) {
			fwrite(fout, (string)"Info");
			fwrite(fout, Info);
		}
		if (!DiceExp.empty()) {
			fwrite(fout, (string)"DiceExp");
			fwrite(fout, DiceExp);
		}
		if (!Note.empty()) {
			fwrite(fout, (string)"Note");
			fwrite(fout, Note);
		}
		fwrite(fout, (string)"END");
	}
	void readb(std::ifstream& fin) {
		string tag = fread<string>(fin);
		while (tag != "END") {
			switch (mCardTag[tag]) {
			case 1:
				Name = fread<string>(fin);
				break;
			case 2:
				Type = fread<string>(fin);
				break;
			case 11:
				Attr = fread<string, short>(fin);
				break;
			case 21:
				DiceExp = fread<string, string>(fin);
				break;
			case 102:
				Info = fread<string, string>(fin);
				scanImage(Note, sReferencedImage);
				break;
			case 101:
				Note = fread<string>(fin);
				scanImage(Note, sReferencedImage);
				break;
			default:
				break;
				break;
			}
			tag = fread<string>(fin);
		}
		pTemplet = getmCardTemplet().count(Type) ? &getmCardTemplet()[Type] : &getmCardTemplet()["COC7"];
	}
};

class Player {
private:
	short indexMax = 0;
	map<unsigned short, CharaCard>mCardList;
	map<string, unsigned short>mNameIndex;
	map<unsigned long long, unsigned short>mGroupIndex;
	// ���￨����
	std::mutex cardMutex;
public:
	Player() {
		mCardList = { {0,{"��ɫ��"}} };
		mGroupIndex = { {0,0} };
	}
	Player(const Player& pl) {
		*this = pl;
	}
	Player& operator=(const Player& pl) {
		indexMax = pl.indexMax;
		mCardList = pl.mCardList;
		mNameIndex = pl.mNameIndex;
		mGroupIndex = pl.mGroupIndex;
		return *this;
	}
	size_t size()const {
		return mCardList.size();
	}
	bool count(long long group)const {
		return mGroupIndex.count(group);
	}
	bool count(string name)const {
		return mNameIndex.count(name);
	}
	int newCard(string& s,long long group = 0) {
		std::lock_guard<std::mutex> lock_queue(cardMutex);
		//���￨��������
		if (mCardList.size() > 16)return -1;
		string type = "COC7";
		s = strip(s);
		std::stack<string> vOption;
		int Cnt = s.rfind(":");
		if (Cnt != string::npos) {
			type = s.substr(0, Cnt);
			s.erase(s.begin(), s.begin() + Cnt + 1);
			if (type == "COC")type = "COC7";
		}
		else if (getmCardTemplet().count(s)) {
			type = s;
			s.clear();
		}
		while ((Cnt = type.rfind(':')) != string::npos) {
			vOption.push(type.substr(Cnt + 1));
			type.erase(type.begin() + Cnt, type.end());
		}
		//��Чģ��
		if (!getmCardTemplet().count(type))return -2;
		if (mNameIndex.count(s))return -4;
		if (s.find("=") != string::npos)return -6;
		mCardList[++indexMax] = CharaCard(s, type);
		CharaCard& card = mCardList[indexMax];
		// CardTemp& temp = mCardTemplet[type];
		while(!vOption.empty()) {
			string para = vOption.top();
			vOption.pop();
			card.build(para);
			if (card.Name.empty()) {
				std::vector<string>list = getmCardTemplet()[type].mBuildOption[para].vNameList;
				while (!list.empty()) {
					s = CardDeck::draw(list[0]);
					if (mNameIndex.count(s))list.erase(list.begin());
					else {
						card.Name = s;
						break;
					}
				}
			}
		}
		if (card.Name.empty()) {
			std::vector<string>list = getmCardTemplet()[type].mBuildOption[""].vNameList;
			while (!list.empty()) {
				s = CardDeck::draw(list[0]);
				if (mNameIndex.count(s))list.erase(list.begin());
				else {
					card.Name = s;
					break;
				}
			}
			if (card.Name.empty())card.Name = to_string(indexMax + 1);
		}
		s = card.Name;
		mNameIndex[s] = indexMax;
		mGroupIndex[group] = indexMax;
		return 0;
	}
	int buildCard(string& name, bool isClear ,long long group = 0) {
		string strName = name;
		string strType;
		if (name.find(":") != string::npos) {
			strName = strip(name.substr(name.rfind(":") + 1));
			strType = name.substr(0, name.rfind(":"));
		}
		//���������½����￨
		if (!strName.empty() && !mNameIndex.count(strName)) {
			if (int res = newCard(name, group))return res;
			name = getCard(strName, group).Name;
			(*this)[name].buildv();
		}
		else {
			name = getCard(strName, group).Name;
			if (isClear)(*this)[name].clear();
			(*this)[name].buildv(strType);
		}
		return 0;
	}
	int changeCard(string name, long long group) {
		if (name.empty()) {
			mGroupIndex.erase(group);
			return 1;
		}
		if (!mNameIndex.count(name))return -5;
		mGroupIndex[group] = mNameIndex[name];
		return 0;
	}
	int removeCard(string name) {
		std::lock_guard<std::mutex> lock_queue(cardMutex);
		if (!mNameIndex.count(name))return -5;
		if (!mNameIndex[name])return -7;
		for (auto it : mGroupIndex) {
			if (it.second == mNameIndex[name])mGroupIndex.erase(it.first);
		}
		mCardList.erase(mNameIndex[name]);
		while(!mCardList.count(indexMax))indexMax--;
		mNameIndex.erase(name);
		return 0;
	}
	int renameCard(string name, string name_new) {
		std::lock_guard<std::mutex> lock_queue(cardMutex);
		if (mNameIndex.count(name_new))return -4;
		if (name_new.find(":") != string::npos)return -6;
		int i = mNameIndex[name_new] = mNameIndex[name];
		mNameIndex.erase(name);
		mCardList[i].Name = name_new;
		return 0;
	}
	int copyCard(string name1, string name2, long long group = 0) {
		if (name1.empty() || name2.empty())return -3;
		//���������½����￨
		if (!mNameIndex.count(name1)) {
			std::lock_guard<std::mutex> lock_queue(cardMutex);
			//���￨��������
			if (mCardList.size() > 16)return -1;
			if (name1.find(":") != string::npos)return -6;
			mCardList[++indexMax] = name1;
			mNameIndex[name1] = indexMax;
		}
		(*this)[name1] << (*this)[name2];
		return 0;
	}
	string listCard(){
		ResList Res;
		for (auto it : mCardList) {
			Res << "[" + to_string(it.first) + "]" + it.second.Name;
		}
		Res << "default:" + (*this)[0].Name;
		return Res.show();
	}
	string listMap() {
		ResList Res;
		for (auto it : mGroupIndex) {
			if (!it.first)Res << "default:" + mCardList[it.second].Name;
			else Res << "(" + to_string(it.first) + ")" + mCardList[it.second].Name;
		}
		return Res.show();
	}
	CharaCard& getCard(string name, long long group = 0) {
		if (mNameIndex.count(name))return mCardList[mNameIndex[name]];
		else if (mGroupIndex.count(group))return mCardList[mGroupIndex[group]];
		else if (mGroupIndex.count(0))return mCardList[mGroupIndex[0]];
		else return mCardList[0];
	}
	CharaCard& operator[](long long id){
		if (mGroupIndex.count(id))return mCardList[mGroupIndex[id]];
		else if(mGroupIndex.count(0))return mCardList[mGroupIndex[0]];
		else return mCardList[0];
	}
	CharaCard& operator[](string name) {
		if (mNameIndex.count(name))return mCardList[mNameIndex[name]];
		else if (mGroupIndex.count(0))return mCardList[mGroupIndex[0]];
		else return mCardList[0];
	}
	bool load(std::ifstream& fin) {
		string s;
		if (!(fin >> s))return false;
		Unpack pack(base64_decode(s));
		indexMax = pack.getInt();
		Unpack cards = pack.getUnpack();
		while (cards.len() > 0) {
			Unpack card = cards.getUnpack();
			int nIndex = card.getInt();
			string name = card.getstring();
			string type = card.getstring();
			mCardList[nIndex] = CharaCard(name, type);
			mNameIndex[name] = nIndex;
			Unpack skills = card.getUnpack();
			string skillname;
			while (skills.len() > 0) {
				skillname = skills.getstring();
				mCardList[nIndex][skillname] = skills.getInt();
			}
			mCardList[nIndex].Note = card.getstring();
		}
		Unpack groups = pack.getUnpack();
		while (groups.len() > 0) {
			mGroupIndex[groups.getLong()] = groups.getInt();
		}
		return true;
	}
	void save(std::ofstream& fout) {
		Unpack pack;
		pack.add(indexMax);
		Unpack cards;
		for (auto it : mCardList) {
			Unpack card;
			card.add(it.first);
			card.add(it.second.Name);
			card.add(it.second.Type);
			Unpack skills;
			for (auto skill : it.second.Attr) {
				skills.add(skill.first);
				skills.add(skill.second);
			}
			card.add(skills);
			card.add(it.second.Note);
			cards.add(card);
		}
		pack.add(cards);
		Unpack groups;
		for (auto it : mGroupIndex) {
			groups.add((long long)it.first);
			groups.add(it.second);
		}
		pack.add(groups);
		fout << base64_encode(pack.getAll());
	}
	void writeb(std::ofstream& fout) {
		fwrite(fout, indexMax);
		fwrite(fout, mCardList);
		fwrite(fout, mGroupIndex);
	}
	void readb(std::ifstream& fin) {
		indexMax = fread<short>(fin);
		mCardList = fread<unsigned short,CharaCard>(fin);
		for (auto card : mCardList) {
			mNameIndex[card.second.Name] = card.first;
		}
		mGroupIndex = fread<unsigned long long, unsigned short>(fin);
	}
};

inline map<long long, Player>PList;

Player& getPlayer(long long qq);

string getPCName(long long qq, long long group);
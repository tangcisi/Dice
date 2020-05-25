/*
 * ��̨ϵͳ
 * Copyright (C) 2019-2020 String.Empty
 */
#include <windows.h>
#include <xutility>
#include <string_view>
#include "ManagerSystem.h"

string DiceDir = "DiceData";
 //�����õ�ͼƬ�б�
set<string> sReferencedImage;

const map<string, short> mChatConf{//0-Ⱥ����Ա��2-������2����3-������3����4-����Ա��5-ϵͳ����
	{"����",4},
	{"������Ϣ",0},
	{"ͣ��ָ��",0},
	{"���ûظ�",0},
	{"����jrrp",0},
	{"����draw",0},
	{"����me",0},
	{"����help",0},
	{"����ob",0},
	{"���ʹ��",1},
	{"δ���",1},
	{"����",2},
	{"���",4},
	{"δ��",5},
	{"����",5}
};

User& getUser(long long qq) {
	if (!UserList.count(qq))UserList[qq].id(qq);
	return UserList[qq];
}
short trustedQQ(long long qq) {
	if (qq == console.master())return 256;
	else if (!UserList.count(qq))return 0;
	else return UserList[qq].nTrust;
}
int clearUser() {
	vector<long long> QQDelete;
	for (const auto& [qq, user] : UserList) {
		if (user.empty()) {
			QQDelete.push_back(qq);
		}
	}
	for (const auto& qq : QQDelete)
	{
		UserList.erase(qq);
	}
	return QQDelete.size();
}

string getName(long long QQ, long long GroupID)
{
	string nick;
	if (UserList.count(QQ) && getUser(QQ).getNick(nick, GroupID))return nick;
	if (GroupID && !(nick = strip(CQ::getGroupMemberInfo(GroupID, QQ).GroupNick)).empty())return nick;
	CQ::FriendInfo frd = CQ::getFriendList()[QQ];
	if (!(nick = strip(frd.remark)).empty())return nick;
	if (!(nick = strip(frd.nick)).empty())return nick;
	if (!(nick = strip(CQ::getStrangerInfo(QQ).nick)).empty())return nick;
	return GlobalMsg["stranger"] + "(" + to_string(QQ) + ")";
}
void filter_CQcode(string& nick, long long fromGroup) {
	size_t posL(0);
	while ((posL = nick.find(CQ_AT)) != string::npos) {
		//���at��ʽ
		if (size_t posR = nick.find(']',posL); posR != string::npos) {
			std::string_view stvQQ(nick);
			stvQQ = stvQQ.substr(posL + 10, posR - posL - 10);
			//���QQ�Ÿ�ʽ
			bool isDig = true;
			for (auto ch: stvQQ) {
				if (!isdigit(static_cast<unsigned char>(ch))) {
					isDig = false;
					break;
				}
			}
			//ת��
			if (isDig && posR - posL < 29) {
				nick.replace(posL, posR - posL + 1, "@" + getName(stoll(string(stvQQ)), fromGroup));
			}
			else if (stvQQ == "all") {
				nick.replace(posL, posR - posL + 1, "@ȫ���Ա");
			}
			else {
				nick.replace(posL, posR - posL + 1, "@");
			}
		}
		else return;
	}
	while ((posL = nick.find("[CQ:")) != string::npos) {
		if (size_t posR = nick.find(']', posL); posR != string::npos) {
			nick.erase(posL, posR - posL + 1);
		}
		else return;
	}
}

Chat& chat(long long id) {
	if (!ChatList.count(id))ChatList[id].id(id);
	return ChatList[id];
}
int groupset(long long id, string st) {
	if (!ChatList.count(id))return -1;
	else return ChatList[id].isset(st);
}
string printChat(Chat& grp) {
	if (CQ::getGroupList().count(grp.ID))return CQ::getGroupList()[grp.ID] + "(" + to_string(grp.ID) + ")";
	if (grp.isset("Ⱥ��"))return grp.strConf["Ⱥ��"] + "(" + to_string(grp.ID) + ")";
	if (grp.isGroup) return "Ⱥ" + to_string(grp.ID) + "";
	return "������" + to_string(grp.ID) + "";
}

void scanImage(string s, set<string>& list) {
	int l = 0, r = 0;
	while ((l = s.find('[', r)) != string::npos && (r = s.find(']', l)) != string::npos) {
		if (s.substr(l, 15) != CQ_IMAGE)continue;
		string strFile = s.substr(l + 15, r - l - 15);
		if (strFile.length() > 35)strFile += ".cqimg";
		list.insert(strFile);
	}
}

void scanImage(const vector<string>& v, set<string>& list) {
	for (auto it : v) {
		scanImage(it, sReferencedImage);
	}
}


int clearImage() {
	scanImage(GlobalMsg, sReferencedImage);
	scanImage(HelpDoc, sReferencedImage);
	scanImage(CardDeck::mPublicDeck, sReferencedImage);
	scanImage(CardDeck::mReplyDeck, sReferencedImage);
	scanImage(CardDeck::mGroupDeck, sReferencedImage);
	scanImage(CardDeck::mPrivateDeck, sReferencedImage);
	for (auto it : ChatList) {
		scanImage(it.second.strConf, sReferencedImage);
	}
	string strLog = "����" + GlobalMsg["strSelfName"] + "������ͼƬ" + to_string(sReferencedImage.size()) + "��";
	console.log(strLog, 0b0, printSTNow());
	return clrDir("data\\image\\", sReferencedImage);
}

DWORD getRamPort() {
	MEMORYSTATUSEX memory_status;
	memory_status.dwLength = sizeof(memory_status);
	GlobalMemoryStatusEx(&memory_status);
	return memory_status.dwMemoryLoad;
}

__int64 compareFileTime(FILETIME& ft1, FILETIME& ft2) {
	__int64 t1 = ft1.dwHighDateTime;
	t1 = t1 << 32 | ft1.dwLowDateTime;
	__int64 t2 = ft2.dwHighDateTime;
	t2 = t2 << 32 | ft2.dwLowDateTime;
	return t1 - t2;
}

__int64 getWinCpuUsage() {
	HANDLE hEvent;
	BOOL res;
	FILETIME preidleTime;
	FILETIME prekernelTime;
	FILETIME preuserTime;
	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;

	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
	preidleTime = idleTime;
	prekernelTime = kernelTime;
	preuserTime = userTime;

	hEvent = CreateEventA(NULL, FALSE, FALSE, NULL); // ��ʼֵΪ nonsignaled ������ÿ�δ������Զ�����Ϊnonsignaled
	//WaitForSingleObject(hEvent, 1000);
	Sleep(2000);
	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);

	__int64 idle = compareFileTime(idleTime, preidleTime);
	__int64 kernel = compareFileTime(kernelTime, prekernelTime);
	__int64 user = compareFileTime(userTime, preuserTime);

	__int64 cpu = (kernel + user - idle) * 100 / (kernel + user);
	return cpu;
}

int getProcessCpu()
{
	HANDLE hProcess = GetCurrentProcess();
	//if (INVALID_HANDLE_VALUE == hProcess){return -1;}

	SYSTEM_INFO si;
	GetSystemInfo(&si);
	__int64 iCpuNum = si.dwNumberOfProcessors;

	FILETIME ftPreKernelTime, ftPreUserTime;
	FILETIME ftKernelTime, ftUserTime;
	FILETIME ftCreationTime, ftExitTime;
	std::ofstream log("System.log");

	if (!GetProcessTimes(hProcess, &ftCreationTime, &ftExitTime, &ftPreKernelTime, &ftPreUserTime)) { return -1; }
	log << ftPreKernelTime.dwLowDateTime << "\n" << ftPreUserTime.dwLowDateTime << "\n";
	HANDLE hEvent = CreateEventA(NULL, FALSE, FALSE, NULL); // ��ʼֵΪ nonsignaled ������ÿ�δ������Զ�����Ϊnonsignaled
	if (hEvent == nullptr) { return -1; }
	WaitForSingleObject(hEvent, 1000);
	if (!GetProcessTimes(hProcess, &ftCreationTime, &ftExitTime, &ftKernelTime, &ftUserTime)) { return -1; }
	log << ftKernelTime.dwLowDateTime << "\n" << ftUserTime.dwLowDateTime << "\n";
	__int64 ullKernelTime = compareFileTime(ftKernelTime, ftPreKernelTime);
	__int64 ullUserTime = compareFileTime(ftUserTime, ftPreUserTime);
	log << ullKernelTime << "\n" << ullUserTime << "\n" << iCpuNum;
	__int64 dCpu = (ullKernelTime + ullUserTime) / (iCpuNum * 100);
	return (int)dCpu;
}
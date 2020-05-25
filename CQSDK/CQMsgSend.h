#pragma once
#include "bufstream.h"

namespace CQ
{
	enum class msgtype : int { Private = 0, Group = 1, Discuss = 2 };

	class msg final : public CQstream
	{
		long long ID;
		int subType = 0;

	public:
		/*
		Type:
		0=msgtype::����
		1=msgtype::Ⱥ
		2=msgtype::������
		*/
		msg(long long GroupID_Or_QQID, msgtype Type);
		/*
		Type:
		0=����
		1=Ⱥ
		2=������
		*/
		msg(long long GroupID_Or_QQID, int Type);

		void send() override;
	};
}

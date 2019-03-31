# Dice!
QQ Dice Robot For TRPG Based on CoolQ 

[![License](https://img.shields.io/github/license/w4123/Dice.svg)](http://www.gnu.org/licenses)
[![Build status](https://ci.appveyor.com/api/projects/status/6qm1l31k07dst0rk?svg=true)](https://ci.appveyor.com/project/w4123/dice)
[![Doc Build status](https://readthedocs.org/projects/dice-for-qq/badge/?badge=latest)](http://docs.kokona.tech)
[![Downloads](https://img.shields.io/github/downloads/w4123/dice/total.svg)](https://github.com/w4123/Dice/releases)
[![GitHub contributors](https://img.shields.io/github/contributors/w4123/dice.svg)](https://github.com/w4123/Dice/graphs/contributors)
[![GitHub last commit](https://img.shields.io/github/last-commit/w4123/dice.svg)](https://github.com/w4123/Dice/commits)

## 简介

Dice!是一款基于酷Q的QQ跑团掷骰机器人 交流QQ群:941980833或624807593(已满)

GitHub Page: <http://kokona.tech/>

Doc: <http://docs.kokona.tech/>

Latest Stable Release: [![GitHub release](https://img.shields.io/github/release/w4123/dice.svg)](https://github.com/w4123/Dice/releases) [![GitHub Release Date](https://img.shields.io/github/release-date/w4123/dice.svg)](https://github.com/w4123/Dice/releases)

Latest Release: [![GitHub release](https://img.shields.io/github/release-pre/w4123/dice.svg)](https://github.com/w4123/Dice/releases) [![GitHub Release Date](https://img.shields.io/github/release-date-pre/w4123/dice.svg)](https://github.com/w4123/Dice/releases)

## 使用手册

//部分功能现仅能由四号姬实现
//井号后表示使用样例，实际指令不需要加任何引号、括号，且以点号开头
.help 帮助指令

.bot on 骰子开关
================
用法：(@骰子).bot (on/off)([骰子QQ或末四位])
.bot						//群里所有骰子都会应答
.bot on 1605271653				//开启指定骰子
@kokona .bot on					//先at以开启指定骰子
.bot off 8350					//输入QQ末四位与QQ号等效
//群中只有管理员有权限开关骰子

.dismiss 退群指令
================
用法：(@骰子).dismiss ([骰子QQ或末四位])
.dismiss 1401
//群中只有管理员有权限移除骰子

.rules 规则速查
================
用法：.rules ([规则]):[待查词条] 或.rules set [规则]
.rules 跳跃					//复数规则有相同词条时，择一返回
.rules COC：大失败					//coc默认搜寻coc7的词条
.rules dnd：语言
.rules set dnd					//设置后有限查询dnd同名词条
.rules set					//清空默认规则，一般会先查询coc词条

.r 普通掷骰指令
================
用法：.r [掷骰表达式] ([掷骰原因])
[掷骰表达式]：([掷骰次数]#)[骰子个数]d[骰子面数](b[奖励骰个数])(p[惩罚骰个数])(k[取点数最大的骰子数])
.r						//骰子面数默认100，可通过.set修改默认值
.r 1d4+2 中型刀伤害				//个数范围1-100，面数范围1-1000，否则非法
.r 3#1d6 3发.22伤害				//每次结果分开发送
.r 1d10#	 乌波·萨斯拉的子嗣				//掷骰次数范围1-10
.r3d6k2						//取点数最大的2个骰子
.r3#p 手枪连射					//奖惩骰固定为百面骰
.rb2 瞄准后偷袭					//2个奖励骰
.rh 心理学					//暗骰，结果通过私聊发送
.rs1D10+1D6+3 沙鹰伤害				//省略单个骰子的点数，直接给结果
//现版本开头的r均可用o或d代替，但群聊中.ob会被识别为旁观指令

.ob 旁观模式
================
用法：.ob (exit/list/clr/on/off)
.ob						//加入旁观可以看到他人暗骰结果
.ob exit						//退出旁观模式
.ob list						//查看群内旁观者
.ob clr						//清除所有旁观者
.ob on						//全群允许旁观模式
.ob off						//禁用旁观模式
//暗骰与旁观仅在群聊中有效

.set 设置默认骰
================
用法：.set [默认骰子面数]
.set 20						//合法参数为1-1000
.set 						//重置为默认D100

.name 随机昵称
================
用法：.name (cn/jp/en)([生成数量])
.name 10					//默认三类名称随机生成
.name en					//后接cn/jp/en则限定生成中文/日文/英文名

.nn 设置昵称
================
用法：.nn [昵称] / .nn / .nnn(cn/jp/en)
.nn kp						//掷骰时昵称前的./！等符号会被自动忽略
.nn						//视为删除昵称
.nnn						//设置为随机昵称
.nnn jp						//设置限定随机昵称					
//昵称仅在群聊中有效，且每个群的昵称是独立的

.coc COC人物作成
================
用法：.coc([7/6])(d)([生成数量])
.coc 10						//默认生成7版人物	
.coc6d						//接d为详细作成，一次只能作成一个
//仅用作骰点法人物作成，可应用变体规则，参考.rules创建调查员的其他选项

.dnd DND人物作成
================
用法：.dnd([生成数量])
.dnd 5

.st 属性设置
================
用法：.st (del/clr/show) [属性名] [属性值]	
//目前用户的属性是所有群互通的，因此无法在pl多开时使用同一个骰子设定属性
.st力量:50 体质:55 体型:65 敏捷:45 外貌:70 智力:75 意志:35 教育:65 幸运:75
.st del kp裁决					//删除已保存的属性
.st clr						//清空所有记录 	
.st show 灵感
//部分COC属性会被视为同义词，如智力/灵感、理智/san、侦查/侦察

.ra/rc 检定指令
================
用法：.ra/rc [属性名]	([成功率])	
//已经.st时，可省略最后的参数
.ra体质 重伤检定
.rc kp裁决 99
.ra 敏捷-10					//修正后成功率必须在1-1000内
.rap 手枪					//奖惩骰至多9个
//当前版本下，ra不成功就不可能大成功，不失败就不可能大失败

.sc San Check指令
================
用法：.sc[成功损失]/[失败损失] ([当前san值])	
//已经.st了理智/san时，可省略最后的参数
.sc0/1 70
.sc1d10/1d100 直面外神
//当调用st的san时，san会自动更新为sc后的剩余值
//程序上可以损失负数的san，也就是可以用.sc-1d6/-1d6来回复san，但请避免这种奇怪操作

.ti/li 疯狂症状
================
.ti 临时疯狂症状
.li 总结疯狂症状
//适用coc7版规则，6版请自行用百面骰配合查表

.en 成长检定
================
用法：.en [技能名称]([技能值])
//已经.st时，可省略最后的参数
.en 教育 60 教育增强				//用法见.rules
.en 幸运 幸运成长					//调用st时，成长后的值会自动更新

.ri 先攻掷骰
================
用法：.ri([加值])([昵称])
.ri -1 某pc					//自动记入先攻列表
.ri +5 boss
//先攻仅在群聊中有效

.init 先攻列表
================
.init						//查看先攻列表
.init clr						//清除先攻列表

.w 骰池
================
用法：.w(w) [骰子个数]a[加骰参数]
//.w会直接给出结果而.ww会给出每个骰子的点数
//固定10面骰，每有一个骰子点数达到加骰参数，则加骰一次，最后计算点数达到8的骰子数
//具体用法请参考相关游戏规则

.me 第三人称动作
================
用法：.me([])[动作]
.me 笑出了声					//仅限群聊使用
.me 941980833 抱群主				//仅限私聊使用，此命令可伪装成骰子在群里说话
.me off						//禁用me
.me on						//启用me

.jrrp今日人品
================
.jrrp						//一天一个人品值
.jrrp off						//禁用jrrp
.jrrp on						//启用jrrp
//2.3.5版本后随机值为均匀分布

.welcome 欢迎词
================
用法：.welcome 
.welcome {@}欢迎{nick}入群！			//{@}视为at入群者，{nick}会替换为新人的昵称
.welcome					//清除欢迎词

.group 群管理操作
================
用法：.group ban [禁言对象QQ] [时长表达式（分钟）]	//时长小于0时视为解除禁言
.group ban 123456789 1d100

.master 插件管理
================
//该功能默认关闭，需要在酷Q应用菜单处切换，开启后master将接收被踢、被禁言的通知
.master 		//认主
.master delete //删除master
.master addgroup [入群发言]
.master bot [附加信息]
.master help [自定义帮助]
.master boton [群号]	//遥控关闭
.master botoff [群号]	//遥控开启
.master dismiss [群号]	//遥控退群

.draw 抽牌
================
.draw [牌堆名称] ([抽牌数量])	//抽牌数量不能超过牌堆数量
当前可用牌堆名：
硬币
东方角色
人偶暗示
人偶宝物
人偶记忆碎片
英雄天赋
调查员职业
煤气灯
个人描述
思想信念
重要之人
重要之人理由
意义非凡之地
宝贵之物
调查员特点
即时症状
总结症状
恐惧症状
狂躁症状
阵营
哈罗花色
冒险点子
麻将
扑克花色
扑克
塔罗牌
塔罗牌占卜
## 开发者

贡献者:w4123溯洄 jh123111 緋色月下、スイカを食う

感谢:Flandre Cirno 回転 他是王家乐。白いとう 哞哞哞哞哞哞哞哞哞哞哞哞 丸子 黯星 一盏大师 初音py2001 Coxxs orzFly 等(排名不分先后)(如有缺漏请务必联系溯洄:QQ1840686745) 

## 编译须知

从GitHub克隆源码时请不要直接从master分支克隆, 因为所有的更改都会提交到此分支, 很有可能包含最新的测试性更改, 未经过测试无法保证稳定 请选择Tag中最新的Release进行下载

请使用最新版Visual Studio **2015或2017** (或其独立编译器)进行编译, 项目主文件为Dice.sln, 编译时务必使用Win32模式否则无法编译成功

新增: 现在可以用GCC/Clang编译, 只测试了几个版本, 编译出现问题请反馈, 下面列出编译选项, 正在写cmake

- GCC: ` g++ -shared -static -std=c++14 -O2 -o com.w4123.dice.dll -Wl,--kill-at -I CQSDK\ -I Dice\ CQSDKCPP\*.cpp Dice\*.cpp Dice\CQP.lib -pthread -lWinInet -luser32 `
- Clang+MSVC: ` clang++ --target=i686-pc-windows-msvc  -m32 -shared -static -o com.w4123.dice.dll -Xclang -flto-visibility-public-std -std=c++14 -O2 -I CQSDK\ -I Dice\ CQSDKCPP\*.cpp Dice\*.cpp Dice\CQP.lib -lWinInet -luser32 -pthread -Wno-invalid-source-encoding `
- Clang+GCC: ` clang++ --target=i686-pc-windows-gnu -m32 -shared -static -o com.w4123.dice.dll -Xclang -flto-visibility-public-std -Wl,--kill-at -std=c++14 -O2 -I CQSDK\ -I Dice\ CQSDKCPP\*.cpp Dice\*.cpp Dice\CQP.lib -lWinInet -luser32 -pthread -Wno-invalid-source-encoding  `

编译后会得到com.w4123.dice.dll文件, 请勿更改此文件的名称! 请从Releases中下载对应的json文件(或自己编写), 放至酷Q app文件夹下, 并开启开发模式, 在应用管理中合成cpk文件即可正常使用

## Issue提交

提交Issue前请务必确认没有其他人提交过相同的Issue, 善用搜索功能 提交Bug时最好附有截图以及问题复现方法, 同时也欢迎新功能建议

## License

Dice! QQ Dice Robot for TRPG

Copyright (C) 2018-2019 w4123溯洄

This program is free software: you can redistribute it and/or modify it under the terms
of the GNU Affero General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this
program. If not, see <http://www.gnu.org/licenses/>.


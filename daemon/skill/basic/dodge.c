#include <ansi.h>

inherit SKILL;

string *dodge_msg = ({
    "但是和$p$l偏了几寸",
    "但是被$p机灵地躲开了",
    "但是$n身子一侧，闪了开去",
    "但是被$p及时避开",
    "但是$n已有准备，不慌不忙的躲开",
});

string query_class() { return "basic"; }

void create()
{
    seteuid(getuid());
    DAEMON_D->register_skill_daemon("dodge");
    setup();
}

// 显示用，用于 score
int dodge_show(object me, int ability)
{
    if( !me ) return 0;
    if( !me->skill_mapped("dodge") ) return 0;
    if( living(me) ) me->set_temp("evade_message", dodge_msg[random(sizeof(dodge_msg))]);
    return me->query_skill("dodge");
}

// 使用dodge
int dodge_using(object me, int ability)
{
    if( !me ) return 0;
    if( !me->skill_mapped("dodge") ) return 0;
    if( living(me) ) me->set_temp("evade_message", dodge_msg[random(sizeof(dodge_msg))]);

    // 增加踪跃闪躲技能经验
    me->improve_skill("dodge", 1);

    return me->query_skill("dodge");
}

// 技能增加经验, 不需做其它事
void skill_improved(object me, string skill) { }

// 技能升级后会呼叫此函式，必要！
void skill_advanced(object me, string skill)
{
    // 在这里设定技能下次升级所需的经验

    int lv;

    lv = me->query_skill(skill, 1);					// 技能等级

    switch( lv ) {
    	case 189: me->set_skill_require(skill, 3000000); break; // 关卡3 : 189升190
    	case 149: me->set_skill_require(skill, 2000000); break; // 关卡2 : 149升150
    	case  99: me->set_skill_require(skill, 1000000); break; // 关卡1 :  99升100
    	default:  me->set_skill_require(skill, lv*lv*15); break;
    }
}
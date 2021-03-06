/* savage_hit.c 暴击
*/

#include <ansi.h>
inherit SKILL;

string query_class() { return "basic"; }

void create()
{
    seteuid(getuid());
    DAEMON_D->register_skill_daemon("savage hit");
    setup();
}

// 显示用
int savage_hit_show(object me, int ability)
{
    if( !me ) return 0;
    if( !me->skill_mapped("savage hit") ) return 0;
        
    return me->query_skill("savage hit") * ability / 200;
}


// 使用暴击
int savage_hit_using(object me, int ability)
{
    if( !me ) return 0;
    if( !me->skill_mapped("savage hit") ) return 0;

    // 增加经验
    me->improve_skill("savage hit", 1);

    return me->query_skill("savage hit") * ability / 200;
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
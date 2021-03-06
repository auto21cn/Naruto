/* apheal2.c 
   状态 ap 之恢复效能，恢复量是固定值

   资料结构
   ([ "duration": 时效 ])
*/
#include <ansi.h>

inherit CONDITION;

private void create()
{
    seteuid(getuid());
    DAEMON_D->register_condition_daemon("apheal2");
}

void condition_update(object me, string cnd, mixed cnd_data)
{
    if( !mapp(cnd_data) || (cnd_data["duration"]--) < 1 ) {
        me->delete_condition(cnd);
        return;
    }

    // 恢复AP值
    me->heal_stat("ap", 20);
}


void food_condition_apply(object me, mapping cnd_data)
{
    mapping apheal;
    
    if( !mapp(apheal = me->query_condition("apheal2")) )
        apheal = ([]);
    
    apheal["duration"] += cnd_data["duration"];
    apheal["name"] = BGRN"Ｂ级提神术"NOR;
    
    if( apheal["duration"] <= 0 ) apheal["duration"] = 1;
    else if( apheal["duration"] > 180 ) apheal["duration"] = 180;
    
    me->set_condition("apheal2", apheal);
}

/*
void condition_apply(object me, string cnd, mapping cnd_data) { }
void condition_unapply(object me, string cnd, mapping cnd_data) { }
*/
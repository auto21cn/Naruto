// stake.c 假人 
// 2002/04/13 -Tmr
// update by -Acme-

inherit NPC;

#define LIFE 10000

void create()
{
    set_name("假人", ({ "wood stake", "stake", "_OBJ_STAKE_" }) );
    set_race("human");
    set("long", "一个木制的假人，专门给大家练功\用的。\n");
    // 假人的生命值
    set("stake_life", LIFE);
    setup();
    set_attr("str", 10);
    set_attr("con", 10);
    set_attr("dex", 10);
    set_attr("int", 10);
    delete_stat("hp");
}

// 绝对接受攻击
int accept_fight(object ob) { return 1; }

// 假人不会回避
varargs int evade(int ability, object from) { return 0; }

// 假人无法防御
varargs int defend(int damage, object from) { return damage; }

//假人不会攻击
varargs int attack(object opponent) { return 0; }

varargs int receive_damage(int damage, object from, object attacker)
{
    // 没有攻击者
    if( !attacker ) return 0;

    // 假人的生命 判断
    add("stake_life",-damage);
    if( query("stake_life") < 0 ) {
        message_vision("“喀啦”数声响，$N变成花花碎片，散落满地了。\n",this_object());
        remove_all_killer();
        call_out( (:destruct($1):),1,this_object());
    }       
    return damage;
}

#include <ansi.h>
#include <command.h>

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    int value;
    object ob;
    seteuid(geteuid(this_player(1)));

    if( me->is_busy() ) return notify_fail("你现在没空！\n");

    if( !arg || arg=="" ) return notify_fail("你要贡献什么？\n");

    ob = present(arg, me);
    if( !ob ) ob = present(arg, environment(me));
    if( !ob ) return notify_fail("这附近没有这样东西...\n");
    if( ob->is_corpse() && ob->query("usper") ) return notify_fail("你不能贡献玩家的尸体。\n");
    if( ob->is_character() && !ob->is_corpse() ) return notify_fail("你只能贡献“东西”。\n");
    if( ob->query("no_sac") ) return notify_fail("这样东西无法贡献...\n");
    if( ob->query("money_id") ) return notify_fail("这样东西无法贡献...\n");
    if( ob->query("no_get") ) return notify_fail("这样东西无法贡献...\n");
    if( ob->query("equipped") ) return notify_fail("要贡献这样东西，请先卸除装备...\n");
    if( ob->query("keep_this") ) return notify_fail("这样东西已经keep，请先解除keep才能贡献。\n");
    if( (!ob->query("value") || ob->query("value") == 0) && !ob->is_corpse() ) return notify_fail("这东西太破旧了，无法贡献。\n");

    message_vision(YEL"$N睁开眼睛，嘴里轻诵流影(Acme)的名号，慢慢地$n"+YEL"被一阵黑烟包住消失了。\n"NOR, me, ob);
 
    destruct(ob);
    if( ob ) {
        write("你无法将这个物件强制摧毁。\n");
        return 1;
    } 

    value = random(10)+1;
    write("你银行增加了" + chinese_number(value) + "银两。\n");
    me->add("bank",value);

    return 1;
}

int help()
{
    write(@TEXT
指令格式：sac <物品>

这个指令让你将没有用的东西贡献给大地，并获得一些银两。

TEXT
    );
    return 1;
}
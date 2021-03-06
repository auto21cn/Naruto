// guard.c

inherit F_CLEAN_UP;

int main(object me, string arg)
{
        object ob;

	if( me->query("life_form") == "ghost" )
		return notify_fail("人死了就一了百了, 快去找城隍复活吧!!\n");

        if( !arg || arg=="" ) {
                if( ob = me->query_guarding() ) {
                        write("你现在保护的对象是" + ob->name() + "。\n");
                        return 1;
                }
                write("你现在并没有选定保护的对象。\n");
                return 1;
        }

        if( arg=="cancel" ) {
                write("Ok.\n");
                me->guard_ob(0);
                return 1;
        }

        ob = present(arg, environment(me));
        if( !ob ) return notify_fail("这里没有这个人。\n");
        if( ob==me ) return notify_fail("你“理所当然”的会保护自己。\n");
        if( !userp(ob) )
                return notify_fail("他不需要你的保护。\n");
        if( me->is_fighting(ob) )
                return notify_fail("你想保护正在跟你战斗的对手？\n");

        me->guard_ob(ob);
        write("Ok.\n");

        return 1;
}

int help(object me)
{
        write(@TEXT
指令格式﹕guard [<战斗目标>|cancel]

你可以用这个指令设定你要保护的对象，当你保护的对象遭受攻击时，你的人物会在
第一时间加入战斗助战。

要取消 guard 模式﹐可以用 guard cancel﹐不加任何参数则显示你目前所保护的对
象·
TEXT
        );
        return 1;
}

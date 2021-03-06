/*  wield.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

inherit F_CLEAN_UP;

varargs int do_wield(object me, object ob);

int main(object me, string arg)
{
    string skill;
    object ob, *inv;
    int i, count;

    if( me->is_busy() ) return notify_fail("你现在没有空﹗\n");

    if( !arg ) return notify_fail("你要装备什么武器﹖\n");

    if(arg=="all") {
        inv = all_inventory(me);
        for(count=0, i=0; i<sizeof(inv); i++) {
            if( inv[i]->query("equipped") ) continue;
            if(do_wield(me, inv[i]) ) count ++;
        }
        write("Ok.\n");
        return 1;
    }

    // Get skill type if specified.
    sscanf(arg, "%s as %s", arg, skill);

    if( !objectp(ob = present(arg, me)) )
        return notify_fail("你身上没有这样东西。\n");

    return do_wield(me, ob, skill);
}

varargs int do_wield(object me, object ob, string skill)
{
    string str;

    if( ob->wield(skill) ) {
        if( !stringp(str = ob->query("wield_msg")) )
            str = "$N装备$n作武器。\n";
        message_vision(str, me, ob);
        return 1;
    } else
        return 0;
}

int help(object me)
{
    write(@HELP
指令格式﹕wield <武器名称> [as <左、右手>]
 
这个指令让你装备某件物品作武器，你必需要拥有这样物品。

wield blade                 将刀子装备在右手，不加参数是右手
wield blade as righthand    将刀子装备在右手
wield blade as lefthand     将刀子装备在左手
wield blade 2 as twohanded  将身上的第二把刀子装备在双手

某件武器的握法可能有一种以上，握法不同时引发的武器威力可能也不同，
你可以在握起后 identify 来获得其他资讯。
HELP);
    return 1;
}

/*  force.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <command.h>

inherit F_CLEAN_UP;

static void create()
{
    seteuid(getuid());
}

int
main(object me, string arg)
{
    string dest, cmd;
    object ob;

    SECURED_ARCH_COMMAND;

    if( me != this_player(1) ) return 0;

    if( !arg || sscanf(arg, "%s to %s", dest, cmd) != 2 )
        return notify_fail("指令格式﹕force <某人> to <指令>\n");

    if( !(ob = present(dest, environment(me))) )
        return notify_fail("这里没有 " + dest + "。\n");

    if( !ob->is_character() )
        return notify_fail("这个物件不能执行命令。\n");

    if( wiz_level(me) <= wiz_level(ob) )
        return notify_fail("你没有强制" + ob->name() + "执行指令的权力。\n");

    if( userp(ob) )
        log_file("FORCE_PLAYER", sprintf("[%s] %s forces %s to %s\n",  
            ctime(time()), geteuid(me), geteuid(ob), cmd));

    return ob->force_me(cmd);
}

int help(object me)
{
    write(@Help
指令格式﹕force <某人> to <指令>

强迫某人做某事, 但你的巫师等级必须比对方高, 而且必须是在同一个房间里。
Help
    );
    return 1;
}

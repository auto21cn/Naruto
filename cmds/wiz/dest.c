/*  dest.c

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

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    string option, target, msg;
    object obj;

    SECURED_WIZARD_COMMAND;

    seteuid(geteuid(this_player(1)));

    if (!arg) return notify_fail("指令格式 : dest <物件之名称或档名>\n" );

    if( sscanf(arg, "-%s %s", option, target)!=2 ) target = arg;

    if( option == "r" ) {
	if( wizhood(me)=="(admin)" ) seteuid(ROOT_UID);
	else return notify_fail("你没有这个权限。\n");
    }

    obj = find_object(target);
    if (!obj) obj = present(target, me);
    if (!obj) obj = present(target, environment(me));
    if (!obj) obj = find_object( resolve_path(me->query("cwd"), target) );
    if (!obj) return notify_fail("没有这样物件....。\n");

    if( environment(me)==environment(obj) )
    if( !stringp(msg = me->query("env/msg_dest")) )
        msg = "$N召唤出一个黑洞﹐将$n吞没了。\n";
    else
        message_vision(msg + "\n", me, obj);

    destruct(obj);
    if(obj) write("你无法将这个物件强制摧毁。\n");
    else write("Ok.\n");

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : dest [-r] <物件之名称或档名>

利用此一指令可将一个物件(object)或物件定义(class)从记忆体中清除﹐若清除物
件定义(即﹕指定档名的情况下)则下一次参考到这个物件的时候会重新将它编译。

具有 (admin) 身分的巫师可以用 -r 选项以 ROOT_UID 身分来清除被保护的物件如
使用者。

若物件在被清除过程中发生了错误﹐除使用者以外的物件仍然会被强制清除( 使用者
物件只有在使用 ROOT_UID 身份时才能强制清除 )﹐而其错误讯息会被写至
/log/destruct 中。
  
参考资料﹕ destruct()
HELP
    );
    return 1;
}

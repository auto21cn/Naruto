/*  chfn.c

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

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    object link;

    SECURED_COMMAND;

    if( link = me->link() ) {
	write("您的电子邮件信箱地址 [" + link->query("email") + "]: ");
	input_to("get_email", link);
	return 1;
    }
    return notify_fail("断线使用者不能修改个人资料。\n");
}

void get_email(string new_email, object link)
{
    if( new_email != "" )
	link->set("email", new_email);

    write("您的自我介绍: (finger时显示)\n");
    link->body()->edit((: call_other, __FILE__, "get_profile", link :));
}

void get_profile(object link, string str)
{
    link->set("profile", str);
    write("您的签名格式: (post时用)\n");
    link->body()->edit((: call_other, __FILE__, "get_signature", link :));
}

void get_signature(object link, string str)
{
    if( strlen(str) > 400 )
	write("签名格式太长﹗请重新设定。\n");
    else
	link->set("signature", str);
    write("请记得用 save 储存您所作的修改。\n");
}

int help(object me)
{
    write(@HELP
指令格式 : chfn

用来修改你的个人资料，包括电子邮件，自我介绍，以及签名。
HELP
    );
    return 1;
}

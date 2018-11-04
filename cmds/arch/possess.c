/*  possess.c

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

int
main(object me, string arg)
{
    object ob;

    SECURED_ARCH_COMMAND;

    if( !arg || me != this_player(1)) return 0;
    ob = present(arg, environment(me));
    if( !ob || !living(ob))
        return notify_fail("����û��������\n");
    if( geteuid(ob) != geteuid(me) )
        return notify_fail( ob->name() + "�� euid ���㲻�ϩo�޷����и�����\n");
    if( ob->query_temp("body_ob") )
        return notify_fail( ob->name() + "�Ѿ����˸������ˡ�\n");
    message_vision("$N����һ����â���$n���ڡ�\n", me, ob);
    LOGIN_D->enter_world(me, ob, 1);

    return 1;
}

int help()
{
    write(@TEXT
ָ���ʽ�sposssess <Ŀ������>

��������һ�������ϩo�����������������ͬ�� EUID��
�������� quit ָ���ص�ԭ�������塣
TEXT
    );
    return 1;
}
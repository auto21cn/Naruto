/*  cleanup.c

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

private void create() { seteuid(getuid()); }

int main(object me, string str)
{
    object ob, where, *ob_list;
    int old_size, i;

    if (!str) return notify_fail("指令格式﹕cleanup <档名>\n"); 

    str = resolve_path(me->query("cwd"), str);
    ob_list = children(str);
    old_size = sizeof(ob_list);
    ob_list->clean_up();
    foreach(ob in ob_list)
        if( ob && clonep(ob) && !environment(ob) )
//        if( ob && ob->query_volume()==0 )
            destruct(ob);
    ob_list -= ({ 0 });
    printf("清除 %d 个物件。\n", old_size - sizeof(ob_list));
    return 1;
}

int help(object me)
{
    write(@HELP
指令格式﹕cleanup <档名>

对所有指定档名的物件呼叫 clean_up﹐并且清除所有被复制出来的﹐但是又没有
被放在另一个物件中的散失物件。
HELP
    );
    return 1;
}


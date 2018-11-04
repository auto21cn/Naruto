/*  info.c

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

int help(object me);

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    object obj;
    string str;
    mixed file_stat;

    if (!arg) return help(me);

    if( arg=="me" ) obj = me;
    else if( arg=="my_link" ) obj = me->link();
    else {
        obj = present(arg, me);
        if (!obj) obj = present(arg, environment(me));
        if (!obj) obj = find_object( resolve_path(me->query("cwd"), arg) );
        if (!obj) return notify_fail("û���������....��\n");
    }

    printf("��� [%O]\n-----------------------------------------------------\n", obj);
    file_stat = stat(base_name(obj) + ".c");
    write("�����s\t\t" + base_name(obj) + ".c\n");
    if( file_stat[2] )
        write("����ʱ��s\t" + CHINESE_D->chinese_period(time()-file_stat[2]) + "\n");
    write("Ȩ�ީs\t\tuid = " + getuid(obj) + ", euid = " + geteuid(obj) + "\n");
    write("�ȼ��s\t\t" + wizhood(obj) + "\n");
    write("ʹ�ü�����s\t" + memory_info(obj) + "\n");
    str = "";
    if( living(obj) )		str += "���� ";
    if( userp(obj) )		str += "ʹ���� ";
    if( interactive(obj) )	str += "��̸ʽ ";
    if( wizardp(obj) )		str += "��ʦ ";
    if( clonep(obj) )		str += "���� ";
    if( virtualp(obj) )		str += "���� ";
    if( query_heart_beat(obj) )	str += "����:" + query_heart_beat(obj) + " ";
    write("���ԩs\t\t" + str + "\n");
    write("���Ƹ����s\t" + sizeof(children(base_name(obj)+".c")) + "\n");
    write("�ο�����s\t" + refs(obj) + "\n");
    return 1;
}

int help(object me)
{
  write(@HELP
ָ���ʽ : info <���֮���ƻ���>

���ô�һָ��ɵ�֪һЩ�йظ��������Ѷ��
HELP
    );
    return 1;
}
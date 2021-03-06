/*  option.h

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/
#include <ansi.h>
#include <command.h>

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    string term;
    mixed v;

    if( !arg )
    {
        mapping opt = me->query("option");
        string str = "你目前设定的使用者选项；\n";

        if( !mapp(opt) || !sizeof(opt) )
            str += "    无。\n";
        else
            foreach(term, v in opt)
                str += sprintf("    %-20s：%O\n", term, v);
        write(str);
        return 1;
    }

    if( sscanf(arg, "%s %d", term, v)==2 || sscanf(arg, "%s %s", term, v)==2 ) {
        if( !v ) me->delete("option/" + term);
        else me->set("option/" + term, v);
        if( term == "map_unfreeze" ) {
            if( environment(me)->is_area() ) {
                if( !v ) {
                    me->set_temp("window/freeze", 1);
                    write(CLR+R(14, me->query_temp("window/height")));
                } else {
                    me->delete_temp("window/freeze");
                    write(SAVEC+R("","")+RESTC);
                }
            } else {
                if( v ) {
                    me->delete_temp("window/freeze");
                    write(SAVEC+R("","")+RESTC);
                }
            }
        }
    } else return notify_fail("指令格式：option <选项> <设定值>\n");

    write("Ok.\n");
    return 1;
}

int help()
{
    write(@TEXT
指令格式：option <选项> <设定值>

这个指令让你设定一些有关使用者介面的选项。其中设定值 0 皆为系统预设值。

选项：                说明：                                  设定值：
map_unfreeze          地图系统中取消冻结地图画面于视窗上方    (0:关, 1:开)
map_hidden            地图系统是否隐藏地图画面                (0:否, 1:是)
map_obj_hidden        地图系统中是否隐藏环境物品              (0:否, 1:是)
map_obj_icon_hidden   地图系统中是否隐藏即时的物品图示        (0:否, 1:是)
map_block             地图系统中是否将图示以障碍物来区分      (0:否, 1:是)
map_through           地图系统中是否启用穿墙模式(巫师专用)    (0:否, 1:是)
brief_room            是否简化房间的详细叙述                  (0:否, 1:是)
brief_short           是否只显示物品及人物的中文名称          (0:否, 1:是)
TEXT);
    return 1;
}

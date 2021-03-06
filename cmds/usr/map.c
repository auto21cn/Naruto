/* map.c */

#include <command.h>
#include <ansi.h>

inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
	string file, text;
	object area;
	
	if( !(area = environment(me)) ) return 0;
	if( !area->is_area() ) return notify_fail("这个指令只适用在区域环境中。\n");
	
	// 区域大，分成左、右二部分的图
	if( area->query("x_axis_size") > 49 ) {
		if( me->query("area_info/x_axis") <= 49 ) file = file_name(area) + "_left.ansi";
		else file = file_name(area) + "_right.ansi";
	} else file = file_name(area) + ".ansi";

    if( file_size(file) <= 0 ) return notify_fail("目前这个区域没有地图档。\n");
    
    if( !(text = read_file(file)) ) text = "";

    me->start_more(text);

	return 1;
}

int help(object me)
{
    write(@HELP
指令格式： map

这个指令可以让你查看区域环境的全图，只适用在区域环境中，一般
房间环境无法使用。
HELP);
	return 1;
}
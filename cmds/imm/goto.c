/* goto.c */
#include <command.h>

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    int x, y;
    string file, out_msg, in_msg;
    object obj;

    SECURED_COMMAND;

    if( !arg ) return notify_fail("你要去哪里﹖\n");

    if( sscanf(arg, "goto_out_msg %s", out_msg) == 1 ) {
    	me->set("goto_out_msg", color(out_msg));
    	write("Ok.\n");
    	return 1;
    } else if( sscanf(arg, "goto_in_msg %s", in_msg ) == 1 ) {
    	me->set("goto_in_msg", color(in_msg));
    	write("Ok.\n");
    	return 1;
    }

    if( me->query("goto_out_msg") ) out_msg = me->query("goto_out_msg") + "\n";
    else out_msg = "只见一阵烟雾过后，$N的身影已经不见了。\n";

    if( me->query("goto_in_msg") ) in_msg = me->query("goto_in_msg") + "\n";
    else in_msg = "只见一阵烟雾过后，$N的身影已经来到了这里。\n";

    // goto 到区域 by -Acme-
    if( sscanf(arg, "%s %d %d", file, x, y) == 3 ) {
        file = resolve_path(me->query("cwd"), file);
        if( !sscanf(file, "%*s.c") ) file += ".c";
        if( file_size(file) >=0 && (obj=load_object(file)) ) {
			if( !obj->is_area() ) return 1;
	    	message_vision(out_msg, me);
			area_move(obj, me, x, y);
		    message_vision(in_msg, me);
		    return 1;
        } else {
        	write("这个区域档不存在。\n");
        	return 1;
        }
        
    // 自已身处域区的座标goto
    } else if( sscanf(arg, "%d %d", x, y) == 2 ) {
    	if( !environment(me)->is_area() ) return 1;
    	message_vision(out_msg, me);
		area_move(environment(me), me, x, y);
	    message_vision(in_msg, me);
   		return 1;
    }

    obj = find_player(arg);
    if( !obj ) obj = find_living(arg);
    if( obj == me ) {
    	write("你要移到自已身旁？\n");
    	return 1;
    }

    // 移到一般房间
    if( !obj || !obj->visible(me)) {
        arg = resolve_path(me->query("cwd"), arg);
        if( !sscanf(arg, "%*s.c") ) arg += ".c";
        if( file_size(arg) >=0 && (obj=load_object(arg)) ) {
            if( obj->is_area() ) {
            	write("移动到区域物件，必需加上座标才能移动。\n");
            	return 1;
            }
   	        message_vision(out_msg, me);
            me->move(obj);
   	        message_vision(in_msg, me);
    		return 1;
        } else return notify_fail("没有这个玩家、生物、或地方。\n");
    }

    if( !environment(obj) ) return notify_fail("这个物件没有环境可以 goto。\n");

    if( environment(obj)->is_area() ) {
    	if( area_environment(obj, me) ) return notify_fail("你已经在他身旁了。\n");

        message_vision(out_msg, me);
		if( !area_move(environment(obj), me, obj->query("area_info/x_axis"), obj->query("area_info/y_axis")) ) {
            tell_object(me, "因为某种理由，你的 goto 失败了。\n");
		}
    } else {
        message_vision(out_msg, me);
        if( !me->move(environment(obj)) ) {
            tell_object(me, "因为某种理由，你的 goto 失败了。\n");
        }
    }

    message_vision(in_msg, me);

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : goto <目标>
 
这个指令会将你传送到指定的目标。目标可以是一个living 或房间
的档名。如果目标是living，你会被移到跟那个人同样的环境。

你可以自行设定goto的离开讯息，或是goto的进来讯息。
如： goto goto_out_msg $HIW$你可以听到$N哇哈哈的一声，离开了这里。
     goto goto_in_msg  $HIW$你可以听到$N哇哈哈的一声，来到了这里。
使用色码系统在最后会自动加上$NOR$所以可以不需要再加一次。
HELP);
    return 1;
}

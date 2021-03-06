/*  update.c - compile/load an object

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

int update_player(object me);

int main(object me, string file)
{
    int i;
    object obj, *inv;
    string err;

    SECURED_WIZARD_COMMAND;

    seteuid(geteuid(me));

    if( !file ) file = me->query("cwf");
    if( !file ) return notify_fail("你要重新编译什么档案﹖\n");

    // We check if the file string with file_size directly here is
    // important because the resolve_path may be buggy that never
    // return correct path, so we preserve the chance to update it
    // by typing its full-name here.

    if( file_size(file) < 0 ) {
		switch(file) {
			case "here" : file = file_name(environment(me)); break;
			case "me" : return update_player(me);
			default:
	    		if( (obj = present(file, environment(me))) && interactive(obj) )
					return update_player(obj);
		}

		file = resolve_path(me->query("cwd"), file);

		if( file_size(file) < 0 && file[<2..<1] != ".c" ) file += ".c";
		if( file_size(file) < 0 )	/* -1: not exist, -2: directory */
	    	return notify_fail("没有 " + file + " 这个档案。\n");
    }

    if( file==__FILE__ ) {
		write("下一次使用 update 指令将会重新编译 update 指令。\n");
		destruct(this_object());
		return 1;
    }

    me->set("cwf", file);

    if (obj = find_object(file)) {
		if( obj==environment(me) ) {
		    if( file_name(obj)==VOID_OB )
				return notify_fail("你不能在 VOID_OB 里重新编译 VOID_OB。\n");
		    inv = all_inventory(obj);
		    i = sizeof(inv);
	    	while(i--)
				if( userp(inv[i]) ) {
			    	inv[i]->set_temp("last_location", file);
			    	inv[i]->move(VOID_OB, 1);
				} else inv[i] = 0;
		}
		destruct(obj);
    }

    if (obj) return notify_fail("无法清除旧程式码。\n");

    log_file("static/UPDATE", sprintf("%s(%s) 编译 %s at %s\n", me->name(1), geteuid(me), file,ctime(time()) ) );
    write("重新编译 " + file + " ...\n");
    err = catch(obj = load_object(file));

    if(!err) {
		write("成功\。\n");
		i = sizeof(inv);
		while(i--)
		    if( inv[i] && userp(inv[i]) ) {
		        if( obj->is_area() ) area_move(obj, inv[i], inv[i]->query("area_info/x_axis"), inv[i]->query("area_info/y_axis"));
		        else inv[i]->move(obj, 1);
		    }
    } else write("编译失败﹕" + err);
                
    return 1;
}

int update_player(object me)
{
    object env, link_ob, obj;

    env = environment(me);

    link_ob = me->link();
    if( !link_ob ) {
	// If update a linkdead player, simply destruct it.
	me->save();
	seteuid(getuid());
	destruct(me);
	return 1;
    }

    seteuid(getuid());
    if( !(obj = LOGIN_D->make_body(link_ob)) )
	return notify_fail("无法复制新的身体物件﹗\n");

    exec(link_ob, me);

    // Make a careful check before copying body.
    if( (string)me->query_save_file() == (string)obj->query_save_file() ) {
	    me->save();
	    obj->restore();
    }
    seteuid(getuid());
    destruct(me);

    LOGIN_D->enter_world(link_ob, obj, 1);

    write("身体更新完毕。\n\n");
    
    if( env->is_area() ) area_move(env, me, me->query("area_info/x_axis"), me->query("area_info/y_axis"));
    else obj->move(env);
    obj->write_prompt();

    return 1;
}

int help(object me)
{
  write(@HELP
指令格式：update <档名|here|me|玩家名>
 
这个指令可以更新档案, 并将新档的内容载入记忆体内. 若目标为
'here' 则更新所在环境. 若目标为 'me' 则更新自己的人物. 若目
标为玩家则可更新玩家物件.
 
HELP
    );
    return 1;
}

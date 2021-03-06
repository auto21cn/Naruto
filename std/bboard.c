/*  bboard.c */

#include <ansi.h>

#define	BOARD_CAPACITY	query("capacity")
#define	NO_PLAYER_POST	query("NO_PLAYER_POST")

inherit ITEM;
inherit F_SAVE;

void setup()
{
    string loc;

    seteuid(getuid());
    ::setup();
    if( stringp(loc = query("location")) )
	move(loc);
    set("no_get", 1);
    restore();
}

void init()
{
    add_action("do_post", "post");
    add_action("do_read", "read");
    add_action("do_discard", "discard");
    add_action("do_followup", "followup");
    add_action("do_mark","mark");
    if( wizhood(this_player()) == "(admin)" ) add_action("do_save_article", "savepost");
}

string query_save_file()
{
    string id;

    if( !stringp(id = query("board_id")) ) return 0;
    return DATA_DIR + "board/" + id + __SAVE_EXTENSION__;
}

string short()
{
    mapping *notes;
    int i, unread, last_read_time;

    notes = query("notes");
    if( !pointerp(notes) || !sizeof(notes) )
	return ::short() + " [ 没有任何留言 ]";

    if( this_player() && this_player()->link() ) {
	last_read_time = (int)this_player()->link()->query("board_last_read/" + (string)query("board_id"));
	for(unread = 0, i=sizeof(notes)-1; i>=0; i--, unread ++)
	    if( notes[i]["time"] <= last_read_time ) break;
    }
    if( unread )
	return sprintf("%s [ %d 张留言﹐%d 张未读 ]", ::short(), sizeof(notes), unread);
    else
	return sprintf("%s [ %d 张留言 ]", ::short(), sizeof(notes));
}

string long()
{
    mapping *notes;
    int i, last_time_read;
    string msg, tmp;

    notes = query("notes");
    msg = query("long");
    if( !msg ) msg = "";

    last_time_read = this_player()->link()->query("board_last_read/" + (string)query("board_id"));

    msg += "╭────────────────────────────────────────╮\n";
    msg += sprintf("│%-67s  容量%|5d篇│\n", query("name"), BOARD_CAPACITY);
    msg += "├────────────────────────────────────────┤\n";
    msg += sprintf("│"BGRN"%s"NOR" "BRED"%|33s"NOR" "BBLU"%|24s"NOR" "BMAG"%|16s"NOR"│\n",
                   "编号", "主题", "留言者", "时间");
    if( pointerp(notes) && sizeof(notes) ) {
         for(i=0; i<sizeof(notes); i++) {
             msg += sprintf("│%|4s%s%-33s %|24s %16s│\n",
	                        notes[i]["time"] > last_time_read ? sprintf(HIC"%|4d"NOR, i+1) : ""+(i+1),
             	            notes[i]["mark"] ? HIY"M"NOR : " ",
             	            strlen(notes[i]["title"]) > 32 ? notes[i]["title"][0..29] + ".." : notes[i]["title"],
             	            notes[i]["author"],
             	            CHINESE_D->chinese_date_brief(notes[i]["time"]));
         }
    }
    msg += "├────────────────────────────────────────┤\n";
    msg += "│使用方法请输入 help board                                                       │\n";
    msg += "╰────────────────────────────────────────╯";

    return msg;
}

// This is the callback function to process the string returned from the
// editor defined in F_EDIT of player object.
void done_post(object me, mapping note, string text)
{
    mapping *notes;
    string sig;
    int i=0,j=0;

    if( stringp(sig = me->link()->query("signature")) )
	text += "--\n" + sig;   // 加 -- 好在 followup 可以知道

    note["msg"] = text+NOR;
    notes = query("notes");
    if( !pointerp(notes) || !sizeof(notes) )
	notes = ({ note });
    else
	notes += ({ note });

    // Truncate the notes if maximum capacity exceeded.
    if( sizeof(notes) > BOARD_CAPACITY ) {
	while( i < BOARD_CAPACITY && !j ) {
		if( notes[i]["mark"] ) i++;
		else {
			notes -= ({ notes[i] });
			j++;
		}
	}

    }

    set("notes", notes);
    tell_object(me, "留言完毕。\n");

    save();
    return;
}

int do_post(string arg)
{
    mapping note;
    if(!arg) return notify_fail("留言请指定一个标题。\n");

    // add by ueiren ..
    if ( NO_PLAYER_POST && (!wizardp(this_player())))
    return notify_fail("玩家不可在此公布栏留言。\n");

    if( sizeof(query("notes")) > BOARD_CAPACITY )
	return notify_fail("留言版没有空位，无法进行留言。\n");

    if( strlen(arg) > 40 )
	return notify_fail("您的标题太长了﹐换一个 40 个字元以内的吧。\n");
        
    note = allocate_mapping(4);
    note["title"] = arg;
    note["author"] = this_player()->name() + "(" +this_player()->query("id") + ")";
    note["time"] = time();
    this_player()->edit( (: done_post, this_player(), note :) );
    return 1;
}

int do_read(string arg)
{
    int num, tmp;
    mapping *notes, last_read_time;
    string myid, msg;

    last_read_time = this_player()->link()->query("board_last_read");
    myid = query("board_id");
    notes = query("notes");

    if( !pointerp(notes) || !sizeof(notes) )
	return notify_fail("留言板上目前没有任何留言。\n");

    if( !arg ) return notify_fail("指令格式﹕read <留言编号>|new|next\n");
    if( arg=="new" || arg=="next" ) {
	if( !mapp(last_read_time) || undefinedp(last_read_time[myid]) )
	    num = 1;
	else
	    for(num = 1; num<=sizeof(notes); num++)
		if( notes[num-1]["time"] > last_read_time[myid] ) break;
                        
    } else if( !sscanf(arg, "%d", num) )
	return notify_fail("你要读第几张留言﹖\n");

    if( num < 1 || num > sizeof(notes) ) return notify_fail("没有这张留言。\n");
    num--;

    msg = sprintf(BGRN"%|4d"NOR" "BRED"%-33s"NOR" "BBLU"%|24s"NOR" "BMAG"%|16s"NOR"\n",
    	           num+1,
     	           strlen(notes[num]["title"]) > 32 ? notes[num]["title"][0..29] + ".." : notes[num]["title"],
             	   notes[num]["author"],
                   CHINESE_D->chinese_date_brief(notes[num]["time"]) );
//             	   ctime(notes[num]["time"])[0..9] + ctime(notes[num]["time"])[19..23] );
    msg += "────────────────────────────────────────\n";
    msg += notes[num]["msg"];
    msg += "────────────────────────────────────────";

    this_player()->start_more(msg);

    if( !mapp(last_read_time) ) {
	    this_player()->link()->set("board_last_read", ([ myid: notes[num]["time"] ]) );
    } else if( undefinedp(last_read_time[myid]) || notes[num]["time"] > last_read_time[myid] )
	    last_read_time[myid] = notes[num]["time"];

    return 1;
}


int do_discard(string arg)
{
    mapping *notes;
    int num;

    if( !arg || sscanf(arg, "%d", num)!=1 )
	return notify_fail("指令格式﹕discard <留言编号>\n");
    notes = query("notes");
    if( !arrayp(notes) || num < 1 || num > sizeof(notes) )
	return notify_fail("没有这张留言。\n");
    num--;
    /* 多个判断，版主应该要能删留言 by -acme- */
    if( notes[num]["author"] != (string) this_player(1)->query("name")+ "(" + this_player(1)->query("id") + ")"
    &&	wiz_level(this_player(1)) < 4 && query("board_owner") != this_player()->query_id() )
	return notify_fail("这个留言不是你写的。\n");

    if( notes[num]["mark"] ) 
	return notify_fail("这篇留言已设定标记，请先清除标记。\n");

    notes = notes[0..num-1] + notes[num+1..sizeof(notes)-1];
    set("notes", notes);
    save();
    write("删除第 " + (num+1) + " 号留言....Ok。\n");
    return 1;
}

int do_save_article(string arg)
{
    int num;
    string file;
    mapping *notes;

    if( !wizardp(this_player()) ) return 0;
    if( !arg || sscanf(arg, "%d to %s", num, file)!=2 ) return 0;
        
    if( !arrayp(notes = query("notes"))
    ||	num<1
    ||	num>sizeof(notes) )
	return notify_fail("没有这张留言。\n");
    num--;
    file = resolve_path(this_player()->query("cwd"), file);
    if( write_file(file, notes[num]["msg"]) )
	write("Ok.\n");
    else
	return notify_fail("储存失败。\n");
    return 1;
}

int do_followup(string str)
{
    mapping *notes, note;
    string *text, title;
    int i, num;
        
    // add by dragoon
    if ( NO_PLAYER_POST && (!wizardp(this_player())))
	return notify_fail("玩家不可在此公布栏留言。\n");

    if( !str )
	return notify_fail("指令格式﹕followup <留言编号> [新的标题]\n");

    if( sscanf(str, "%d %s", num, title) != 2 )
	if( sscanf(str, "%d", num) != 1 )
	    return notify_fail("你要回第几篇的留言﹖\n");
        
    notes = query("notes");
        
    if( num < 1 || num > sizeof(notes) )
	return notify_fail("没有这篇留言。\n");

    num--;
    if( !title ) title = "Re﹕" + notes[num]["title"];
    note = allocate_mapping(4);
    note["title"]  = title;
    note["author"] = this_player()->query("name") + "(" + this_player()->query("id") + ")";
    note["time"]   = time();

    text = explode( notes[num]["msg"], "\n" );
    // title, num 这两个变数拿来再利用
    title = sprintf(GRN"> %s 在 %s 留下这篇留言﹕\n"NOR,
	notes[num]["author"],
	"(" + ctime(notes[num]["time"])[0..9] + ")" );

    num = sizeof(text);
    for( i=0; i<num; i++ ) 
    {
	// 空行跳过, 上上一篇跳过
	if( text[i] == "" || strsrch(text[i], "> ") != -1)
	    continue;
	// 签名档, 结束
	if( text[i][0..1] == "--" )
	    i=num;
	else title = title + GRN + "> " + text[i] + NOR + "\n";
    }
    note["msg"] = title;
    this_player()->edit( (: done_post, this_player(), note :), title );
    return 1;
}

int do_mark(string arg)
{
    mapping *notes;
    int num;

    if( !arg || sscanf(arg, "%d", num)!=1 )
	return notify_fail("指令格式﹕mark <留言编号>\n");
    notes = query("notes");
    if( !arrayp(notes) || num < 1 || num > sizeof(notes) )
	return notify_fail("没有这张留言。\n");
    num--;

    if(	wiz_level(this_player(1)) < 4 && query("board_owner") != this_player()->query_id() )
	return notify_fail("你没有权利标记留言。\n");

    if( !notes[num]["mark"] ) {
	write("将第"+(num+1)+"篇留言设定标记。\n");
        notes[num]["mark"]=1;
    } else {
	write("将第"+(num+1)+"篇留言取消标记。\n");
        notes[num]["mark"]=0;
    }
    save();
    return 1;
}

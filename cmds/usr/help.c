/*  help.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include "/doc/help.h"

inherit F_CLEAN_UP;

static string *default_search = DEFAULT_SEARCH_PATHS;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    string file, *search;

    if( !arg ) {
        cat(HELP_DIR + "help/topics");
        return 1;
    }

    arg = replace_string(arg, " ", "_");

    // Else, try if a command name is specified.
    seteuid(getuid());
    if( stringp(file = me->find_command(arg)) ) {
        notify_fail("有这个指令存在﹐但是并没有详细的说明文件。\n");
        return file->help(me);
    }

    // Support efun/lfun help with same name as other topics such as
    // ed() and ed command.
    sscanf(arg, "%s()", arg);

    search = DEFAULT_SEARCH_PATHS;
    if( wizardp(me) )
        search += WIZARD_SEARCH_PATHS;
        
    foreach(string path in search)
    {
	if( file_size(path + arg) < 0 ) continue;
	if( wizardp(me) )
	    write("说明文件：" + path + arg
		+ "\n---------------------------------------------------------------------\n");
	me->start_more( read_file(path + arg) ); 
	return 1;
    }

    return notify_fail("没有针对这项主题的说明文件。\n");
}

int help(object me)
{
    write(@HELP
指令格式﹕help <主题>              例如﹕> help all
          help <函数名称>()        例如﹕> help call_out()

这个指令提供你针对某一主题的详细说明文件﹐若是不指定主题﹐则提供你有关主题
的文件﹐所有的主题与函数名称皆为英文。
HELP
    );
    if( wizardp(me) )
        write(@HELP
本指令会先搜寻主题是否为 command, ( 若呼叫者为巫师, 则接著搜寻 /doc/help.h
中的 WIZARD_SEARCH_PATHS ), 再搜寻 DEFAULT_SEARCH_PATHS。 
若有同档名之文件处于这些搜寻位置中, 则位于后搜路径者将不被搜寻, 即只看到先
前搜寻之 doc, 编制新文件时请留意。
HELP
        );
    return 1;
}

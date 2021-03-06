/*  wizlist.c */

#include <ansi.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    int i;
    string *list, str;
    string *admin = ({}), *arch = ({}), *wizard = ({}), *apprentice = ({}), *immortal = ({}), *unknown = ({});

    list = SECURITY_D->query_wizlist();
    for( i = 0; i < sizeof( list ); i++ ) {
        switch( SECURITY_D->get_status( list[i] ) ) {
            case "(admin)": admin += ({ list[i] }); break;
            case "(arch)": arch += ({ list[i] }); break;
            case "(wizard)": wizard += ({ list[i] }); break;
            case "(apprentice)": apprentice += ({ list[i] }); break;
            case "(immortal)": immortal += ({ list[i] }); break;
            default: unknown += ({ list[i] }); break;
        }
    }

    str  = "╭──────────────────────────────────╮\n";
    str += sprintf("│%-68s│\n", MUD_NAME + "管理群");
    str += "├──────────────────────────────────╯";

    str += "\n│"HIY+BRED"ＡＤＭＩＮ          \n"NOR"│";
    if( admin ) for(i=0;i<sizeof(admin);i++) {
        str += sprintf("%-12s%s", admin[i], (i % 6 == 5 ) ? "\n│" : "");
    }

    str += "\n│\n│"HIY+BRED"ＡＲＣＨ            \n"NOR"│";
    if( arch ) for(i=0;i<sizeof(arch);i++) {
        str += sprintf("%-12s%s", arch[i], (i % 6 == 5 ) ? "\n│" : "");
    }

    str += "\n│\n│"HIY+BRED"ＷＩＺＡＲＤ        \n"NOR"│";
    if( wizard ) for(i=0;i<sizeof(wizard);i++) {
        str += sprintf("%-12s%s", wizard[i], (i % 6 == 5 ) ? "\n│" : "");
    }

    str += "\n│\n│"HIY+BRED"ＡＰＰＲＥＮＴＩＣＥ\n"NOR"│";
    if( apprentice ) for(i=0;i<sizeof(apprentice);i++) {
        str += sprintf("%-12s%s", apprentice[i], (i % 6 == 5 ) ? "\n│" : "");
    }

    str += "\n│\n│"HIY+BRED"ＩＭＭＯＲＴＡＬ    \n"NOR"│";
    if( immortal ) for(i=0;i<sizeof(immortal);i++) {
        str += sprintf("%-12s%s", immortal[i], (i % 6 == 5 ) ? "\n│" : "");
    }
    str += "\n╰──────────────────────────────────╯\n";

    write( str );

    return 1;
}

int help(object me)
{
write(@HELP
指令格式 : wizlist

用途: 列出目前所有的巫师名单.

参考: finger, who -w
HELP
     );
     return 1;
}

// demongorgon.c

#include <ansi.h>

inherit NPC;

void create()
{
        set_name( HIW "死神" NOR, ({ "demogorgon" }) );
        set_level(100);
        set_race("human"); 
       set("long",
                "    死神正冷酷地站在你面前，他高大的身躯跟一股令人窒息的\n"
                "可怕杀气正压得你几乎喘不过气来，你的心里似乎有一个声音在告\n"
                "诉你﹕“ 快离开 ！！”\n");

        set("age", 500);

        advance_stat("kee",900);
        advance_stat("gin",900);

        set_attr("str",35);
        set_attr("dex",42);
        set_attr("con",35);
         set_attr("spi",45);
        set_attr("cor",45);

        setup();

}

void start_shutdown()
{
        if( geteuid(previous_object()) != ROOT_UID ) return;

        message("system",
                HIR "\n你听到天空中传来一阵可怕的咆哮 ....\n\n"
                HIW     "死神" HIR "冷酷低沉的声音传入你的耳内：\n\n"
                        "\t\t想要命的，你们还有五分钟可以赶快离开吧！！\n\n"
                        "\t\t不然的话，后果自行负责～～\n\n" NOR,
                users() );
        call_out("countdown", 240, 61);
}

private void countdown(int sec)
{
        sec--;
        if( sec > 59) {
                message("system",
                        HIW "\n死神" HIR "冷酷低沉的声音传进你的耳内：\n\n"
                "\t\t你们还有" + chinese_number(sec) + "秒钟可以离开。\n\n"
                    "\t\t不要命的你就试试看吧！！\n\n" NOR,
                        users() );
                call_out("countdown", 49, 11);
        }else if( sec > 0) {
                message("system",
                        HIW "\n死神" HIR "冷酷低沉的声音传进你的耳内：\n\n"
                                "\t\t你们还有" + chinese_number(sec) + "秒钟的时间可以逃！！\n\n"
                                "\t\t不想死的就赶快逃吧！！！\n\n" NOR,
                        users() );
                call_out("countdown", 1, sec);
        }
        else {
                message("system",
                        HIW "\n死神" HIR "用愤怒的声音咆哮道：\n\n"
                            "\t\t天啊～ 地啊～  神啊～ \n\n" NOR,
                        users() );
                call_out("do_shutdown", 2);
        }
}

private void do_shutdown()
{
        message("system",
                HIW "\n\n死神" HIR "用愤怒的声音喝道﹕\n\n"
                HIW "\t\t通 通 和 我 一 起 下 地 狱 吧 ～～～～ ﹗\n\n"
                HIR "\t\t声音一毕，天空立刻变得血红，不....你眼前的一切都变得血红...\n\n"
                        "\t\t所有的一切都在瞬间陷入火海....\n\n"
                        "\t\t然后你的眼前是一片黑暗....无止尽的黑暗....\n\n" NOR,
                users() );
        shutdown(0);
}

void die()
{
        object ob;

        if( !ob = query_temp("last_damage_from") )
                ob = this_player(1);

        if( !ob ) return;

        message("system",
                HIR "\n\n你听到一声带著愤恨、惊恐、与野兽般的咆哮声响彻整个天空。\n\n"
                HIW "死神" HIR "嘶哑地吼著﹕可恶的" + ob->short() + "﹐我一定会报仇的～～～\n\n"
                        "然后一道黑色火焰呼啸著冲上云端﹐大地又恢复了宁静。\n\n" NOR,
                users() );
        
        destruct(this_object());
}

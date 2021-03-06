#include <ansi.h>
#include <armor.h>

inherit F_NECK_EQ;

void create()
{
    set_name(RED"护身符"NOR, ({ "amulet" }) );
    set_weight(600);
    setup_neck_eq();

    if( !clonep() ) {
        set("unit", "个");
        set("value", 450);
        set("long", "一个庙里求来的护身符，能够保佑你的平安。\n");
        set("wear_as", "neck_eq");
        set("apply_armor/neck_eq", ([
            "evade": 3,
        ]));
    }

    setup();
}

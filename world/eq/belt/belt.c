#include <ansi.h>
#include <armor.h>

inherit F_WAIST_EQ;

void create()
{
    set_name("皮带", ({ "leather belt", "belt" }) );
    set_weight(1000);
    setup_waist_eq();

    if( !clonep() ) {
        set("unit", "条");
        set("value", 200);
        set("long", "一条系在腰上让裤子不会掉的皮带。\n");
        set("wear_as", "waist_eq");
        set("apply_armor/waist_eq", ([
            "armor": 2,
        ]));
    }

    setup();
}

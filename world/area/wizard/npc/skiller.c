#include <npc.h>

inherit F_VENDOR;

void create()
{
    set_name("术大师", ({ "skill master", "master" }));
    set_race("human");
    set_level(50);
    set("age", random(50)+18);

    set("long", "无奸不成商，十足的奸商样的老奸商人。\n");

    // 设定商人所贩卖的物品
    // set_goods(代码, 种类, 编号, 数量)
    // 商人能贩卖的东西只能由 query 指令里查 eq, wp, food 能查出来的物品
    // 如 set_goods(1, "food", 1, 10) 表示贩卖 query 指令查出来的food编号为1的食品十个
    // 若数量设定 -1 表示无限量供应
    // 代码是用于 buy 指令上，如 buy 1 则会向商人买下代码为1的商品

    setup();
}

/* look.c write by -Acme-
*/

#pragma save_binary
#include <ansi.h>

inherit F_CLEAN_UP;

mapping named_apply = ([
    "head_eq": HIK"["NOR"头部"HIK"]"NOR,
    "face_eq": HIK"["NOR"脸部"HIK"]"NOR,
    "neck_eq": HIK"["NOR"颈部"HIK"]"NOR,
    "wrist_eq": HIK"["NOR"手腕"HIK"]"NOR,
    "hand_eq": HIK"["NOR"手部"HIK"]"NOR,
    "finger_eq": HIK"["NOR"手指"HIK"]"NOR,
    "cloth": HIK"["NOR"衣服"HIK"]"NOR,
    "armor": HIK"["NOR"铠甲"HIK"]"NOR,
    "waist_eq": HIK"["NOR"腰部"HIK"]"NOR,
    "pants": HIK"["NOR"裤子"HIK"]"NOR,
    "leg_eq": HIK"["NOR"腿部"HIK"]"NOR,
    "feet_eq": HIK"["NOR"足部"HIK"]"NOR,
    "lefthand": HIK"["NOR"左手"HIK"]"NOR,
    "righthand": HIK"["NOR"右手"HIK"]"NOR,
    "twohanded": HIK"["NOR"双手"HIK"]"NOR,
]);

string status_msg(string who, int ratio)
{
    switch( ratio ) {
		case    100: return HIG + who + "看起来并没有受伤。\n" NOR; break;
		case 96..99: return HIG + who + "似乎受了点轻伤，不过光从外表看不大出来。\n" NOR; break;
		case 91..95: return HIG + who + "看起来可能受了点轻伤。\n" NOR; break;
		case 81..90: return HIG + who + "受了几处伤，不过似乎并不碍事。\n" NOR; break;
		case 61..80: return HIY + who + "受伤不轻，看起来状况并不太好。\n" NOR; break;
		case 41..60: return HIY + who + "气息粗重，动作开始散乱，看来所受的伤著实不轻。\n" NOR; break;
		case 31..40: return HIY + who + "已经伤痕累累，正在勉力支撑著不倒下去。\n" NOR; break;
		case 21..30: return HIR + who + "受了相当重的伤，只怕会有生命危险。\n" NOR; break;
		case 11..20: return HIR + who + "伤重之下已经难以支撑，眼看就要倒在地上。\n" NOR; break;
		case  5..10: return HIR + who + "受伤过重，已经奄奄一息，命在旦夕了。\n" NOR; break;
		default:     return HIR + who + "受伤过重，已经有如风中残烛，随时都可能断气。\n" NOR; break;
    }    
}

string inventory_look(object obj, int flag)
{
    string str, type, sub_type;

    str = obj->short();
    if( type = obj->query("equipped") ) {
        if( sscanf(type, "%s/%s", type, sub_type) == 2 ) {
            str = sprintf("  %s %s",
            	          !undefinedp(named_apply[sub_type]) ? named_apply[sub_type] : sub_type,
            	          str );
        } else str = HIY "  ＊"NOR + str;
    } else if( !flag ) str = "    " + str;
    else return 0;

    return str;
}

int look_living(object me, object obj)
{
    int age;
    string msg, race, pro;
    mixed *inv;

    race = obj->query_race();
    pro = (obj==me) ? gender_self(obj->query("gender")) : gender_pronoun(obj->query("gender"));
    age = obj->query("age");

    if( me != obj ) message("vision", me->name() + "正盯著你看﹐不知道打些什么主意。\n", obj);

    msg = sprintf("%s看起来%s岁。\n", pro, age > 10 ? "约" + chinese_number(age/10*10) + "多" : "不到十" );
    msg += obj->long();
    if( obj->query_stat_maximum("hp") )
        msg += status_msg(pro, obj->query_stat_current("hp")*100/obj->query_stat_maximum("hp"));

    inv = all_inventory(obj);
    if( sizeof(inv) ) {
        inv = map_array(inv, "inventory_look", this_object(), obj->is_corpse() ? 0 : 1 );
        inv -= ({ 0 });
        if( sizeof(inv) )
            msg += sprintf( obj->is_corpse() ? "%s的遗物有﹕\n%s\n" : "%s身上带著﹕\n%s\n", pro, implode(inv, "\n") );
    }

    message("vision", msg, me);

    return 1;
}

int look_item(object me, object obj)
{
    mixed *inv;

    me->start_more(obj->long());
    inv = all_inventory(obj);
    if( sizeof(inv) ) {
        if( sizeof(inv) < 30 ) {
            inv = map_array(inv, "inventory_look", this_object() );
            message("vision", sprintf("里面有﹕\n  %s\n", implode(inv, "\n  ") ), me);
        }
        else tell_object(me, YEL"里面杂七杂八的放著许多东西..\n"NOR);
    }
    return 1;
}

int main(object me, string arg)
{
    object obj;

    if( !environment(me) )
        return notify_fail("你现在什么也看不到！\n");

    if( me->query_temp("block_msg/vision") )
        return notify_fail("你现在什么也看不到！\n");

    if( !arg ) return environment(me)->do_look(me, arg);
    
    if( (obj = present(arg, me)) || (obj = present(arg, environment(me)))) {
        if( obj->is_character() ) return look_living(me, obj);
            else return look_item(me, obj);
    } else return environment(me)->do_look(me, arg);
    
    return 1;
}

int help (object me)
{
    write(@HELP
指令格式: look [<物品>|<生物>]
 
这个指令让你查看你所在的环境、某件物品、生物。
 
HELP);
    return 1;
}
/* buy.c write by -Acme- */

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
	int amount, number;
	string from;
	object who;
	
	if( !arg ) return notify_fail("你想要买什么东西？\n");
	
	if( sscanf(arg, "%d %d from %s", amount, number, from) == 3 ) {
		if( !objectp(who=present(from, environment(me))) )
		    return notify_fail("你附近没有这个人。\n");
	    if( !who->to_sell(me, number, amount) )
	    	return notify_fail("这个人似乎不卖这个东西。\n");
	} else if( sscanf(arg, "%d from %s", number, from) == 2 ) {
		if( !objectp(who=present(from, environment(me))) )
		    return notify_fail("你附近没有这个人。\n");
	    if( !who->to_sell(me, number, 1) )
	    	return notify_fail("这个人似乎不卖这个东西。\n");
	} else if( sscanf(arg, "%d %d", amount, number) == 2 ) {
		if( !environment(me)->to_sell(me, number, amount) )
			return notify_fail("这里似乎不是商店。\n");
	} else if( sscanf(arg, "%d", number) == 1 ) {
        if( !environment(me)->to_sell(me, number, 1) )
            return notify_fail("这里似乎不是商店。\n");
	} else return notify_fail("你想要买什么东西？\n");
	
	return 1;
}

int help(object me)
{
   write(@HELP
指令格式: buy [数量] 商品编号 [from 某人]

这指令可以让你从商店买下某件商品，也可从某人身上买商品，当你
从商店买物品时，指令格式中不需要加上 from 某人。例：

buy 3                从商店买下编号为3的商品
buy 2 5              从商店买下二个编号为5的商品
buy 2 from waiter    从water身上买下编号为2的商品
buy 10 7 from waiter 从water身上买下十个编号为7的商品

相关指令： list, sell
HELP);
   return 1;
}
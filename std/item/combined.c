// combined_item.c

inherit ITEM;

// variables

int amount;

// implementations

int query_amount() { return amount; }

private void destruct_me() { destruct(this_object()); }

void set_amount(int v)
{
    if( v < 0 ) error("combine:set_amount less than 1.\n");
    amount = v;

    if( v == 0 ) call_out("destruct_me", 0);
    else this_object()->set_weight((int)(v * (float)query("base_weight")));
}

void add_amount(int v) { set_amount(amount+v); }

varargs string short(int raw)
{
    if( raw ) return ::short(raw);
    return chinese_number(query_amount()) + (query("base_unit") ? query("base_unit") : query("unit") )+ ::short(raw);
}

varargs int move(mixed dest, int silent)
{
	string file = base_name(this_object());
    object ob;
    if( ! ::move(dest, silent) ) return 0;
    
    // 已经装备起来的物品，不应该与其它物品合并
    if( this_object()->query("equipped") ) return 1;
	
	// 已经改变的物品资料，不能与其它的物品合并(因为可能资料不同，一合就变一样了)
    if( this_object()->query("changed") ) return 1;

    // 自动合并样同物件，不限定是否是人物身上
    if( environment()->is_area() ) {
    	mapping info = this_object()->query("area_info");
    	if( !mapp(info) ) return 0;
    	foreach(ob in environment()->query_inventory(info["x_axis"], info["y_axis"]) ) {
		    if( ob == this_object() ) continue;
		    if( base_name(ob) == file ) {
				amount += (int)ob->query_amount();
				destruct(ob);
		    }
    	}
    } else {
		foreach(ob in all_inventory(environment())) {
		    if( ob == this_object() ) continue;
		    if( base_name(ob) == file ) {
				amount += (int)ob->query_amount();
				destruct(ob);
		    }
		}
	}
    set_amount(amount);

    return 1;
}

int render_value()
{
    return to_int(query("base_value") * amount);
}

void setup()
{
    ::setup();

    if( !amount ) set_amount(1);

    set("value", (: render_value :));
}
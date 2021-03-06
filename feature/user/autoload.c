// autoload.c

mapping autoload = ([]);

void clean_up_autoload() { autoload = ([]); }

void save_autoload()
{
	object *inv;
	int i, amount;
	mapping dbase;

	inv = all_inventory();
	for(i=0;i<sizeof(inv);i++) {
		if( !(amount = inv[i]->query_amount()) ) amount = 1;
		autoload[i] = ([]);
		
		// 有改变过物品的设定(与原始程式码不同之设定)，才必须储存该物品的dbase
		// 方便在载入时重新设定其dbase
		if( inv[i]->query("changed") && (dbase = inv[i]->query_entire_dbase()) )
			autoload[i]["dbase"] = dbase;
		
		autoload[i]["file"] = base_name(inv[i]);
		autoload[i]["amount"] = amount;
	}
}

void restore_autoload()
{
    int i;
    string err;
	object ob;
	
    for(i=0;i<sizeof(autoload);i++) {
		err = catch(ob = new(autoload[i]["file"]), this_object());
		if( err || !ob ) {
			write("你觉得似乎失落了什么重要的东西，最好通知一下巫师。\n");
			log_file("AUTOLOAD", sprintf("Fail to autoload %s of %s, error %s\n", autoload[i]["file"], this_object()->query("name"), err));
			continue;
		}
		
		export_uid(ob);

		if( !ob->move(this_object(), 1) ) {
			write("你觉得似乎失落了什么重要的东西，最好通知一下巫师。\n");
		    log_file("AUTOLOAD", sprintf("Fail to autoload %s of %s\n", autoload[i]["file"], this_object()->query("name")));
		    destruct(ob);
		    continue;
		} else this_object()->add_encumbrance( ob->weight() );

//        ob->autoload(autoload[i]["amount"]);
		
        // 复合物品
		if( !undefinedp(autoload[i]["amount"]) ) ob->set_amount(autoload[i]["amount"]);
		
		if( !undefinedp(autoload[i]["dbase"]) ) {
			string *key;
			int j;
			key = keys(autoload[i]["dbase"]);
			j = sizeof(key);
			while(j--) ob->set(key[j], autoload[i]["dbase"][key[j]]);
			ob->delete("equipped");
		}
    }
	clean_up_autoload();
}

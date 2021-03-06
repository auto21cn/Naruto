/*  object.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

varargs int getoid(object ob)
{
    int id;

    if (!ob) ob = previous_object();
    sscanf(file_name(ob), "%*s#%d", id);
    return id;
}

// Get the owner of a file.  Used by log_error() in master.c.
string file_owner(string file)
{
    string name, rest, dir;

    if (file[0] != '/') file = "/" + file;
    if (sscanf(file, "/u/%s/%s/%s", dir, name, rest) == 3) {
        return name;
    }
    return 0;
}

/*  creator_file()
 *
 *  这个函数是 ES2 安全系统的核心，每当一个物件被 load 的时候，系统会
 *  将这个物件的档名传给 simul_efun 中的 creator_file() 函数，并且用
 *  这个函数的传回值作为此一物件的 uid。在 ES2 的安全系统中，物件的
 *  uid 表示这个物件的最高权限( 物件可以经由 seteuid 来提高或降低自己
 *  的权限 )，这个最高权限是单纯由物件的档名决定的，因此你也可以说ES2
 *  的安全系统是根基在档案读写权的管制上。
 *
 *  ES2 目前对 uid 的发放方式请参考程式码中的注解。
 */
string creator_file(string file)
{
    string *path, euid;

    path = explode(file, "/") - ({ "" });
    if( ! sizeof(path) )
	return 0;	/* 这个情况应该不会发生，只是预防万一 */

    switch(path[0])
    {
	// ----------------------------------------------------------
	// 在 /adm 和 /cmds 下的物件拥有最高的权限 ROOT_UID，可以做任
	// 何事。
	// ----------------------------------------------------------
	case "adm":
	case "world":
	case "cmds":	return ROOT_UID;

	// -----------------------------------------------------------
	// 在 /u 及 /open 下的物件如果是由使用者载入，则获得使用者的相
	// 同权限，如果不是由使用者载入，则获得和载入它的物件相同的权
	// 限。
	// -----------------------------------------------------------
	case "u":
	case "open":
	    if( this_player(1) ) euid = getuid(this_player(1));
	    if( !euid ) euid = geteuid(previous_object(1));
	    if( euid && euid != ROOT_UID ) return euid;
            return "NONAME";

	// ------------------------------------------------------------
	// 在 /obj 下的物件其最高权限由载入该物件的权限(euid)决定，换句
	// 话说由具有 ROOT_UID 权限的物件载入的 /obj 物件，就具有 ROOT
	// 权限，由使用者载入的物件就具有使用者的权限。一般具有 ROOT 权
	// 限的物件如果要载入 /obj 物件，通常会先 seteuid 成适当的权限
	// 再进行载入。
	// ------------------------------------------------------------
	case "obj":
	    if( previous_object(1) ) return geteuid(previous_object(1));
	    return "NONAME";

	// ------------------------------------------------------------
	// 其他目录下的物件都给予 MUDLIB_UID 的权限
	// ------------------------------------------------------------
	case "daemon":
	default:
	    return MUDLIB_UID;
    }
}

void destruct(object ob)
{
    string err;

    if( !ob ) return;

    err = catch(ob->remove());

    if( err ) log_file("destruct", sprintf("[%s] %O: %s", ctime(time()), ob, err));

    if( userp(ob) && (geteuid(previous_object())!=ROOT_UID) && (geteuid(previous_object())!=geteuid(ob)) ) {
		log_file("destruct", sprintf("[%s] %O: Destructed by %s denied\n", ctime(time()), ob, err));
		error("Destruction denied.\n");
    }

    // 区域使用 by -Acme-
    if( environment(ob) && environment(ob)->is_area() ) {
    	environment(ob)->move_out(ob->query("area_info/x_axis"),
    		                      ob->query("area_info/y_axis"), ob);
    }

    efun::destruct(ob);
}

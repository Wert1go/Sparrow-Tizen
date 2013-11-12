var a = API.messages.getDialogs({"offset" : 0, "count":2});
var l = a.items@.user_id;
var c = a.items@.chat_id; 
var i = 0; 
var uids = [];
var j;
while (i < c.length) { 
	i=i+1; 
	j = API.messages.getChatUsers({"chat_id" : c[i]}); 
	if (j != false) {
		uids = uids + [j]; 
		if (parseInt(c[i]) != 0) 
		l = l + j; 
	}
};
var b = API.users.get({"user_ids": l, "fields": "photo_100,photo_50,online"});
return {"chat_uids" : uids, "users": b, "messages": a};

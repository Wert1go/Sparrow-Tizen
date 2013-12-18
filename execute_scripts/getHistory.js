var a = API.messages.getHistory({"chat_id" : 90});

var l = a.items@.fwd_messages;

var i = 0; 
var uids = [];
var j;

while (i < l.length) { 
	

	var fwd = l[i]@.user_id;
	uids = uids + fwd; 
	i = i+ 1; 
};
var b = [];
if (uids.length > 0) {
	b = API.users.get({"user_ids": uids, "fields": "photo_100,photo_50,online,is_friend,photo_200"});
}
return {"users": b, "count" : a.count, "items" : a.items};
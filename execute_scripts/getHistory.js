var a = API.messages.getHistory({"user_id" : 12121645});

var l = a.items@.fwd_messages;

var i = 0; 
var uids = [];
var j;

while (i < l.length) { 
	i = i+ 1; 

	var fwd = l[i]@.user_id;
	uids = uids + fwd; 
};
var b = [];
if (uids.length > 0) {
	b = API.users.get({"user_ids": uids, "fields": "photo_100,photo_50,online,is_friend,photo_200"});
}
return {"users": b, "count" : a.count, "items" : a.items};
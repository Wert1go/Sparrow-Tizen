var a = API.messages.getHistory({"user_id" : 183384680});

var l = a.items@.fwd_messages;

var i = 0; 
var i1 = 0; 
var i2 = 0; 
var i3 = 0; 
var i4 = 0; 

var uids = [];
var j;

while (i < l.length) { 
	
	var fwd = l[i]@.user_id;
	uids = uids + fwd; 
	
	var fwd_msgs = l[i]@.fwd_messages;
	var i1 = 0;
	var i2 = 0;
	while (i1 < fwd_msgs.length) { 
		var fwd1 = fwd_msgs[i1]@.user_id;
		uids = uids + fwd1; 
		i1 = i1+ 1;
	}

	i = i+ 1;
};

var b = [];
if (uids.length > 0) {
	b = API.users.get({"user_ids": uids, "fields": "photo_100,photo_50,online,is_friend,photo_200"});
}
return {"users": b, "count" : a.count, "items" : a.items};
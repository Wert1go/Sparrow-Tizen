var a = API.friends.getRequests({"offset" : 0, "extended":0, "out":0});
var b = API.users.get({"user_ids": a.items, "fields":"photo_50,photo_100,last_seen,online,is_friend"});
return {"items": b};
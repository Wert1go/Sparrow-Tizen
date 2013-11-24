var a = API.friends.getSuggestions({"count" : 20, "fields":"photo_50,photo_100,last_seen,online,is_friend"});
return {"items": a};
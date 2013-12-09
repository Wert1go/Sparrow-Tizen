var a = API.messages.send({"user_id" : 30143161, "message":"qwqwqw"});
var m = API.messages.getById({"message_ids" : a});

var result = m.items[0];

return {"message_id" : a, "message" : result};
var a = API.messages.get({"offset" : 0, "count":200, "out":1});
var c = a.items@.read_state;
var count = 0;
var i = 0;
while (i < c.length) { 
	i=i+1; 
	if (parseInt(c[i]) == 0) {
		count = count + 1;
	}
};

return {"response" : count};

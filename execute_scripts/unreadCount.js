var a = API.messages.get({"offset" : 0, "count":200, "out":0});
var c = a.items@.read_state;
var count = 0;
var i = 0;
while (i < c.length) { 
	if (parseInt(c[i]) == 0) {
		count = count + 1;
	}
	i=i+1; 
};

return {"count" : count};

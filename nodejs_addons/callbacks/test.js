const addon = require('./build/Release/callbacks');

addon((msg) => {
	console.log(msg);
});

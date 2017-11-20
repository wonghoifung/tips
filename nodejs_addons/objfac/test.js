const addon = require('./build/Release/objfac');
const obj1 = addon('hello');
const obj2 = addon('world');
console.log(obj1.msg, obj2.msg);
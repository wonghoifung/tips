const addon = require('./build/Release/objfac');
const obj1 = addon('hello1');
const obj2 = addon('world2');
console.log(obj1.msg, obj2.msg);
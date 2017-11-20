const addon = require('./build/Release/cppobj');
const obj = new addon.MyObj(10);
console.log(obj.plusOne());
console.log(obj.plusOne());
console.log(obj.plusOne());
const obj2 = addon.MyObj(20)
console.log(obj2.plusOne());
console.log(obj2.plusOne());
console.log(obj2.plusOne());
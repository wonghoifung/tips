const addon = require('./build/Release/cppobj');
const obj = new addon.MyObj(10);
console.log(obj.plusOne());
console.log(obj.plusOne());
console.log(obj.plusOne());
const obj2 = addon.MyObj(20)
console.log(obj2.plusOne());
console.log(obj2.plusOne());
console.log(obj2.plusOne());

obj.value = 100;
obj2.value= 200;
console.log(obj.value);
console.log(obj2.value);

console.log(obj2.multiply(-1).value);
console.log(obj.multiply().value);
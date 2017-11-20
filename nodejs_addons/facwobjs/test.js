const createObject = require('./build/Release/facwobjs');

const obj = createObject(10);
console.log(obj.plusOne());
console.log(obj.plusOne());
console.log(obj.plusOne());

const obj2 = createObject(20);
console.log(obj2.plusOne());
console.log(obj2.plusOne());
console.log(obj2.plusOne());
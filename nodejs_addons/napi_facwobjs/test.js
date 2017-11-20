const createObject = require('./build/Release/facwobjs');

const obj = createObject(10);
console.log(obj.plusOne());
console.log(obj.plusOne());
console.log(obj.plusOne());

const obj2 = createObject(20);
console.log(obj2.plusOne());
console.log(obj2.plusOne());
console.log(obj2.plusOne());

// const obj3 = new createObject.MyObj(30);
// console.log(obj3.plusOne());
// console.log(obj3.plusOne());
// console.log(obj3.plusOne());
[@bs.val] external document: 'a = "";

document##body##style##margin #= "0";

let console = document##createElement("div");
console##style##width #= "200px";
console##style##height #= "120px";
console##style##overflow #= "scroll";
console##style##webkitOverflowScrolling #= "touch";
document##body##appendChild(console);

let log = msg => {
  let node = document##createElement("div");
  node##innerText #= msg;
  console##prepend(node);
};
log("log here");

let canvas = document##querySelector("#index");
canvas##width #= 800;
canvas##height #= 400;
canvas##style##outline #= "1px solid black";
let context = canvas##getContext("2d");

canvas##addEventListener("touchstart", e => {
  e##preventDefault();
  log("heeey");
});

canvas##addEventListener("touchmove", e => {
  e##preventDefault();
  let touch = e##touches[0];
  context##fillRect(touch##clientX, touch##clientY, 2, 2);
  log("mmoved");
});

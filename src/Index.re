[@bs.val] external document: 'a = "";
[@bs.val] external window: 'a = "";

document##body##style##margin #= "0";

let console = document##createElement("div");
console##style##width #= "300px";
console##style##maxHeight #= "120px";
console##style##overflow #= "scroll";
console##style##webkitOverflowScrolling #= "touch";
console##style##color #= "white";
console##style##position #= "absolute";
console##style##bottom #= "0px";
console##style##margin #= "0px";
console##style##fontFamily #= "-apple-system, BlinkMacSystemFont, sans-serif";
document##body##appendChild(console);

let log = msg => {
  let node = document##createElement("div");
  node##innerText #= Js.Json.stringifyAny(msg);
  ignore(console##prepend(node));
};

log("Try drawing with many fingers on iPad");
let canvas = document##querySelector("#index");
let (width, height) = (window##innerWidth, window##innerHeight);
canvas##width #= width;
canvas##height #= height;
canvas##style##outline #= "1px solid black";

let context = canvas##getContext("2d");
context##fillStyle #= "black";
context##fillRect(0, 0, width, height);

type point = {
  x: int,
  y: int,
};
let strokes: array(array('a)) = [||];

canvas##addEventListener("touchstart", e => {
  e##preventDefault();
  Js.Array.push([||], strokes)->ignore;
});

[@bs.set] external setLength: (array('a), int) => unit = "length";

canvas##addEventListener("touchmove", e => {
  e##preventDefault();
  let lineWidth = 2;
  context##fillStyle #= "white";
  context##strokeStyle #= "white";
  context##lineWidth #= lineWidth;
  context##globalAlpha #= 0.2;

  Js.Array.forEach(
    touch => {
      let point = {x: touch##clientX, y: touch##clientY};
      switch (strokes) {
      | [||] =>
        context##fillRect(point.x, point.y, lineWidth, lineWidth);
        Js.Array.push([|point|], strokes)->ignore;
      | strokes =>
        let latestStroke = strokes[Js.Array.length(strokes) - 1];
        switch (latestStroke) {
        | [||] =>
          context##fillRect(point.x, point.y, lineWidth, lineWidth);
          Js.Array.push(point, latestStroke)->ignore;
        | latestStroke =>
          let prevPoint = latestStroke[Js.Array.length(latestStroke) - 1];
          context##beginPath();
          context##moveTo(prevPoint.x, prevPoint.y);
          context##lineTo(point.x, point.y);
          context##stroke();
          context##closePath();
          Js.Array.push(point, latestStroke)->ignore;
        };
      };
    },
    Js.Array.from(e##touches),
  );
});

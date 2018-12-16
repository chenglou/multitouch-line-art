[@bs.val] external document: 'a = "";
[@bs.val] external window: 'a = "";

document##body##style##margin #= "0";

let console = document##createElement("div");
console##style##width #= "200px";
console##style##height #= "120px";
console##style##overflow #= "scroll";
console##style##webkitOverflowScrolling #= "touch";
document##body##appendChild(console);

let log = msg => {
  let node = document##createElement("div");
  node##innerText #= Js.Json.stringifyAny(msg);
  ignore(console##prepend(node));
};

let canvas = document##querySelector("#index");
let (width, height) = (window##innerWidth, 500);
canvas##width #= width;
canvas##height #= height;
canvas##style##outline #= "1px solid black";

let context = canvas##getContext("2d");
context##fillStyle #= "black";
context##fillRect(0, 0, width, height);

let previousDrawing = ref(None);
type touching =
  | Down(float)
  | Up(float)
  | First;
let touching = ref(First);
type point = {
  x: int,
  y: int,
};
let strokes: array(array('a)) = [||];
let maxTimeForCountingSomethingAsTouch = 150.;
let objectsOnScene = [||];
let objectTypes = [|Objs.obj1|];
/* next: Objs.obj1 has too many points */

canvas##addEventListener("touchstart", e => {
  e##preventDefault();
  touching := Down(Js.Date.now());
  Js.Array.push([||], strokes)->ignore;
});

[@bs.set] external setLength: (array('a), int) => unit = "length";

canvas##addEventListener("touchend", e => {
  e##preventDefault();
  switch (touching^) {
  | First
  | Up(_) => ()
  | Down(startTime) =>
    let ellapsedTime = Js.Date.now() -. startTime;
    log(ellapsedTime);
    if (ellapsedTime < maxTimeForCountingSomethingAsTouch) {
      log("commit drawing");
      /* turn dots into an object */
      previousDrawing := None;
      log(strokes);
      setLength(strokes, 0);
    } else {
      ();
        /* nothing happening here yet */
    };
  };
  touching := Up(Js.Date.now());
});

canvas##addEventListener("touchmove", e => {
  e##preventDefault();
  let lineWidth = 2;
  context##fillStyle #= "white";
  context##strokeStyle #= "white";
  context##lineWidth #= lineWidth;
  context##globalAlpha #= 0.2;

  let addPoint = e => {
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
  };

  addPoint(e);
  previousDrawing := Some("hipattern");
});

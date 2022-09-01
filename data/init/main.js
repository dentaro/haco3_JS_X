// setup = function(){
// }
// c = 0

// error = function(e, x, y){
//   color(255,0,0)
//   text("E:" + e, x, y)
//   color(255,255,255)
// }
// loop = function(){
//     color(0,0,0)
//     fillrect(0,0,128,128)

//     var y = 0
//     color(0,0,255)
//     fillrect(0,0,128,9)
//     color(255,255,255)
//     text("== o-bako API test ==",0,y)

//     y += 10
//     text("text",0,y)
//     text("text",50+c/10%20,y)
//     c++

//     y += 10
//     text("drawrect", 0, y)
//     drawrect(50+c/10%20, y, 8, 8)

//     y += 10
//     text("fillrect", 0, y)
//     fillrect(50+c/10%20, y, 8, 8)

//     try{
//       y += 10
//       text("pset", 0, y)
//       pset(50+c/10%20, y)
//     }catch(e){
//       error(e, 50, y)
//     }

//     try{
//       y += 10
//       text("spr", 0, y)
//       spr(50+c/10%20, y, 16, 16, 0, 0)
//     }catch(e){
//       error(e, 50, y)
//     }
// }


setup = function(){
  color(0,0,0)
  fillrect(0,0,128,128)
}

c = 0
x = 16
y = 32
var t = 1

loop = function(){
  color(0,0,0)
  fillrect(0,0,128,128)
  //背景を描く
  // for (var i = 0; i<15; i++){
  //    for (var j = 0; j<15; j++){
  //      spr(i*8, j*8, 8, 8, 8*1, 8*13)
  //    }
  // }
　if(btn(0) > 0){walk = 0;step = 1;x -= step;}
　if(btn(1) > 0){walk = 0;step = 1;x += step;}
　if(btn(2) > 0){walk = 0;step = 1;y -= step;}
　if(btn(3) > 0){walk = 0;step = 1;y += step;}
  spr(x%128, y%128, 8, 8, 8*4, 8*14)
  c ++
}
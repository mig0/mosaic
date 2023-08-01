R"CSS(

button.none   { background-image: image(rgb(180, 180, 180)); border-color: rgb(80, 80, 80); }
button.red    { background-image: image(rgb(180, 30, 30));   border-color: rgb(80, 10, 10); }
button.white  { background-image: image(rgb(240, 240, 240)); border-color: rgb(120, 120, 120); }
button.yellow { background-image: image(rgb(240, 210, 30));  border-color: rgb(120, 100, 10); }
button.orange { background-image: image(rgb(240, 140, 30));  border-color: rgb(120, 70, 10);  }
button.blue   { background-image: image(rgb(30, 30, 180));   border-color: rgb(10, 10, 90); }
button.green  { background-image: image(rgb(30, 180, 30));   border-color: rgb(10, 100, 10); }
button.red:hover    { background-image: image(rgb(220, 48, 48)); }
button.white:hover  { background-image: image(rgb(255, 190, 190)); }
button.yellow:hover { background-image: image(rgb(255, 240, 48)); }
button.orange:hover { background-image: image(rgb(255, 170, 48)); }
button.blue:hover   { background-image: image(rgb(48, 48, 220)); }
button.green:hover  { background-image: image(rgb(48, 220, 48)); }
button.red:active    { background-image: image(rgb(140, 18, 18)); }
button.white:active  { background-image: image(rgb(190, 190, 190)); }
button.yellow:active { background-image: image(rgb(190, 170, 18)); }
button.orange:active { background-image: image(rgb(190, 100, 18)); }
button.blue:active   { background-image: image(rgb(18, 18, 140)); }
button.green:active  { background-image: image(rgb(18, 140, 18)); }
button { border: solid 1px; font: 8px "Sans"; margin: 0px; padding: 0px; min-width: 20px; min-height: 20px; border-radius: 4px; }
button.active-cell { border-radius: 12px; }
button.active-cell.red    { background: linear-gradient(to left top, white, rgb(180, 30, 30)   75%); }
button.active-cell.white  { background: linear-gradient(to left top, white, rgb(255, 175, 175) 75%); }
button.active-cell.yellow { background: linear-gradient(to left top, white, rgb(240, 210, 30)  75%); }
button.active-cell.orange { background: linear-gradient(to left top, white, rgb(240, 140, 30)  75%); }
button.active-cell.blue   { background: linear-gradient(to left top, white, rgb(30, 30, 180)   75%); }
button.active-cell.green  { background: linear-gradient(to left top, white, rgb(30, 180, 30)   75%); }

)CSS"

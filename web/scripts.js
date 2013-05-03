/* generic function to either show or hide an element */
function showHide(el)
{
  id = document.getElementById(el).style;
  id.display = (id.display != 'block')? 'block' : 'none';
}


/**
 * In theory, this should create a tooltip
 **/
function simple_tooltip(target_items, name){
 $(target_items).each(function(i){
                $("body").append("<div class='"+name+"' id='"+name+i+"'><p>"+$(this).attr('title')+"</p></div>");
                var my_tooltip = $("#"+name+i);

                if($(this).attr("title") != ""){ // checks if there is a title

                $(this).removeAttr("title").mouseover(function(){
                                my_tooltip.css({opacity:0.8, display:"none"}).fadeIn(400);
                }).mousemove(function(kmouse){
                                my_tooltip.css({left:kmouse.pageX+15, top:kmouse.pageY+15});
                }).mouseout(function(){
                                my_tooltip.fadeOut(400);
                });

                }
        });
}

$(document).ready(function(){
         simple_tooltip("a","tooltip");
});


/*****************************/
/*   Functions of the Menu   */
/*****************************/

/* hides the Add Host "box" from the menu frame and set the status*/
function hideAddHostBox()
{
  showHide('trAddHost');
  document.getElementById("divStatusMenu").innerHTML="";
}

function sendHostToServer()
{
  var xmlhttp;
  if(window.XMLHttpRequest)
  {
      xmlhttp=new XMLHttpRequest();
  }
  else
  {
    xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
  }
  xmlhttp.onreadystatechange=function()
  {
    if (xmlhttp.readyState == 4 && xmlhttp.status == 200)
    {
      document.getElementById("divStatusMenu").innerHTML=xmlhttp.responseText;
    }
  }
  s="/add_host?txtAddHostIp=";
  s+=document.getElementById("txtAddHostIp").value;
  s+="&txtAddHostName=";
  s+=document.getElementById("txtAddHostName").value;
  xmlhttp.open("GET", s, true);
  xmlhttp.send();
}

/* generic function to either show or hide an element */
function showHide(el)
{
  id = document.getElementById(el).style;
  id.display = (id.display != 'block')? 'block' : 'none';
}

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
